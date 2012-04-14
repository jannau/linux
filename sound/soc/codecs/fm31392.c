/*
 * FM31-392 Echo Canceller Driver
 *
 **Copyright (C) 2010 LGE Inc.
 * Copyright (C) Janne Grunau, <j@jannau.net>
 *
 **This program is free software; you can redistribute if and/or modify
 **it under the terms of the GNU General Public License version 2 as
 **published by the Free Software Foundation.
 *
 */


#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/i2c.h>
#include <linux/gpio.h>
#include <mach/gpio-names.h>
#include <linux/wakelock.h>
#include <sound/soc.h>
#include <sound/fm31392.h>

#define ECHO_SYNC_WORD                  0xFCF3
#define ECHO_MEM_WRITE                  0x3B
#define ECHO_MEM_READ                   0x37
#define ECHO_REG_READ                   0x60
#define READ_LOW_BYTE                   0x25
#define READ_HIGH_BYTE                  0x26

enum echo_cancel_mode {
	headset,
	voip,
};

struct fm31392 {
	struct device *dev;
	struct i2c_client *i2c;
	struct wake_lock wlock;
	struct clk *clk;

	int bypass;
	enum echo_cancel_mode mode;

	int gpio_power;
	int gpio_reset;
	int gpio_bypass;
};

unsigned char rd_bytes[2][4] = {
	{ (ECHO_SYNC_WORD >> 8) & 0xFF, (ECHO_SYNC_WORD >> 0) & 0xFF, ECHO_REG_READ,
	  READ_LOW_BYTE	 },
	{ (ECHO_SYNC_WORD >> 8) & 0xFF, (ECHO_SYNC_WORD >> 0) & 0xFF, ECHO_REG_READ,
	  READ_HIGH_BYTE }
};

static int echo_write_register(struct fm31392 *echo, int reg, int value)
{
	unsigned char arr[7];
	int addr = echo->i2c->addr;
	struct i2c_msg msg[] = {
		{ .addr = addr, .flags = 0, .buf = &arr[0], .len = 1 },
		{ .addr = addr, .flags = 0, .buf = &arr[1], .len = 1 },
		{ .addr = addr, .flags = 0, .buf = &arr[2], .len = 1 },
		{ .addr = addr, .flags = 0, .buf = &arr[3], .len = 1 },
		{ .addr = addr, .flags = 0, .buf = &arr[4], .len = 1 },
		{ .addr = addr, .flags = 0, .buf = &arr[5], .len = 1 },
		{ .addr = addr, .flags = 0, .buf = &arr[6], .len = 1 },
	};

	arr[0] = (ECHO_SYNC_WORD >> 8) & 0xFF;
	arr[1] = (ECHO_SYNC_WORD >> 0) & 0xFF;
	arr[2] = ECHO_MEM_WRITE;
	arr[3] = (reg >> 8) & 0xff;
	arr[4] = (reg >> 0) & 0xff;
	arr[5] = (value >> 8) & 0xff;
	arr[6] = (value >> 0) & 0xff;
/*
 *      if (i2c_transfer(echo->i2c->adapter, msg, 7) != 7)
 *      {
 *              dev_err(&echo->i2c->dev, "i2c write error\n");
 *              return -EIO;
 *      }
 */
	if (i2c_transfer(echo->i2c->adapter, &msg[0], 1) != 1) return -EIO;
	if (i2c_transfer(echo->i2c->adapter, &msg[1], 1) != 1) return -EIO;
	if (i2c_transfer(echo->i2c->adapter, &msg[2], 1) != 1) return -EIO;
	if (i2c_transfer(echo->i2c->adapter, &msg[3], 1) != 1) return -EIO;
	if (i2c_transfer(echo->i2c->adapter, &msg[4], 1) != 1) return -EIO;
	if (i2c_transfer(echo->i2c->adapter, &msg[5], 1) != 1) return -EIO;
	if (i2c_transfer(echo->i2c->adapter, &msg[6], 1) != 1) return -EIO;

	return 0;
}

