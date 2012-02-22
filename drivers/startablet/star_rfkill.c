/*
 * drivers/startablet/star_rfkill.c
 *
 * Star Primary rfkill client driver
 *
 * Copyright (c) 2010, LG Electronics Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/rfkill.h>
#include <linux/gpio.h>

#include <mach/gpio-names.h>

#include "star_rfkill.h"

#define BLUETOOTH_GPIO_ENABLE	TEGRA_GPIO_PQ2

#define DRIVER_NAME    "tegra_rfkill"
#define DRIVER_DESC    "LG Electronics rfkill"

#define GPIO_LABEL     "bluetooth_enable"
#define BLUETOOTH_NAME "tegra-bluetooth"

static struct rfkill *bt = NULL;

void rfkill_switch_all(enum rfkill_type type, bool blocked);
void msleep(unsigned int msecs);

static int bluetooth_set_power(void *data, enum rfkill_user_states state)
{
	switch (state) {
	case RFKILL_USER_STATE_UNBLOCKED:
		/* Pulse a reset */
		gpio_set_value(BLUETOOTH_GPIO_ENABLE, 0);

		/* Configure as output */
		//gpio_direction_output(BLUETOOTH_GPIO_ENABLE, 0);

		/* Give 5 milli seconds for the reset pulse */
		msleep(5);

		/* Enable power */
		gpio_set_value(BLUETOOTH_GPIO_ENABLE, 1);
		
		printk(KERN_INFO "Bluetooth power ON\n");
		break;

	case RFKILL_USER_STATE_SOFT_BLOCKED:
		/* Disable power */
		gpio_set_value(BLUETOOTH_GPIO_ENABLE, 0);

		/* Configure as output */
		//gpio_direction_output(BLUETOOTH_GPIO_ENABLE, 0);

		printk(KERN_INFO "Bluetooth power OFF\n");
		break;

	default:
		printk(KERN_ERR "Bad bluetooth rfkill state %d\n", state);
	}
	
	return 0;
	
}

static int tegra_rfkill_set(void *data, bool blocked)
{
	if(blocked){	
		bluetooth_set_power(NULL, RFKILL_USER_STATE_SOFT_BLOCKED);
	}else{
		bluetooth_set_power(NULL, RFKILL_USER_STATE_UNBLOCKED);
	}

	return 0;
}

static struct rfkill_ops tegra_rfkill_ops = {
	.set_block = tegra_rfkill_set,
};

static int __init tegra_rfkill_probe(struct platform_device *pdev)
{
	int rc, ret;

	printk(KERN_INFO "call tegra_rfkill_probe\n");	
	
	ret = gpio_request(BLUETOOTH_GPIO_ENABLE, GPIO_LABEL);
 	if (ret < 0) {
		printk(KERN_INFO "Fail to request BLUETOOTH_GPIO_ENABLE\n");
		return ret;
	}
    
      tegra_gpio_enable(BLUETOOTH_GPIO_ENABLE);
	
	/* Configure as output */
	gpio_direction_output(BLUETOOTH_GPIO_ENABLE, 0);	
	
	rfkill_switch_all(RFKILL_TYPE_BLUETOOTH, true);
	
	bluetooth_set_power(NULL, RFKILL_USER_STATE_SOFT_BLOCKED);

	bt = rfkill_alloc(BLUETOOTH_NAME, &pdev->dev, RFKILL_TYPE_BLUETOOTH, &tegra_rfkill_ops, NULL);
	if (!bt) {
		rc = -ENOMEM;
		goto fail;
	}

	bt->name = BLUETOOTH_NAME;
	bt->state = RFKILL_USER_STATE_SOFT_BLOCKED;
	bt->data = NULL;

	rc = rfkill_register(bt);
	if (rc)
		goto fail;

	return rc;

fail:
	if (bt)
		rfkill_destroy(bt);
	return rc;
}

static int __init tegra_rfkill_remove(struct platform_device *pdev)
{
	rfkill_switch_all(RFKILL_TYPE_BLUETOOTH, true);
	bluetooth_set_power(NULL, RFKILL_USER_STATE_SOFT_BLOCKED);

	if (bt) {
		rfkill_unregister(bt);
		rfkill_destroy(bt);
	}

	gpio_free(BLUETOOTH_GPIO_ENABLE);
	return 0;
}

static struct platform_driver tegra_rfkill_driver = {
	.probe	= tegra_rfkill_probe,
	.remove	= tegra_rfkill_remove,
	.driver	= {
		.name	= DRIVER_NAME,
		.owner	= THIS_MODULE,
	},
};

static int __init tegra_rfkill_init(void)
{
	return platform_driver_register(&tegra_rfkill_driver);
}

module_init(tegra_rfkill_init);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");
