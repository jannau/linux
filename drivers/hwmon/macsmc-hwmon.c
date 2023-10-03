// SPDX-License-Identifier: GPL-2.0-only OR MIT
/*
 * Apple SMC hwmon driver for Apple Silicon platforms
 *
 * The System Management Controller on Apple Silicon devices is responsible for
 * measuring data from sensors across the SoC and machine. These include power,
 * temperature, voltage and current sensors. Some "sensors" actually expose
 * derived values. An example of this is the key PHPC, which is an estimate
 * of the heat energy being dissipated by the SoC.
 *
 * While each SoC only has one SMC variant, each platform exposes a different
 * set of sensors. For example, M1 MacBooks expose battery telemetry sensors
 * which are not present on the M1 Mac mini. For this reason, the available
 * sensors for a given platform are described in the device tree in a child
 * node of the SMC device. We must walk this list of available sensors and
 * populate the required hwmon data structures at runtime.
 *
 * Copyright (C) The Asahi Linux Contributors
 */

#include <linux/device.h>
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>
#include <linux/mfd/macsmc.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>

#define MAX_LABEL_LENGTH 32

struct macsmc_hwmon_key {
	smc_key key;
	struct apple_smc_key_info info;
};

struct macsmc_hwmon_sensor {
	struct macsmc_hwmon_key input;
	char label[MAX_LABEL_LENGTH];
};

struct macsmc_hwmon_fan {
	struct macsmc_hwmon_key input;
	struct macsmc_hwmon_key min;
	struct macsmc_hwmon_key max;
	struct macsmc_hwmon_key target;
	char label[MAX_LABEL_LENGTH];
	u32 attributes;
};

struct macsmc_hwmon {
	struct device *dev;
	struct apple_smc *smc;
	struct device *hwmon_dev;
	struct macsmc_hwmon_fan *fan;
	struct macsmc_hwmon_sensor *pwr;
	struct macsmc_hwmon_sensor *temp;
	struct macsmc_hwmon_sensor *volt;
	struct macsmc_hwmon_sensor *curr;
	u32 num_fan;
	u32 num_pwr;
	u32 num_temp;
	u32 num_volt;
	u32 num_curr;
};


static int macsmc_hwmon_read_label(struct device *dev,
				enum hwmon_sensor_types type, u32 attr,
				int channel, const char **str)
{
	struct macsmc_hwmon *hwmon = dev_get_drvdata(dev);

	switch (type) {
	case hwmon_fan:
		if (channel >= hwmon->num_fan)
			return -EINVAL;
		*str = hwmon->fan[channel].label;
		break;
	case hwmon_power:
		if (channel >= hwmon->num_pwr)
			return -EINVAL;
		*str = hwmon->pwr[channel].label;
		break;
	case hwmon_temp:
		if (channel >= hwmon->num_temp)
			return -EINVAL;
		*str = hwmon->temp[channel].label;
		break;
	case hwmon_in:
		if (channel >= hwmon->num_volt)
			return -EINVAL;
		*str = hwmon->volt[channel].label;
		break;
	case hwmon_curr:
		if (channel >= hwmon->num_curr)
			return -EINVAL;
		*str = hwmon->curr[channel].label;
		break;
	default:
		return -EOPNOTSUPP;
	}

	return 0;
}

static int macsmc_hwmon_read_key(struct apple_smc *smc, struct macsmc_hwmon_key *k,
				 long *val, int scale)
{
	int ret;

	switch (k->info.type_code) {
	case _SMC_KEY("flt "): {
		u32 flt32;
		ret = apple_smc_read_f32_scaled(smc, k->key, &flt32, scale);
		if (ret < 0)
			return ret;
		*val = flt32;
		return 0;
	}
	case _SMC_KEY("ioft"): {
		u64 ui64;
		ret = apple_smc_read_ioft_scaled(smc, k->key, &ui64, scale);
		if (ret)
			return ret;
		*val = ui64;
		return 0;
	}
	default:
		return -EOPNOTSUPP;
	}
}