static int echo_read_register(struct fm31392 *echo, int reg)
{
	int ret_lo = 0, ret_hi = 0, value = 0;
	int addr = echo->i2c->addr;
	unsigned char w_buf[5], r_buf[2];
	struct i2c_msg msg[] = {
		{ .addr = addr, .flags = 0,	.buf = &w_buf[0],	.len = 1 },
		{ .addr = addr, .flags = 0,	.buf = &w_buf[1],	.len = 1 },
		{ .addr = addr, .flags = 0,	.buf = &w_buf[2],	.len = 1 },
		{ .addr = addr, .flags = 0,	.buf = &w_buf[3],	.len = 1 },
		{ .addr = addr, .flags = 0,	.buf = &w_buf[4],	.len = 1 },
		{ .addr = addr, .flags = 0,	.buf = &rd_bytes[0][0], .len = 1 },
		{ .addr = addr, .flags = 0,	.buf = &rd_bytes[0][1], .len = 1 },
		{ .addr = addr, .flags = 0,	.buf = &rd_bytes[0][2], .len = 1 },
		{ .addr = addr, .flags = 0,	.buf = &rd_bytes[0][3], .len = 1 },
		{ .addr = addr, .flags = I2C_M_RD, .buf = &r_buf[0],	.len = 1 },
		{ .addr = addr, .flags = 0,	.buf = &rd_bytes[1][0], .len = 1 },
		{ .addr = addr, .flags = 0,	.buf = &rd_bytes[1][1], .len = 1 },
		{ .addr = addr, .flags = 0,	.buf = &rd_bytes[1][2], .len = 1 },
		{ .addr = addr, .flags = 0,	.buf = &rd_bytes[1][3], .len = 1 },
		{ .addr = addr, .flags = I2C_M_RD, .buf = &r_buf[1],	.len = 1 },
	};

	w_buf[0] = (ECHO_SYNC_WORD >> 8) & 0xFF;
	w_buf[1] = (ECHO_SYNC_WORD >> 0) & 0xFF;
	w_buf[2] = ECHO_MEM_READ;
	w_buf[3] = (reg >> 8) & 0xFF;
	w_buf[4] = (reg >> 0) & 0xFF;
/*
 *      if (i2c_transfer(echo->i2c->adapter, msg, 15) != 15)
 *      {
 *              dev_err(&echo->i2c->dev, "i2c read error\n");
 *              return -EIO;
 *      }
 */

	if (i2c_transfer(echo->i2c->adapter, &msg[0], 1) != 1) return -EIO;
	if (i2c_transfer(echo->i2c->adapter, &msg[1], 1) != 1) return -EIO;
	if (i2c_transfer(echo->i2c->adapter, &msg[2], 1) != 1) return -EIO;
	if (i2c_transfer(echo->i2c->adapter, &msg[3], 1) != 1) return -EIO;
	if (i2c_transfer(echo->i2c->adapter, &msg[4], 1) != 1) return -EIO;
	if (i2c_transfer(echo->i2c->adapter, &msg[5], 1) != 1) return -EIO;
	if (i2c_transfer(echo->i2c->adapter, &msg[6], 1) != 1) return -EIO;
	if (i2c_transfer(echo->i2c->adapter, &msg[7], 1) != 1) return -EIO;
	if (i2c_transfer(echo->i2c->adapter, &msg[8], 1) != 1) return -EIO;
	if (i2c_transfer(echo->i2c->adapter, &msg[9], 1) != 1) return -EIO;
	if (i2c_transfer(echo->i2c->adapter, &msg[10], 1) != 1) return -EIO;
	if (i2c_transfer(echo->i2c->adapter, &msg[11], 1) != 1) return -EIO;
	if (i2c_transfer(echo->i2c->adapter, &msg[12], 1) != 1) return -EIO;
	if (i2c_transfer(echo->i2c->adapter, &msg[13], 1) != 1) return -EIO;
	if (i2c_transfer(echo->i2c->adapter, &msg[14], 1) != 1) return -EIO;

	ret_lo = r_buf[0];
	ret_hi = r_buf[1];
	value = (ret_hi << 8) | ret_lo;

	return value;
}

