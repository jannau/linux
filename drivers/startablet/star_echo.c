
/*
*Echo Canceller Driver
*
*Copyright (C) 2010 LGE Inc.
*
*This program is free software; you can redistribute if and/or modify
*it under the terms of the GNU General Public License version 2 as
*published by the Free Software Foundation.
*
*/

#include <linux/mutex.h>

#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/i2c.h>
#include <linux/gpio.h>
#include <mach/gpio-names.h>
#include <linux/wakelock.h>

#define ECHO_SYNC_WORD			0xFCF3
#define ECHO_MEM_WRITE			0x3B
#define ECHO_MEM_READ             	0x37
#define ECHO_REG_READ             	0x60
#define READ_LOW_BYTE             	0x25
#define READ_HIGH_BYTE            	0x26

#define GPIO_ECHO_BP_N		        TEGRA_GPIO_PJ6
#define GPIO_ECHO_PWDN_N          	TEGRA_GPIO_PJ5
#define GPIO_ECHO_RST_N           	TEGRA_GPIO_PU4
#define GPIO_ECHO_SCL             	TEGRA_GPIO_PI5
#define GPIO_ECHO_SDA             	TEGRA_GPIO_PH1

DEFINE_MUTEX(bypass_mutex);
DEFINE_MUTEX(voip_mutex);
DEFINE_MUTEX(voip_headset_mutex);

static struct i2c_client *echo_client;

struct wake_lock wlock;

unsigned char rd_bytes[2][4] ={
	{ (ECHO_SYNC_WORD >> 8 ) & 0xFF, (ECHO_SYNC_WORD >> 0 ) & 0xFF, ECHO_REG_READ, READ_LOW_BYTE },
	{ (ECHO_SYNC_WORD >> 8 ) & 0xFF, (ECHO_SYNC_WORD >> 0 ) & 0xFF, ECHO_REG_READ, READ_HIGH_BYTE }
};

static int echo_write_register(int reg, int value);
static int echo_read_register(int reg);
void echo_set_bypass_parameters(void);
static ssize_t rw_register(struct device *dev,
        struct device_attribute *attr, const char *buf, size_t size);
void echo_set_parameters(void);
int echo_loop_test(void);

static int echo_write_register(int reg, int value)
{
	unsigned char arr[7];
	struct i2c_msg msg[] = {
		{ .addr = echo_client->addr, .flags = 0, .buf = &arr[0], .len = 1 },
		{ .addr = echo_client->addr, .flags = 0, .buf = &arr[1], .len = 1 },
		{ .addr = echo_client->addr, .flags = 0, .buf = &arr[2], .len = 1 },
		{ .addr = echo_client->addr, .flags = 0, .buf = &arr[3], .len = 1 },
		{ .addr = echo_client->addr, .flags = 0, .buf = &arr[4], .len = 1 },
		{ .addr = echo_client->addr, .flags = 0, .buf = &arr[5], .len = 1 },
		{ .addr = echo_client->addr, .flags = 0, .buf = &arr[6], .len = 1 },
	};

	arr[0] = (ECHO_SYNC_WORD >> 8 ) & 0xFF;
	arr[1] = (ECHO_SYNC_WORD >> 0 ) & 0xFF;
	arr[2] = ECHO_MEM_WRITE;
	arr[3] = (reg >> 8) & 0xff;
	arr[4] = (reg >> 0) & 0xff;
	arr[5] = (value >> 8) & 0xff;
	arr[6] = (value >> 0) & 0xff;
/*
	if (i2c_transfer(echo_client->adapter, msg, 7) != 7)
	{
		dev_err(&echo_client->dev, "i2c write error\n");
		return -EIO;
	}
*/
	if ( i2c_transfer(echo_client->adapter, &msg[0], 1)!= 1) return -EIO;
	if ( i2c_transfer(echo_client->adapter, &msg[1], 1)!= 1) return -EIO;
	if ( i2c_transfer(echo_client->adapter, &msg[2], 1)!= 1) return -EIO;
	if ( i2c_transfer(echo_client->adapter, &msg[3], 1)!= 1) return -EIO;
	if ( i2c_transfer(echo_client->adapter, &msg[4], 1)!= 1) return -EIO;
	if ( i2c_transfer(echo_client->adapter, &msg[5], 1)!= 1) return -EIO;
	if ( i2c_transfer(echo_client->adapter, &msg[6], 1)!= 1) return -EIO;

	return 0;
}

