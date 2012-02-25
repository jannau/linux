/*
 * Copyright (C) 2009 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/hrtimer.h>
#include <linux/irq.h>
#include <linux/serial_core.h>
#include <mach/bcm_bt_lpm.h>
//#include <asm/gpio.h>

#include <linux/input.h>
#include <linux/gpio.h>
#include <mach/gpio-names.h>
#include <linux/interrupt.h>
#include <linux/wakelock.h>

#define GPIO_BT_WAKE       TEGRA_GPIO_PX4
#define GPIO_BT_HOST_WAKE       TEGRA_GPIO_PC7

#define GPIO_LABEL_BTLPM     "bluetooth_lpm"

#define WAKELOCK_TIMEOUT



/*
 * Manage WAKE and HOST_WAKE low power mode signals for Broadcom
 * Bluetooth chipsets.
 *
 * This driver needs to be tightly coupled with a uart driver that supports
 * request_clock_off_locked() and request_clock_on_locked(), to clock off and
 * on the uart indepdently of Linux suspend/resume.
 *
 * The uart driver needs to call bcm_bt_lpm_exit_lpm_locked() every time it
 * begins TX, to ensure this driver keeps WAKE asserted during TX.
 *
 * The callbacks and hijacking of the uart_port struct are not a clean API,
 * but the Linux tty and serial core layers do not have a better alternative
 * right now: there is no good way to plumb uart clock control through these
 * layers. See http://lkml.org/lkml/2008/12/19/213 for more background.
 */

struct bcm_bt_lpm {
	unsigned int gpio_wake;
	unsigned int gpio_host_wake;

	int wake;
	int host_wake;
	int host_wake_irq;

	int bt_enable;

	struct work_struct host_wake_work;

	struct hrtimer check_hostwakeup_timer;
	ktime_t check_hostwakeup_delay;

	struct hrtimer enter_lpm_timer;
	ktime_t enter_lpm_delay;

	struct uart_port *uport;

	struct wake_lock bt_wake_lock;
	struct wake_lock host_wake_lock;

/*	void (*request_clock_off_locked)(struct uart_port *uport);
	void (*request_clock_on_locked)(struct uart_port *uport);
	*/
} bt_lpm;

void set_bt_lpm_enable(int bt_enable_status)
{

	bt_lpm.bt_enable = bt_enable_status;

}
EXPORT_SYMBOL(set_bt_lpm_enable);


static void set_wake_locked(int wake)
{
	//int bt_wake;
	//int temp_btstatus;
	if (wake == bt_lpm.wake)
		return;
	bt_lpm.wake = wake;

	printk(KERN_ERR "[bcm_bt_lpm] %s GPIO %d set value: %d\n", __func__, bt_lpm.gpio_wake, wake);

/*	if (wake || bt_lpm.host_wake)
		bt_lpm.request_clock_on_locked(bt_lpm.uport);
	else
		bt_lpm.request_clock_off_locked(bt_lpm.uport);
*/
	gpio_set_value(bt_lpm.gpio_wake, wake);

	if (bt_lpm.wake)
	{
		wake_lock(&bt_lpm.bt_wake_lock);
	}
	else
	{
		wake_unlock(&bt_lpm.bt_wake_lock);
	}

//	bt_wake = gpio_get_value(bt_lpm.gpio_wake);
//	printk(KERN_NOTICE "[bcm_bt_lpm] %s GPIO %d \n set value: %d, real value: %d", __func__, bt_lpm.gpio_wake, wake, bt_wake );

}

static enum hrtimer_restart enter_lpm(struct hrtimer *timer) {
	unsigned long flags;

	printk(KERN_NOTICE "[bcm_bt_lpm] %s\n", __func__);

	set_wake_locked(0);

	return HRTIMER_NORESTART;
}

void bcm_bt_lpm_exit_lpm_locked(struct uart_port *uport) {

	bt_lpm.uport = uport;

	//printk(KERN_NOTICE "[bcm_bt_lpm] %s\n", __func__);

	hrtimer_try_to_cancel(&bt_lpm.enter_lpm_timer);

	//printk(KERN_NOTICE "[bcm_bt_lpm] %s\n", __func__);

	set_wake_locked(1);

	hrtimer_start(&bt_lpm.enter_lpm_timer, bt_lpm.enter_lpm_delay,
			HRTIMER_MODE_REL);
}
EXPORT_SYMBOL(bcm_bt_lpm_exit_lpm_locked);

static void update_host_wake_locked(int host_wake)
{
	if (host_wake == bt_lpm.host_wake)
		return;
	bt_lpm.host_wake = host_wake;

	printk(KERN_NOTICE "[bcm_bt_lpm] wake lock HOST_WAKE host=%d, bt=%d\n",host_wake,bt_lpm.wake);
	if (host_wake)
	{
		wake_lock(&bt_lpm.host_wake_lock);
	}
	else
	{
		wake_unlock(&bt_lpm.host_wake_lock);
	}
}

static irqreturn_t host_wake_isr(int irq, void *dev)
{
	int host_wake;
	//unsigned long flags;
	host_wake = gpio_get_value(bt_lpm.gpio_host_wake);

#ifdef WAKELOCK_TIMEOUT
	schedule_work(&bt_lpm.host_wake_work);
	return IRQ_HANDLED;
#endif

	set_irq_type(bt_lpm.host_wake_irq, host_wake ? IRQF_TRIGGER_LOW : IRQF_TRIGGER_HIGH);

	schedule_work(&bt_lpm.host_wake_work);

	return IRQ_HANDLED;
}

