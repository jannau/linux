/*
 * drivers/misc/bcm4329_rfkill.c
 *
 * Copyright (c) 2010, NVIDIA Corporation.
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

#include <linux/err.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/rfkill.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <mach/gpio-names.h>
#include <linux/interrupt.h>
#include <mach/bcm_bt_lpm.h>

#define MODULE_TAG	"[RFKILL] "
#define log_format(fmt) MODULE_TAG fmt

#define RFKILL_LOG_I(fmt, ...)	printk( KERN_INFO log_format(fmt) , ##__VA_ARGS__ )
#define RFKILL_LOG_E(fmt, ...)	printk( KERN_ERR log_format(fmt) , ##__VA_ARGS__ )
#define RFKILL_LOG_D(fmt, ...)	printk( KERN_DEBUG log_format(fmt) , ##__VA_ARGS__ )
#define RFKILL_LOG_W(fmt, ...)	printk( KERN_WARNING log_format(fmt) , ##__VA_ARGS__ )

#define GPIO_BT_HOST_WAKE       TEGRA_GPIO_PC7

#define WAKEUP_ENABLE	1
#define WAKEUP_DISABLE	0

#define ENABLE			1
#define DISABLE			0

struct bcm4329_rfkill_data {
	int gpio_reset;
	int gpio_shutdown;
	int delay;
	int host_wakeup_irq;
	int	wakeup_enable;
	struct clk *bt_32k_clk;
};

static int bt_32k_clk_count = 0;


static struct bcm4329_rfkill_data *bcm4329_rfkill;

static void do_bt_32k_clk( bool enable )
{
	if ( !bcm4329_rfkill->bt_32k_clk )
		return ;

	RFKILL_LOG_I(" %s : %s \n", __func__, (enable ? "enable" : "disable" ) );
	if ( enable )
	{
		bt_32k_clk_count++;
        clk_enable(bcm4329_rfkill->bt_32k_clk);
	}
	else
	{
		bt_32k_clk_count--;
		clk_disable(bcm4329_rfkill->bt_32k_clk);
	}

	if ( bt_32k_clk_count < 0 )
		RFKILL_LOG_W(" clock ref count is negative ( ref cnt : %d ) !!!! \n", bt_32k_clk_count );
}

static void set_bt_wakeup_enable( bool enable )
{
	if( bcm4329_rfkill->wakeup_enable == enable )
	{
		RFKILL_LOG_I( " already %s \n", ( enable ? "enabled" : "disabled" ) );
		return ;
	}

	if( set_irq_wake(bcm4329_rfkill->host_wakeup_irq, enable ) )
	{
		RFKILL_LOG_W( " Fail: bluetooth wakeup source : %s \n", ( enable ? "ON" : "OFF" ) );
		return;
	}

	bcm4329_rfkill->wakeup_enable = enable;
	RFKILL_LOG_W( " Success: bluetooth wakeup source : %s \n", ( enable ? "ON" : "OFF" ) );
}

static void block_rf(void)
{
	if(bcm4329_rfkill->gpio_shutdown)
    {
		gpio_direction_output(bcm4329_rfkill->gpio_shutdown, 0);
    }

    gpio_direction_output(bcm4329_rfkill->gpio_reset, 0);
	set_bt_lpm_enable(0);

	do_bt_32k_clk( DISABLE );

	set_bt_wakeup_enable( WAKEUP_DISABLE );
}

static void release_rf(void)
{
	do_bt_32k_clk( ENABLE );

    if(bcm4329_rfkill->gpio_shutdown)
    {
		gpio_direction_output(bcm4329_rfkill->gpio_shutdown, 1);
    }

	set_bt_wakeup_enable( WAKEUP_ENABLE );

	gpio_direction_output(bcm4329_rfkill->gpio_reset, 0);
	msleep(30);
    gpio_direction_output(bcm4329_rfkill->gpio_reset, 1);
	msleep(400);
	set_bt_lpm_enable(1);
}

static int bcm4329_bt_rfkill_set_power(void *data, bool blocked)
{
    if (blocked)
	{
		block_rf();
		msleep(300);
    }
	else
	{
		release_rf();
    }

    return 0;
}

static const struct rfkill_ops bcm4329_bt_rfkill_ops = {
	.set_block = bcm4329_bt_rfkill_set_power,
};

static int bcm4329_rfkill_probe(struct platform_device *pdev)
{
	struct rfkill *bt_rfkill;
	struct resource *res;
	int ret;
	bool enable = false;  /* off */
	bool default_sw_block_state;

	bcm4329_rfkill = kzalloc(sizeof(*bcm4329_rfkill), GFP_KERNEL);
	if (!bcm4329_rfkill)
		return -ENOMEM;

	bcm4329_rfkill->bt_32k_clk = clk_get(&pdev->dev, "bcm4329_32k_clk");
	if (IS_ERR(bcm4329_rfkill->bt_32k_clk)) {
		pr_warn("can't find bcm4329_32k_clk. assuming clock to chip\n");
		bcm4329_rfkill->bt_32k_clk = NULL;
	}

	res = platform_get_resource_byname(pdev, IORESOURCE_IO,
						"bcm4329_nreset_gpio");
	if (!res) {
		pr_err("couldn't find reset gpio\n");
		goto free_bcm_32k_clk;
	}
	bcm4329_rfkill->gpio_reset = res->start;
	tegra_gpio_enable(bcm4329_rfkill->gpio_reset);
	ret = gpio_request(bcm4329_rfkill->gpio_reset,
					"bcm4329_nreset_gpio");
	if (unlikely(ret))
		goto free_bcm_32k_clk;

	res = platform_get_resource_byname(pdev, IORESOURCE_IO,
						"bcm4329_nshutdown_gpio");
	if (!res) {
		pr_err("couldn't find shutdown gpio. assuming module\n");
	/*	gpio_free(bcm4329_rfkill->gpio_reset);
		goto free_bcm_32k_clk;
	*/
	}
	else
	{
		tegra_gpio_enable(bcm4329_rfkill->gpio_shutdown);
		ret = gpio_request(bcm4329_rfkill->gpio_shutdown,
						"bcm4329_nshutdown_gpio");
		if (unlikely(ret)) {
			gpio_free(bcm4329_rfkill->gpio_reset);
			goto free_bcm_32k_clk;
		}
	}

	bcm4329_rfkill->wakeup_enable = WAKEUP_DISABLE;
	bcm4329_rfkill->host_wakeup_irq = gpio_to_irq(GPIO_BT_HOST_WAKE);

	if (bcm4329_rfkill->bt_32k_clk && enable)
	{
		bt_32k_clk_count++;
		clk_enable(bcm4329_rfkill->bt_32k_clk);
	}

	if (bcm4329_rfkill->gpio_shutdown)
		gpio_direction_output(bcm4329_rfkill->gpio_shutdown, enable);
	gpio_direction_output(bcm4329_rfkill->gpio_reset, enable);

	bt_rfkill = rfkill_alloc("bcm4329_rfkill", &pdev->dev,
				RFKILL_TYPE_BLUETOOTH, &bcm4329_bt_rfkill_ops,
				NULL);

	if (unlikely(!bt_rfkill))
		goto free_bcm_res;

	default_sw_block_state = !enable;
	rfkill_set_states(bt_rfkill, default_sw_block_state, false);

	ret = rfkill_register(bt_rfkill);

	if (unlikely(ret)) {
		rfkill_destroy(bt_rfkill);
		goto free_bcm_res;
	}

	return 0;

