/*
 * linux/drivers/video/backlight/tegra_lm1_bl.c
 *
 * Tegra LM1 pin based backlight driver
 *
 * Copyright (C) 2010 NVIDIA Corporation
 * Author: Renuka Apte <rapte@nvidia.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/fb.h>
#include <linux/backlight.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/delay.h>

#include <mach/dc.h>
#include <mach/tegra_lm1_bl.h>
#include <mach/gpio-names.h>
#include <mach/hardware.h>

struct tegra_lm1_bl_data {
	struct device		*dev;
	unsigned int		period;
	unsigned int		clk_div;
	unsigned int		clk_select;
};

/* checking previus brighness to enter power control routine */
static int s_old_brightness = 0;

static int tegra_lm1_backlight_update_status(struct backlight_device *bl)
{
	struct tegra_lm1_bl_data *tbl = dev_get_drvdata(&bl->dev);
	int brightness = bl->props.brightness;
	int max = bl->props.max_brightness;
	struct tegra_dc *dc;

	//printk("calling %s brightness = %d !\n", __func__, brightness);
	//printk("bl->props.power = %d  bl->props.fb_blank = %d !\n", bl->props.power, bl->props.fb_blank);
	/* WR for fast operation in resume*/
	if(!(brightness != 0 && s_old_brightness == 0)) {
		if (bl->props.power != FB_BLANK_UNBLANK)
			brightness = 0;

		if (bl->props.fb_blank != FB_BLANK_UNBLANK)
			brightness = 0;
	}

	if(brightness > max) {
		dev_err(&bl->dev, "Invalid brightness value\n");
	}

	/* Call tegra display controller function to update backlight */

	s_old_brightness = brightness;

	dc = tegra_dc_get_dc(0);
	if(dc) {
		tegra_dc_config_bl(dc, LM1, tbl->period, tbl->clk_div, tbl->clk_select);
		tegra_dc_update_bl(dc, brightness);
	}
	else {
		dev_err(&bl->dev, "tegra display controller not available\n");
	}

	return 0;
}

static int tegra_lm1_backlight_get_brightness(struct backlight_device *bl)
{
	return bl->props.brightness;
}

static const struct backlight_ops tegra_lm1_backlight_ops = {
	.update_status	= tegra_lm1_backlight_update_status,
	.get_brightness	= tegra_lm1_backlight_get_brightness,
};

static int tegra_lm1_backlight_probe(struct platform_device *pdev)
{
	struct backlight_properties props;
	struct platform_tegra_lm1_backlight_data *data = pdev->dev.platform_data;
	struct backlight_device *bl;
	struct tegra_lm1_bl_data *tbl;
	int ret;

	if (!data) {
		dev_err(&pdev->dev, "failed to find platform data\n");
		return -EINVAL;
	}

	tbl = kzalloc(sizeof(*tbl), GFP_KERNEL);
	if (!tbl) {
		dev_err(&pdev->dev, "no memory for state\n");
		ret = -ENOMEM;
		goto err_alloc;
	}

	tbl->dev = &pdev->dev;
	tbl->period = data->period;
	tbl->clk_div = data->clk_div;
	tbl->clk_select = data->clk_select;

	memset(&props, 0, sizeof(struct backlight_properties));
	props.max_brightness = data->max_brightness;
	bl = backlight_device_register(dev_name(&pdev->dev), &pdev->dev, tbl,
			&tegra_lm1_backlight_ops, &props);
	if (IS_ERR(bl)) {
		dev_err(&pdev->dev, "failed to register backlight\n");
		ret = PTR_ERR(bl);
		goto err_bl;
	}

	bl->props.brightness = data->dft_brightness;
	tegra_lm1_backlight_update_status(bl);
	platform_set_drvdata(pdev, bl);
	return 0;

err_bl:
	kfree(tbl);
err_alloc:
	return ret;
}

static int tegra_lm1_backlight_remove(struct platform_device *pdev)
{
	struct backlight_device *bl = platform_get_drvdata(pdev);
	struct tegra_lm1_bl_data *tbl = dev_get_drvdata(&bl->dev);

	backlight_device_unregister(bl);
	kfree(tbl);
	return 0;
}

static struct platform_driver tegra_lm1_backlight_driver = {
	.driver		= {
		.name	= "tegralm1bl",
		.owner	= THIS_MODULE,
	},
	.probe		= tegra_lm1_backlight_probe,
	.remove		= tegra_lm1_backlight_remove,
};

static int __init tegra_lm1_backlight_init(void)
{
	return platform_driver_register(&tegra_lm1_backlight_driver);
}
/* This function should be called after dc driver is initialized. */
device_initcall_sync(tegra_lm1_backlight_init);

static void __exit tegra_lm1_backlight_exit(void)
{
	platform_driver_unregister(&tegra_lm1_backlight_driver);
}
module_exit(tegra_lm1_backlight_exit);

MODULE_DESCRIPTION("Tegra LM1 based Backlight Driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:tegralm1-backlight");