static ssize_t fm31_status(struct fm31392 *echo)
{
	clk_enable(echo->clk);

	printk("0x23f8 = 0x%x \n", echo_read_register(echo, 0x23f8));
	printk("0x232c = 0x%x \n", echo_read_register(echo, 0x232C));
	printk("0x2304 = 0x%x \n", echo_read_register(echo, 0x2304));
	printk("0x2308 = 0x%x \n", echo_read_register(echo, 0x2308));
	printk("0x230b = 0x%x \n", echo_read_register(echo, 0x230B));
	printk("0x230d = 0x%x \n", echo_read_register(echo, 0x230D));
	printk("0x230e = 0x%x \n", echo_read_register(echo, 0x230E));
	printk("0x2311 = 0x%x \n", echo_read_register(echo, 0x2311));
	printk("0x2312 = 0x%x \n", echo_read_register(echo, 0x2312));
	printk("0x2315 = 0x%x \n", echo_read_register(echo, 0x2315));
	printk("0x2316 = 0x%x \n", echo_read_register(echo, 0x2316));
	printk("0x231a = 0x%x \n", echo_read_register(echo, 0x231A));
	printk("0x231d = 0x%x \n", echo_read_register(echo, 0x231D));
	printk("0x2352 = 0x%x \n", echo_read_register(echo, 0x2352));
	printk("0x2392 = 0x%x \n", echo_read_register(echo, 0x2392));
	printk("0x230c = 0x%x \n", echo_read_register(echo, 0x230C));

	clk_disable(echo->clk);

	return 0;
}

static int fm31_check_status(struct fm31392 *echo)
{
	int rval, rval1, rval2, ret = 0;
	int rw_count = 0;

	rval  = echo_read_register(echo, 0x230C);
	rval1 = echo_read_register(echo, 0x2300);
	rval2 = echo_read_register(echo, 0x3FE4);

	if ((rval != 0x5A5A) || (rval1 != 0x0004) || (rval2 != 0x129E)) {
		dev_warn(echo->dev,
			 "%s: 0x230C = %x, 0x2300 = %x, 0x3FE4 = %x\n",
			 __func__, rval, rval1, rval2);

		while (rw_count < 10) {
			echo_write_register(echo, 0x3FE4, 0x361E);
			msleep(50);
			rval = echo_read_register(echo, 0x3FE4);

			if (rval == 0x361E) {
				dev_info(echo->dev,
					 "0x3FE4 rewrite %d times and success.\n",
					 rw_count);
				return ret;
			} else {
				dev_err(echo->dev,
					"0x3FE4 rewrite Fail, Return value is not 0x361E, Return value =  %x\n",
					rval);
			}
			rw_count++;
		}
		ret = -1;
	}

	return ret;
}

struct fm31392_parameters {
	int reg;
	int value;
};

struct fm31392_parameters bypass_parameters[] = {
	{ 0x2308, 0x005F },
	{ 0x232C, 0x0025 },
	{ 0x2300, 0x0004 },
	{ 0x2302, 0x0024 },
	{ 0x23F7, 0x001E },
	{ 0x230C, 0x0000 },
	{ 0,      0      }
};

struct fm31392_parameters voip_parameters[] = {
	{ 0x2300, 0x0000 },
	{ 0x2302, 0x0024 },
	{ 0x23F7, 0x0048 },
	{ 0x22C0, 0x8101 },
	{ 0x22C1, 0xF9C2 },
	{ 0x22C2, 0xFCE1 },
	{ 0x22C3, 0xB514 },
	{ 0x22C4, 0xDA8A },
	{ 0x2308, 0x005F },
	{ 0x230B, 0x0001 },
	{ 0x230D, 0x0380 },
	{ 0x230E, 0x0400 },
	{ 0x2311, 0x0101 },
	{ 0x2312, 0x6981 },
	{ 0x2315, 0x03DD },
	{ 0x2316, 0x0044 },
	{ 0x2317, 0x1000 },
	{ 0x231D, 0x0200 },
	{ 0x232C, 0x0025 },
	{ 0x233A, 0x7000 },
	{ 0x233B, 0x0280 },
	{ 0x2351, 0x4000 },
	{ 0x2352, 0x2000 },
	{ 0x2391, 0x000D },
	{ 0x2392, 0x0400 },
	{ 0x23E0, 0x0000 },
	{ 0x23F8, 0x4003 },
	{ 0x2336, 0x0001 },
	{ 0x2301, 0x0002 },
	{ 0x2353, 0x2000 },
	{ 0x2318, 0x0400 },
	{ 0x2319, 0x0400 },
	{ 0x231C, 0x2000 },
	{ 0x23D9, 0x0C7D },
	{ 0x23DA, 0x544A },
	{ 0x23DB, 0x7FFF },
	{ 0x23DC, 0x7FFF },
	{ 0x2344, 0x0A00 },
	{ 0x2325, 0x7FFF },
	{ 0x23A9, 0x0800 },
	{ 0x23B0, 0x3000 },
	{ 0x231B, 0x0001 },
	{ 0x2335, 0x000F },
	{ 0x23FC, 0x000D },
	{ 0x23CD, 0x4000 },
	{ 0x23CE, 0x4000 },
	{ 0x230C, 0x0000 },
	{ 0,      0      }
};

