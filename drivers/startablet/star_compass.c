/* drivers/i2c/chips/ami304.c - AMI304 compass driver
 *
 * Copyright (C) 2009 AMIT Technology Inc.
 * Author: Kyle Chen <sw-support@amit-inc.com>
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

#include <linux/interrupt.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/irq.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <linux/input.h>
#include <linux/workqueue.h>
#include <linux/kobject.h>
#include <linux/slab.h>

#include <asm/uaccess.h>

#define DEBUG 1

//andy.choi@lge.com START
#include "star_compass.h"
#include <mach/gpio-names.h>

#define STAR_COMPASS_DEVICE_NAME	"star_compass"
#define STAR_COMPASS_DRIVER_NAME	"star_compass_driver"

static struct i2c_client *ami304_i2c_client = NULL;

/* Addresses to scan */
static unsigned short normal_i2c[] = { AMI304_I2C_ADDRESS, I2C_CLIENT_END };

struct _ami302_data {
	rwlock_t lock;
	int mode;
	int rate;
	volatile int updated;
} ami304_data;

struct _ami304mid_data {
	rwlock_t datalock;
	rwlock_t ctrllock;	
	int controldata[10];	
	int yaw;
	int roll;
	int pitch;
	int nmx;
	int nmy;
	int nmz;
	int nax;
	int nay;
	int naz;
	int mag_status;
} ami304mid_data;

struct ami304_i2c_data {
	struct input_dev *input_dev;
	struct i2c_client *client;
};

struct ami304_i2c_data *g_ami304_data = NULL;

static const struct i2c_device_id ami304_id[] = {
	{STAR_COMPASS_DEVICE_NAME, 0},
	{},
};

static atomic_t dev_open_count;
static atomic_t hal_open_count;
static atomic_t daemon_open_count;

int ami304_i2c_read(u8 RegAddr, u8 *RegValue, u8 len) {
	int Ret;
	struct i2c_client *client = g_ami304_data->client;

	if (client == NULL) {
		printk("%s - no client initialized\n", __FUNCTION__);
		return -1;
	}

	Ret = i2c_read_block_data(client, DEVICE_I2C_ADDRESS_SIZE_1BYTE, len, RegAddr, RegValue);

	if (Ret < 0) {
		printk("%s - failed \n", __FUNCTION__);
		return -1;
	}
	
	return 0;
}

int ami304_i2c_write(u8 RegAddr, u8 *RegValue, u8 len) {
	int Ret;
	struct i2c_client *client = g_ami304_data->client;

	if (client == NULL) {
		printk("%s - no client initialized\n", __FUNCTION__);
		return -1;
	}

	Ret = i2c_write_block_data(client, DEVICE_I2C_ADDRESS_SIZE_1BYTE, len, RegAddr, RegValue);

	if (Ret < 0) {
		printk("%s - failed \n", __FUNCTION__);
		return -1;
	}

	return 0;
}

