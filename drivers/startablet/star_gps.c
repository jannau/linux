/*
 * arch/arm/mach-tegra/tegra_gps.c
 *
 *GPS device using NVIDIA Tegra ODM kit
 * board_nvodm.c
  */

#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>

#include <asm/mach-types.h>
#include <asm/mach/arch.h>

#include <mach/hardware.h>
#include <mach/gpio-names.h>


#define GPS_GPIO_RESET		TEGRA_GPIO_PJ0
#define GPS_GPIO_POWERON	TEGRA_GPIO_PJ2

static ssize_t gps_gpio_reset_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	int pinValue = gpio_get_value(GPS_GPIO_RESET);

	printk(KERN_INFO "gps_gpio_reset_show\n");

	return sprintf(buf, "%d\n", pinValue);
}

static ssize_t gps_gpio_reset_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	int value;
	printk(KERN_INFO "gps_gpio_reset_store\n");
	sscanf(buf, "%d", &value);
	gpio_set_value(GPS_GPIO_RESET, value);
	return size;
}



static ssize_t gps_gpio_poweron_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	int pinValue = gpio_get_value(GPS_GPIO_POWERON);
	printk(KERN_INFO "gps_gpio_poweron_show\n");

	return sprintf(buf, "%d\n", pinValue);
}

static ssize_t gps_gpio_poweron_store(
                struct device *dev, struct device_attribute *attr,
                const char *buf, size_t size)
{
	int value;
	printk(KERN_INFO "gps_gpio_poweron_store\n");

	sscanf(buf, "%d", &value);
	gpio_set_value(GPS_GPIO_POWERON, value);

	return size;
}

static DEVICE_ATTR(reset, S_IRUGO | S_IWUSR, gps_gpio_reset_show, gps_gpio_reset_store);
static DEVICE_ATTR(poweron, S_IRUGO | S_IWUGO, gps_gpio_poweron_show, gps_gpio_poweron_store);


static int tegra_gps_gpio_probe(struct platform_device *pdev)
{
	int retval;

	printk(KERN_INFO "tegra_gps_reset_probe\n");

    gpio_request(GPS_GPIO_RESET, "gps_reset");
    tegra_gpio_enable(GPS_GPIO_RESET);
#ifdef CONFIG_STARTABLET_GPS_BCM4751
    gpio_direction_output(GPS_GPIO_RESET, 1);
#else
    gpio_direction_output(GPS_GPIO_RESET, 0);
#endif
    gpio_request(GPS_GPIO_POWERON, "gps_poweron");
    tegra_gpio_enable(GPS_GPIO_POWERON);
    gpio_direction_output(GPS_GPIO_POWERON, 0);

	retval = device_create_file(&pdev->dev, &dev_attr_reset);
	if (retval)
		goto error;

	retval = device_create_file(&pdev->dev, &dev_attr_poweron);
	if (retval)
		goto error;

	return retval;
error:
	printk(KERN_ERR "tegra_gps_reset_probe -Error\n");
	device_remove_file(&pdev->dev, &dev_attr_reset);
	device_remove_file(&pdev->dev, &dev_attr_poweron);

	return 0;
}



static int tegra_gps_gpio_remove(struct platform_device *pdev)
{
	printk(KERN_INFO "tegra_gps_reset_remove\n");
	device_remove_file(&pdev->dev, &dev_attr_reset);
	device_remove_file(&pdev->dev, &dev_attr_poweron);
	return 0;
}



// platform_driver
static struct platform_driver tegra_gps_gpio_driver = {
    .probe      = tegra_gps_gpio_probe,
    .remove     = tegra_gps_gpio_remove,
    .driver     = {
        .name   = "tegra_gps_gpio"
    },
};



static int __devinit gps_gpio_init(void)
{
	printk(KERN_INFO "gps_gpio_init\n");
    return platform_driver_register(&tegra_gps_gpio_driver);
}

static void __exit gps_gpio_exit(void)
{
	printk(KERN_INFO "gps_gpio_exit\n");
    platform_driver_unregister(&tegra_gps_gpio_driver);
}

module_init(gps_gpio_init);
module_exit(gps_gpio_exit);

MODULE_DESCRIPTION("heaven GPS Driver");