static int macsmc_hwmon_read_fan(struct macsmc_hwmon *hwmon, u32 attr, int channel, long *val)
{
	if (!(hwmon->fan[channel].attributes & BIT(attr)))
		return -EINVAL;

	switch (attr) {
	case hwmon_fan_input:
		return macsmc_hwmon_read_key(
			hwmon->smc, &hwmon->fan[channel].input, val, 1);
	case hwmon_fan_min:
		return macsmc_hwmon_read_key(hwmon->smc,
					     &hwmon->fan[channel].min, val, 1);
	case hwmon_fan_max:
		return macsmc_hwmon_read_key(hwmon->smc,
					     &hwmon->fan[channel].max, val, 1);
	case hwmon_fan_target:
		return macsmc_hwmon_read_key(
			hwmon->smc, &hwmon->fan[channel].target, val, 1);
	default:
		return -EINVAL;
	}
}

/*
 * The SMC has keys of multiple types, denoted by a FourCC of the same format
 * as the key ID. We must be able to read all of them as we only know which
 * type a key will be at runtime. FourCCs sometimes have three characters and
 * a space, e.g. 'flt ' for an IEEE 754 float.
 *
 * TODO: non-float key types
 */
static int macsmc_hwmon_read(struct device *dev, enum hwmon_sensor_types type,
			u32 attr, int channel, long *val)
{
	struct macsmc_hwmon *hwmon = dev_get_drvdata(dev);

	switch (type) {
	case hwmon_fan:
		if (channel >= hwmon->num_fan)
			return -EINVAL;
		return macsmc_hwmon_read_fan(hwmon, attr, channel, val);
	case hwmon_power:
		if (channel >= hwmon->num_pwr)
			return -EINVAL;
		return macsmc_hwmon_read_key(
			hwmon->smc, &hwmon->pwr[channel].input, val, 1000000);
	case hwmon_temp:
		if (channel >= hwmon->num_temp)
			return -EINVAL;
		return macsmc_hwmon_read_key(
			hwmon->smc, &hwmon->temp[channel].input, val, 1000);
	case hwmon_in:
		if (channel >= hwmon->num_volt)
			return -EINVAL;
		return macsmc_hwmon_read_key(
			hwmon->smc, &hwmon->volt[channel].input, val, 1000);
	case hwmon_curr:
		if (channel >= hwmon->num_curr)
			return -EINVAL;
		return macsmc_hwmon_read_key(
			hwmon->smc, &hwmon->curr[channel].input, val, 1);
	default:
		return -EOPNOTSUPP;
	}
}

static int macsmc_hwmon_write(struct device *dev, enum hwmon_sensor_types type,
			u32 attr, int channel, long val)
{
	return -EOPNOTSUPP;
}

static umode_t macsmc_hwmon_is_visible(const void *data,
				enum hwmon_sensor_types type, u32 attr,
				int channel)
{
	return 0444;
}


static const struct hwmon_ops macsmc_hwmon_ops = {
	.is_visible = macsmc_hwmon_is_visible,
	.read = macsmc_hwmon_read,
	.read_string = macsmc_hwmon_read_label,
	.write = macsmc_hwmon_write,
};

static struct hwmon_chip_info macsmc_hwmon_info = {
	.ops = &macsmc_hwmon_ops,
	.info = NULL, /* Filled at runtime */
};

static int macsmc_hwmon_parse_key(struct device *dev, struct apple_smc *smc,
				  struct device_node *node, const char *prop,
				  struct macsmc_hwmon_key *key_info)
{
	const char *key_str;
	smc_key key;
	int ret;

	ret = of_property_read_string(node, prop, &key_str);
	if (ret) {
		dev_err(dev, "Could not find %s for %s\n", prop,
			of_node_full_name(node));
		return ret;
	}
	key = _SMC_KEY(key_str);

	ret = apple_smc_get_key_info(smc, key, &key_info->info);
	if (ret) {
		dev_err(dev, "Failed to retrieve key info for %s: %d\n",
			key_str, ret);
		return ret;
		;
	}
	key_info->key = key;