static int echo_read_register(int reg)
{
	int ret_lo = 0, ret_hi = 0,value = 0;
	unsigned char w_buf[5], r_buf[2];
	struct i2c_msg msg[] = {
		{ .addr = echo_client->addr, .flags = 0, .buf = &w_buf[0], .len = 1 },
		{ .addr = echo_client->addr, .flags = 0, .buf = &w_buf[1], .len = 1 },
		{ .addr = echo_client->addr, .flags = 0, .buf = &w_buf[2], .len = 1 },
		{ .addr = echo_client->addr, .flags = 0, .buf = &w_buf[3], .len = 1 },
		{ .addr = echo_client->addr, .flags = 0, .buf = &w_buf[4], .len = 1 },
		{ .addr = echo_client->addr, .flags = 0, .buf = &rd_bytes[0][0], .len = 1 },
		{ .addr = echo_client->addr, .flags = 0, .buf = &rd_bytes[0][1], .len = 1 },
		{ .addr = echo_client->addr, .flags = 0, .buf = &rd_bytes[0][2], .len = 1 },
		{ .addr = echo_client->addr, .flags = 0, .buf = &rd_bytes[0][3], .len = 1 },
		{ .addr = echo_client->addr, .flags = I2C_M_RD, .buf=&r_buf[0], .len = 1},
		{ .addr = echo_client->addr, .flags = 0, .buf = &rd_bytes[1][0], .len = 1 },
		{ .addr = echo_client->addr, .flags = 0, .buf = &rd_bytes[1][1], .len = 1 },
		{ .addr = echo_client->addr, .flags = 0, .buf = &rd_bytes[1][2], .len = 1 },
		{ .addr = echo_client->addr, .flags = 0, .buf = &rd_bytes[1][3], .len = 1 },
		{ .addr = echo_client->addr, .flags = I2C_M_RD, .buf=&r_buf[1], .len = 1},
	};

	w_buf[0] = (ECHO_SYNC_WORD >> 8 ) & 0xFF;
	w_buf[1] = (ECHO_SYNC_WORD >> 0 ) & 0xFF;
	w_buf[2] = ECHO_MEM_READ;
	w_buf[3] = (reg >> 8) & 0xFF ;
	w_buf[4] = (reg >> 0) & 0xFF ;
/*
	if (i2c_transfer(echo_client->adapter, msg, 15) != 15)
	{
		dev_err(&echo_client->dev, "i2c read error\n");
		return -EIO;
	}
*/

	if (i2c_transfer(echo_client->adapter, &msg[0], 1) != 1) return -EIO;
	if (i2c_transfer(echo_client->adapter, &msg[1], 1) != 1) return -EIO;
	if (i2c_transfer(echo_client->adapter, &msg[2], 1) != 1) return -EIO;
	if (i2c_transfer(echo_client->adapter, &msg[3], 1) != 1) return -EIO;
	if (i2c_transfer(echo_client->adapter, &msg[4], 1) != 1) return -EIO;
	if (i2c_transfer(echo_client->adapter, &msg[5], 1) != 1) return -EIO;
	if (i2c_transfer(echo_client->adapter, &msg[6], 1) != 1) return -EIO;
	if (i2c_transfer(echo_client->adapter, &msg[7], 1) != 1) return -EIO;
	if (i2c_transfer(echo_client->adapter, &msg[8], 1) != 1) return -EIO;
	if (i2c_transfer(echo_client->adapter, &msg[9], 1) != 1) return -EIO;
	if (i2c_transfer(echo_client->adapter, &msg[10], 1)!= 1) return -EIO;
	if (i2c_transfer(echo_client->adapter, &msg[11], 1)!= 1) return -EIO;
	if (i2c_transfer(echo_client->adapter, &msg[12], 1)!= 1) return -EIO;
	if (i2c_transfer(echo_client->adapter, &msg[13], 1)!= 1) return -EIO;
	if (i2c_transfer(echo_client->adapter, &msg[14], 1)!= 1) return -EIO;

	ret_lo = r_buf[0];
	ret_hi = r_buf[1];
	value = ( ret_hi << 8 ) | ret_lo ;

	return value;
}