static int AMI304_Init(int mode)
{
	u8 RegAddr;
	u8 ctrl1, ctrl2, ctrl3;
	int Ret;
	
	RegAddr = AMI304_REG_CTRL1;
	Ret = ami304_i2c_read(RegAddr, &ctrl1, 1);
	if (Ret < 0) {
		printk("%s - failed to read AMI304_REG_CTRL1\n", __FUNCTION__);
		return -1;
	}
	printk("%s - AMI304_REG_CTRL1 : %x\n", __FUNCTION__, ctrl1);
	
	RegAddr = AMI304_REG_CTRL2;
	Ret = ami304_i2c_read(RegAddr, &ctrl2, 1);
	if (Ret < 0) {
		printk("%s - failed to read AMI304_REG_CTRL2\n", __FUNCTION__);
		return -1;
	}
	
	RegAddr = AMI304_REG_CTRL3;
	Ret = ami304_i2c_read(RegAddr, &ctrl3, 1);		
	if (Ret < 0) {
		printk("%s - failed to read AMI304_REG_CTRL3\n", __FUNCTION__);
		return -1;
	}
	
	RegAddr = AMI304_REG_CTRL1;
	if( mode==AMI304_FORCE_MODE )
	{
		ctrl1 |= AMI304_CTRL1_PC1 | AMI304_CTRL1_FS1_FORCE;
		write_lock(&ami304_data.lock);
		ami304_data.mode = AMI304_FORCE_MODE;
		write_unlock(&ami304_data.lock);			
	}
	else	
	{
		ctrl1 |= AMI304_CTRL1_PC1 | AMI304_CTRL1_FS1_NORMAL | AMI304_CTRL1_ODR1;
		write_lock(&ami304_data.lock);
		ami304_data.mode = AMI304_NORMAL_MODE;
		write_unlock(&ami304_data.lock);			
	}
	Ret = ami304_i2c_write(RegAddr, &ctrl1, 1);

	RegAddr = AMI304_REG_CTRL2;
	ctrl2 |= AMI304_CTRL2_DREN;
	Ret = ami304_i2c_write(RegAddr, &ctrl2, 1);	
	
	RegAddr = AMI304_REG_CTRL3;
	ctrl3 |= AMI304_CTRL3_B0_LO_CLR;
	Ret = ami304_i2c_write(RegAddr, &ctrl3, 1);
	return 0;
}

static int AMI304_SetMode(int newmode)
{
	int mode = 0;
	
	read_lock(&ami304_data.lock);
	mode = ami304_data.mode;
	read_unlock(&ami304_data.lock);		
	
	if (mode == newmode) 
		return 0;	
			
	return AMI304_Init(newmode);
}

static int AMI304_ReadChipInfo(char *buf, int bufsize)
{
	if ((!buf)||(bufsize<=30))
		return -1;
	if (!ami304_i2c_client)
	{
		*buf = 0;
		return -2;
	}

	sprintf(buf, "AMI304 Chip");
	return 0;
}

static int AMI304_ReadSensorData(char *buf, int bufsize)
{
	int Ret;
	int mode = 0;	
	u8 RegAddr, RegValue;
	unsigned char databuf[6];

	if ((!buf)||(bufsize<=80))
		return -1;
	if (!g_ami304_data->client)
	{
		*buf = 0;
		return -2;
	}
	
	read_lock(&ami304_data.lock);	
	mode = ami304_data.mode;
	read_unlock(&ami304_data.lock);		

	RegAddr = AMI304_REG_CTRL3;
	RegValue = AMI304_CTRL3_FORCE_BIT;
	Ret = ami304_i2c_write(RegAddr, &RegValue, 1);
	
	// We can read all measured data in once
	RegAddr = AMI304_REG_DATAXH;
	Ret = ami304_i2c_read(RegAddr, databuf, 6);
	
	sprintf(buf, "%02x %02x %02x %02x %02x %02x", databuf[0], databuf[1], databuf[2], databuf[3], databuf[4], databuf[5]);
	printk("%02x %02x %02x %02x %02x %02x\n", databuf[0], databuf[1], databuf[2], databuf[3], databuf[4], databuf[5]);
	
	return 0;
}

static int AMI304_ReadPostureData(char *buf, int bufsize)
{
	if ((!buf)||(bufsize<=80))
		return -1;

	read_lock(&ami304mid_data.datalock);
	sprintf(buf, "%d %d %d %d", ami304mid_data.yaw, ami304mid_data.pitch, ami304mid_data.roll, ami304mid_data.mag_status);
	read_unlock(&ami304mid_data.datalock);
	return 0;
}

static int AMI304_ReadCaliData(char *buf, int bufsize)
{
	if ((!buf)||(bufsize<=80))
		return -1;

	read_lock(&ami304mid_data.datalock);
	sprintf(buf, "%d %d %d %d %d %d %d", ami304mid_data.nmx, ami304mid_data.nmy, ami304mid_data.nmz,ami304mid_data.nax,ami304mid_data.nay,ami304mid_data.naz,ami304mid_data.mag_status);
	read_unlock(&ami304mid_data.datalock);
	return 0;
}