	return 0;
}

static int macsmc_hwmon_populate_fans(struct device *dev, struct apple_smc *smc,
				      struct device_node *hwmon_node,
				      struct macsmc_hwmon_fan **fans,
				      u32 *num_keys)
{
	struct device_node *fan_list, *fan_node;
	int ret = 0;
	int i = 0;
	pr_err("%s:\n", __func__);

	*num_keys = 0;
	*fans = NULL;

	fan_list = of_get_child_by_name(hwmon_node, "apple,fan-keys");
	if (!fan_list) {
		dev_info(dev, "Sensor node %s not found\n", "apple,fan-keys");
		return -EOPNOTSUPP;
	}

	*num_keys = of_get_available_child_count(fan_list);
	if (!num_keys) {
		of_node_put(fan_list);
		dev_err(dev, "No keys found in %s!\n",
			of_node_full_name(fan_list));
		return -EOPNOTSUPP;
	}

	*fans = devm_kzalloc(dev, sizeof(struct macsmc_hwmon_fan) * *num_keys,
			     GFP_KERNEL);
	if (!(*fans)) {
		of_node_put(fan_list);
		return -ENOMEM;
	}

	for_each_available_child_of_node(fan_list, fan_node) {
		struct macsmc_hwmon_fan *fan = (*fans) + i;
		const char *label;

		ret = macsmc_hwmon_parse_key(dev, smc, fan_node, "apple,key-id", &fan->input);
		if (ret < 0)
			continue;

		ret = of_property_read_string(fan_node, "apple,key-desc",  &label);
		if (ret) {
			fan->label[0] = (fan->input.key >> 24) & 0xFF;
			fan->label[1] = (fan->input.key >> 16) & 0xFF;
			fan->label[2] = (fan->input.key >> 8) & 0xFF;
			fan->label[3] = fan->input.key & 0xFF;
			fan->label[4] = '\0';
		} else
			strscpy(fan->label, label, sizeof(fan->label));

		fan->attributes = HWMON_F_INPUT | HWMON_F_LABEL;

		if (!macsmc_hwmon_parse_key(dev, smc, fan_node,
					    "apple,fan-minimum", &fan->min))
			fan->attributes |= HWMON_F_MIN;

		if (!macsmc_hwmon_parse_key(dev, smc, fan_node,
					    "apple,fan-maximum", &fan->max))
			fan->attributes |= HWMON_F_MAX;

		if (!macsmc_hwmon_parse_key(dev, smc, fan_node,
					    "apple,fan-target", &fan->target))
			fan->attributes |= HWMON_F_TARGET;

		i += 1;
	}
	of_node_put(fan_list);

	/*
	 * The SMC firmware interface is unstable and we may lose some keys from time to time.
	 * Handle this gracefully by only considering the number of *parsed* keys
	 */
	*num_keys = i;
	if (!num_keys) {
		dev_err(dev, "No valid keys found in %s\n",
			of_node_full_name(fan_list));
		return -EOPNOTSUPP;
	}

	return 0;
}

