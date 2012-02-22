/*
 * LED driver
 *
 * Copyright (C) 2009 LGE Inc.
 *
 * ch.han@lge.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/semaphore.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/mfd/tps6586x.h>
#include <mach/gpio-names.h>
#include <mach/hardware.h>

#define LED_LIGHT_REMOVAL
#if defined (LED_LIGHT_REMOVAL)
#include "bh1721_als.h"

#define R_FACTOR	(7 + 3)
#define G_FACTOR	(36 + 17)
#define B_FACTOR	(22 + 11)
#define RGB_RESOL	(0xff + 0x7f)

#endif
#define OFF	0
#define GPIO_PRIVACY_LED	TEGRA_GPIO_PP0

#define LED_DEBUG_INFO

#ifdef LED_DEBUG_INFO
#define D(fmt, args...) printk(KERN_INFO fmt, ##args)
#else
#define D(fmt, args...) do {} while (0)
#endif

#if defined (LED_LIGHT_REMOVAL)
struct	semaphore star_led_sem;

static int rgb_to_brightness(int value)
{
	return ((R_FACTOR * ((value >> 16) & 0x00ff))
		+ (G_FACTOR * ((value >> 8) & 0x00ff)) +
		(B_FACTOR * (value & 0x00ff)))/RGB_RESOL;
}
#endif

static int rgb_to_leds(int value)
{
	int red, green, blue;

	red	= (value >> 16) & 0x00ff;
	green	= (value >> 8) & 0x00ff;
	blue	=  value & 0x00ff;

	if(!red && !green && blue) {		/* B	*/
		blue = 0x20;
	} else if(!red && green && !blue) {	/* G	*/
		green = 0x1a;
	} else if(!red && green && blue) {	/* GB	*/
		green = 0x10; blue = 0x10;
	} else if(red && !green && !blue) {	/* R	*/
		red = 0x6f;
	} else if(red && !green && blue) {	/* RB	*/
		red = 0x5f; blue =0x10;
	} else if(red && green && !blue) {	/* RG	*/
		red = 0x4f; green = 0x17;
	} else if(red && green && blue) {	/* RGB	*/
		red = 0x20; green = 0x10; blue = 0x10;
	}

	return (red << 16 | green << 8 | blue);
}


void led_blink (int ontime, int offtime)
{
	down_interruptible(&star_led_sem);
	if(ontime) {
		D("[LED][%s] period setting\n", __func__);
#if defined (LED_LIGHT_REMOVAL)
		if(offtime == 0)
			als_cmd(ALS_LED_BLINK, 0);
		else
			als_cmd(ALS_LED_BLINK, 1);
#endif
		tps6586x_set_led_onoff_time (ontime, offtime);
	}
	else {
#if defined (LED_LIGHT_REMOVAL)
		als_cmd(ALS_LED_BLINK, 0);
#endif
		D("[LED][%s] NO period setting\n", __func__);
		tps6586x_set_led_onoff_time (OFF, OFF);
	}
	up(&star_led_sem);
}

void led_rgb_brightness(int value)
{
	down_interruptible(&star_led_sem);

	value = rgb_to_leds(value);
	if (value) {
		D("[LED][%s] RGB Led On  (RGB:0x%x)!!!\n", __func__, value);

#if defined (LED_LIGHT_REMOVAL)
		als_cmd(ALS_LED_VALUE, rgb_to_brightness(value));
#endif
		tps6586x_set_led(value);
	}
	else {
		D("[LED][%s] RGB Off !!!\n", __func__);
		tps6586x_set_led(OFF);
#if defined (LED_LIGHT_REMOVAL)
		als_cmd(ALS_LED_VALUE, rgb_to_brightness(value));
#endif
	}
	up(&star_led_sem);
}

static ssize_t led_blink_show(struct device *dev,  struct device_attribute *attr,  char *buf)
{
	return sprintf(buf, "%d\n", 0);
}

static ssize_t led_blink_store(struct device *dev, struct device_attribute *attr,
                               const char *buf, size_t count)
{
	int on_time_ms=-1, off_time_ms=-1;
	int err;

	err = sscanf(buf, "%d %d", &on_time_ms, &off_time_ms);

	D("[LED][%s] blinking [%d-%d]\n", __func__, on_time_ms, off_time_ms);
	if (!err){
		printk(KERN_ERR "[Led] flashing error[%d] !!\n",err);
		return err;
	}

	if (on_time_ms < 0 || off_time_ms < 0)
		return -EINVAL;

	else
		led_blink (on_time_ms, off_time_ms);

	return count;
}


static ssize_t led_rgb_brightness_show(struct device *dev,  struct device_attribute *attr,  char *buf)
{
	return sprintf(buf, "%d\n", 0);
}

static ssize_t led_rgb_brightness_store(struct device *dev, struct device_attribute *attr,
                                        const char *buf, size_t count)
{
	unsigned long state;
	int err;

	D("[LED][%s] RGB On/Off: [0x%lx]\n", __func__, state);

	err = strict_strtoul(buf, 0, &state);
	if (err)
		return err;

	if(state)
	{
		if (state < 0)
			return -EINVAL;

		led_rgb_brightness(state);
	}
	else
	{
		led_rgb_brightness(0);
	}

	return err ?: count;
}

static DEVICE_ATTR(led_blink,0666,led_blink_show,led_blink_store);
static DEVICE_ATTR(led_rgb_brightness,0666,led_rgb_brightness_show,led_rgb_brightness_store);

static struct attribute *led_attributes[] = {
	&dev_attr_led_blink.attr,
	&dev_attr_led_rgb_brightness.attr,
	NULL
};

static const struct attribute_group led_group = {
	.attrs = led_attributes,
};

static int __init led_probe(struct platform_device *pdev)
{
	int err;

	struct device *dev = &pdev->dev;

	printk(KERN_INFO "[%s] start\n",__FUNCTION__);

	if (get_hw_rev() <= REV_E)
	{
		tegra_gpio_enable(GPIO_PRIVACY_LED);
		gpio_request_one(GPIO_PRIVACY_LED, GPIOF_OUT_INIT_LOW, "privacy_led");
	}

	if ((err = sysfs_create_group(&dev->kobj, &led_group)))
	{
		printk(KERN_INFO "[LGE led driver] sysfs_create_group FAIL \n");
		goto err_sysfs_create;
	}

	return 0;

err_sysfs_create:
	printk(KERN_ERR "[LGE led driver] led_device_register_failed\n");

	return err;
}

static int led_remove(struct platform_device *pdev)
{
	return 0;
}

static struct platform_driver led_driver = {
	.probe    = led_probe,
	.remove   =  led_remove,
	.driver =  {
		.name = "tegra_leds",
		.owner = THIS_MODULE,
	},
};
static int __init led_init(void)
{
	int err;
	printk(KERN_INFO "[%s]\n",__FUNCTION__);
	init_MUTEX(&star_led_sem);

	err = platform_driver_register(&led_driver);
	return 0;
}
static void __exit led_exit(void)
{
	printk(KERN_INFO "lge led_exit was unloaded!\nTwinkle Twinkle little star!:-) \n");
	return;
}

module_init(led_init);
module_exit(led_exit);

MODULE_AUTHOR("LG Electronics");
MODULE_DESCRIPTION("LED Driver for StarTablet");
MODULE_LICENSE("GPL");