static ssize_t fm31_status(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	printk("0x23f8 = 0x%x \n", echo_read_register (0x23f8));
	printk("0x232c = 0x%x \n", echo_read_register (0x232C));
	printk("0x2304 = 0x%x \n", echo_read_register (0x2304));
	printk("0x2308 = 0x%x \n", echo_read_register (0x2308));
	printk("0x230b = 0x%x \n", echo_read_register (0x230B));
	printk("0x230d = 0x%x \n", echo_read_register (0x230D));
	printk("0x230e = 0x%x \n", echo_read_register (0x230E));
	printk("0x2311 = 0x%x \n", echo_read_register (0x2311));
	printk("0x2312 = 0x%x \n", echo_read_register (0x2312));
	printk("0x2315 = 0x%x \n", echo_read_register (0x2315));
	printk("0x2316 = 0x%x \n", echo_read_register (0x2316));
	printk("0x231a = 0x%x \n", echo_read_register (0x231A));
	printk("0x231d = 0x%x \n", echo_read_register (0x231D));
	printk("0x2352 = 0x%x \n", echo_read_register (0x2352));
	printk("0x2392 = 0x%x \n", echo_read_register (0x2392));
	printk("0x230c = 0x%x \n", echo_read_register (0x230C));
	return 0;
}

void echo_set_bypass_parameters()
{
	int i = 0, ret = 0, rval = 0, rval1 = 0, rval2 = 0;

	int err = 0, fm31 = 0, rw_count = 0;

	printk("\n[FM31] Start bypass parameter writing !\n");

	ret = mutex_trylock( &bypass_mutex );
	if (ret != 0) {		//get the lock
		do {
			if (i > 3) {
				printk("[FM31]bypass parameter try to write 4 times but fail.\n");
				break;
			}
			err = 0, fm31 = 0, rw_count = 0;

			gpio_set_value(GPIO_ECHO_PWDN_N, 1);
			gpio_set_value(GPIO_ECHO_RST_N,0);
			msleep(3);
			gpio_set_value(GPIO_ECHO_RST_N,1);

			msleep(10);

			if ( echo_write_register(0x2308, 0x005F) != 0) err = 1;
			if ( echo_write_register(0x232C, 0x0025) != 0) err = 1;
			if ( echo_write_register(0x2300, 0x0004) != 0) err = 1;
			if ( echo_write_register(0x2302, 0x0024) != 0) err = 1;
			if ( echo_write_register(0x23F7, 0x001E) != 0) err = 1;
			if ( echo_write_register(0x230C, 0x0000) != 0) err = 1;
			msleep(50);

		//Check FM31 state//
			rval = echo_read_register(0x230C);
			rval1 = echo_read_register(0x2300);
			rval2 = echo_read_register(0x3FE4);
			if ((rval != 0x5A5A) || (rval1 != 0x0004) || (rval2 != 0x129E)) {
					printk("[FM31]Bypass Parameter Write  Fail :  0x230C = %x, 0x2300 = %x, 0x3FE4 = %x  \n",rval, rval1, rval2);

						while(rw_count < 10){
							echo_write_register(0x3FE4, 0x361E);
							msleep(50);
							rval = echo_read_register(0x3FE4);

							if(rval == 0x361E) {
								printk("[FM31] 0x3FE4 rewrite %d times and success. \n", rw_count);
								break;
							} else {
								printk("[FM31] 0x3FE4 rewrite Fail, Return value is not 0x361E, Return value =  %x  \n", rval);
							}
							rw_count++;
						}
				fm31 = 1;
			}

			if (err !=0) printk("[FM31] I2C write Fail\n");
			if (fm31 != 0) printk("[FM31] FM31 DSP running Fail \n");
			i++;
		} while ((err != 0) || (fm31 != 0));

		mutex_unlock(&bypass_mutex);
		return ;
	} else {
		return ;// Did not get the lock
	}
}