static int AMI304_ReadMiddleControl(char *buf, int bufsize)
{
	if ((!buf)||(bufsize<=80))
		return -1;

	read_lock(&ami304mid_data.ctrllock);
	sprintf(buf, "%d %d %d %d %d %d %d %d %d %d", 
		ami304mid_data.controldata[0], ami304mid_data.controldata[1], ami304mid_data.controldata[2],ami304mid_data.controldata[3],ami304mid_data.controldata[4],
		ami304mid_data.controldata[5], ami304mid_data.controldata[6], ami304mid_data.controldata[7], ami304mid_data.controldata[8], ami304mid_data.controldata[9]);
	read_unlock(&ami304mid_data.ctrllock);
	return 0;
}

static int AMI304_Report_Value(int en_dis)
{
	struct ami304_i2c_data *data = i2c_get_clientdata(ami304_i2c_client);
	
	if( !en_dis )
		return 0;
		
	input_report_abs(data->input_dev, ABS_RX, ami304mid_data.yaw);	/* yaw */
	input_report_abs(data->input_dev, ABS_RY, ami304mid_data.pitch);/* pitch */
	input_report_abs(data->input_dev, ABS_RZ, ami304mid_data.roll);/* roll */
	input_report_abs(data->input_dev, ABS_RUDDER, ami304mid_data.mag_status);/* status of orientation sensor */

	input_report_abs(data->input_dev, ABS_X, ami304mid_data.nax);/* x-axis raw acceleration */
	input_report_abs(data->input_dev, ABS_Y, ami304mid_data.nay);/* y-axis raw acceleration */
	input_report_abs(data->input_dev, ABS_Z, ami304mid_data.naz);/* z-axis raw acceleration */

	input_report_abs(data->input_dev, ABS_HAT0X, ami304mid_data.nmx); /* x-axis of raw magnetic vector */
	input_report_abs(data->input_dev, ABS_HAT0Y, ami304mid_data.nmy); /* y-axis of raw magnetic vector */
	input_report_abs(data->input_dev, ABS_BRAKE, ami304mid_data.nmz); /* z-axis of raw magnetic vector */
	input_report_abs(data->input_dev, ABS_WHEEL, ami304mid_data.mag_status);/* status of magnetic sensor */

	input_sync(data->input_dev);
	   
	return 0;
}

static ssize_t show_chipinfo_value(struct device *dev, struct device_attribute *attr, char *buf)
{
	char strbuf[AMI304_BUFSIZE];
	AMI304_ReadChipInfo(strbuf, AMI304_BUFSIZE);
	return sprintf(buf, "%s\n", strbuf);		
}

static ssize_t show_sensordata_value(struct device *dev, struct device_attribute *attr, char *buf)
{
	char strbuf[AMI304_BUFSIZE];
	AMI304_ReadSensorData(strbuf, AMI304_BUFSIZE);
	return sprintf(buf, "%s\n", strbuf);			
}

static ssize_t show_posturedata_value(struct device *dev, struct device_attribute *attr, char *buf)
{
	char strbuf[AMI304_BUFSIZE];
	AMI304_ReadPostureData(strbuf, AMI304_BUFSIZE);
	return sprintf(buf, "%s\n", strbuf);			
}

static ssize_t show_calidata_value(struct device *dev, struct device_attribute *attr, char *buf)
{
	char strbuf[AMI304_BUFSIZE];
	AMI304_ReadCaliData(strbuf, AMI304_BUFSIZE);
	return sprintf(buf, "%s\n", strbuf);			
}

static ssize_t show_midcontrol_value(struct device *dev, struct device_attribute *attr, char *buf)
{
	char strbuf[AMI304_BUFSIZE];
	AMI304_ReadMiddleControl(strbuf, AMI304_BUFSIZE);
	return sprintf(buf, "%s\n", strbuf);			
}

