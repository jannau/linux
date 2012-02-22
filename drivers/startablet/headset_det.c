/*
 * Star - Headset Detection Driver
 *
 * Copyright (C) 2010 LGE, Inc.
 *
 * Author: <bergkamp.cho@lge.com>
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

#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/fb.h>
#include <linux/string.h>
#include <asm/gpio.h>
#include <asm/system.h>
#include <linux/interrupt.h>
#include <linux/switch.h>
#include <linux/workqueue.h>
#include <linux/input.h>
#include <linux/mfd/tps6586x.h>
#include <mach/gpio-names.h>

#include "headset_det.h"

struct headset_switch_data	*headset_sw_data;

unsigned int headset_status = 0;
unsigned int headset_gpio_status = 0;
unsigned int hookkey_gpio_status = 0;
unsigned int HS_MIC_BIAS_EN_gpio_status = 0;
unsigned int headset_detecting = 0;

headset_type_enum headset_type = STAR_NONE;
unsigned int hook_status = HOOK_RELEASED;

enum {
	NO_DEVICE,
	HEADSET_WITH_MIC,
	HEADSET_WITHOUT_MIC,
};
struct headset_switch_data {
	struct switch_dev sdev;
	unsigned gpio;
	unsigned hook_gpio;
	const char *name_on;
	const char *name_off;
	const char *state_on;
	const char *state_off;
	int irq;
	int hook_irq;
	struct work_struct work;
	struct delayed_work delayed_work;
	struct delayed_work hook_delayed_work;
	struct input_dev *ip_dev;	//20100421 bergkamp.cho@lge.com for headset driver [LGE]
};

unsigned int get_headset_type(void)
{
    return (unsigned int)headset_type;
}
EXPORT_SYMBOL(get_headset_type);


int type_detection_time = 700;
int remove_detection_time = 60;

extern void star_headsetdet_bias_star_codec(int bias);

#define HEADSET_GUID                NV_ODM_GUID('h','e','a','d','s','e','t','_')


static void headset_det_work(struct work_struct *work)
{

	printk(KERN_ERR "@@(Headset_det.c)@@ headset_det_work(), headset_status = %d\n",headset_status);

}

static void type_det_work(struct work_struct *work)
{

	int gpio_value;
	int voltage;

	printk(KERN_ERR "@@(Headset_det.c)@@ type_det_work(), headset_type = %d\n",headset_type);

	gpio_value = gpio_get_value(TEGRA_GPIO_PW3);

	if(gpio_value==1){
		gpio_set_value(TEGRA_GPIO_PX6, 1);
		printk(KERN_ERR "EAR SENSE IN [1] \n");
		mdelay(10);
		tps6586x_adc_read(ANLG_1,&voltage);

		printk(KERN_ERR "original voltage [%d] \n", voltage);
		voltage = voltage * (5100 + 7500) / 7500;

		printk(KERN_ERR "voltage [%d] \n", voltage);
		if(voltage > 1000){
			printk(KERN_ERR "HEADSET \n");
			switch_set_state (&headset_sw_data->sdev, HEADSET_WITH_MIC);
		}else{
			 printk(KERN_ERR "HEADPHONE \n");
			 switch_set_state (&headset_sw_data->sdev, HEADSET_WITHOUT_MIC);
		}
	}else {
		printk(KERN_ERR "EAR SENSE OUT [0] \n");
		switch_set_state (&headset_sw_data->sdev, NO_DEVICE);
		gpio_set_value(TEGRA_GPIO_PX6, 0);
	}
}


static irqreturn_t headset_int_handler(int irq, void *_switch_data)
{
	struct headset_switch_data *switch_data =
	    (struct headset_switch_data *)_switch_data;

	printk(KERN_ERR "##(Headset_det.c)## headset_int_handler()!!\n");
	schedule_delayed_work(&switch_data->delayed_work,	0);

//	struct	synaptics_data *switch_data = _switch_data;
//	unsigned long	flags;

//	spin_lock_irqsave(&synap->lock, flags);

	/* Send the signal only if falling edge generated the irq. */
	//disable_irq(synap->irq);
//	cancel_delayed_work(&synap->dwork);
//	schedule_delayed_work(&synap->dwork, 0);