static int macsmc_hwmon_populate_sensors(struct device *dev,
					 struct apple_smc *smc,
					 struct device_node *hwmon_node,
					 struct macsmc_hwmon_sensor **sensors,
					 const char *sensor_node,
					 u32 *num_keys)
{
	struct device_node *sensors_node = NULL;
	struct device_node *key_node = NULL;
	const char *key, *label;
	int ret = 0;
	int i = 0;

	*num_keys = 0;
	*sensors = NULL;

	sensors_node = of_get_child_by_name(hwmon_node, sensor_node);
	if (!sensors_node) {
		dev_info(dev, "Sensor node %s not found\n", sensor_node);
		return -EOPNOTSUPP;
	}

	*num_keys = of_get_child_count(sensors_node);
	if (!num_keys) {
		of_node_put(sensors_node);
		dev_err(dev, "No keys found in %s!\n", sensor_node);
		return -EOPNOTSUPP;
	}

	*sensors = devm_kzalloc(dev, sizeof(struct macsmc_hwmon_sensor) * *num_keys,
				GFP_KERNEL);
	if (!(*sensors)) {
		of_node_put(sensors_node);
		return -ENOMEM;
	}

	for_each_child_of_node(sensors_node, key_node) {
		struct macsmc_hwmon_sensor *sensor = (*sensors) + i;

		ret = of_property_read_string(key_node, "apple,key-id", &key);
		if (ret) {
			dev_err(dev, "Could not find apple,key-id for node %d\n", i);
			continue;
		}

		ret = apple_smc_get_key_info(smc, _SMC_KEY(key), &sensor->input.info);
		if (ret) {
			dev_err(dev, "Failed to retrieve key info for %s\n", key);
			continue;
		}
		sensor->input.key = _SMC_KEY(key);

		ret = of_property_read_string(key_node, "apple,key-desc",
					      &label);
		if (ret)
			strscpy(sensor->label, key, sizeof(sensor->label));
		else
			strscpy(sensor->label, label, sizeof(sensor->label));

		i += 1;
	}

	/*
	 * The SMC firmware interface is unstable and we may lose some keys from time to time.
	 * Handle this gracefully by only considering the number of *parsed* keys
	 */
	*num_keys = i;
	if (!num_keys) {
		dev_err(dev, "No valid keys found in %s\n", sensor_node);
		return -EOPNOTSUPP;
	}

	of_node_put(sensors_node);
	return 0;
}

/*
 * Create a NULL-terminated array of u32 config flags for
 * each fan hwmon channel in the hwmon_channel_info struct.
 */
static void macsmc_hwmon_populate_fan_configs(u32 *configs,
					      struct macsmc_hwmon_fan *fans,
					      u32 num_fans)
{
	pr_err("%s: num:%u\n", __func__, num_fans);
	int idx = 0;

	for (idx = 0; idx < num_fans; idx += 1)
		configs[idx] = fans[idx].attributes;

	configs[idx + 1] = 0;
}

/*
 * Create a NULL-terminated array of u32 config flags for
 * each hwmon channel in the hwmon_channel_info struct.
 */
static void macsmc_hwmon_populate_configs(u32 *configs,
					u32 num_keys, u32 flags)
{
	int idx = 0;

	for (idx = 0; idx < num_keys; idx += 1)
		configs[idx] = flags;

	configs[idx + 1] = 0;
}

/*
 * Create hwmon_channel_info list from the data gathered from the device tree.
 *
 * We have allocated a contiguous region of memory at *info, which we will turn
 * into a list of pointers to hwmon_channel_info structs, the structs themselves,
 * and the array of config flags for each sensor. A hwmon_channel_info struct must
 * exist for the chip itself, and then each hwmon_sensor_types supported by the chip.
 * The config array is NULL-terminated.
 *
 * The list of pointers lives at the top of the memory region, with the structs and
 * their config arrays interleaved below that.
 */
static void macsmc_hwmon_populate_info(struct macsmc_hwmon *hwmon,
				      struct hwmon_channel_info **info, u32 n_chans)
{
	int i = 0;
	int j = 0; /* offset from config[0] */

	info[i] = (struct hwmon_channel_info *)(info + n_chans);
	info[i]->type = hwmon_chip;
	info[i]->config = (u32 *)(info[i] + 1);
	macsmc_hwmon_populate_configs((u32 *)info[i]->config, 1, HWMON_C_REGISTER_TZ);
	j = 2; /* number of configs for chip (1 + NULL) */

	if (hwmon->num_fan) {
		i += 1;
		/* Pointer to page below the last config array */
		info[i] = (struct hwmon_channel_info *)(info[i - 1]->config + j);
		info[i]->type = hwmon_fan;
		/* Pointer to page below hwmon_channel_info */
		info[i]->config = (u32 *)(info[i] + 1);
		/* Fill page at info[i]->config with channel flags */
		macsmc_hwmon_populate_fan_configs((u32 *)info[i]->config,
						  hwmon->fan, hwmon->num_fan);
		j = hwmon->num_pwr + 1;
	}

