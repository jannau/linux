/*
 *  atmel_maxtouch.c - Atmel maXTouch Touchscreen Controller
 *
 *  Version ??
 *
 *  An early alpha version of the maXTouch Linux driver.
 *
 *
 *  Copyright (C) 2010 Iiro Valkonen <iiro.valkonen@atmel.com>
 *  Copyright (C) 2009 Ulf Samuelsson <ulf.samuelsson@atmel.com>
 *  Copyright (C) 2009 Raphael Derosso Pereira <raphaelpereira@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/hrtimer.h>
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/i2c.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/string.h>
#include <linux/proc_fs.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/types.h>
#include <linux/random.h>
#include <linux/major.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <linux/poll.h>
#include <linux/mutex.h>
#include <linux/rcupdate.h>
#include <linux/smp_lock.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/workqueue.h>
#include <linux/wakelock.h>  
#include <linux/freezer.h>
#include <linux/i2c.h>

#include <mach/gpio-names.h>

#include "star_touchkey.h"


/* Driver datastructure */
struct synaptics_data {
	struct i2c_client *client;
	struct input_dev  *input;
	char phys_name[32];
	int irq;

	wait_queue_head_t msg_queue;
	struct semaphore msg_sem;

	spinlock_t lock;
	struct delayed_work  dwork;
};

int synaptics_read_block(struct i2c_client *client,
		   u16 addr,
		   u16 length,
		   u8 *value)
{
	struct i2c_adapter *adapter = client->adapter;
	struct i2c_msg msg[2];
	__le16	le_addr;
	struct synaptics_data *synap;
	
	synap = i2c_get_clientdata(client);
	
	le_addr = cpu_to_le16(addr);
	msg[0].addr  = client->addr;
	msg[0].flags = 0x00;
	msg[0].len   = 2;
	msg[0].buf   = (u8 *) &le_addr;
	
	msg[1].addr  = client->addr;
	msg[1].flags = I2C_M_RD;
	msg[1].len   = length;
	msg[1].buf   = (u8 *) value;
	if  (i2c_transfer(adapter, msg, 2) == 2)
		return 0;
	else
		return -EIO;
}

/* Writes a block of bytes (max 256) to given address in mXT chip. */
int synaptics_write_block(struct i2c_client *client,
		    u16 addr,
		    u16 length,
		    u8 *value)
{
	int i;
	struct {
			__le16	le_addr;
			u8	data[256];
	} i2c_block_transfer;
	struct synaptics_data *synap;
	
	synap = i2c_get_clientdata(client);

	for (i = 0; i < length; i++)
		i2c_block_transfer.data[i] = *value++;
	
	i2c_block_transfer.le_addr = cpu_to_le16(addr);
	i = i2c_master_send(client, (u8 *) &i2c_block_transfer, length + 2);
	
	if (i == (length + 2))
		return length;
	else
		return -EIO;
}

u32 previous_key=0;
 int synaptics_touchkey_report(struct	synaptics_data *synap, u16 report_id)
 {
 	int input_key= 0;
	switch(report_id)
	{
		case 0x800 : 
			input_key = KEY_MENU;
			printk("report key KEY_MENU\n");
			break;
		case 0x20 : 
			input_key = KEY_BACK;
			printk("report key KEY_BACK\n");
			break;
		case 0x2 : 
			input_key = KEY_HOME;
			printk("report key KEY_HOME\n");
			break;
		case 0x0 :
			printk("report key release\n");
			if(previous_key == 0)
				return 0;
			break;
		default :
			printk("report key value not supported, val : 0x%x\n",report_id);
			return 0;
			break;
	}
	// Report_id mustbe 0x800, 0x20, 0x2
	
	if(report_id)
	{
		// Press Event
		input_report_key(synap->input, input_key, 1);
		//LED On For Notice
		gpio_set_value(TEGRA_GPIO_PK1,1); 
		msleep(100);gpio_set_value(TEGRA_GPIO_PK1,0);
	}
	else // Release Event
		input_report_key(synap->input, previous_key, 0);
	
	input_sync(synap->input);
	previous_key = input_key;

	return 1;
 }