void echo_set_parameters()
{
	int i = 0, ret = 0, rval = 0, rval1 = 0, rval2 = 0;

	int err = 0, fm31 = 0, rw_count = 0;

	ret = mutex_trylock( &voip_mutex );
	if (ret != 0) {		//get the lock

		do {
			if (i > 3) {
				printk("[FM31] voip parameter try to write 4 times but fail.\n");
				break;
			}
			err = 0, fm31 = 0, rw_count = 0;

			gpio_set_value(GPIO_ECHO_PWDN_N, 1);
			gpio_set_value(GPIO_ECHO_RST_N,0);
			msleep(3);
			gpio_set_value(GPIO_ECHO_RST_N,1);
			msleep(10);

			if ( echo_write_register(0x2300, 0x0000) != 0) err = 1;
			if ( echo_write_register(0x2302, 0x0024) != 0) err = 1;
			if ( echo_write_register(0x23F7, 0x0048) != 0) err = 1;
			if ( echo_write_register(0x22C0, 0x8101) != 0) err = 1;
			if ( echo_write_register(0x22C1, 0xF9C2) != 0) err = 1;
			if ( echo_write_register(0x22C2, 0xFCE1) != 0) err = 1;
			if ( echo_write_register(0x22C3, 0xB514) != 0) err = 1;
			if ( echo_write_register(0x22C4, 0xDA8A) != 0) err = 1;
			if ( echo_write_register(0x2308, 0x005F) != 0) err = 1;
			if ( echo_write_register(0x230B, 0x0001) != 0) err = 1;
			if ( echo_write_register(0x230D, 0x0380) != 0) err = 1;
			if ( echo_write_register(0x230E, 0x0400) != 0) err = 1;
			if ( echo_write_register(0x2311, 0x0101) != 0) err = 1;
			if ( echo_write_register(0x2312, 0x6981) != 0) err = 1;
			if ( echo_write_register(0x2315, 0x03DD) != 0) err = 1;
			if ( echo_write_register(0x2316, 0x0044) != 0) err = 1;
			if ( echo_write_register(0x2317, 0x1000) != 0) err = 1;
			if ( echo_write_register(0x231D, 0x0200) != 0) err = 1;
			if ( echo_write_register(0x232C, 0x0025) != 0) err = 1;
			if ( echo_write_register(0x233A, 0x7000) != 0) err = 1;
			if ( echo_write_register(0x233B, 0x0280) != 0) err = 1;
			if ( echo_write_register(0x2351, 0x4000) != 0) err = 1;
			if ( echo_write_register(0x2352, 0x2000) != 0) err = 1;
			if ( echo_write_register(0x2391, 0x000D) != 0) err = 1;
			if ( echo_write_register(0x2392, 0x0400) != 0) err = 1;
			if ( echo_write_register(0x23E0, 0x0000) != 0) err = 1;
			if ( echo_write_register(0x23F8, 0x4003) != 0) err = 1;
			if ( echo_write_register(0x2336, 0x0001) != 0) err = 1;
			if ( echo_write_register(0x2301, 0x0002) != 0) err = 1;
			if ( echo_write_register(0x2353, 0x2000) != 0) err = 1;
			if ( echo_write_register(0x2318, 0x0400) != 0) err = 1;
			if ( echo_write_register(0x2319, 0x0400) != 0) err = 1;
			if ( echo_write_register(0x231C, 0x2000) != 0) err = 1;
			if ( echo_write_register(0x23D9, 0x0C7D) != 0) err = 1;
			if ( echo_write_register(0x23DA, 0x544A) != 0) err = 1;
			if ( echo_write_register(0x23DB, 0x7FFF) != 0) err = 1;
			if ( echo_write_register(0x23DC, 0x7FFF) != 0) err = 1;
			if ( echo_write_register(0x2344, 0x0A00) != 0) err = 1;
			if ( echo_write_register(0x2325, 0x7FFF) != 0) err = 1;
			if ( echo_write_register(0x23A9, 0x0800) != 0) err = 1;
			if ( echo_write_register(0x23B0, 0x3000) != 0) err = 1;
			if ( echo_write_register(0x231B, 0x0001) != 0) err = 1;
			if ( echo_write_register(0x2335, 0x000F) != 0) err = 1;
			if ( echo_write_register(0x23FC, 0x000D) != 0) err = 1;
			if ( echo_write_register(0x23CD, 0x4000) != 0) err = 1;
			if ( echo_write_register(0x23CE, 0x4000) != 0) err = 1;
			if ( echo_write_register(0x230C, 0x0000) != 0) err = 1;
			msleep(50);

		//Check FM31 state//
			rval = echo_read_register(0x230C);
			rval1 = echo_read_register(0x2300);
			rval2 = echo_read_register(0x3FE4);
			if ((rval != 0x5a5a) || (rval1 != 0x0000) || (rval2 != 0x12C8)) {
					printk("[FM31]Normal Parameter Write  Fail :  0x230C = %x, 0x2300 = %x, 0x3FE4 = %x  \n",rval, rval1, rval2);

						while(rw_count < 10){
							echo_write_register(0x3FE4, 0x361E);
							msleep(50);
							rval = echo_read_register(0x3FE4);

							if(rval == 0x361E) {
								printk("[FM31] 0x3FE4 rewrite %d times and success. \n", rw_count);
								break;
							} else {
								printk("[FM31] 0x3FE4 rewrite Fail, Return value is not 0x361E, Return value =  %x  \n", rval);
							}
							rw_count++;
						}
				fm31 = 1;
			}

			if (err !=0) printk("[FM31] I2C write Fail\n");
			if (fm31 != 0) printk("[FM31] FM31 DSP running Fail \n");
			i++;
		} while ((err != 0) || (fm31 != 0));

		mutex_unlock(&voip_mutex);
		return ;

	} else {
		return ;// Did not get the lock
	}

}