struct fm31392_parameters headset_parameters[] = {
	{ 0x2300, 0x0000 },
	{ 0x2302, 0x0024 },
	{ 0x23F7, 0x0048 },
	{ 0x22C0, 0x8101 },
	{ 0x22C1, 0x0000 },
	{ 0x22C2, 0x0000 },
	{ 0x22C3, 0x0000 },
	{ 0x22C4, 0x0000 },
	{ 0x2308, 0x005F },
	{ 0x230B, 0x0001 },
	{ 0x230D, 0x0400 },
	{ 0x230E, 0x0100 },
	{ 0x2311, 0x0101 },
	{ 0x2315, 0x03DD },
	{ 0x2316, 0x0045 },
	{ 0x2317, 0x1000 },
	{ 0x231D, 0x0200 },
	{ 0x232C, 0x0025 },
	{ 0x233A, 0x0100 },
	{ 0x233B, 0x0004 },
	{ 0x2351, 0x2000 },
	{ 0x2352, 0x2000 },
	{ 0x2391, 0x000D },
	{ 0x2392, 0x0400 },
	{ 0x23E0, 0x0000 },
	{ 0x23F8, 0x4003 },
	{ 0x2336, 0x0001 },
	{ 0x2301, 0x0002 },
	{ 0x2353, 0x2000 },
	{ 0x2318, 0x0400 },
	{ 0x2319, 0x0400 },
	{ 0x231C, 0x2000 },
	{ 0x23D9, 0x0C7D },
	{ 0x23DA, 0x544A },
	{ 0x23DB, 0x7FFF },
	{ 0x23DC, 0x7FFF },
	{ 0x2344, 0x0CCC },
	{ 0x2325, 0x7FFF },
	{ 0x23A9, 0x0800 },
	{ 0x23B0, 0x3000 },
	{ 0x231B, 0x0001 },
	{ 0x2335, 0x000F },
	{ 0x23FC, 0x000D },
	{ 0x23CD, 0x4000 },
	{ 0x23CE, 0x4000 },
	{ 0x2312, 0x6881 },
	{ 0x230C, 0x0000 },
	{ 0,      0      }
};

static int fm31392_write_parameters (struct fm31392 *fm31392,
				     struct fm31392_parameters *param)
{
	int ret = 0;

	clk_enable(fm31392->clk);

	gpio_set_value(fm31392->gpio_power, 1);
	gpio_set_value(fm31392->gpio_reset, 0);

	msleep(3);

	gpio_set_value(fm31392->gpio_reset, 1);

	msleep(10);

	while (param->reg) {
		ret = echo_write_register(fm31392, param->reg, param->value);
		param++;
		if (ret)
			goto err_clk;
	}

	msleep(50);

	if (fm31_check_status(fm31392) < 0) {
		dev_err(fm31392->dev, "FM31 DSP running Fail\n");
		ret = -EIO;
	}

err_clk:
	clk_disable(fm31392->clk);

	return ret;
}

static int fm31392_set_mode(struct fm31392 *fm31392)
{
	int ret = 0;

	if (fm31392->mode == voip)
		ret = fm31392_write_parameters(fm31392, voip_parameters);
	else if (fm31392->mode == headset)
		ret = fm31392_write_parameters(fm31392, headset_parameters);

	gpio_set_value(fm31392->gpio_bypass, 0);

	return ret;
}