/* Processes messages when the interrupt line (CHG) is asserted. */

static void synaptics_worker(struct work_struct *work)
{
	struct synaptics_data *synap = NULL;
	u16	msg_len=2, msg_addr=0x901 /*reg 0x109 */,msg_val;
	u8	msg[2];

	synap = container_of(work, struct synaptics_data, dwork.work);

	if( synaptics_read_block(synap->client, msg_addr, msg_len, msg)  < 0)
		printk("Touchkey I2C Read Error #1\n");
	// Make 16bit value
	msg_val = msg[0]<<8 | msg[1];

	//Report to App
	synaptics_touchkey_report(synap, msg_val);
	
	if (down_interruptible(&synap->msg_sem)) {
		printk("synaptics_worker Interrupted while waiting for msg_sem!\n");
		return;
	}
	
	up(&synap->msg_sem);
	wake_up_interruptible(&synap->msg_queue);

	//enable_irq(synap->irq);
}

static irqreturn_t synaptics_irq_handler(int irq, void *_synap)
{
	struct	synaptics_data *synap = _synap;
	unsigned long	flags;

	spin_lock_irqsave(&synap->lock, flags);

	/* Send the signal only if falling edge generated the irq. */
	//disable_irq(synap->irq);
	cancel_delayed_work(&synap->dwork);
	schedule_delayed_work(&synap->dwork, 0);
	
	spin_unlock_irqrestore(&synap->lock, flags);

	return IRQ_HANDLED;
}

// Initial Setting Value for Touch Key
int synaptics_config_init(struct i2c_client *client)
{
	int i=0;
	u8 buf[50];
	u16 address = 0x0;
	
	address = 0x0000;
	
	synaptics_write_block(client, address , 50, Synaptics_Config);

	if (synaptics_read_block(client, address, 50, buf) < 0) {
		dev_err(&client->dev,"synaptics Object %d could not be read\n", i);
		return -EIO;
	}

	printk("[%s]TouchKey Default Value Setup \n",__FUNCTION__);

	return 0;
}