static enum hrtimer_restart check_hostwakeup(struct hrtimer *timer) {
	int host_wake;

	host_wake = gpio_get_value(bt_lpm.gpio_host_wake);

	printk(KERN_ERR "[bcm_bt_lpm] check_hostwakeup bt_enable: %d, host_wake: %d \n", bt_lpm.bt_enable, host_wake);
	if(host_wake && bt_lpm.bt_enable)
	{
        schedule_work(&bt_lpm.host_wake_work);
	}

	return HRTIMER_NORESTART;
}

static void bt_host_wake_work_func(struct work_struct *ignored)
{
	int host_wake;

	host_wake = gpio_get_value(bt_lpm.gpio_host_wake);
	printk(KERN_NOTICE "[bcm_bt_lpm] %s host_wake = %d\n", __func__, host_wake);

#ifdef WAKELOCK_TIMEOUT
	if(host_wake)
	{
	hrtimer_try_to_cancel(&bt_lpm.check_hostwakeup_timer);
	hrtimer_start(&bt_lpm.check_hostwakeup_timer, bt_lpm.check_hostwakeup_delay,
			HRTIMER_MODE_REL);

	wake_lock_timeout(&bt_lpm.host_wake_lock, 5*HZ);
	}
	return;
#endif
	update_host_wake_locked(host_wake);
}

static int bcm_bt_lpm_probe(struct platform_device *pdev)
{
	int irq;
	int ret;
	//int bt_wake;

	printk(KERN_NOTICE "[bcm_bt_lpm] %s\n", __func__);

	bt_lpm.bt_enable = 0;

	if (bt_lpm.gpio_wake == GPIO_BT_WAKE) {
		printk(KERN_ERR "Cannot register two bcm_bt_lpm drivers\n");
		return -EINVAL;
	}
	memset(&bt_lpm, 0x00, sizeof(bt_lpm));

	wake_lock_init(&bt_lpm.bt_wake_lock, WAKE_LOCK_SUSPEND,
				"bt_wake");
	wake_lock_init(&bt_lpm.host_wake_lock, WAKE_LOCK_SUSPEND,
				"host_wake");

	bt_lpm.gpio_wake = GPIO_BT_WAKE;
	bt_lpm.gpio_host_wake = GPIO_BT_HOST_WAKE;

	INIT_WORK(&bt_lpm.host_wake_work, bt_host_wake_work_func);

	hrtimer_init(&bt_lpm.enter_lpm_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	bt_lpm.enter_lpm_delay = ktime_set(1, 0);  /* 1 sec */
	bt_lpm.enter_lpm_timer.function = enter_lpm;

	hrtimer_init(&bt_lpm.check_hostwakeup_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	bt_lpm.check_hostwakeup_delay = ktime_set(5, 0);  /* 5 sec */
	bt_lpm.check_hostwakeup_timer.function = check_hostwakeup;

	ret = gpio_request(bt_lpm.gpio_wake, GPIO_LABEL_BTLPM);
		if (ret < 0) {
			printk(KERN_ERR "Fail to request BLUETOOTH_Wakeup_GPIO_ENABLE\n");
			return ret;
		}
	tegra_gpio_enable(bt_lpm.gpio_wake);

	gpio_direction_output(bt_lpm.gpio_wake, 0);
	//tegra_gpio_enable(bt_lpm.gpio_wake);

	gpio_request(bt_lpm.gpio_host_wake, "bt_host_wakeup");
    tegra_gpio_enable(bt_lpm.gpio_host_wake);
    gpio_direction_input(bt_lpm.gpio_host_wake);

	irq = gpio_to_irq(bt_lpm.gpio_host_wake);
	bt_lpm.host_wake_irq = irq;

#ifdef WAKELOCK_TIMEOUT
	set_irq_type(irq, IRQ_TYPE_EDGE_RISING);

	ret = request_irq(irq, host_wake_isr, 0,
			"bt host_wake", NULL);
#else
	ret = request_irq(irq, host_wake_isr, IRQF_TRIGGER_HIGH,
			"bt host_wake", NULL);
#endif

	if (ret)
		return ret;

	return 0;
}

static int bcm_bt_lpm_remove(struct platform_device *pdev)
{
	//struct rfkill *rfkill = platform_get_drvdata(pdev);

	gpio_free(bt_lpm.gpio_wake);
	gpio_free(bt_lpm.gpio_host_wake);
	wake_lock_destroy(&bt_lpm.bt_wake_lock);
	wake_lock_destroy(&bt_lpm.host_wake_lock);

	return 0;

}

static struct platform_driver bcm_bt_lpm_driver = {
	.probe = bcm_bt_lpm_probe,
	.remove = bcm_bt_lpm_remove,
	.driver = {
		.name = "bcm_bt_lpm",
		.owner = THIS_MODULE,
	},
};

static int __init bcm_bt_lpm_init(void)
{
	return platform_driver_register(&bcm_bt_lpm_driver);
}

static void __exit bcm_bt_lpm_exit(void)
{
    platform_driver_unregister(&bcm_bt_lpm_driver);
}

module_init(bcm_bt_lpm_init);
module_exit(bcm_bt_lpm_exit);

MODULE_DESCRIPTION("Broadcom Bluetooth low power mode driver");
MODULE_AUTHOR("Nick Pelly <npelly@google.com>");
MODULE_LICENSE("GPL");
