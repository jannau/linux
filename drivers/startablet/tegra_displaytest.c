#include <linux/kernel.h>
#include <linux/device.h>

#include <linux/platform_device.h>
#include <linux/gpio.h>

#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <mach/gpio.h>

#include <linux/delay.h>
#include <mach/gpio-names.h>

#define LCD_GPIO_PIN	TEGRA_GPIO_PB2
#define	Backlight_GPIO_PIN	TEGRA_GPIO_PS6
#define HDMI_GPIO_PIN	TEGRA_GPIO_PN7

static ssize_t lcdtest_control_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	static int pinValue_lcd, pinValue_bl;
	pinValue_lcd = gpio_get_value(LCD_GPIO_PIN);
	pinValue_bl = gpio_get_value(Backlight_GPIO_PIN);

	return sprintf(buf, "lcd pin: %d bl pin: %d\n", pinValue_lcd, pinValue_bl);
}
static ssize_t hdmi_connection_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	static int pinValue_hdmi = 0;
	pinValue_hdmi = gpio_get_value(HDMI_GPIO_PIN);

	return sprintf(buf, "%d \n", pinValue_hdmi);
}

static void lcd_control(int i)
{
	if(i)
	{
		gpio_set_value(LCD_GPIO_PIN,i);
		msleep(500);
		gpio_set_value(Backlight_GPIO_PIN,i);
	}else
	{
		gpio_set_value(Backlight_GPIO_PIN,i);
		msleep(500);
		gpio_set_value(LCD_GPIO_PIN,i);
	}
}

static ssize_t lcdtest_control_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	int cmd = -1;

	printk(KERN_ERR "lcdtest_control_store\n");
	sscanf(buf, "%d", &cmd);
	if (cmd == 0 || cmd == 1)
		lcd_control(cmd);
	else return -1;

	return size;
}

static ssize_t hdmi_connection_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	return 0;
}
static DEVICE_ATTR(control, S_IRUGO | S_IWUGO, lcdtest_control_show, lcdtest_control_store);
static DEVICE_ATTR(connection, S_IRUGO | S_IWUGO, hdmi_connection_show, hdmi_connection_store);

static int tegra_displaytest_probe(struct platform_device *pdev)
{

	if (device_create_file(&pdev->dev, &dev_attr_control))
	{
		printk("lcd test control file create error \n");
		goto create_file_error;
	}
	if (device_create_file(&pdev->dev, &dev_attr_connection))
	{
		printk("hdmi test control file create error \n");
		goto create_file_error_hdmi;
	}

	return 0;

create_file_error:
	device_remove_file(&pdev->dev, &dev_attr_control);
create_file_error_hdmi:
	device_remove_file(&pdev->dev, &dev_attr_connection);

	return -ENOSYS;
}


static int tegra_displaytest_remove(struct platform_device *pdev)
{
	printk(KERN_ERR "tegra_displaytest_remove\n");
	device_remove_file(&pdev->dev, &dev_attr_control);
	device_remove_file(&pdev->dev, &dev_attr_connection);

	gpio_free(LCD_GPIO_PIN);
	gpio_free(Backlight_GPIO_PIN);
	gpio_free(HDMI_GPIO_PIN);

	return 0;
}

static struct platform_driver tegra_displaytest_driver = {
	.probe      = tegra_displaytest_probe,
	.remove     = tegra_displaytest_remove,
	.driver     = {
		.name   = "tegra_displaytest"
	},
};

static int __devinit tegra_displaytest_init(void)
{
	printk(KERN_ERR "tegra_displaytest_init\n");
	return platform_driver_register(&tegra_displaytest_driver);
}

static void __exit tegra_displaytest_exit(void)
{
	printk(KERN_ERR "tegra_displaytest_exit\n");
	platform_driver_unregister(&tegra_displaytest_driver);
}

module_init(tegra_displaytest_init);
module_exit(tegra_displaytest_exit);

MODULE_DESCRIPTION("Tegra DISPLAY test Driver");
