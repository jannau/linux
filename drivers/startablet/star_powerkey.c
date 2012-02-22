/*
 * star(lgp990) power key
 *
 * Copyright (C) 2009 LGE, Inc.
 *
 * Author: Changsu Ha <cs77.ha@lge.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

/**
	@brief		 star(lgp990) power key

	@author		 cs77.ha@lge.com
	@date		 2010-04-13

	@version	 V1.00		 2010.04.13		 Changsu Ha	 Create
	@version	 V1.10		 2010.06.17		 Jinwoo Nam	 Porting to Linux native driver
*/

#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#include <linux/input.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/regulator/consumer.h>

#include <mach/gpio-names.h>

#define GPIO_POWERKEY       TEGRA_GPIO_PV2
#define POWERKEY_ACTIVE_LOW 0

typedef struct PowerKeyDeviceRec
{
    struct input_dev    *inputDev;
    struct work_struct  work;
    struct task_struct  *task;
} PowerKeyDevice;

static PowerKeyDevice s_powerkey;
static int key_pressed = 0;

extern int battery_get_charge_mode(void);

static void powerkey_handle(struct work_struct *wq)
{
    uint pinValue;

    // WBT 196168
    if(!s_powerkey.inputDev)
    {
        printk("POWERKEY handler error\n");
        return;
    }

    pinValue = (gpio_get_value(GPIO_POWERKEY) ? 1 : 0) ^ POWERKEY_ACTIVE_LOW;

    if(pinValue){
        printk("[PWR_KEY] POWERKEY press\n");
        input_report_key(s_powerkey.inputDev, KEY_POWER, 1);
        input_sync(s_powerkey.inputDev);
        key_pressed = 1;
    }else{
        if (0 == key_pressed)
        {
#ifdef CONFIG_STARTABLET_BATTERY
            // During at test, power key release (w/o press) occurs. Ignore it.
            if (battery_get_charge_mode())
            {
                printk("[PWR_KEY] at charge mode\n");
                return;
            }
            else
#endif
            {
                printk("[PWR_KEY] POWERKEY press-2\n");
                input_report_key(s_powerkey.inputDev, KEY_POWER, 1);
                input_sync(s_powerkey.inputDev);
                key_pressed = 1;
            }
        }

        printk("[PWR_KEY] POWERKEY release\n");
        input_report_key(s_powerkey.inputDev, KEY_POWER, 0);
        input_sync(s_powerkey.inputDev);
        key_pressed = 0;
    }
}

static irqreturn_t powerkey_interrupt_handler(int irq, void *dev_id)
{
    PowerKeyDevice *powerKeyDevice = (PowerKeyDevice *)dev_id;

    schedule_work(&powerKeyDevice->work);

	return IRQ_HANDLED;
}

typedef struct {
    char* regul_name;
    int voltage_mV;
} regul_info;


static regul_info init_power_on_reguls [] = {
//    { "sensors", 3300},         // LDO0,
//    { "avdd_usb", 3300},        // LDO3,
    { "avdd_osc", 1800},        // LDO4,
//    { "mcore_mmc", 2850},       // LDO5,
//    { "vdd_fuse", 2850},        // LDO6,
    { "vdd_ddr_rx", 2850},      // LDO9,

#ifdef LGE_LOAD_SWITCH
    { "vdd_5v0", 5000},      // VDD_5V0,
    { "vdd_3v3", 3300 },
#endif
};

int __init regulator_init_on(void)
{
    int i;
    struct regulator* regul;

    for (i = 0; i < ARRAY_SIZE(init_power_on_reguls); i++)
    {
        //printk(KERN_INFO "[Regul] %s()  Try to get regulator(%s)\n", __func__, init_power_on_reguls[i]);
        regul = regulator_get(NULL,  init_power_on_reguls[i].regul_name);  /* digital core */
        if (IS_ERR(regul)) {
            printk(KERN_ERR "[Regul] %s()  Failed to get regulator(%s)\n", __func__, init_power_on_reguls[i].regul_name);
            continue;
        }
/*
        if (regulator_set_voltage(regul, init_power_on_reguls[i].voltage_mV * 1000,        \
                                    init_power_on_reguls[i].voltage_mV * 1000))
        {
            printk(KERN_ERR "[Regul] %s()  Failed to set voltage(%s, %dmV)\n", __func__, init_power_on_reguls[i].regul_name);
            continue;
        }
*/
        if (regulator_enable(regul))
        {
            printk(KERN_ERR "[Regul] %s()  Failed to enable regulator(%s)\n", __func__, init_power_on_reguls[i].regul_name);
        }
        // WBT 196167
        regulator_put(regul);
    }
    return 0;
}