void echo_set_headset_parameters()
{
	int i = 0, ret = 0, rval = 0, rval1 = 0, rval2 = 0;

	int err = 0, fm31 = 0, rw_count = 0;


	ret = mutex_trylock( &voip_headset_mutex );
	if (ret != 0) {		//get the lock

		do {
			if (i > 3) {
				printk("[FM31] voip headset parameter try to write 4 times but fail.\n");
				break;
			}
			err = 0, fm31 = 0, rw_count = 0;

			gpio_set_value(GPIO_ECHO_PWDN_N, 1);
			gpio_set_value(GPIO_ECHO_RST_N,0);
			msleep(3);
			gpio_set_value(GPIO_ECHO_RST_N,1);
			msleep(10);

			if ( echo_write_register(0x2300, 0x0000) != 0) err = 1;
			if ( echo_write_register(0x2302, 0x0024) != 0) err = 1;
			if ( echo_write_register(0x23F7, 0x0048) != 0) err = 1;
			if ( echo_write_register(0x22C0, 0x8101) != 0) err = 1;
			if ( echo_write_register(0x22C1, 0x0000) != 0) err = 1;
			if ( echo_write_register(0x22C2, 0x0000) != 0) err = 1;
			if ( echo_write_register(0x22C3, 0x0000) != 0) err = 1;
			if ( echo_write_register(0x22C4, 0x0000) != 0) err = 1;
			if ( echo_write_register(0x2308, 0x005F) != 0) err = 1;
			if ( echo_write_register(0x230B, 0x0001) != 0) err = 1;
			if ( echo_write_register(0x230D, 0x0400) != 0) err = 1;
			if ( echo_write_register(0x230E, 0x0100) != 0) err = 1;
			if ( echo_write_register(0x2311, 0x0101) != 0) err = 1;
			if ( echo_write_register(0x2315, 0x03DD) != 0) err = 1;
			if ( echo_write_register(0x2316, 0x0045) != 0) err = 1;
			if ( echo_write_register(0x2317, 0x1000) != 0) err = 1;
			if ( echo_write_register(0x231D, 0x0200) != 0) err = 1;
			if ( echo_write_register(0x232C, 0x0025) != 0) err = 1;
			if ( echo_write_register(0x233A, 0x0100) != 0) err = 1;
			if ( echo_write_register(0x233B, 0x0004) != 0) err = 1;
			if ( echo_write_register(0x2351, 0x2000) != 0) err = 1;
			if ( echo_write_register(0x2352, 0x2000) != 0) err = 1;
			if ( echo_write_register(0x2391, 0x000D) != 0) err = 1;
			if ( echo_write_register(0x2392, 0x0400) != 0) err = 1;
			if ( echo_write_register(0x23E0, 0x0000) != 0) err = 1;
			if ( echo_write_register(0x23F8, 0x4003) != 0) err = 1;
			if ( echo_write_register(0x2336, 0x0001) != 0) err = 1;
			if ( echo_write_register(0x2301, 0x0002) != 0) err = 1;
			if ( echo_write_register(0x2353, 0x2000) != 0) err = 1;
			if ( echo_write_register(0x2318, 0x0400) != 0) err = 1;
			if ( echo_write_register(0x2319, 0x0400) != 0) err = 1;
			if ( echo_write_register(0x231C, 0x2000) != 0) err = 1;
			if ( echo_write_register(0x23D9, 0x0C7D) != 0) err = 1;
			if ( echo_write_register(0x23DA, 0x544A) != 0) err = 1;
			if ( echo_write_register(0x23DB, 0x7FFF) != 0) err = 1;
			if ( echo_write_register(0x23DC, 0x7FFF) != 0) err = 1;
			if ( echo_write_register(0x2344, 0x0CCC) != 0) err = 1;
			if ( echo_write_register(0x2325, 0x7FFF) != 0) err = 1;
			if ( echo_write_register(0x23A9, 0x0800) != 0) err = 1;
			if ( echo_write_register(0x23B0, 0x3000) != 0) err = 1;
			if ( echo_write_register(0x231B, 0x0001) != 0) err = 1;
			if ( echo_write_register(0x2335, 0x000F) != 0) err = 1;
			if ( echo_write_register(0x23FC, 0x000D) != 0) err = 1;
			if ( echo_write_register(0x23CD, 0x4000) != 0) err = 1;
			if ( echo_write_register(0x23CE, 0x4000) != 0) err = 1;
			if ( echo_write_register(0x2312, 0x6881) != 0) err = 1;
			if ( echo_write_register(0x230C, 0x0000) != 0) err = 1;
			msleep(50);

		//Check FM31 state//
			rval = echo_read_register(0x230C);
			rval1 = echo_read_register(0x2300);
			rval2 = echo_read_register(0x3FE4);
			if ((rval != 0x5A5A) || (rval1 != 0x0000) || (rval2 != 0x12C8)) {
					printk("[FM31]Headset Parameter Write  Fail :  0x230C = %x, 0x2300 = %x, 0x3FE4 = %x  \n",rval, rval1, rval2);

						while(rw_count < 10){
							echo_write_register(0x3FE4, 0x361E);
							msleep(50);
							rval = echo_read_register(0x3FE4);

							if(rval == 0x361E) {
								printk("[FM31] 0x3FE4 rewrite %d times and success. \n", rw_count);
								break;
							} else {
								printk("[FM31] 0x3FE4 rewrite Fail, Return value is not 0x361E, Return value =  %x  \n", rval);
							}
							rw_count++;
						}
				fm31 = 1;
                        }

			if (err !=0) printk("[FM31] I2C write Fail\n");
			if (fm31 != 0) printk("[FM31] FM31 DSP running Fail \n");
			i++;
		} while ((err != 0) || (fm31 != 0));

		mutex_unlock(&voip_headset_mutex);
		return ;

	} else {
		return ;// Did not get the lock
	}

}