static ssize_t store_midcontrol_value(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	write_lock(&ami304mid_data.ctrllock);
	memcpy(&ami304mid_data.controldata[0], buf, sizeof(int)*10);	
 	write_unlock(&ami304mid_data.ctrllock);		
	return count;			
}

static ssize_t show_mode_value(struct device *dev, struct device_attribute *attr, char *buf)
{
	int mode=0;
	read_lock(&ami304_data.lock);
	mode = ami304_data.mode;
	read_unlock(&ami304_data.lock);		
	return sprintf(buf, "%d\n", mode);			
}

static ssize_t store_mode_value(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	int mode = 0;
	sscanf(buf, "%d", &mode);	
 	AMI304_SetMode(mode);
	return count;			
}

static DEVICE_ATTR(chipinfo, S_IRUGO, show_chipinfo_value, NULL);
static DEVICE_ATTR(sensordata, S_IRUGO, show_sensordata_value, NULL);
static DEVICE_ATTR(posturedata, S_IRUGO, show_posturedata_value, NULL);
static DEVICE_ATTR(calidata, S_IRUGO, show_calidata_value, NULL);
static DEVICE_ATTR(midcontrol, S_IRUGO | S_IWUSR, show_midcontrol_value, store_midcontrol_value );
static DEVICE_ATTR(mode, S_IRUGO | S_IWUSR, show_mode_value, store_mode_value );

static int ami304_open(struct inode *inode, struct file *file)
{	
	int ret = -1;
	if( atomic_cmpxchg(&dev_open_count, 0, 1)==0 ) {
		printk(KERN_INFO "Open device node:ami304\n");
		ret = nonseekable_open(inode, file);
	}	
	return ret;
}

static int ami304_release(struct inode *inode, struct file *file)
{
	atomic_set(&dev_open_count, 0);
	printk(KERN_INFO "Release device node:ami304\n");		
	return 0;
}

static int ami304_ioctl(struct inode *inode, struct file *file, unsigned int cmd,unsigned long arg)
{
	char strbuf[AMI304_BUFSIZE];
	int controlbuf[10];
	void __user *data;
	int retval=0;
	int mode=0;

    //check the authority is root or not
    if(!capable(CAP_SYS_ADMIN)) {
        retval = -EPERM;
        goto err_out;
	}
		
	switch (cmd) {
		case AMI304_IOCTL_INIT:
			read_lock(&ami304_data.lock);
			mode = ami304_data.mode;
			read_unlock(&ami304_data.lock);
			AMI304_Init(mode);			
			break;
		
		case AMI304_IOCTL_READ_CHIPINFO:
			data = (void __user *) arg;
			if (data == NULL)
				break;	
			AMI304_ReadChipInfo(strbuf, AMI304_BUFSIZE);
			if (copy_to_user(data, strbuf, strlen(strbuf)+1)) {
				retval = -EFAULT;
				goto err_out;
			}				
			break;

		case AMI304_IOCTL_READ_SENSORDATA:
			data = (void __user *) arg;
			if (data == NULL)
				break;	
			AMI304_ReadSensorData(strbuf, AMI304_BUFSIZE);
			if (copy_to_user(data, strbuf, strlen(strbuf)+1)) {
				retval = -EFAULT;
				goto err_out;
			}				
			break;				
						
		case AMI304_IOCTL_READ_POSTUREDATA:
			data = (void __user *) arg;
			if (data == NULL)
				break;	
			AMI304_ReadPostureData(strbuf, AMI304_BUFSIZE);
			if (copy_to_user(data, strbuf, strlen(strbuf)+1)) {
				retval = -EFAULT;
				goto err_out;
			}				
			break;			
	 
	        case AMI304_IOCTL_READ_CALIDATA:
			data = (void __user *) arg;
			if (data == NULL)
				break;	
			AMI304_ReadCaliData(strbuf, AMI304_BUFSIZE);
			if (copy_to_user(data, strbuf, strlen(strbuf)+1)) {
				retval = -EFAULT;
				goto err_out;
			}				
	        	break;
	        
	        case AMI304_IOCTL_READ_CONTROL:
			read_lock(&ami304mid_data.ctrllock);
			memcpy(controlbuf, &ami304mid_data.controldata[0], sizeof(controlbuf));
			read_unlock(&ami304mid_data.ctrllock);			
			data = (void __user *) arg;
			if (data == NULL)
				break;	
			if (copy_to_user(data, controlbuf, sizeof(controlbuf))) {
				retval = -EFAULT;
				goto err_out;
			}						        
	        	break;

		case AMI304_IOCTL_SET_CONTROL:
			data = (void __user *) arg;
			if (data == NULL)
				break;	
			if (copy_from_user(controlbuf, data, sizeof(controlbuf))) {
				retval = -EFAULT;
				goto err_out;
			}	
			write_lock(&ami304mid_data.ctrllock);
			memcpy(&ami304mid_data.controldata[0], controlbuf, sizeof(controlbuf));
			write_unlock(&ami304mid_data.ctrllock);		
			break;
			
		case AMI304_IOCTL_SET_MODE:
			data = (void __user *) arg;
			if (data == NULL)
				break;	
			if (copy_from_user(&mode, data, sizeof(mode))) {
				retval = -EFAULT;
				goto err_out;
			}		
			AMI304_SetMode(mode);				
			break;
					        				
		default:
			printk(KERN_ERR "%s not supported = 0x%04x", __FUNCTION__, cmd);
			retval = -ENOIOCTLCMD;
			break;
	}
	
err_out:
	return retval;	
}