static int __devinit synaptics_probe(struct i2c_client *client,
			       const struct i2c_device_id *id)
{
	struct synaptics_data *synap;
	struct input_dev *input;
	int error;
	u8 buf[8];

//1 Allocate structure - we need it to identify device 
	synap = kzalloc(sizeof(struct synaptics_data), GFP_KERNEL);
	input = input_allocate_device();
	if (!synap || !input) {
		dev_err(&client->dev, "insufficient memory\n");
		error = -ENOMEM;
		goto err_free_mem;
	}

	synap->client = client;
	synap->input  = input;
	synap->irq = gpio_to_irq(TEGRA_GPIO_PG1);

//1 Work Queue & Semaphore Setup
	INIT_DELAYED_WORK(&synap->dwork, synaptics_worker);
	init_waitqueue_head(&synap->msg_queue);
	init_MUTEX(&synap->msg_sem);
	spin_lock_init(&synap->lock);

//1  GPIO IRQ Setup
	gpio_request(TEGRA_GPIO_PG1, "touchkey_int_n");	
    	tegra_gpio_enable(TEGRA_GPIO_PG1);

	gpio_direction_input(TEGRA_GPIO_PG1);
	
	error = request_irq(
			synap->irq,//gpio_to_irq(TEGRA_GPIO_PG1)
			synaptics_irq_handler,
			(IRQF_TRIGGER_FALLING),//IRQF_TRIGGER_FALLING,//IRQF_TRIGGER_LOW,// 0,
			"touchkey_int_n",
			synap
		);

	// LED Initial Lighting
	gpio_request(TEGRA_GPIO_PK1, "touchkey_LED");	
    	tegra_gpio_enable(TEGRA_GPIO_PK1);

	gpio_direction_output(TEGRA_GPIO_PK1,0);msleep(100);
	gpio_set_value(TEGRA_GPIO_PK1,1);msleep(100);
	gpio_set_value(TEGRA_GPIO_PK1,0);

//1  Input Device Setup
	snprintf(synap->phys_name,sizeof(synap->phys_name),	"%s/input0",dev_name(&client->dev));
	input->name = "Synaptics SO281001 Touchkey controller";
	input->phys = synap->phys_name;
	input->id.bustype = BUS_I2C;
	input->dev.parent = &client->dev;

	set_bit(EV_SYN, input->evbit);
	set_bit(EV_KEY,input->evbit); 		// Event is key input type
	set_bit(BTN_TOUCH,input->keybit);  // virtual key is BTN_TOUCH
	set_bit(EV_ABS, input->evbit);  		// Input values are in absoulte values

	// Button Code Setup
	set_bit(KEY_HOME, input->keybit);
	set_bit(KEY_MENU, input->keybit);
	set_bit(KEY_BACK, input->keybit);
	synap->input = input;

	if (input_register_device(synap->input) < 0) {
		dev_err(&client->dev,"Failed to register input device\n");
		goto err_free_irq;
	}

//1  I2C Set
	i2c_set_clientdata(client, synap);
	input_set_drvdata(input, synap);

//1 Setup Touchkey default Value
	if( synaptics_config_init(client) < 0)
	{
		printk("Touchkey Setup Fail ####################\n");
		goto err_free_irq;
	}
	synaptics_read_block(client, 0x901, 2, buf);// Dummy Read,, required

////1 Start Workqueue
	//disable_irq(synap->irq);
	//cancel_delayed_work(&synap->dwork);
	//schedule_delayed_work(&synap->dwork, 0);

	return 0;

err_free_irq:
	if (synap->irq)
		free_irq(synap->irq, synap);
	input_free_device(input);
err_free_mem:
	kfree(synap);
	
	return error;
}

static int __devexit synaptics_remove(struct i2c_client *client)
{
	struct synaptics_data *synap;

	synap = i2c_get_clientdata(client);

	/* Release IRQ so no queue will be scheduled */
	if (synap->irq)
		free_irq(synap->irq, synap);

	cancel_delayed_work_sync(&synap->dwork);
	input_unregister_device(synap->input);
	kfree(synap);
	i2c_set_clientdata(client, NULL);

	return 0;
}

static int synaptics_suspend(struct i2c_client *client, pm_message_t mesg)
{
	struct synaptics_data *synap = i2c_get_clientdata(client);

	if (device_may_wakeup(&client->dev))
		enable_irq_wake(synap->irq);
	return 0;
}

static int synaptics_resume(struct i2c_client *client)
{
	struct synaptics_data *synap = i2c_get_clientdata(client);

	if (device_may_wakeup(&client->dev))
		disable_irq_wake(synap->irq);
	return 0;
}

static struct i2c_device_id synaptics_idtable[] = {
	{"star_touchkey", 0,},
	{ /* end of list */ },
};

static const struct dev_pm_ops synaptics_pm_ops = {
	.suspend	= synaptics_suspend,
	.resume		= synaptics_resume,
}

static struct i2c_driver synaptics_driver = {
	.probe		= synaptics_probe,
	.remove		= __devexit_p(synaptics_remove),
	.id_table	= synaptics_idtable,
	.driver = {
		.name	= "star_touchkey",
		.owner  = THIS_MODULE,
		.pm		= &synaptics_pm_ops,
	},
};

static int __init synaptics_init(void)
{
	return i2c_add_driver(&synaptics_driver);
}

static void __exit synaptics_exit(void)
{
	i2c_del_driver(&synaptics_driver);
}


module_init(synaptics_init);
module_exit(synaptics_exit);

MODULE_AUTHOR("synaptics");
MODULE_DESCRIPTION("Driver for Synaptics OneTouch SO281001 Touchkey Controller");
MODULE_LICENSE("GPL");