static ssize_t rw_register(struct device *dev,
        struct device_attribute *attr, const char *buf, size_t size)
{
        char cmd=0;
	int prereg=0, wrval=0;
        int value=0;
	int ret = 0, pass = 0, i = 0;
        sscanf(buf, "%c %x %x", &cmd, &prereg, &wrval);

        if (cmd == 'r') {
                value = echo_read_register(prereg);
                printk( "\n[FM31] Read register 0x%x =  0x%x \n\n", prereg, value);
        } else if (cmd == 'w') {
                value = echo_write_register(prereg,wrval);
        } else if (cmd == 'b') {
		//printk("\n[FM31] bypass \n");
                echo_set_bypass_parameters();
        } else if (cmd == 'n') {
		printk("\n[FM31] set_main_mic_speaker_parameter  \n");
                echo_set_parameters();
        } else if (cmd == 'h') {
		printk("\n[FM31] set_headset_parameter  \n");
                echo_set_headset_parameters();
        }else if (cmd == 'p') {
                //printk("\n[FM31] prevent_suspend  \n");
                wake_lock(&wlock);
        } else if (cmd == 'a') {
                //printk("\n[FM31] allow_suspend\n");
                 wake_unlock(&wlock);
        }

        return size;
}

static ssize_t bypass_status(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	int value=0;
	value = gpio_get_value(GPIO_ECHO_BP_N);
	printk("ECHO BP = %d\n", value);
	return sprintf(buf, "%d\n", value);
}