//	spin_unlock_irqrestore(&synap->lock, flags);

	return IRQ_HANDLED;
}

static void headset_hook_int_handler(void *dev_id)
{
	//CHR_TEST
	printk(KERN_ERR "##(Headset_det.c)## headset_hook_int_handler()!!\n");
}

static int startablet_print_name (struct switch_dev *sdev, char *buf)
{
	switch (switch_get_state(sdev)) {
	case NO_DEVICE:
		return sprintf(buf, "No Device\n");
	case HEADSET_WITH_MIC:
		return sprintf(buf, "Headset with mic\n");
	case HEADSET_WITHOUT_MIC:
		return sprintf(buf, "Headset without mic\n");
	}

	return -EINVAL;
}

static int headsetdet_probe(struct platform_device *pdev)
{
	struct gpio_switch_platform_data *pdata = pdev->dev.platform_data;
	struct headset_switch_data *switch_data;
	int ret = 0;
	int error;
	int temp_value;

	printk( "[%s] \n", __FUNCTION__);

	if (!pdata)
		return -EBUSY;
	switch_data = kzalloc(sizeof(struct headset_switch_data), GFP_KERNEL);
	if (!switch_data)
		return -ENOMEM;

	switch_data->sdev.name = "h2w";//&pdata->name;
	switch_data->sdev.print_name = startablet_print_name;
	switch_data->gpio = &pdata->gpio;
	switch_data->name_on = &pdata->name_on;
	switch_data->name_off = &pdata->name_off;
	switch_data->state_on = &pdata->state_on;
	switch_data->state_off = &pdata->state_off;
//	switch_data->sdev.print_state = switch_gpio_print_state;


	printk(KERN_ERR "##(Headset_det.c)## headsetdet_probe() => headset detection register_dev started..!!\n");	//20100421 bergkmap.cho@lge.com [LGE]

    ret = switch_dev_register(&switch_data->sdev);	//20100421 bergkamp.cho@lge.com Headset Detection by Headset Observer [LGE]
	if (ret < 0)
		goto err_switch_dev_register;

	printk(KERN_ERR "##(Headset_det.c)## headset detection - switch device registered..!!\n");	//20100421 bergkmap.cho@lge.com [LGE]




	gpio_request(TEGRA_GPIO_PX6, "hc_mic_bias");
	tegra_gpio_enable(TEGRA_GPIO_PX6);
	gpio_direction_output(TEGRA_GPIO_PX6,0);
	temp_value = gpio_get_value(TEGRA_GPIO_PX6);
	printk("hc_mic_bias output temp_value %d\n",temp_value);


	switch_data->irq = gpio_to_irq(TEGRA_GPIO_PW3);

	gpio_request(TEGRA_GPIO_PW3, "headset_det");
	tegra_gpio_enable(TEGRA_GPIO_PW3);
	gpio_direction_input(TEGRA_GPIO_PW3);

/*====================== nVidia GPIO Control(S) =======================*/

	INIT_WORK(&switch_data->work, headset_det_work);

	INIT_DELAYED_WORK(&switch_data->delayed_work, type_det_work);
	printk(KERN_ERR "##(Headset_det.c)## headset detection - INIT_WORK() & INIT_DELAYED_WORK() success..!!\n");	//20100421 bergkmap.cho@lge.com [LGE]

	error = request_irq(switch_data->irq, headset_int_handler, (IRQF_TRIGGER_FALLING|IRQF_TRIGGER_RISING), "headset_det",switch_data);

	printk(KERN_ERR "##(Headset_det.c)## headset detection - NvOdmGpioInterruptRegister() success..!!\n");	//20100421 bergkmap.cho@lge.com [LGE]

/*====================== nVidia GPIO Control(E) =======================*/

	/* Perform initial detection */
	headset_sw_data = switch_data;
	type_det_work(&switch_data->work);
	printk(KERN_ERR "##(Headset_det.c)## headset detection - headset_det_work() first detection - success..!!\n"); //20100421 bergkmap.cho@lge.com [LGE]

	return 0;

err_open_gpio_fail:
    switch_dev_unregister(&switch_data->sdev);
err_switch_dev_register:
	kfree(switch_data);
err_open_gpio_pin_acquire_fail:

err_get_interrupt_handler:

	return ret;
}