free_bcm_res:
	gpio_free(bcm4329_rfkill->gpio_shutdown);
	gpio_free(bcm4329_rfkill->gpio_reset);
free_bcm_32k_clk:
	if (bcm4329_rfkill->bt_32k_clk && enable)
		clk_disable(bcm4329_rfkill->bt_32k_clk);
	if (bcm4329_rfkill->bt_32k_clk)
		clk_put(bcm4329_rfkill->bt_32k_clk);
	kfree(bcm4329_rfkill);
	return -ENODEV;
}

static int bcm4329_rfkill_remove(struct platform_device *pdev)
{
	struct rfkill *bt_rfkill = platform_get_drvdata(pdev);

	if (bcm4329_rfkill->bt_32k_clk)
		clk_put(bcm4329_rfkill->bt_32k_clk);
	rfkill_unregister(bt_rfkill);
	rfkill_destroy(bt_rfkill);
	if (bcm4329_rfkill->gpio_shutdown)
		gpio_free(bcm4329_rfkill->gpio_shutdown);
	gpio_free(bcm4329_rfkill->gpio_reset);
	kfree(bcm4329_rfkill);

	return 0;
}

static struct platform_driver bcm4329_rfkill_driver = {
	.probe = bcm4329_rfkill_probe,
	.remove = bcm4329_rfkill_remove,
	.driver = {
		   .name = "bcm4329_rfkill",
		   .owner = THIS_MODULE,
	},
};

static int __init bcm4329_rfkill_init(void)
{
	return platform_driver_register(&bcm4329_rfkill_driver);
}

static void __exit bcm4329_rfkill_exit(void)
{
	platform_driver_unregister(&bcm4329_rfkill_driver);
}

module_init(bcm4329_rfkill_init);
module_exit(bcm4329_rfkill_exit);

MODULE_DESCRIPTION("BCM4329 rfkill");
MODULE_AUTHOR("NVIDIA");
MODULE_LICENSE("GPL");
