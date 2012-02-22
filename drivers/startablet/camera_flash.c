/*
	@brief		 Camera flash LED driver for Q-test
        @author		 hyungmoo.huh@lge.com
	@date		 2010.11.11
*/

#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <asm/gpio.h>
#include <asm/system.h>
//#include <linux/workqueue.h>
//#include <linux/device.h>
#include <linux/string.h>
#include <mach/hardware.h>
#include <asm/uaccess.h>
#include <linux/ioctl.h>
#include <linux/miscdevice.h>

#define T20_GPIO_PBB4		220
#define T20_GPIO_PT2		154
#define T20_GPIO_PA0		0


#define FLASH_LED_EN					T20_GPIO_PBB4
#define FLASH_LED_TORCH				T20_GPIO_PT2
#define FLASH_LED_INH					T20_GPIO_PA0


void camera_flash_control (int onoff)
{

	if (onoff)
	{

//	    gpio_request(FLASH_LED_EN, "FLASH_LED_EN");
//	    tegra_gpio_enable(FLASH_LED_EN);
	    gpio_direction_output(FLASH_LED_EN, 0);

//	    gpio_request(FLASH_LED_TORCH , "FLASH_LED_SET");
//	    tegra_gpio_enable(FLASH_LED_TORCH);
	    gpio_direction_output(FLASH_LED_TORCH, 0);

//	    gpio_request(FLASH_LED_INH, "FLASH_LED_INHIBIT");
//	    tegra_gpio_enable(FLASH_LED_INH);
	    gpio_direction_output(FLASH_LED_INH, 0);

	    mdelay(30);

	    gpio_direction_output(FLASH_LED_TORCH, 1);
	    mdelay(10);
	}
	else
	{
	    gpio_direction_output(FLASH_LED_EN, 0);
	    gpio_direction_output(FLASH_LED_TORCH, 0);
	    gpio_direction_output(FLASH_LED_INH, 0);

	    mdelay(10);
//	    gpio_free(FLASH_LED_EN);
//	    gpio_free(FLASH_LED_TORCH);
//	    gpio_free(FLASH_LED_INH);
//	    mdelay(1);

	}

}

static ssize_t camera_flash_status(struct device *dev,  struct device_attribute *attr,  char *buf)
{
	return sprintf(buf, "FLASH_LED_EN : %d, FLASH_LED_TORCH : %d, FLASH_LED_INH : %d\n", gpio_get_value(FLASH_LED_EN), gpio_get_value(FLASH_LED_TORCH),gpio_get_value(FLASH_LED_INH));
}

static ssize_t camera_flash_onoff(struct device *dev,  struct device_attribute *attr, const char *buf, size_t count)
{
	int val;
	sscanf(buf, "%d", &val);
	if(val){
		camera_flash_control(1);
	//	atomic_set(&accel_flag, 1);
	}else{

		camera_flash_control(0);
	}

	return count;
}

static DEVICE_ATTR(camera_flash_onoff,0644,camera_flash_status,camera_flash_onoff);

static struct attribute *startablet_camera_flash_attributes[] = {
	&dev_attr_camera_flash_onoff.attr,
	NULL
};

static const struct attribute_group startablet_camera_flash_group = {
	.attrs = startablet_camera_flash_attributes,
};



/*---------------------------------------------------------------------------
	 platform device
   ---------------------------------------------------------------------------*/

static int __init camera_flash_probe(struct platform_device *pdev)
{
	int err =0;
       struct device *dev = &pdev->dev;
	printk("[camera_flash] %s, %d \n",__FUNCTION__, __LINE__);
    //GPIO configuration


//    camera_flash_control(0);

	if ((err = sysfs_create_group(&dev->kobj, &startablet_camera_flash_group)))
	{
		printk("[camera_flash] sysfs_create_group FAIL \n");
		return err;
	}

    return 0;
}

static int camera_flash_remove(struct platform_device *pdev)
{

//	gpio_free(FLASH_LED_EN);
//	gpio_free(FLASH_LED_TORCH);
//	gpio_free(FLASH_LED_INH);

    return 0;
}
static struct platform_driver camera_flash_driver = {
    .probe  = camera_flash_probe,
    .remove = camera_flash_remove,
//    .suspend = NULL,
 //   .resume = NULL,
    .driver = {
        .name   = "tegra_camera_flash",
        .owner  = THIS_MODULE,
    },
};


static int __init camera_flash_init(void)
{	int err =0;
	printk("[camera_flash] %s, %d \n",__FUNCTION__, __LINE__);
	err = platform_driver_register(&camera_flash_driver);
	printk("[camera_flash] %s,  error : %d \n",__FUNCTION__, err);

    return 0;
}
static void __exit camera_flash_exit(void)
{
	printk("[camera_flash] %s, %d \n",__FUNCTION__, __LINE__);
	platform_driver_unregister(&camera_flash_driver);
}


module_init(camera_flash_init);
module_exit(camera_flash_exit);

MODULE_AUTHOR("LG Electronics");
MODULE_DESCRIPTION("star power key");
MODULE_LICENSE("GPL");