static int ami304daemon_open(struct inode *inode, struct file *file)
{
	//return nonseekable_open(inode, file);
	int ret = -1;
	if( atomic_cmpxchg(&daemon_open_count, 0, 1)==0 ) {
		printk(KERN_INFO "Open device node:ami304daemon\n");
		ret = 0;
	}
	return ret;	
}

static int ami304daemon_release(struct inode *inode, struct file *file)
{
	atomic_set(&daemon_open_count, 0);
	printk(KERN_INFO "Release device node:ami304daemon\n");	
	return 0;
}

static int ami304daemon_ioctl(struct inode *inode, struct file *file, unsigned int cmd,
	   unsigned long arg)
{
	int valuebuf[4];
	int calidata[7];
	int controlbuf[10];
	char strbuf[AMI304_BUFSIZE];
	void __user *data;
	int retval=0;
	int mode;
	int en_dis_Report;

    //check the authority is root or not
    if(!capable(CAP_SYS_ADMIN)) {
        retval = -EPERM;
        goto err_out;
    }
		
	switch (cmd) {
			
		case AMI304MID_IOCTL_GET_SENSORDATA:
			data = (void __user *) arg;
			if (data == NULL)
				break;	
			AMI304_ReadSensorData(strbuf, AMI304_BUFSIZE);
			if (copy_to_user(data, strbuf, strlen(strbuf)+1)) {
				retval = -EFAULT;
				goto err_out;
			}
			break;
				
		case AMI304MID_IOCTL_SET_POSTURE:
			data = (void __user *) arg;
			if (data == NULL)
				break;	
			if (copy_from_user(&valuebuf, data, sizeof(valuebuf))) {
				retval = -EFAULT;
				goto err_out;
			}				
			write_lock(&ami304mid_data.datalock);
			ami304mid_data.yaw   = valuebuf[0];
			ami304mid_data.pitch = valuebuf[1];
			ami304mid_data.roll  = valuebuf[2];
			ami304mid_data.mag_status = valuebuf[3];
			write_unlock(&ami304mid_data.datalock);	
			break;		
			
		case AMI304MID_IOCTL_SET_CALIDATA:
			data = (void __user *) arg;
			if (data == NULL)
				break;	
			if (copy_from_user(&calidata, data, sizeof(calidata))) {
				retval = -EFAULT;
				goto err_out;
			}	
			write_lock(&ami304mid_data.datalock);			
			ami304mid_data.nmx = calidata[0];
			ami304mid_data.nmy = calidata[1];
			ami304mid_data.nmz = calidata[2];
			ami304mid_data.nax = calidata[3];
			ami304mid_data.nay = calidata[4];
			ami304mid_data.naz = calidata[5];
			ami304mid_data.mag_status = calidata[6];
			write_unlock(&ami304mid_data.datalock);				
			break;								

		case AMI304MID_IOCTL_GET_CONTROL:
			read_lock(&ami304mid_data.ctrllock);
			memcpy(controlbuf, &ami304mid_data.controldata[0], sizeof(controlbuf));
			read_unlock(&ami304mid_data.ctrllock);			
			data = (void __user *) arg;
			if (data == NULL)
				break;	
			if (copy_to_user(data, controlbuf, sizeof(controlbuf))) {
				retval = -EFAULT;
				goto err_out;
			}					
			break;		
			
		case AMI304MID_IOCTL_SET_CONTROL:
			data = (void __user *) arg;
			if (data == NULL)
				break;	
			if (copy_from_user(controlbuf, data, sizeof(controlbuf))) {
				retval = -EFAULT;
				goto err_out;
			}	
			write_lock(&ami304mid_data.ctrllock);
			memcpy(&ami304mid_data.controldata[0], controlbuf, sizeof(controlbuf));
			write_unlock(&ami304mid_data.ctrllock);
			break;	
	
		case AMI304MID_IOCTL_SET_MODE:
			data = (void __user *) arg;
			if (data == NULL)
				break;	
			if (copy_from_user(&mode, data, sizeof(mode))) {
				retval = -EFAULT;
				goto err_out;
			}		
			AMI304_SetMode(mode);				
			break;
		
		//Add for input_device sync			
		case AMI304MID_IOCTL_SET_REPORT:
			data = (void __user *) arg;
			if (data == NULL)
				break;	
			if (copy_from_user(&en_dis_Report, data, sizeof(mode))) {
				retval = -EFAULT;
				goto err_out;
			}				
			read_lock(&ami304mid_data.datalock);			
			AMI304_Report_Value(en_dis_Report);
			read_lock(&ami304mid_data.datalock);						
			break;
						
		default:
			printk(KERN_ERR "%s not supported = 0x%04x", __FUNCTION__, cmd);
			retval = -ENOIOCTLCMD;
			break;
	}
	
err_out:
	return retval;	
}