	if (hwmon->num_pwr) {
		i += 1;
		/* Pointer to page below the last config array */
		info[i] = (struct hwmon_channel_info *)(info[i - 1]->config + j);
		info[i]->type = hwmon_power;
		/* Pointer to page below hwmon_channel_info */
		info[i]->config = (u32 *)(info[i] + 1);
		/* Fill page at info[i]->config with channel flags */
		macsmc_hwmon_populate_configs((u32 *)info[i]->config, hwmon->num_pwr,
					(HWMON_P_INPUT | HWMON_P_LABEL));
		j = hwmon->num_pwr + 1;
	}

	if (hwmon->num_temp) {
		i += 1;
		info[i] = (struct hwmon_channel_info *)(info[i - 1]->config + j);
		info[i]->type = hwmon_temp;
		info[i]->config = (u32 *)(info[i] + 1);
		macsmc_hwmon_populate_configs((u32 *)info[i]->config, hwmon->num_temp,
					(HWMON_T_INPUT | HWMON_T_LABEL));
		j = hwmon->num_temp + 1;
	}

	if (hwmon->num_volt) {
		i += 1;
		info[i] = (struct hwmon_channel_info *)(info[i - 1]->config + j);
		info[i]->type = hwmon_in;
		info[i]->config = (u32 *)(info[i] + 1);
		macsmc_hwmon_populate_configs((u32 *)info[i]->config, hwmon->num_volt,
					(HWMON_I_INPUT | HWMON_I_LABEL));
		j = hwmon->num_volt + 1;
	}

	if (hwmon->num_curr) {
		i += 1;
		info[i] = (struct hwmon_channel_info *)(info[i - 1]->config + j);
		info[i]->type = hwmon_curr;
		info[i]->config = (u32 *)(info[i] + 1);
		macsmc_hwmon_populate_configs((u32 *)info[i]->config, hwmon->num_curr,
					(HWMON_C_INPUT | HWMON_C_LABEL));
		j = hwmon->num_curr + 1;
	}

	i += 1;
	info[i] = (struct hwmon_channel_info *)NULL;
}

