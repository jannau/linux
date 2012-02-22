/*
 * drivers/startablet/cpwatcher.c
 *
 * Star Tablet CP watcher driver
 *
 * Copyright (c) 2011, LG Electronics Corporation.
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

#include <linux/module.h>
#include <linux/types.h>

#include <linux/kernel.h>
#include <linux/serial.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/fcntl.h>
#include <linux/string.h>
#include <linux/major.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/semaphore.h>

#ifdef LGE_FEATURE_CP_HALT_DETECTION
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <mach/gpio-names.h>
#include <mach/hardware.h>
#include <linux/uaccess.h>
#include <linux/workqueue.h>
#endif //LGE_FEATURE_CP_HALT_DETECTION

#define DEBUG 1

#ifdef LGE_FEATURE_CP_HALT_DETECTION
#define GPIO_IFX_RESET_1V8_N  TEGRA_GPIO_PV0

static struct work_struct work_reset;
static int reset_count = 0;
#endif //LGE_FEATURE_CP_HALT_DETECTION

static struct workqueue_struct *workqueue;
static struct work_struct work;

static struct tty_driver *cpwatcher_driver;

struct watcher_struct {
    struct tty_struct	*tty;
	spinlock_t	lock;
    int open_count;
};
static struct watcher_struct watcher;
static DECLARE_MUTEX(watcher_sema);

#ifdef LGE_FEATURE_CP_HALT_DETECTION
static struct watcher_struct reporter;
static int is_reset_requested = 0;

static DECLARE_MUTEX(reporter_sema);

extern void modem_reset(void);
static void do_cp_reset_internal(struct work_struct *work)
{
    if(DEBUG) printk(KERN_INFO "%s : start, reset_count=%d", __FUNCTION__, reset_count);
    if(reset_count > 0) return;

    reset_count++;
	is_reset_requested = 1;
	modem_reset();
	reset_count--;

    if(DEBUG) printk(KERN_INFO "%s : end", __FUNCTION__);
    return;
}

int do_cp_reset(void)
{
    if(DEBUG) printk(KERN_INFO "%s", __FUNCTION__);

    INIT_WORK(&work_reset, do_cp_reset_internal);
    queue_work(workqueue, &work_reset);

	return 0;
}
EXPORT_SYMBOL_GPL(do_cp_reset);
#endif //LGE_FEATURE_CP_HALT_DETECTION

static void notify_to_watcher(struct work_struct *work)
{
    const char* cprestart = "+RESTART\r\n";
    int data_size = strlen(cprestart);

    if(DEBUG) printk(KERN_INFO "%s", __FUNCTION__);

	spin_lock_irq(&watcher.lock);
	if((watcher.tty) && (watcher.tty->ldisc)) {
	    watcher.tty->ldisc->ops->receive_buf(watcher.tty, cprestart, NULL, data_size);
        tty_flip_buffer_push(watcher.tty);
	}
	spin_unlock_irq(&watcher.lock);

	return;
}

void cwc_notify_reset(int status)
{
    if(DEBUG) printk(KERN_INFO "%s, status=%d, is_reset_requested=%d", __FUNCTION__, status, is_reset_requested);

    if(status) {
#ifdef LGE_FEATURE_CP_HALT_DETECTION
        if(is_reset_requested)
            is_reset_requested = 0;
	else {
            if(DEBUG) printk(KERN_INFO "%s,  is_reset_requested=%d, so just returned.", __FUNCTION__, is_reset_requested);
            return;
        }
#else
        return;
#endif //LGE_FEATURE_CP_HALT_DETECTION
    }

    INIT_WORK(&work, notify_to_watcher);
    queue_work(workqueue, &work);
}
EXPORT_SYMBOL_GPL(cwc_notify_reset);

static int cwc_write(struct tty_struct *tty, const unsigned char *buf, int count)
{
	int nRtn = 0;
    if(DEBUG) printk(KERN_INFO "%s", __FUNCTION__);

#ifdef LGE_FEATURE_CP_HALT_DETECTION
	spin_lock_irq(&reporter.lock);
    if(tty && (tty->index == 1) && (strncmp(buf, "+CPHALT", count) == 0))
	{
        do_cp_reset();
    }
	spin_unlock_irq(&reporter.lock);

	nRtn = count;
#endif //LGE_FEATURE_CP_HALT_DETECTION

	return nRtn;
}

static int cwc_write_room(struct tty_struct *tty)
{
    return 16;
}

static int cwc_open(struct tty_struct *tty, struct file *filp)
{
    if(DEBUG) printk(KERN_INFO "%s", __FUNCTION__);

    tty->driver_data = NULL;
#ifdef LGE_FEATURE_CP_HALT_DETECTION
    if(tty->index > 1) return -ENODEV;
#else //LGE_FEATURE_CP_HALT_DETECTION
    if(tty->index > 0) return -ENODEV;
#endif //LGE_FEATURE_CP_HALT_DETECTION

    if(tty->index == 0)
	{
        if(watcher.open_count > 0) return -ENODEV;
        down(&watcher_sema);
        watcher.open_count++;

        /* save our structure within the tty structure */
        tty->driver_data = &watcher;
        watcher.tty = tty;
    }