static int ami304hal_open(struct inode *inode, struct file *file)
{
	//return nonseekable_open(inode, file);	
	atomic_inc_and_test(&hal_open_count);
	printk(KERN_INFO "Open device node:ami304hal %d times.\n", atomic_read(&hal_open_count));	
	return 0;
}

static int ami304hal_release(struct inode *inode, struct file *file)
{
	atomic_dec_and_test(&hal_open_count);
	printk(KERN_INFO "Release ami304hal, remainder is %d times.\n", atomic_read(&hal_open_count));	
	return 0;
}

static int ami304hal_ioctl(struct inode *inode, struct file *file, unsigned int cmd,unsigned long arg)
{
	char strbuf[AMI304_BUFSIZE];
	void __user *data;
	int retval=0;
		
	switch (cmd) {
		
		case AMI304HAL_IOCTL_GET_SENSORDATA:
			data = (void __user *) arg;
			if (data == NULL)
				break;	
			AMI304_ReadSensorData(strbuf, AMI304_BUFSIZE);
			if (copy_to_user(data, strbuf, strlen(strbuf)+1)) {
				retval = -EFAULT;
				goto err_out;
			}		
			break;
									
		case AMI304HAL_IOCTL_GET_POSTURE:
			data = (void __user *) arg;
			if (data == NULL)
				break;	
			AMI304_ReadPostureData(strbuf, AMI304_BUFSIZE);
			if (copy_to_user(data, strbuf, strlen(strbuf)+1)) {
				retval = -EFAULT;
				goto err_out;
			}				
			break;			
	 
		case AMI304HAL_IOCTL_GET_CALIDATA:
			data = (void __user *) arg;
			if (data == NULL)
				break;	
			AMI304_ReadCaliData(strbuf, AMI304_BUFSIZE);
			if (copy_to_user(data, strbuf, strlen(strbuf)+1)) {
				retval = -EFAULT;
				goto err_out;
			}				
	        	break;

		default:
			printk(KERN_ERR "%s not supported = 0x%04x", __FUNCTION__, cmd);
			retval = -ENOIOCTLCMD;
			break;
	}
	
err_out:
	return retval;	
}