static ssize_t bypass_control(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	int mode = 0;

	if (sscanf(buf, "%d", &mode) != 1) {
		return size;
	}
	if(mode == 1) {
	 	msleep(10);
		//gpio_set_value(GPIO_ECHO_BP_N, 1);
	} else if(mode == 0) {
	 	msleep(10);
		//gpio_set_value(GPIO_ECHO_BP_N, 0);
	}
	printk("Echo Bypass mode = %d \n", gpio_get_value(GPIO_ECHO_BP_N));
	return size;
}

static DEVICE_ATTR(fm31_reg, S_IRUGO | S_IWUSR | S_IROTH |S_IWOTH | S_IWUGO, fm31_status, rw_register);
static DEVICE_ATTR(fm31_bypass, S_IRUGO | S_IWUSR | S_IROTH |S_IWOTH | S_IWUGO, bypass_status, bypass_control);

static int __init echocancel_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int i = 0, rval = 0, rval1 = 0, rval2 = 0;

	int err = 0, fm31 = 0, rw_count = 0;

	echo_client = kzalloc(sizeof(*echo_client), GFP_KERNEL);

	if (!echo_client) {
		return -ENOMEM;
	}

	wake_lock_init(&wlock, WAKE_LOCK_SUSPEND, "AudioOutLock");

	memset(echo_client, 0, sizeof(*echo_client));
	echo_client = client;
	i2c_set_clientdata(client, echo_client);

	gpio_request(GPIO_ECHO_SCL, "echo_scl");
	tegra_gpio_enable(GPIO_ECHO_SCL);
	gpio_request(GPIO_ECHO_SDA, "echo_sda");
	tegra_gpio_enable(GPIO_ECHO_SDA);

	gpio_request(GPIO_ECHO_BP_N, "echo_bp_n");
	tegra_gpio_enable(GPIO_ECHO_BP_N);
	gpio_direction_output(GPIO_ECHO_BP_N, 1);

	gpio_request(GPIO_ECHO_PWDN_N, "echo_pwdn_n");
	tegra_gpio_enable(GPIO_ECHO_PWDN_N);
	gpio_direction_output(GPIO_ECHO_PWDN_N, 1);

	gpio_request(GPIO_ECHO_RST_N, "echo_rst_n");
	tegra_gpio_enable(GPIO_ECHO_RST_N);
	gpio_direction_output(GPIO_ECHO_RST_N, 1);

	if (device_create_file(&echo_client->dev, &dev_attr_fm31_reg)) {
		printk("[FM31] reg_rw file create -error \n");
	}

	if (device_create_file(&echo_client->dev, &dev_attr_fm31_bypass)) {
		printk("[FM31] bypass control  file create -error \n");

	}

		do {
			if (i > 3) {
				printk("[FM31]bypass parameter try to write 4 times but fail.\n");
				break;
			}
			err = 0, fm31 = 0, rw_count = 0;

			gpio_set_value(GPIO_ECHO_PWDN_N, 1);
			gpio_set_value(GPIO_ECHO_RST_N,0);
			msleep(3);
			gpio_set_value(GPIO_ECHO_RST_N,1);

			msleep(10);

			if ( echo_write_register(0x2308, 0x005F) != 0) err = 1;
			if ( echo_write_register(0x232C, 0x0025) != 0) err = 1;
			if ( echo_write_register(0x2300, 0x0004) != 0) err = 1;
			if ( echo_write_register(0x2302, 0x0024) != 0) err = 1;
			if ( echo_write_register(0x23F7, 0x001E) != 0) err = 1;
			if ( echo_write_register(0x230C, 0x0000) != 0) err = 1;
			msleep(50);

		//Check FM31 state//
			rval = echo_read_register(0x230C);
			rval1 = echo_read_register(0x2300);
			rval2 = echo_read_register(0x3FE4);
			if ((rval != 0x5A5A) || (rval1 != 0x0004) || (rval2 != 0x129E)) {
					printk("[FM31]Bypass Parameter Write  Fail :  0x230C = %x, 0x2300 = %x, 0x3FE4 = %x  \n",rval, rval1, rval2);

						while(rw_count < 10){
							echo_write_register(0x3FE4, 0x361E);
							msleep(50);
							rval = echo_read_register(0x3FE4);

							if(rval == 0x361E) {
								printk("[FM31] 0x3FE4 rewrite %d times and success. \n", rw_count);
								break;
							} else {
								printk("[FM31] 0x3FE4 rewrite Fail, Return value is not 0x361E, Return value =  %x  \n", rval);
							}
							rw_count++;
						}
				fm31 = 1;
			}

			if (err !=0) printk("[FM31] I2C write Fail\n");
			if (fm31 != 0) printk("[FM31] FM31 DSP running Fail \n");
			i++;
		} while ((err != 0) || (fm31 != 0));

	return 0;
}