static int fm31392_set_bypass(struct fm31392 *fm31392)
{
	int ret = 0;

	gpio_set_value(fm31392->gpio_bypass, fm31392->bypass);

	if (fm31392->bypass)
		ret = fm31392_write_parameters(fm31392, bypass_parameters);

#ifdef DEBUG
	pr_info("%s: bypass: %d\n", __func__, fm31392->bypass);
	fm31_status(fm31392);
#endif

	return ret;
}

static int fm31_get_bypass(struct snd_kcontrol *kcontrol,
			   struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	struct fm31392 *fm31392 = snd_soc_codec_get_drvdata(codec);

	ucontrol->value.enumerated.item[0] = fm31392->bypass;

	return 0;
}

static int fm31_set_bypass(struct snd_kcontrol *kcontrol,
			   struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	struct fm31392 *fm31392 = snd_soc_codec_get_drvdata(codec);

	/* if (fm31392->bypass == ucontrol->value.enumerated.item[0]) */
	/* 	return 0; */
	fm31392->bypass = ucontrol->value.enumerated.item[0];

	return fm31392_set_bypass(fm31392);
}

static int fm31_get_mode(struct snd_kcontrol *kcontrol,
			 struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	struct fm31392 *fm31392 = snd_soc_codec_get_drvdata(codec);

	ucontrol->value.integer.value[0] = fm31392->mode;

	return 0;
}

static int fm31_set_mode(struct snd_kcontrol *kcontrol,
			 struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	struct fm31392 *fm31392 = snd_soc_codec_get_drvdata(codec);

	if (ucontrol->value.integer.value[0] == fm31392->mode)
		return 0;

	fm31392->mode = ucontrol->value.integer.value[0];

	return fm31392_set_mode(fm31392);
}


static const char *fm31_mode_desc[] = {"Headset", "VoIP"};

static const struct soc_enum fm31_mode_enum =
	SOC_ENUM_SINGLE_EXT(2, fm31_mode_desc);

static const struct snd_kcontrol_new fm31392_controls[] = {
	SOC_SINGLE_BOOL_EXT("Echo Cancel Bypass", 1,
			    fm31_get_bypass, fm31_set_bypass),
	SOC_ENUM_EXT("Echo Cancel Mode", fm31_mode_enum,
		     fm31_get_mode, fm31_set_mode),
};

static int fm31392_codec_probe(struct snd_soc_codec *codec)
{
	struct fm31392 *fm31392 = snd_soc_codec_get_drvdata(codec);
	struct snd_soc_dapm_context *dapm = &codec->dapm;
	int ret;

	codec->control_data = fm31392->i2c;

	ret = snd_soc_add_controls(codec, fm31392_controls,
				   ARRAY_SIZE(fm31392_controls));
	if (ret)
		return ret;

	/* ret = snd_soc_dapm_new_controls(dapm, fm31392_dapm_widgets, */
	/* 		ARRAY_SIZE(fm31392_dapm_widgets)); */
	/* if (ret) */
	/* 	return ret; */

	/* ret = snd_soc_dapm_add_routes(dapm, fm31392_routes, */
	/* 		ARRAY_SIZE(fm31392_routes)); */
	/* if (ret) */
	/* 	return ret; */

	snd_soc_dapm_new_widgets(dapm);

	return 0;
}

static int fm31_codec_suspend(struct snd_soc_codec *codec,
			      pm_message_t state)
{
	struct fm31392 *fm31392 = snd_soc_codec_get_drvdata(codec);

	dev_info(fm31392->dev, "%s\n", __func__);

	return 0;
}

static int fm31_codec_resume(struct snd_soc_codec *codec)
{
	struct fm31392 *fm31392 = snd_soc_codec_get_drvdata(codec);

	dev_info(fm31392->dev, "%s\n", __func__);

	return 0;
}