static struct file_operations ami304_fops = {
	.owner = THIS_MODULE,
	.open = ami304_open,
	.release = ami304_release,
	.ioctl = ami304_ioctl,
};

static struct miscdevice ami304_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "ami304",
	.fops = &ami304_fops,
};


static struct file_operations ami304daemon_fops = {
	.owner = THIS_MODULE,
	.open = ami304daemon_open,
	.release = ami304daemon_release,
	.ioctl = ami304daemon_ioctl,
};

static struct miscdevice ami304daemon_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "ami304daemon",
	.fops = &ami304daemon_fops,
};

static struct file_operations ami304hal_fops = {
	.owner = THIS_MODULE,
	.open = ami304hal_open,
	.release = ami304hal_release,
	.ioctl = ami304hal_ioctl,
};

static struct miscdevice ami304hal_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "ami304hal",
	.fops = &ami304hal_fops,
};

static int ami304_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct i2c_client *new_client;
	struct ami304_i2c_data *data;
	int err = 0;

	printk(KERN_INFO "\n\nEnter ami304_i2c_detect!!\n");
	if (!(data = kmalloc(sizeof(struct ami304_i2c_data), GFP_KERNEL))) {
		err = -ENOMEM;
		goto exit;
	}
	memset(data, 0, sizeof(struct ami304_i2c_data));

	new_client = client;
	i2c_set_clientdata(new_client, data);
	new_client->flags = 0;

	strlcpy(new_client->name, "ami304_i2c", I2C_NAME_SIZE);

	data->client = new_client;
	g_ami304_data = data;

	AMI304_Init(AMI304_FORCE_MODE); // default is Force State	
	printk(KERN_INFO "AMI304 registered I2C driver!\n");

	data->input_dev = input_allocate_device();
	if (!data->input_dev) {
		err = -ENOMEM;
		printk(KERN_ERR
		       "ami304_i2c_detect: Failed to allocate input device\n");
		goto exit_input_dev_alloc_failed;
	}

	set_bit(EV_ABS, data->input_dev->evbit);
	/* yaw */
	input_set_abs_params(data->input_dev, ABS_RX, 0, 360, 0, 0);
	/* pitch */
	input_set_abs_params(data->input_dev, ABS_RY, -180, 180, 0, 0);
	/* roll */
	input_set_abs_params(data->input_dev, ABS_RZ, -90, 90, 0, 0);
	/* status of magnetic sensor */	
	input_set_abs_params(data->input_dev, ABS_RUDDER, 0, 5, 0, 0);

	/* x-axis acceleration */
	input_set_abs_params(data->input_dev, ABS_X, -2000, 2000, 0, 0);
	/* y-axis acceleration */
	input_set_abs_params(data->input_dev, ABS_Y, -2000, 2000, 0, 0);
	/* z-axis acceleration */
	input_set_abs_params(data->input_dev, ABS_Z, -2000, 2000, 0, 0);

	/* x-axis of raw magnetic vector */
	input_set_abs_params(data->input_dev, ABS_HAT0X, -3000, 3000, 0, 0);
	/* y-axis of raw magnetic vector */
	input_set_abs_params(data->input_dev, ABS_HAT0Y, -3000, 3000, 0, 0);
	/* z-axis of raw magnetic vector */
	input_set_abs_params(data->input_dev, ABS_BRAKE, -3000, 3000, 0, 0);
	/* status of acceleration sensor */
	input_set_abs_params(data->input_dev, ABS_WHEEL, 0, 5, 0, 0);	

	data->input_dev->name = "Acompass";

	err = input_register_device(data->input_dev);
	if (err) {
		printk(KERN_ERR
		       "ami304_i2c_detect: Unable to register input device: %s\n",
		       data->input_dev->name);
		goto exit_input_register_device_failed;
	}
	    printk(KERN_ERR
		       "register input device successfully!!!\n");

	err = misc_register(&ami304_device);
	if (err) {
		printk(KERN_ERR
		       "ami304_device register failed\n");
		goto exit_misc_device_register_failed;
	}	
	err = device_create_file(&new_client->dev, &dev_attr_chipinfo);
	err = device_create_file(&new_client->dev, &dev_attr_sensordata);
	err = device_create_file(&new_client->dev, &dev_attr_posturedata);
	err = device_create_file(&new_client->dev, &dev_attr_calidata);
	err = device_create_file(&new_client->dev, &dev_attr_midcontrol);
	err = device_create_file(&new_client->dev, &dev_attr_mode);

	err = misc_register(&ami304daemon_device);
	if (err) {
		printk(KERN_ERR
		       "ami304daemon_device register failed\n");
		goto exit_misc_device_register_failed;
	}	

	err = misc_register(&ami304hal_device);
	if (err) {
		printk(KERN_ERR
		       "ami304hal_device register failed\n");
		goto exit_misc_device_register_failed;
	}	
	
	return 0;