//20100421 bergkamp.cho@lge.com for nVidia Headset Driver (Remove) [LGE_START]
#if defined(CONFIG_MACH_STARTABLET)
static int headsetdet_remove(struct platform_device *pdev)
{
#if 0
	printk(KERN_ERR "##(Headset_det.c)## headsetdet_remove() : headset detection ended..!!\n");	//20100421 bergkmap.cho@lge.com [LGE]

    struct headset_switch_data *switch_data = platform_get_drvdata(pdev);

	cancel_work_sync(&switch_data->work);
	cancel_delayed_work_sync(&switch_data->delayed_work);

/*====================== nVidia GPIO Control(S) =======================*/
    NvOdmGpioInterruptUnregister(s_hHeadsetHandle.hGpio, s_hHeadsetHandle.h_Headset_Detection, s_hHeadsetHandle.hGpioInterrupt);
    //NvOdmGpioInterruptUnregister(s_hHeadsetHandle.hGpio, s_hHeadsetHandle.h_Hookkey_Detection, s_hHeadsetHandle.hGpioInterrupt);	//20100421 bergkamp.cho@lge.com for Hookkey [LGE]
    NvOdmGpioReleasePinHandle(s_hHeadsetHandle.hGpio, s_hHeadsetHandle.h_Headset_Detection);
    //NvOdmGpioReleasePinHandle(s_hHeadsetHandle.hGpio, s_hHeadsetHandle.h_Hookkey_Detection);	//20100421 bergkamp.cho@lge.com for Hookkey [LGE]
    NvOdmGpioClose(s_hHeadsetHandle.hGpio);
/*====================== nVidia GPIO Control(E) =======================*/

    switch_dev_unregister(&switch_data->sdev);
	//input_unregister_device(switch_data->ip_dev);	//20100421 bergkamp.cho@lge.com for Hookkey [LGE]
	kfree(switch_data);
#endif
	return 0;
}
#endif /* CONFIG_MACH_STARTABLET */
static int headset_suspend(struct platform_device *pdev, pm_message_t state)
{

	return 0;
}

static int headset_resume(struct platform_device *pdev)
{
	int gpio_value;
	int voltage;

	printk(KERN_ERR "headset_resume + \n");

	gpio_value = gpio_get_value(TEGRA_GPIO_PW3);

	if(gpio_value==1){
		gpio_set_value(TEGRA_GPIO_PX6, 1);
		printk(KERN_ERR "EAR SENSE IN [1] \n");
		mdelay(10);
		tps6586x_adc_read(ANLG_1,&voltage);

		printk(KERN_ERR "original voltage [%d] \n", voltage);
		voltage = voltage * (5100 + 7500) / 7500;

		printk(KERN_ERR "voltage [%d] \n", voltage);
		if(voltage > 1000){
			printk(KERN_ERR "HEADSET \n");
			switch_set_state (&headset_sw_data->sdev, HEADSET_WITH_MIC);
		}else{
			 printk(KERN_ERR "HEADPHONE \n");
			 switch_set_state (&headset_sw_data->sdev, HEADSET_WITHOUT_MIC);
		}
	}else {
		printk(KERN_ERR "EAR SENSE OUT [0] \n");
		switch_set_state (&headset_sw_data->sdev, NO_DEVICE);
	}

	printk(KERN_ERR "headset_resume - \n");

	return 0;
}

static struct platform_driver headsetdet_driver = {
	.probe		= headsetdet_probe,
	.remove		= __devexit_p(headsetdet_remove),
	.suspend    = headset_suspend,
	.resume     = headset_resume,
	.driver		= {
		.name	= "star_headset",
		.owner	= THIS_MODULE,
	},
};

static int __init headsetdet_init(void)
{
	printk("headsetdet_init : kernel =>  [%s][%s]\n",__FUNCTION__,__TIME__);
	return platform_driver_register(&headsetdet_driver);
}

static void __exit headsetdet_exit(void)
{
	platform_driver_unregister(&headsetdet_driver);
}

late_initcall(headsetdet_init);
module_exit(headsetdet_exit);

MODULE_AUTHOR("LG Electronics");
MODULE_DESCRIPTION("Startablet Headset Detection Driver");
MODULE_LICENSE("GPL");