static int macsmc_hwmon_probe(struct platform_device *pdev)
{
	struct apple_smc *smc = dev_get_drvdata(pdev->dev.parent);
	struct macsmc_hwmon *hwmon;
	struct device_node *hwmon_node;
	struct hwmon_channel_info **macsmc_chip_info = NULL;
	int ret = 0;
	u32 info_sz = 0;
	u32 n_chans = 0;

	hwmon = devm_kzalloc(&pdev->dev, sizeof(struct macsmc_hwmon), GFP_KERNEL);
	if (!hwmon)
		return -ENOMEM;

	hwmon->dev = &pdev->dev;
	hwmon->smc = smc;

	hwmon_node = of_find_node_by_name(NULL, "macsmc-hwmon");
	if (!hwmon_node) {
		dev_err(hwmon->dev, "macsmc-hwmon not found in devicetree!\n");
		return -ENODEV;
	}

	ret = macsmc_hwmon_populate_fans(hwmon->dev, hwmon->smc, hwmon_node,
					 &hwmon->fan, &hwmon->num_fan);
	if (ret)
		dev_info(hwmon->dev, "Could not populate fans!\n");

	ret = macsmc_hwmon_populate_sensors(hwmon->dev, hwmon->smc, hwmon_node,
					    &hwmon->pwr, "apple,pwr-keys",
					    &hwmon->num_pwr);
	if (ret)
		dev_info(hwmon->dev, "Could not populate power keys!\n");


	ret = macsmc_hwmon_populate_sensors(hwmon->dev, hwmon->smc, hwmon_node,
					    &hwmon->temp, "apple,temp-keys",
					    &hwmon->num_temp);
	if (ret)
		dev_info(hwmon->dev, "Could not populate temp keys!\n");


	ret = macsmc_hwmon_populate_sensors(hwmon->dev, hwmon->smc, hwmon_node,
					    &hwmon->volt, "apple,volt-keys",
					    &hwmon->num_volt);
	if (ret)
		dev_info(hwmon->dev, "Could not populate voltage keys!\n");


	ret = macsmc_hwmon_populate_sensors(hwmon->dev, hwmon->smc, hwmon_node,
					    &hwmon->curr, "apple,curr-keys",
					    &hwmon->num_curr);
	if (ret)
		dev_info(hwmon->dev, "Could not populate current keys!\n");


	of_node_put(hwmon_node);

	/*
	 * Allocate a contiguous region of memory to store information about
	 * each hwmon channel as determined from the devicetree.
	 *
	 * We need a pointer to the struct, the struct itself, and an array of
	 * config flags.
	 */
	/* Chip info */
	info_sz += (sizeof(struct hwmon_channel_info *) +
			sizeof(struct hwmon_channel_info) +
			sizeof(u32) * 2);
	n_chans += 1;

	if (hwmon->num_fan) {
		info_sz += (sizeof(struct hwmon_channel_info *) +
			    sizeof(struct hwmon_channel_info) +
			    (sizeof(u32) * (hwmon->num_pwr + 1)));
		n_chans += 1;
	}

	if (hwmon->num_pwr) {
		info_sz += (sizeof(struct hwmon_channel_info *) +
				sizeof(struct hwmon_channel_info) +
				(sizeof(u32) * (hwmon->num_pwr + 1)));
		n_chans += 1;
	}

	if (hwmon->num_temp) {
		info_sz += (sizeof(struct hwmon_channel_info *) +
				sizeof(struct hwmon_channel_info) +
				(sizeof(u32) * (hwmon->num_temp + 1)));
		n_chans += 1;
	}

	if (hwmon->num_volt) {
		info_sz += (sizeof(struct hwmon_channel_info *) +
				sizeof(struct hwmon_channel_info) +
				(sizeof(u32) * (hwmon->num_volt + 1)));
		n_chans += 1;
	}

	if (hwmon->num_curr) {
		info_sz += (sizeof(struct hwmon_channel_info *) +
				sizeof(struct hwmon_channel_info) +
				(sizeof(u32) * (hwmon->num_curr + 1)));
		n_chans += 1;
	}

	/* NULL termination */
	info_sz += (sizeof(struct hwmon_channel_info *));
	n_chans += 1;

	macsmc_chip_info = devm_kzalloc(hwmon->dev, info_sz, GFP_KERNEL);
	if (!macsmc_chip_info)
		return -ENOMEM;

	/* Build the chip info from the keys we have. */
	macsmc_hwmon_populate_info(hwmon, macsmc_chip_info, n_chans);

	macsmc_hwmon_info.info = (const struct hwmon_channel_info **)macsmc_chip_info;

	hwmon->hwmon_dev = devm_hwmon_device_register_with_info(&pdev->dev,
						"macsmc_hwmon", hwmon,
						&macsmc_hwmon_info, NULL);
	if (IS_ERR(hwmon->hwmon_dev))
		return dev_err_probe(hwmon->dev, PTR_ERR(hwmon->hwmon_dev),
				     "Probing SMC hwmon device failed!\n");

	dev_info(hwmon->dev, "Registered SMC hwmon device. Sensors:\n");
	dev_info(hwmon->dev,
		 "Fans: %d, Power: %d, Temperature: %d, Voltage: %d, Current: %d",
		 hwmon->num_fan, hwmon->num_pwr, hwmon->num_temp, hwmon->num_volt,
		 hwmon->num_curr);
	return 0;
}

static struct platform_driver macsmc_hwmon_driver = {
	.probe = macsmc_hwmon_probe,
	.driver = {
		.name = "macsmc-hwmon",
	},
};
module_platform_driver(macsmc_hwmon_driver);

MODULE_DESCRIPTION("Apple Silicon SMC hwmon driver");
MODULE_AUTHOR("James Calligeros <jcalligeros99@gmail.com");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_ALIAS("platform:macsmc-hwmon");