exit_misc_device_register_failed:
exit_input_register_device_failed:
	input_free_device(data->input_dev);
exit_input_dev_alloc_failed:
exit_alloc_data_failed:
	kfree(data);
	g_ami304_data = NULL;
exit:
	return err;
}


static int ami304_remove(struct i2c_client *client)
{
	int err;
	struct ami304_i2c_data *data = i2c_get_clientdata(client);

	input_unregister_device(data->input_dev);
		
	ami304_i2c_client = NULL;	
	kfree(data);	
	misc_deregister(&ami304hal_device);
	misc_deregister(&ami304daemon_device);
	misc_deregister(&ami304_device);	
	return 0;
}

static struct i2c_driver ami304_i2c_driver = {
	.probe = ami304_probe,
	.remove = ami304_remove,
	.id_table = ami304_id,
	.driver = {
		.owner = THIS_MODULE, 
		.name	=STAR_COMPASS_DRIVER_NAME,
	},
};

static int __init ami304_init(void)
{
	printk(KERN_INFO "AMI304 MI sensor driver: init\n");
	rwlock_init(&ami304mid_data.ctrllock);
	rwlock_init(&ami304mid_data.datalock);
	rwlock_init(&ami304_data.lock);
	memset(&ami304mid_data.controldata[0], 0, sizeof(int)*10);	
	ami304mid_data.controldata[0] = 25000; //Loop Delay
	ami304mid_data.controldata[1] = 0; // Run	
	ami304mid_data.controldata[2] = 0; // Disable Start-AccCali
	ami304mid_data.controldata[3] = 1; // Enable Start-Cali
	ami304mid_data.controldata[4] = 350; // MW-Timout
	ami304mid_data.controldata[5] = 10; // MW-IIRStrength_M
	ami304mid_data.controldata[6] = 10; // MW-IIRStrength_G	
	atomic_set(&dev_open_count, 0);	
	atomic_set(&hal_open_count, 0);
	atomic_set(&daemon_open_count, 0);
	return i2c_add_driver(&ami304_i2c_driver);
}

static void __exit ami304_exit(void)
{
	atomic_set(&dev_open_count, 0);
	atomic_set(&hal_open_count, 0);
	atomic_set(&daemon_open_count, 0);	
	i2c_del_driver(&ami304_i2c_driver);
}

module_init(ami304_init);
module_exit(ami304_exit);

MODULE_AUTHOR("Kyle K.Y. Chen");
MODULE_DESCRIPTION("AMI304 MI sensor input_dev driver v1.0.5.10");
MODULE_LICENSE("GPL");