static struct snd_soc_codec_driver soc_codec_dev_fm31392 = {
	/* .write	= fm31392_write, */
	/* .read	= lm4857_read, */
	.suspend	= fm31_codec_suspend,
	.resume		= fm31_codec_resume,
	.probe	= fm31392_codec_probe,
	/* .reg_cache_size		= ARRAY_SIZE(lm4857_default_regs), */
	/* .reg_word_size		= sizeof(uint8_t), */
	/* .reg_cache_default	= lm4857_default_regs, */
};

static int __init fm31392_probe(struct i2c_client *client,
				const struct i2c_device_id *id)
{
	int ret = 0;
	struct fm31392_platform_data *pdata;

	struct fm31392 *fm31392 = kzalloc(sizeof(*fm31392), GFP_KERNEL);

	if (!fm31392)
		return -ENOMEM;

	wake_lock_init(&fm31392->wlock, WAKE_LOCK_SUSPEND, "AudioOutLock");

	fm31392->i2c = client;
	i2c_set_clientdata(client, fm31392);

	fm31392->dev = &client->dev;
	dev_set_drvdata(fm31392->dev, fm31392);

	pdata = fm31392->dev->platform_data;

	fm31392->clk = clk_get_sys(NULL, "cdev1");
	if (IS_ERR(fm31392->clk)) {
		ret = PTR_ERR(fm31392->clk);
		dev_err(fm31392->dev, "Can't retrieve clk cdev1\n");
		goto err_free;
	}
	clk_enable(fm31392->clk);

	fm31392->gpio_power = pdata->gpio_power;
	gpio_request(fm31392->gpio_power, "fm31392_power");
	tegra_gpio_enable(fm31392->gpio_power);
	gpio_direction_output(fm31392->gpio_power, 1);

	fm31392->gpio_reset = pdata->gpio_reset;
	gpio_request(fm31392->gpio_reset, "fm31392_reset");
	tegra_gpio_enable(fm31392->gpio_reset);
	gpio_direction_output(fm31392->gpio_reset, 1);

	fm31392->gpio_bypass = pdata->gpio_bypass;
	gpio_request(fm31392->gpio_bypass, "fm31392_bypass");
	tegra_gpio_enable(fm31392->gpio_bypass);
	gpio_direction_output(fm31392->gpio_bypass, 1);

	fm31392->bypass = 1;
	fm31392_write_parameters(fm31392, bypass_parameters);

	ret = snd_soc_register_codec(&fm31392->i2c->dev, &soc_codec_dev_fm31392,
				     NULL, 0);
	if (ret) {
		dev_err(fm31392->dev, "Could not register codec\n");
		goto err_clk;
	}

	clk_disable(fm31392->clk);

	return 0;
err_clk:
	clk_disable(fm31392->clk);
	clk_put(fm31392->clk);
err_free:
	kfree(fm31392);
	return ret;
}

static int fm31392_remove(struct i2c_client *client)
{
	struct fm31392 *fm31392 = i2c_get_clientdata(client);
	wake_lock_destroy(&fm31392->wlock);
	clk_put(fm31392->clk);
	kfree(fm31392);
	return 0;
}

static int fm31392_suspend(struct device *dev)
{
	struct fm31392 *fm31392 = dev_get_drvdata(dev);

	gpio_set_value(fm31392->gpio_power, 0);

	return 0;
}

static int fm31392_resume(struct device *dev)
{
	struct fm31392 *fm31392 = dev_get_drvdata(dev);

	gpio_set_value(fm31392->gpio_power, 1);

	return 0;
}

static const struct i2c_device_id fm31392_i2c_id[] = {
	{ "fm31392", 0 },
	{ },
};

static struct i2c_driver fm31392_driver = {
	.probe		= fm31392_probe,
	.remove		= __devexit_p(fm31392_remove),
	.id_table	= fm31392_i2c_id,
	.driver		= {
		.name	= "fm31392",
		.owner	= THIS_MODULE,
	},
};


static int __devinit fm31392_init(void)
{
	return i2c_add_driver(&fm31392_driver);
}
module_init(fm31392_init);

static void __exit fm31392_exit(void)
{
	i2c_del_driver(&fm31392_driver);
}
module_exit(fm31392_exit);

MODULE_DESCRIPTION("FM31-392 Echo Canceller Driver");
MODULE_LICENSE("GPL");
