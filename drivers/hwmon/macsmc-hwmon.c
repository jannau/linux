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

struct macsmc_hwmon_sensor {
	struct apple_smc_key_info *macsmc_key_info;
	smc_key macsmc_key;
	char label[MAX_LABEL_LENGTH];
};

struct macsmc_hwmon {
	struct device *dev;
	struct apple_smc *smc;
	struct device *hwmon_dev;
	struct macsmc_hwmon_sensor *pwr;
	struct macsmc_hwmon_sensor *temp;
	struct macsmc_hwmon_sensor *volt;
	struct macsmc_hwmon_sensor *curr;
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
	// u64 ui64 = 0;
	u32 flt32 = 0;
	// u16 ui16 = 0;
	// u8 ui8 = 0;
	// s64 si64 = 0;
	// s32 si32 = 0;
	// s16 si16 = 0;
	// s8 int8 = 0;
	int ret = 0;

	switch (type) {
	case hwmon_power:
		switch (hwmon->pwr[channel].macsmc_key_info->type_code) {
		case _SMC_KEY("flt "):
			ret = apple_smc_read_f32_scaled(hwmon->smc,
					hwmon->pwr[channel].macsmc_key,
					&flt32, 1000000);
			if (ret)
				return -EINVAL;
			*val = flt32;
			break;
		default:
			return -EOPNOTSUPP;
		}
		break;
	case hwmon_temp:
		switch (hwmon->temp[channel].macsmc_key_info->type_code) {
		case _SMC_KEY("flt "):
			ret = apple_smc_read_f32_scaled(hwmon->smc,
						hwmon->temp[channel].macsmc_key,
						&flt32, 1000);
			if (ret)
				return -EINVAL;
			*val = flt32;
			break;
		default:
			return -EOPNOTSUPP;
		}
		break;
	case hwmon_in:
		switch (hwmon->volt[channel].macsmc_key_info->type_code) {
		case _SMC_KEY("flt "):
			ret = apple_smc_read_f32_scaled(hwmon->smc,
							hwmon->volt[channel].macsmc_key,
							&flt32, 1000);
				if (ret)
					return -EINVAL;
				*val = flt32;
				break;
		default:
			return -EOPNOTSUPP;
		}
		break;
	case hwmon_curr:
		switch (hwmon->curr[channel].macsmc_key_info->type_code) {
		case _SMC_KEY("flt "):
			ret = apple_smc_read_f32_scaled(hwmon->smc,
							hwmon->curr[channel].macsmc_key,
							&flt32, 1);
			if (ret)
				return -EINVAL;
			*val = flt32;
			break;
		default:
			return -EOPNOTSUPP;
		}
		break;
	default:
		return -EOPNOTSUPP;
	}
	return ret;
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


static int macsmc_hwmon_populate_sensors(struct device *dev,
					 struct apple_smc *smc,
					 struct device_node *hwmon_node,
					 struct macsmc_hwmon_sensor **sensors,
					 const char *sensor_node,
					 u32 *num_keys)
{
	struct device_node *sensors_node = NULL;
	struct device_node *key_node = NULL;
	struct apple_smc_key_info *key_info = NULL;
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
		ret = of_property_read_string(key_node, "apple,key-id", &key);
		if (ret) {
			dev_err(dev, "Could not find apple,key-id for node %d\n", i);
			continue;
		}
		(*sensors)[i].macsmc_key = _SMC_KEY(key);

		key_info = devm_kzalloc(dev, sizeof(struct apple_smc_key_info), GFP_KERNEL);
		if (!key_info)
			continue;
		(*sensors)[i].macsmc_key_info = key_info;

		ret = apple_smc_get_key_info(smc, _SMC_KEY(key), (*sensors)[i].macsmc_key_info);
		if (ret) {
			dev_err(dev, "Failed to retrieve key info for %s\n", key);
			continue;
		}

		ret = of_property_read_string(key_node, "apple,key-desc",
					      &label);
		if (ret) {
			dev_err(dev, "Could not find apple,key-desc for node %d\n", i);
			continue;
		}
		strncpy((*sensors)[i].label, label, strlen(label));

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
	dev_info(hwmon->dev, "Power: %d, Temperature: %d, Voltage: %d, Current: %d",
		 hwmon->num_pwr, hwmon->num_temp, hwmon->num_volt, hwmon->num_curr);
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