#ifdef LGE_FEATURE_CP_HALT_DETECTION
	else if (tty->index == 1)
	{
        if(reporter.open_count > 0) return -ENODEV;
        down(&reporter_sema);
        reporter.open_count++;

        /* save our structure within the tty structure */
        tty->driver_data = &reporter;
        reporter.tty = tty;
    }
#endif //LGE_FEATURE_CP_HALT_DETECTION

    if(DEBUG) printk(KERN_INFO "%s returns", __FUNCTION__);

	return 0;

}

static void cwc_close(struct tty_struct *tty, struct file *filp)
{
    if(DEBUG) printk(KERN_INFO "%s", __FUNCTION__);

    if((tty == NULL) || (filp == NULL)) return;

    if((tty->index == 0) && (tty->driver_data == &watcher))
	{
        watcher.open_count--;
        watcher.tty = NULL;
        up(&watcher_sema);
    }
#ifdef LGE_FEATURE_CP_HALT_DETECTION
	else if((tty->index == 1) && (tty->driver_data == &reporter))
	{
        reporter.open_count--;
        reporter.tty = NULL;
        up(&reporter_sema);
    }
#endif //LGE_FEATURE_CP_HALT_DETECTION

    if(DEBUG) printk(KERN_INFO "%s returns", __FUNCTION__);

    return;
}

struct tty_operations cpwatcher_ops = {
     .open = cwc_open,
     .close = cwc_close,
     .write = cwc_write,
     .write_room = cwc_write_room,
};

static int __init cwc_init(void)
{
    int result;

    if(DEBUG) printk(KERN_INFO "%s: start", __FUNCTION__);

    cpwatcher_driver = alloc_tty_driver(2);

    if (!cpwatcher_driver)
        return -ENOMEM;

    spin_lock_init(&watcher.lock);
	watcher.open_count = 0;

#ifdef LGE_FEATURE_CP_HALT_DETECTION
    spin_lock_init(&reporter.lock);
    reporter.open_count = 0;
#endif //LGE_FEATURE_CP_HALT_DETECTION

    cpwatcher_driver->owner = THIS_MODULE;
    cpwatcher_driver->driver_name = "cpwatcher";

    cpwatcher_driver->name = "cwc";
    cpwatcher_driver->major = 154;
    cpwatcher_driver->minor_start = 0;
    cpwatcher_driver->type = TTY_DRIVER_TYPE_SERIAL;
    cpwatcher_driver->subtype = SERIAL_TYPE_NORMAL;
    cpwatcher_driver->flags = TTY_DRIVER_REAL_RAW | TTY_DRIVER_DYNAMIC_DEV;

    cpwatcher_driver->init_termios = tty_std_termios;
    cpwatcher_driver->init_termios.c_cflag = B38400 | CS8 | CREAD | HUPCL | CLOCAL;

    tty_set_operations(cpwatcher_driver, &cpwatcher_ops);

    result = tty_register_driver(cpwatcher_driver);
    if (result) {
        printk(KERN_ERR "failed to register cp watcher driver");
        put_tty_driver(cpwatcher_driver);
        return result;
    }

    tty_register_device(cpwatcher_driver, 0, NULL);
    tty_register_device(cpwatcher_driver, 1, NULL);

    workqueue = create_singlethread_workqueue("cwcrst");

    if(DEBUG) printk(KERN_INFO "%s: end", __FUNCTION__);

    return 0;
}

static void __exit cwc_exit(void)
{
    if(DEBUG) printk(KERN_INFO "%s", __FUNCTION__);

    tty_unregister_device(cpwatcher_driver, 0);
    tty_unregister_device(cpwatcher_driver, 1);

    tty_unregister_driver(cpwatcher_driver);

    if (workqueue){
        flush_workqueue(workqueue);
        destroy_workqueue(workqueue);
        workqueue = NULL;
    }
}

module_init(cwc_init);
module_exit(cwc_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("CP Watcher");