static int __init powerkey_probe(struct platform_device *pdev)
{
    int ret, error;

    memset(&s_powerkey, 0x00, sizeof(s_powerkey));

    INIT_WORK(&s_powerkey.work, powerkey_handle);

    //GPIO configuration
    gpio_request(GPIO_POWERKEY, "power_key");
    tegra_gpio_enable(GPIO_POWERKEY);
    gpio_direction_input(GPIO_POWERKEY);

    //GPIO interrupt registration
    error = request_irq(gpio_to_irq(GPIO_POWERKEY), powerkey_interrupt_handler,
        (IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING), "power_key", (void*)&s_powerkey);
    if (error) {
        printk(KERN_ERR "[PWR_KEY] interrupt registeration fail! (err:%d)\n", error);
        goto err_request_irq_fail;
    }

    // input device
    // WBT 196166, 196169
    s_powerkey.inputDev = input_allocate_device();
    if (!s_powerkey.inputDev) {
        printk(KERN_ERR "[PWR_KEY] input_allocate_device Error!\n");
        goto err_request_irq_fail;
    }
    s_powerkey.inputDev->name = "powerkey";
    //s_powerkey.inputDev->id.bustype = BUS_HOST;
    s_powerkey.inputDev->evbit[0] = BIT(EV_KEY) | BIT(EV_PWR);
    set_bit(KEY_POWER, s_powerkey.inputDev->keybit);

    ret = input_register_device(s_powerkey.inputDev);
    if (ret) {
        printk(KERN_ERR "[PWR_KEY] input_register_device Error\n");
        goto err_input_device_register_fail;
    }
    regulator_init_on();
    return 0;

err_input_device_register_fail:
    input_free_device(s_powerkey.inputDev);
err_request_irq_fail:
    free_irq(gpio_to_irq(GPIO_POWERKEY), pdev);
    return -ENOSYS;
}

static int powerkey_remove(struct platform_device *pdev)
{
    if (!s_powerkey.inputDev) {
        input_unregister_device(s_powerkey.inputDev);
        input_free_device(s_powerkey.inputDev);
    }
    return 0;
}

static int powerkey_shutdown(struct platform_device *pdev)
{
    disable_irq(gpio_to_irq(GPIO_POWERKEY));
    return 0;
}

int powerkey_suspend(struct platform_device *dev, pm_message_t state)
{
    return 0;
}

int powerkey_resume(struct platform_device *dev)
{
    uint   pinValue;

    printk("[PWR_KEY] %s()\n", __func__);
    if (0 == key_pressed)
    {
        pinValue = (gpio_get_value(GPIO_POWERKEY) ? 1 : 0) ^ POWERKEY_ACTIVE_LOW;

        if (0 == key_pressed && pinValue) {
            printk("POWERKEY press @resume fn.\n");
            if (!s_powerkey.inputDev) {
                input_report_key(s_powerkey.inputDev, KEY_POWER, 1);
                input_sync(s_powerkey.inputDev);
            }
            key_pressed = 1;
        }

    }

    return 0;
}

static struct platform_driver powerkey_driver = {
    .probe  = powerkey_probe,
    .remove = powerkey_remove,
    .shutdown = powerkey_shutdown,
    .suspend = powerkey_suspend,
    .resume = powerkey_resume,
    .driver = {
        .name   = "star_powerkey",
        .owner  = THIS_MODULE,
    },
};

static int __init powerkey_init(void)
{
    return platform_driver_register(&powerkey_driver);
}

static void __exit powerkey_exit(void)
{
    platform_driver_unregister(&powerkey_driver);
}

module_init(powerkey_init);
module_exit(powerkey_exit);

MODULE_AUTHOR("LG Electronics");
MODULE_DESCRIPTION("star power key");
MODULE_LICENSE("GPL");

