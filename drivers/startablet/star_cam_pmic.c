/*
* drivers/startablet/star_cam_pmic.c
*
* Star Camera PMIC i2c client driver
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
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/workqueue.h>
#include <linux/wakelock.h>
#include <linux/freezer.h>
#include <linux/i2c.h>
#include <linux/gpio.h>
#include <mach/gpio-names.h>
#include <mach/hardware.h>
#include "star_cam_pmic.h"
// LDO1 : 2.8V (25uS*4), LDO2 : 2.8V (25uS*4), LDO3 : 1.9V (25uS*2), LDO4 : 1.25V (0us),  (LDO1, LDO3 On)
static u16	 PowerSetForSony5M[] =	{0x0199, 0x0299, 0x034e, 0x0406, 0x0000, 0x0000, 0x080f};
static u16 PowerSetForSS2M[] 	=	{0x011A, 0x02F7, 0x036D, 0x04E5, 0x0000, 0x0000, 0x0805};
static int Cam_Power_EN;
static struct i2c_client* star_cam_power_client;

#define CAMERA_FLASH_EN      TEGRA_GPIO_PBB4
#define CAMERA_FLASH_FINH      TEGRA_GPIO_PA0
#define CAMERA_FLASH_ENSET      TEGRA_GPIO_PT2

static int star_cam_flash_gpio_init(void)
{
	printk("[%s]\n", __FUNCTION__);
	tegra_gpio_enable(CAMERA_FLASH_EN);
	gpio_request(CAMERA_FLASH_EN, "camera_flash_en");
	gpio_direction_output(CAMERA_FLASH_EN, 0);
	gpio_export(CAMERA_FLASH_EN, 0);

	tegra_gpio_enable(CAMERA_FLASH_FINH);
	gpio_request(CAMERA_FLASH_FINH, "camera_flash_finh");
	gpio_direction_output(CAMERA_FLASH_FINH, 0);
	gpio_export(CAMERA_FLASH_FINH, 0);

	tegra_gpio_enable(CAMERA_FLASH_ENSET);
	gpio_request(CAMERA_FLASH_ENSET, "camera_flash_enset");
	gpio_direction_output(CAMERA_FLASH_ENSET, 0);
	gpio_export(CAMERA_FLASH_ENSET, 0);

	return 0;

}

static int star_cam_flash_gpio_release(void)
{

	printk("[%s]\n", __FUNCTION__);

	gpio_set_value(CAMERA_FLASH_EN,0);
	tegra_gpio_disable(CAMERA_FLASH_EN);
	gpio_free(CAMERA_FLASH_EN);

	gpio_set_value(CAMERA_FLASH_FINH,0);
	tegra_gpio_disable(CAMERA_FLASH_FINH);
	gpio_free(CAMERA_FLASH_FINH);


	gpio_set_value(CAMERA_FLASH_ENSET,0);
	tegra_gpio_disable(CAMERA_FLASH_ENSET);
	gpio_free(CAMERA_FLASH_ENSET);

	return 0;

}

int star_cam_Main_power_on(void)
{
	int i =0;
	int ret = 0,retry=0;
	u8 reg;
	u8 data;


	gpio_direction_output(Cam_Power_EN,0);
	mdelay(3);
	for(i=0;i<7;i++) {
		reg = (u8)((PowerSetForSony5M[i]>>8)&0xFF);
		data = (u8)(PowerSetForSony5M[i] & 0x00FF);

		ret = i2c_smbus_write_byte_data(star_cam_power_client, reg, data);
		 while ((ret != 0) && (retry < 5))
		{
			mdelay(1);
			printk("[%s]I2C error err = %d\n ",__FUNCTION__, ret);
			ret = i2c_smbus_write_byte_data(star_cam_power_client, reg, data);
			retry++;
		}
	}

	gpio_set_value(Cam_Power_EN,1);
	mdelay(3);
	return 0;
}

int star_cam_power_off(void)
{
	//int i =0;
	//int ret = 0;
	//u8 reg;
	//u8 data;

//	printk("[%s]\n", __FUNCTION__);
/*
	ret = i2c_smbus_write_byte_data(star_cam_power_client, 0x08, 0x80);
	if(ret !=0)
	{
		printk("[%s]\n I2C error err = %d",__FUNCTION__, ret);
	}
	mdelay(1);
*/
	gpio_set_value(Cam_Power_EN,0);
	mdelay(3);
	return 0;
}
int star_cam_VT_power_on(void)
{
	int i =0;
	int ret = 0,retry=0;
	u8 reg;
	u8 data;

	printk("[%s]\n", __FUNCTION__);

//	tegra_gpio_enable(Cam_Power_EN);
	gpio_direction_output(Cam_Power_EN,0);
/*	mdelay(5);
	ret = i2c_smbus_write_byte_data(star_cam_power_client, 0x80,0x00);
	if(ret<0)
	{
		printk("[%s] I2C error err = %d\n",__FUNCTION__, ret);
		return ret;
	}
*/
	mdelay(5);
	for(i=0;i<7;i++) {
		reg = (u8)((PowerSetForSS2M[i]>>8)&0xFF);
		data = (u8)(PowerSetForSS2M[i] & 0x00FF);

		ret = i2c_smbus_write_byte_data(star_cam_power_client, reg, data);
		 while ((ret != 0) && (retry < 5))
		{
			mdelay(1);
			printk("[%s]I2C error err = %d\n ",__FUNCTION__, ret);
			ret = i2c_smbus_write_byte_data(star_cam_power_client, reg, data);
			retry++;
		}
	}
	mdelay(3);
	gpio_set_value(Cam_Power_EN,1);
	mdelay(5);

	return 0;
}

