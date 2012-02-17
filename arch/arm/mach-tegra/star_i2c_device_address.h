/*
 * arch/arm/mach-tegra/star_i2c_device_address.h
 *
 * Copyright (C) 2010 LG Electronics, Inc.
 *
 * Author:
 *	Andy Choi <andy.choi@lge.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __STAR_I2C_DEVICE_ADDRESS_H_
#define __STAR_I2C_DEVICE_ADDRESS_H_

//Devices that reside on I2C1
//MUIC
#define STAR_I2C_DEVICE_ADDR_MUIC			0x44
#define STAR_I2C_DEVICE_ADDR_TOUCH		0x5B
#define STAR_I2C_DEVICE_ADDR_TOUCH_FIRM_UP		0x35

//Devices that reside on I2C2
#define STAR_I2C_DEVICE_ADDR_WM8994		0x1A
//Accelerometer
#define STAR_I2C_DEVICE_ADDR_ACCELEROMETER	0x19 /*0x0F*/
//Compass
#define STAR_I2C_DEVICE_ADDR_COMPASS		0X0E
//Gyro
#define STAR_I2C_DEVICE_ADDR_GYRO			0x68
//ALS
#define STAR_I2C_DEVICE_ADDR_ALS                        0x23

//Devices that reside on I2C3
//Camera PMIC
#define STAR_I2C_DEVICE_ADDR_CAM_PMIC		0X7D
#define STAR_I2C_DEVICE_ADDR_CAM_IMX072		0x1A
#define STAR_I2C_DEVICE_ADDR_FOCUSER_DW9716		0x0C
#define STAR_I2C_DEVICE_ADDR_CAM_S5K5BAFX        	0x3C
#define STAR_I2C_DEVICE_ADDR_CAM_EEPROM		0x14
//Devices that reside on I2C4
#define STAR_I2C_DEVICE_ADDR_PRI_PMIC		0X34
#define STAR_I2C_DEVICE_ADDR_THERMAL		0X4C
#define STAR_I2C_DEVICE_ADDR_FUEL_GAUGING	0X36    // battery

//Devices that reside on I2C-GPIO
#define STAR_I2C_GPIO_DEVICE_ADDR_ECHO 		0x60

#endif