static int echocancel_remove(struct i2c_client *client)
{
	wake_lock_destroy(&wlock);
	if(echo_client) {
		kfree(echo_client);
		echo_client = NULL;
	}

	return 0;
}

static int echocancel_suspend(struct device *dev)
{
	printk( "%s \n", __func__);
	gpio_set_value(GPIO_ECHO_PWDN_N, 0);

	return 0;
}

static int echocancel_resume(struct device *dev)
{
	printk( "%s \n", __func__);
	gpio_set_value(GPIO_ECHO_PWDN_N, 1);

	return 0;
}

static const struct i2c_device_id echo_ids[] = {
	{ "tegra_echo", 0 },
	{ /*end of list*/},
};

static const struct dev_pm_ops echocancel_pm_ops = {
	.suspend	= echocancel_suspend,
	.resume		= echocancel_resume,
};

static struct i2c_driver echocancel_driver = {
	.probe     = echocancel_probe,
	.remove    = echocancel_remove,
	.id_table  = echo_ids,
	.driver    = {
		.name = "tegra_echo",
		.owner = THIS_MODULE,
		.pm		= &echocancel_pm_ops,
	},
};


static int __devinit echocancel_init(void)
{
	return i2c_add_driver(&echocancel_driver);

}

static void __exit echocancel_exit(void)
{
	i2c_del_driver(&echocancel_driver);
}

module_init(echocancel_init);
module_exit(echocancel_exit);

MODULE_DESCRIPTION("ECHO Canceller Driver for LGE STAR Tablet");
MODULE_LICENSE("GPL");