static int __init star_cam_pmic_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct i2c_client *pclient = client;
	unsigned short addr = pclient->addr;
	hw_rev board_rev=0;
	star_cam_power_client = client;
	star_cam_flash_gpio_init();
	board_rev = get_hw_rev();

	if (board_rev == REV_E || board_rev == REV_F)
		Cam_Power_EN = TEGRA_GPIO_PS5;
	else
		Cam_Power_EN = TEGRA_GPIO_PQ1;

	gpio_request(Cam_Power_EN, "Cam_Power_En");
	tegra_gpio_enable(Cam_Power_EN);
	gpio_direction_output(Cam_Power_EN,0);
	printk("[%s] - client address : %x\n", __FUNCTION__, addr);

	return 0;
}

static int star_cam_pmic_remove(struct i2c_client *client)
{
	gpio_set_value(Cam_Power_EN,0);
	tegra_gpio_disable(Cam_Power_EN);
	gpio_free(Cam_Power_EN);

	star_cam_flash_gpio_release();

	return 0;
}

static int star_cam_pmic_suspend(struct device *dev)
{
	printk( "%s \n", __func__);
	return 0;
}

static int star_cam_pmic_resume(struct device *dev)
{
	printk( "%s \n", __func__);
	return 0;
}

static const struct i2c_device_id star_cam_pmic_ids[] = {
	{ "star_cam_pmic", 0 },
	{ /* end of list */ },
};

static const struct dev_pm_ops star_cam_pmic_pm_ops = {
	.suspend	= star_cam_pmic_suspend,
	.resume		= star_cam_pmic_resume,
};

static struct i2c_driver star_cam_pmic_driver = {
	.probe = star_cam_pmic_probe,
	.remove = star_cam_pmic_remove,
	.id_table = star_cam_pmic_ids,
	.driver = {
		.name = "star_cam_pmic",
		.owner = THIS_MODULE,
		.pm		= &star_cam_pmic_pm_ops,
	},
};

static int __devinit star_cam_pmic_init(void)
{
	return i2c_add_driver(&star_cam_pmic_driver);
}

static void __exit star_cam_pmic_exit(void)
{
	i2c_del_driver(&star_cam_pmic_driver);
}

module_init(star_cam_pmic_init);
module_exit(star_cam_pmic_exit);

MODULE_AUTHOR("andy.choi@lge.com");
MODULE_DESCRIPTION("star Camera PMIC Driver");
MODULE_LICENSE("GPL");
