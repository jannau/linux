/*
    mpu-dev.c - mpu3050 char device interface

    Copyright (C) 1995-97 Simon G. Vogl
    Copyright (C) 1998-99 Frodo Looijaard <frodol@dds.nl>
    Copyright (C) 2003 Greg Kroah-Hartman <greg@kroah.com>
    Copyright (C) 2010 InvenSense Corporation, All Rights Reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/* Code inside mpudev_ioctl_rdrw is copied from i2c-dev.c
 */

#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>
#include <linux/gpio.h>
#include <linux/signal.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <linux/pm.h>

#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif

#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/input.h>
#include <linux/delay.h>

#include "mlsl.h"
#include "mpu-i2c.h"
#include "mldl_cfg.h"
#include "mpu.h"
#include "gyro_accel.h"

#define MPU_NAME "mpu"
#define MPU_SLAVE_ADDR (0x68)
#define ACC_SLAVE_ADDR (0x19)
#define DEFAULT_DELAY 200

/* Platform data for the MPU */
struct mpu_private_data {
	struct mldl_cfg mldl_cfg;

#ifdef CONFIG_HAS_EARLYSUSPEND
	struct early_suspend early_suspend;
#endif
};

static int pid = 0;
static struct i2c_client *this_client;
static struct input_dev *ginput_dev;

static bool dmp_enabled=1;			//earlySuspend check;
static bool powerOffFlag=0;			//force sensor data on/off
static bool check_sensor=0;			//check escape condition while routine
static int test_count=0;	/*For Sensor Test*/
static int test_report=0;	/*For Sensor Test*/

#ifdef USE_WAIT_QUEUE
static bool sensor_wait_status=0;	//check sensor_wait queue status
static bool sensorOff=1;			//sensor power on/off
static DECLARE_WAIT_QUEUE_HEAD(sensor_wait);
#endif

static int 	sensor_data[16]={0,};
static int	sensor_flag = 0;
static int	current_delay =200;

static unsigned long sensor_bit_mask=0;

int gyro_enabled=0;			// 1: enable  0:disable
int gyro_bypassed=0;		//1: gyro_bypassed  0:not bypass
static int acc_sleep=0;		//1:acc_only_mode_sleep  0: on state
static int acc_bias[3]={0,0,0};
static signed short n16Value[3]={0,};
static unsigned char acc_data[6]={0,};
static int cnt=0;

static int mpu_open(struct inode *inode, struct file *file)
{
	printk("mpu_open : current->pid %d\n", current->pid);
	pid = current->pid;
	file->private_data = this_client;
//	sensorWaitCheck();
	return 0;
}

// close function - called when the "file" /dev/mpu is closed in userspace
static int mpu_release(struct inode *inode, struct file *file)
{
	struct i2c_client *client =
		(struct i2c_client *) file->private_data;
	struct mpu_private_data *mpu =
		(struct mpu_private_data *) i2c_get_clientdata(client);
	struct mldl_cfg *mldl_cfg = &mpu->mldl_cfg;
	int result = 0;

	pid = 0;

	if (!mldl_cfg->is_suspended) {
		struct i2c_adapter * accel_adapter;
		struct i2c_adapter * compass_adapter;
		accel_adapter = i2c_get_adapter(mldl_cfg->pdata->accel.adapt_num);
		compass_adapter = i2c_get_adapter(mldl_cfg->pdata->compass.adapt_num);
		result = mpu3050_suspend(mldl_cfg, client->adapter,
					accel_adapter,
					compass_adapter,
					TRUE, TRUE);
		printk("mpu_release_E\n");
	}
//	sensorWaitCheck();

	printk("mpu_release_2\n");

	return result;
}

static noinline int mpudev_ioctl_rdrw(struct i2c_client *client,unsigned long arg)
{
	struct i2c_rdwr_ioctl_data rdwr_arg;
	struct i2c_msg *rdwr_pa;
	u8 __user **data_ptrs;
	int i, res;

	if (copy_from_user(&rdwr_arg,
			(struct i2c_rdwr_ioctl_data __user *) arg,
			sizeof(rdwr_arg)))
		return -EFAULT;

	/* Put an arbitrary limit on the number of messages that can
	 * be sent at once */
	if (rdwr_arg.nmsgs > I2C_RDRW_IOCTL_MAX_MSGS)
		return -EINVAL;

	rdwr_pa = (struct i2c_msg *)
		kmalloc(rdwr_arg.nmsgs * sizeof(struct i2c_msg), GFP_KERNEL);
	if (!rdwr_pa)
		return -ENOMEM;

	if (copy_from_user(rdwr_pa, rdwr_arg.msgs,
			rdwr_arg.nmsgs * sizeof(struct i2c_msg))) {
		kfree(rdwr_pa);
		return -EFAULT;
	}

	data_ptrs =
	kmalloc(rdwr_arg.nmsgs * sizeof(u8 __user *), GFP_KERNEL);
	if (data_ptrs == NULL) {
		kfree(rdwr_pa);
		return -ENOMEM;
	}

	res = 0;
	for (i = 0; i < rdwr_arg.nmsgs; i++) {
		/* Limit the size of the message to a sane amount;
		 * and don't let length change either. */
		if ((rdwr_pa[i].len > 8192) ||
		(rdwr_pa[i].flags & I2C_M_RECV_LEN)) {
			res = -EINVAL;
			break;
		}
		data_ptrs[i] = (u8 __user *) rdwr_pa[i].buf;
		rdwr_pa[i].buf = kmalloc(rdwr_pa[i].len, GFP_KERNEL);
		if (rdwr_pa[i].buf == NULL) {
			res = -ENOMEM;
			break;
		}
		if (copy_from_user(rdwr_pa[i].buf, data_ptrs[i],
				rdwr_pa[i].len)) {
			++i;	/* Needs to be kfreed too */
			res = -EFAULT;
			break;
		}
	}
	if (res < 0) {
		int j;
		for (j = 0; j < i; ++j)
			kfree(rdwr_pa[j].buf);
		kfree(data_ptrs);
		kfree(rdwr_pa);
		return res;
	}

	res = i2c_transfer(client->adapter, rdwr_pa, rdwr_arg.nmsgs);
	while (i-- > 0) {
		if (res >= 0 && (rdwr_pa[i].flags & I2C_M_RD)) {
			if (copy_to_user(data_ptrs[i], rdwr_pa[i].buf,
					 rdwr_pa[i].len))
				res = -EFAULT;
		}
		kfree(rdwr_pa[i].buf);
	}
	kfree(data_ptrs);
	kfree(rdwr_pa);
	return res;
}


static int
mpu_ioctl_set_mpu_pdata(struct i2c_client *client, unsigned long arg)
{
	int result;
	struct mpu_private_data *mpu =
		(struct mpu_private_data *) i2c_get_clientdata(client);
	struct mldl_cfg *mldl_cfg = &mpu->mldl_cfg;
	int accel_adapt_num = mldl_cfg->pdata->accel.adapt_num;
	int compass_adapt_num = mldl_cfg->pdata->compass.adapt_num;
	int accel_bus = mldl_cfg->pdata->accel.bus;
	int compass_bus = mldl_cfg->pdata->compass.bus;

	result = copy_from_user(mldl_cfg->pdata,
				(unsigned char __user *) arg,
				sizeof(struct mpu3050_platform_data));
	/* Don't allow userspace to change the adapter number or bus */
	mldl_cfg->pdata->accel.adapt_num = accel_adapt_num;
	mldl_cfg->pdata->compass.adapt_num = compass_adapt_num;
	mldl_cfg->pdata->accel.bus = accel_bus;
	mldl_cfg->pdata->compass.bus = compass_bus;

	return result;
}

static int
mpu_ioctl_set_mpu_config(struct i2c_client *client, unsigned long arg)
{
	struct mpu_private_data *mpu =
		(struct mpu_private_data *) i2c_get_clientdata(client);
	struct mldl_cfg *mldl_cfg = &mpu->mldl_cfg;

	printk("%s\n", __func__);
	/*
	 * User space is not allowed to modify accel compass or pdata structs,
	 * as well as silicon_revision product_id or trim
	 */
	if (copy_from_user(mldl_cfg,
			(struct mldl_cfg *) arg,
			offsetof(struct mldl_cfg, silicon_revision)))
		return -EFAULT;

	return 0;
}

static int
mpu_ioctl_get_mpu_config(struct i2c_client *client, unsigned long arg)
{
	/* Have to be careful as there are 3 pointers in the mldl_cfg
	 * structure */
	struct mpu_private_data *mpu =
	(struct mpu_private_data *) i2c_get_clientdata(client);
	struct mldl_cfg *mldl_cfg = &mpu->mldl_cfg;
	struct mldl_cfg *local_mldl_cfg;
	int retval = 0;

	local_mldl_cfg = kzalloc(sizeof(struct mldl_cfg), GFP_KERNEL);
	if (NULL == local_mldl_cfg)
		return -ENOMEM;

	retval =
	copy_from_user(local_mldl_cfg, (struct mldl_cfg __user *) arg,
			sizeof(struct mldl_cfg));
	if (retval)
		goto out;

	/* Fill in the accel, compass and pdata pointers */
	if (mldl_cfg->accel) {
		retval = copy_to_user((void __user *)local_mldl_cfg->accel,
				mldl_cfg->accel,
				sizeof(*mldl_cfg->accel));
		if (retval)
			goto out;
	}

	if (mldl_cfg->compass) {
		retval = copy_to_user((void __user *)local_mldl_cfg->compass,
				mldl_cfg->compass,
				sizeof(*mldl_cfg->compass));
		if (retval)
			goto out;
	}

	if (mldl_cfg->pdata) {
		retval = copy_to_user((void __user *)local_mldl_cfg->pdata,
				mldl_cfg->pdata,
				sizeof(*mldl_cfg->pdata));
		if (retval)
			goto out;
	}

	/* Do not modify the accel, compass and pdata pointers */
	retval = copy_to_user((struct mldl_cfg __user *) arg,
			mldl_cfg, offsetof(struct mldl_cfg, accel));

	out:
		kfree(local_mldl_cfg);
		return retval;
}

#ifdef USE_WAIT_QUEUE
void sensorWaitCheck(void)
{
	if(sensor_wait_status==1)
	{
		printk("sensorWaitCheck\n");
		sensor_wait_status=0;
		wake_up_interruptible(&sensor_wait);
	}
}
#endif

void send_accel_data(int x,int y,int z)
{
	if((x==0 && y==0 && z==0) || (dmp_enabled==0))
	{
		printk("[%s:] No Acc data: %d %d %d\n",__FUNCTION__,x , y, z );
		return;
	}
	x=(x*10000)>>10;
	y=(y*10000)>>10;
	z=(z*10000)>>10;

	input_report_abs(ginput_dev,ABS_X,x);
	input_report_abs(ginput_dev,ABS_Y,y);
	input_report_abs(ginput_dev,ABS_Z,z);
	input_sync(ginput_dev);
}


void motion_send_sensor_data(int *sensor_data)
{

	if(sensor_bit_mask&STAR_ACCELEROMETER)
	{
		if(sensor_data[0]==0 && sensor_data[1]==0 && sensor_data[2]==0) return;
		input_report_abs(ginput_dev,ABS_X,sensor_data[0]);
		input_report_abs(ginput_dev,ABS_Y,sensor_data[1]);
		input_report_abs(ginput_dev,ABS_Z,sensor_data[2]);
	}

	if(sensor_bit_mask&STAR_MAGNETIC)
	{
		input_report_abs(ginput_dev, ABS_HAT0X, sensor_data[3]); /* x-axis of raw magnetic vector */
		input_report_abs(ginput_dev, ABS_HAT0Y, sensor_data[4]); /* y-axis of raw magnetic vector */
		input_report_abs(ginput_dev, ABS_BRAKE, sensor_data[5]); /* z-axis of raw magnetic vector */
		input_report_abs(ginput_dev, ABS_WHEEL, sensor_data[12]); /* status in the magnetic field */
	}

	if(sensor_bit_mask&STAR_ORIENTATION)
	{
		input_report_abs(ginput_dev,ABS_RX,sensor_data[6]);
		input_report_abs(ginput_dev,ABS_RY,sensor_data[7]);
		input_report_abs(ginput_dev,ABS_RZ,sensor_data[8]);
		input_report_abs(ginput_dev,ABS_RUDDER,sensor_data[13]); /* status in the magnetic field */
	}

	if(sensor_bit_mask&STAR_GYRO)
	{
		input_report_abs(ginput_dev,ABS_TILT_X,sensor_data[9]);
		input_report_abs(ginput_dev,ABS_TILT_Y,sensor_data[10]);
		input_report_abs(ginput_dev,ABS_TOOL_WIDTH,sensor_data[11]);
	}

	input_sync(ginput_dev);
}

int motion_sensor_power_off(void *mlsl_handle, int sleep)
{
	int result;

    if(sleep==1)
    {
		result = MLSLSerialWriteSingle(mlsl_handle,ACC_SLAVE_ADDR,0x20,0x00);
		ERROR_CHECK(result);
#ifdef USE_WAIT_QUEUE
		sensorOff=1;
		printk("sensorOff:%d, %d \n",sensorOff,__LINE__);
#endif
    }
	else
	{
		result = MLSLSerialWriteSingle(mlsl_handle,ACC_SLAVE_ADDR,0x20,0x37);
		ERROR_CHECK(result);
	}
	return 0;
}

/*	test_count=0 : test_off
	test_count=1 : test_start
	test_count=2 : test_finish */

void motion_factory_test_result(int result_test_result, int result_test_list)
{
	int buf[2];
	/*
		returnFactoryCal
		buf[0] : 0 : All Success (Mag ADC, Accel, Gyro RMS/PLL )
		buf[0] : 1 : Fail (Mag ADC,  Accel,  Gyro RMS/PLL )

		buf[1] : 1111 : 15 = (Rotation Matrix, Mag ADC, Accel, Gyro RMS/PLL, ) -failed
		buf[1] : 1101 : 14 = (Rotation Matrix, Mag ADC, Accel) -failed
		buf[1] : 1101 : 13 = (Rotation Matrix, Mag ADC, Gyro RMS/PLL ) -failed
		buf[1] : 1100 : 12 = (Rotation Matrix, Mag ADC) -failed
		buf[1] : 0011 : 11 = (Rotation Matrix,Accel , Gyro RMS/PLL)  -failed : (OLD : Accel/Mag ADC)
		buf[1] : 0010 : 10 =(Rotation Matrix,Accel) -failed
		buf[1] : 0001 : 9 = (Rotation Matrix,Gyro RMS/PLL) -failed
		buf[1] : 1000 : 8 = (Rotation Matrix ) -failed

		buf[1] : 0111 : 7 = (Mag ADC, Accel, Gyro RMS/PLL, ) -failed
		buf[1] : 0110 : 6 = (Mag ADC, Accel ) -failed
		buf[1] : 0101 : 5 = (Mag ADC, Gyro RMS/PLL ) -failed
		buf[1] : 0100 : 4 = (Mag ADC) -failed
		buf[1] : 0011 : 3 = (Accel , Gyro RMS/PLL)  -failed : (OLD : Accel/Mag ADC)
		buf[1] : 0010 : 2 = (Accel) -failed
		buf[1] : 0001 : 1 = (Gyro RMS/PLL) -failed
		buf[1] : 0 = returnFactoryCal -success
		buf[1] :  -1 = returnFactoryCal - not work */

	buf[0] = result_test_result;
	buf[1] = result_test_list;

	printk("SEND ............. result :%d , result_list : %d  ...........\n", buf[0], buf[1]);

	if(buf[0]!=0)
	{
		if( test_count == 2 )
		{
			printk("[%d] ............. result :%d , result_list : %d  ...........\n", __LINE__, buf[0], buf[1]);
			test_report = buf[1];
#if 1
			if(( buf[1] == 8)) test_report = 16;
			//---------------------------------
			if(( buf[1] == 9)) test_report = 1;
			if(( buf[1] == 10)) test_report = 2;
			if(( buf[1] == 11)) test_report = 3;
			if(( buf[1] == 12)) test_report = 4;
			if(( buf[1] == 13)) test_report = 5;
			if(( buf[1] == 14)) test_report = 6;
			//---------------------------------
			if(( buf[1] == 15)) test_report = 7;
#endif
		}
	}
	else if((buf[0] == 0) && ( buf[1]==0 ))
	{
			test_report = 16;
	}
	else
	{
		printk("[%d] This status is unknown.. result :%d , result_list : %d.\n", __LINE__, buf[0], buf[1]);
		test_report = -2;
	}

	return ;
}

void motion_factory_test_close(void)
{
	clear_bit(STAR_FACTORY_TEST_BIT,&sensor_bit_mask);
	test_count=2;
	return ;
}

// ioctl - I/O control
static long mpu_ioctl(struct file *file,
		unsigned int cmd, unsigned long arg)
{
	struct i2c_client *client =
		(struct i2c_client *) file->private_data;
	struct mpu_private_data *mpu =
		(struct mpu_private_data *) i2c_get_clientdata(client);
	struct mldl_cfg *mldl_cfg = &mpu->mldl_cfg;
	int retval = 0;
	struct i2c_adapter * accel_adapter;
	struct i2c_adapter * compass_adapter;

	accel_adapter = i2c_get_adapter(mldl_cfg->pdata->accel.adapt_num);
	compass_adapter = i2c_get_adapter(mldl_cfg->pdata->compass.adapt_num);

	switch (cmd) {
	case I2C_RDWR: //use
		mpudev_ioctl_rdrw(client, arg);
		break;
	case I2C_SLAVE: //use
		if ((arg & 0x7E) != (client->addr & 0x7E)) {
		printk("%s: Invalid I2C_SLAVE arg %lu \n",  __func__, arg);
		}
		break;
	case MPU_SET_MPU_CONFIG:   //set
		retval = mpu_ioctl_set_mpu_config(client, arg);
		break;

	case MPU_SET_PLATFORM_DATA:
		retval = mpu_ioctl_set_mpu_pdata(client, arg);
		break;

	case MPU_GET_MPU_CONFIG:
		retval = mpu_ioctl_get_mpu_config(client, arg);
		break;

	case MPU_SUSPEND: //dmp_close
		{
			struct mpu_suspend_resume suspend;
			retval =
				copy_from_user(&suspend,
					(struct mpu_suspend_resume *)
					arg, sizeof(suspend));
			if (retval)
				break;
			if (suspend.gyro) {
				retval =
					mpu3050_suspend(mldl_cfg,
							client->adapter,
							accel_adapter,
							compass_adapter,
							suspend.accel,
							suspend.compass);
			} else {
				/* Cannot suspend the compass or accel while
				 * the MPU is running */
				retval = ML_ERROR_FEATURE_NOT_IMPLEMENTED;
			}
		}
#ifdef USE_WAIT_QUEUE
		sensorOff=1;
		printk("sensorOff:%d, %d \n", sensorOff,__LINE__);
#endif
		break;

	case MPU_RESUME:   //dmp_start
		{
			struct mpu_suspend_resume resume;
			retval =
			copy_from_user(&resume,
					(struct mpu_suspend_resume *)
					arg, sizeof(resume));
			if (retval)
				break;
			if (resume.gyro) {
				retval =
				mpu3050_resume(mldl_cfg,
						client->adapter,
						accel_adapter,
						compass_adapter,
						resume.accel,
						resume.compass);
			} else if (mldl_cfg->is_suspended) {
				if (resume.accel) {
					retval =
					mldl_cfg->accel->
						resume(accel_adapter,
						mldl_cfg->accel,
						&mldl_cfg->pdata->
						accel);
					if (retval)
						break;
				}

				if (resume.compass)
					retval =
						mldl_cfg->compass->
						resume(compass_adapter,
						mldl_cfg->compass,
						&mldl_cfg->pdata->
						compass);
			} else {
				/* Cannot resume the compass or accel while
				 * the MPU is running */
				retval = ML_ERROR_FEATURE_NOT_IMPLEMENTED;
			}
		}
		break;

	case MPU_READ_ACCEL: //not use
		{
			unsigned char data[6];
			retval =
			mpu3050_read_accel(mldl_cfg, client->adapter,data);
			if (ML_SUCCESS == retval)
				retval =
				    copy_to_user((unsigned char __user *) arg,
						data, sizeof(data));
		}
		break;
	case MPU_READ_COMPASS:
		{
			unsigned char data[6];

			retval =mpu3050_read_compass(mldl_cfg, compass_adapter, data);
			if (ML_SUCCESS == retval)
				retval =copy_to_user((unsigned char __user *) arg, data, sizeof(data));
		}
		break;

	case MOTION_IOCTL_SENSOR_DELAY:
		printk("MOTION_IOCTL_SENSOR_DELAY\n");
		clear_bit(STAR_DELAY_BIT,&sensor_bit_mask);
		if (copy_to_user((void __user *)arg, &current_delay, sizeof(current_delay)))
		{
			printk("ERR :MOTION_IOCTL_DELAY\n");
			return -EFAULT;
		}
		break;

	case MOTION_IOCTL_ENABLE_DISABLE:

		sensor_flag=sensor_bit_mask;
		if(!dmp_enabled || powerOffFlag==1)
		{
			sensor_flag=STAR_SENSOR_NONE;
			msleep(200);
		}

#ifdef USE_WAIT_QUEUE
		if(sensor_flag==STAR_SENSOR_NONE && sensorOff==1)
		{
			printk("[sensor]#\n");
			sensor_wait_status=1;
			interruptible_sleep_on(&sensor_wait);
			sensor_wait_status=0;
			if(!dmp_enabled) sensor_flag=sensor_bit_mask;
			printk("[sensor]##\n");
		}
#endif
		if (copy_to_user((void __user *)arg,&sensor_flag, sizeof(sensor_flag)))
		{
			return -EFAULT;
		}
		break;

	case MOTION_IOCTL_ACCEL_RAW:
	{
		if(gyro_bypassed==0)
		{
			/* This puts the bus into bypass mode */
			if(ML_SUCCESS==MLDLSetI2CBypass(mldl_cfg, client->adapter, 1))
			{
				gyro_bypassed=1;
				printk("[sensor]set_byPass_mode_\n");
			}
			else printk("[sensor]err:set_byPass_mode_\n");
		}

		while(dmp_enabled && !powerOffFlag)
		{
			if(check_sensor)
			{
				check_sensor=0;
				break;
			}
			retval = mpu3050_read_accel(mldl_cfg,client->adapter,acc_data);
			if (ML_SUCCESS == retval)
			{
				for(cnt=0;cnt<3;cnt++)
				{
					n16Value[cnt]= acc_data[cnt*2+1]<<8;
					n16Value[cnt]|= acc_data[cnt*2];
					n16Value[cnt] = n16Value[cnt]>>4;
					if ((n16Value[cnt]&0x8000)==1)
					{
						n16Value[cnt]|=0xF000;
					}
					n16Value[cnt]=n16Value[cnt]-acc_bias[cnt];
				}
			//Landscape
				send_accel_data(n16Value[1], -n16Value[0], n16Value[2]);
			//Portrait
				//send_accel_data(-n16Value[0], -n16Value[1], n16Value[2]);
			}
			msleep(current_delay);
		}
	}
	break;

	case MOTION_IOCTL_ACCEL_BIAS:
		{
			int acc_bias_raw[3]={0,0,0};

			if (copy_from_user(&acc_bias_raw, (void __user *)arg, sizeof(acc_bias_raw)))
			{
				return -EFAULT;
			}
			acc_bias[0] = acc_bias_raw[0]>>6;
			acc_bias[1] = acc_bias_raw[1]>>6;
			acc_bias[2] = acc_bias_raw[2]>>6;
		}
		break;

	case MOTION_IOCTL_COMPOSITE:
		if (copy_from_user(&sensor_data, (void __user *)arg, sizeof(sensor_data)))
		{
			return -EFAULT;
		}
		/*	   buf[0], [1], [2] = gyro_x,  gyro_y,	gyro_z; */
		if(dmp_enabled && !powerOffFlag) motion_send_sensor_data(sensor_data);
		break;


	case MOTION_IOCTL_TESTRESULT:
	{
		int buf_factroy[2]={0,};

		printk("START.............MOTION_IOCTL_TESTRESULT................\n");
		if (copy_from_user(&buf_factroy,(void __user *)arg, sizeof(buf_factroy)))
		{
			return -EFAULT;
		}
		/*   buf[0] : 1:  - fail
			buf[0] : 0:  - pass*/

		/*	buf[1] : 1 = returnFactoryCal -failed
			buf[1] : 0 = returnFactoryCal -success
			buf[1] :  -1 = returnFactoryCal - not work */

		motion_factory_test_close();
		motion_factory_test_result(buf_factroy[0],buf_factroy[1]);
		printk("END .............MOTION_IOCTL_TESTRESULT................\n");
	}
		break;


	case MOTION_IOCTL_ACCEL_SLEEP_MODE:

		if (copy_from_user(&acc_sleep,(void __user *)arg, sizeof(acc_sleep)))
		{
			return -EFAULT;
		}

		printk(".......MOTION_IOCTL_ACCEL_SLEEP_MODE : %d.....\n",acc_sleep);
		motion_sensor_power_off(accel_adapter,acc_sleep);
		break;

	case MOTION_IOCTL_MPU3050_SLEEP_WAKE_UP:

		if (copy_from_user(&gyro_enabled,(void __user *)arg, sizeof(gyro_enabled)))
		{
			return -EFAULT;
		}
		printk(".......MOTION_IOCTL_MPU3050_SLEEP_WAKE_UP:%d\n",gyro_enabled);
		break;


	default:
		printk("%s:[Error]Unknown cmd %d, arg %lu \n", __func__, cmd,arg);
		retval = -EINVAL;
	}

	return retval;
}


#if 1

/*---------------------------------------------------------------------------
	 sensor enable/disable (Sensor HAL)
   ---------------------------------------------------------------------------*/

static ssize_t sensor_onoff_show(struct device *dev,  struct device_attribute *attr,  char *buf)
{
	u32    val;
	val=sensor_bit_mask;
	return sprintf(buf, "%d\n", val);
}

static ssize_t sensor_onoff_store(struct device *dev,  struct device_attribute *attr, const char *buf, size_t size)
{
	int val;
	sscanf(buf, "%d", &val);
	printk("%s:[%d]\n",__func__,val);

#ifdef USE_WAIT_QUEUE
	sensorWaitCheck();
#endif

	sensor_bit_mask =val;

	printk("new_sensors:[%ld]\n",sensor_bit_mask);
	check_sensor=1;
	return size;
}

static ssize_t factory_test_onoff_store(struct device *dev,  struct device_attribute *attr, const char *buf, size_t size)
{
	int val;
	sscanf(buf, "%d", &val);
	/* 	0: test off
		1: test on */
	printk("[factory_test_onoff_store] test_onoff [%d]\n",val);

	if( val == 1){
		if((sensor_bit_mask == STAR_SENSOR_NONE) || (sensor_bit_mask == STAR_SUSPEND))
		{
			printk("[factory_test_onoff_store] sensor_bit_mask [%ld]\n",sensor_bit_mask);
			return size;
		}
			set_bit(STAR_FACTORY_TEST_BIT,&sensor_bit_mask);
			test_count=1;
	}else{
		clear_bit(STAR_FACTORY_TEST_BIT,&sensor_bit_mask);
		test_count=0;
	}
	check_sensor=1;
	return size;
}

static ssize_t factory_test_onoff_show(struct device *dev,  struct device_attribute *attr,  char *buf)
{
	unsigned int    val;
	/* 	0: Not start
		1: Testing
		2: Test Finished */
	val=sensor_bit_mask;
	if( test_count == 0 ) val=0;
	else if( test_count == 2 ) val=2;
	else if(((STAR_FACTORY_TEST_BIT & val ) == STAR_FACTORY_TEST_BIT ) && (test_count == 1)) val=1;
	else val=test_count;

	printk("[motion_factory_test_onoff_show] test_onoff [%d] , sensor_bit_mask [%ld] \n",val, sensor_bit_mask);

	return sprintf(buf, "%d\n", val);
}

static ssize_t factory_test_result_show(struct device *dev,  struct device_attribute *attr,  char *buf)
{
	int val;
	/* 	test_report 0: Not start
		test_report 1: [FAIL] cal fail, matrix : fail
		test_report 2: [FAIL] cal fail, matrix : ok
		test_report 3: [FAIL] cal ok, matrix : fail
		test_report 4: [ OK ] cal ok, matrix : ok  */

	val=test_report;
	printk("[motion_factory_test_result_show] result [%d]\n",val);
	return sprintf(buf, "%d\n", val);
}

static ssize_t sensor_delay_show(struct device *dev,  struct device_attribute *attr, char *buf)
{
	printk("%s:[%d]\n",__func__,current_delay);
	return sprintf(buf, "%d\n", current_delay);
}

static ssize_t sensor_delay_store(struct device *dev,  struct device_attribute *attr, const char *buf, size_t size)
{
	int val;
	sscanf(buf, "%d", &val);
	if((val>0) && (val<300)) current_delay=val;
	else current_delay=200;
	set_bit(STAR_DELAY_BIT,&sensor_bit_mask);
	check_sensor=1;
	printk("%s:[%d]\n",__func__,val);
	return size;
}

static ssize_t sensor_disable_store(struct device *dev,  struct device_attribute *attr,  const char *buf, size_t size)
{
	int val;
	sscanf(buf, "%d", &val);
	printk("%s: [%d]\n",__func__,val);
	check_sensor=1;
	powerOffFlag=1;
	return size;
}

static DEVICE_ATTR(sensor_onoff,(S_IRUGO|S_IWUSR),sensor_onoff_show,sensor_onoff_store);
static DEVICE_ATTR(set_sensor_delay,(S_IRUGO|S_IWUSR),sensor_delay_show,sensor_delay_store);
static DEVICE_ATTR(factory_test_onoff,(S_IRUGO|S_IWUSR),factory_test_onoff_show,factory_test_onoff_store);
static DEVICE_ATTR(factory_test_result,(S_IRUGO|S_IWUSR),factory_test_result_show,NULL);
static DEVICE_ATTR(set_sensor_disable,(S_IRUGO|S_IWUSR),NULL,sensor_disable_store);

static struct attribute *sensors_sysfs_attr[] = {
	&dev_attr_sensor_onoff.attr,
	&dev_attr_set_sensor_delay.attr,
	&dev_attr_factory_test_onoff.attr,
	&dev_attr_factory_test_result.attr,
	&dev_attr_set_sensor_disable.attr,
	NULL
};

static const struct attribute_group sensors_attribute_group = {
	.attrs = sensors_sysfs_attr,
};

#endif

#ifdef CONFIG_HAS_EARLYSUSPEND
void mpu3050_early_suspend(struct early_suspend *h)
{
	dmp_enabled=0;
	printk("mpu3050_early_suspend\n");
	return;
}

void mpu3050_rate_resume(struct early_suspend *h)
{
	dmp_enabled=1;
	powerOffFlag=0;
//	sensorWaitCheck();
	printk("mpu3050_rate_resume\n");
	return;
}
#endif

void mpu_shutdown(struct i2c_client *client)
{
	struct mpu_private_data *mpu =
	(struct mpu_private_data *) i2c_get_clientdata(client);
	struct mldl_cfg * mldl_cfg = &mpu->mldl_cfg;
	struct i2c_adapter * accel_adapter;
	struct i2c_adapter * compass_adapter;

	if (!mpu->mldl_cfg.is_suspended)
	{
	accel_adapter = i2c_get_adapter(mldl_cfg->pdata->accel.adapt_num);
	compass_adapter = i2c_get_adapter(mldl_cfg->pdata->compass.adapt_num);

	(void) mpu3050_suspend(mldl_cfg, this_client->adapter,
			accel_adapter, compass_adapter,TRUE, TRUE);
	}
	printk("%s\n", __func__);
}

int mpu_suspend(struct i2c_client *client, pm_message_t mesg)
{

	struct mpu_private_data *mpu =
	(struct mpu_private_data *) i2c_get_clientdata(client);

	struct mldl_cfg * mldl_cfg = &mpu->mldl_cfg;
	struct i2c_adapter * accel_adapter;
	struct i2c_adapter * compass_adapter;

	if (!mpu->mldl_cfg.is_suspended) {
	accel_adapter = i2c_get_adapter(mldl_cfg->pdata->accel.adapt_num);
	compass_adapter = i2c_get_adapter(mldl_cfg->pdata->compass.adapt_num);

		printk("%s: suspending on event %d\n", __func__,mesg.event);
		(void) mpu3050_suspend(mldl_cfg, this_client->adapter,
				accel_adapter, compass_adapter,
				TRUE, TRUE);
	} else {
		printk("%s: Already suspended %d\n", __func__, mesg.event);
	}
	return 0;
}

int mpu_resume(struct i2c_client *client)
{
/* This code below is reterated, when working in the mode of accelerometer
, so that this is skipped.                                                */
#if 0
	if (pid) {
	struct mpu_private_data *mpu =
	(struct mpu_private_data *) i2c_get_clientdata(client);
	struct mldl_cfg * mldl_cfg = &mpu->mldl_cfg;
	struct i2c_adapter * accel_adapter;
	struct i2c_adapter * compass_adapter;

	accel_adapter = i2c_get_adapter(mldl_cfg->pdata->accel.adapt_num);
	compass_adapter = i2c_get_adapter(mldl_cfg->pdata->compass.adapt_num);
	printk("%s:1 for pid %d\n", __func__, pid);

		(void) mpu3050_resume(mldl_cfg, this_client->adapter,
				accel_adapter, compass_adapter,
				TRUE, TRUE);
	}
	else
	{
		printk("%s:2 for pid %d\n", __func__, pid);
	}

#endif
	printk("%s:2 for pid %d\n", __func__, pid);
	return 0;
}

// define which file operations are supported
const struct file_operations mpu_fops = {
	.owner = THIS_MODULE,
	//.read = mpu_read,
	.unlocked_ioctl = mpu_ioctl,
	.open = mpu_open,
	.release = mpu_release,
};

static unsigned short normal_i2c[] = { I2C_CLIENT_END };

#if 0
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,32)
I2C_CLIENT_INSMOD;
#endif
#endif

static struct miscdevice i2c_mpu_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = MPU_NAME,
	.fops = &mpu_fops,
};

int mpu3050_probe(struct i2c_client *client,
		const struct i2c_device_id *devid)
{
	struct mpu3050_platform_data *pdata;
	struct mpu_private_data *mpu;
	int res = 0;

	printk("%s\n", __func__);

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		res = -ENODEV;
		goto out_check_functionality_failed;
	}

	mpu = kzalloc(sizeof(struct mpu_private_data), GFP_KERNEL);
	if (!mpu) {
		res = -ENOMEM;
		goto out_alloc_data_failed;
	}

	i2c_set_clientdata(client, mpu);
	this_client = client;
	ginput_dev = input_allocate_device();
	if (!ginput_dev)
	{
		pr_err("%s: could not allocate input device\n", __func__);
		res = -ENOMEM;
		goto out_alloc_data_failed;
	}
 	ginput_dev->name = "MPU_INPUT";
	ginput_dev->id.bustype = BUS_I2C;
	ginput_dev->dev.parent = &client->dev;

	set_bit(EV_ABS,ginput_dev->evbit);
	set_bit(EV_SYN,ginput_dev->evbit);

	input_set_abs_params(ginput_dev, ABS_X, -40000, 40000, 0, 0);
	input_set_abs_params(ginput_dev, ABS_Y, -40000, 40000, 0, 0);
	input_set_abs_params(ginput_dev, ABS_Z, -40000, 40000, 0, 0);

	input_set_abs_params(ginput_dev, ABS_HAT0X, -700000, 700000, 0, 0);
	input_set_abs_params(ginput_dev, ABS_HAT0Y, -700000, 700000, 0, 0);
	input_set_abs_params(ginput_dev, ABS_BRAKE, -700000, 700000, 0, 0);
	input_set_abs_params(ginput_dev, ABS_WHEEL, 0, 5, 0, 0);

	input_set_abs_params(ginput_dev, ABS_RX, 0, 360, 0, 0);
	input_set_abs_params(ginput_dev, ABS_RY, -180, 180, 0, 0);
	input_set_abs_params(ginput_dev, ABS_RZ, -90, 90, 0, 0);
//	input_set_abs_params(ginput_dev, ABS_RUDDER, 0, 5, 0, 0);

	/*gyroscope*/
	input_set_abs_params(ginput_dev, ABS_TILT_X, -3000000, 3000000, 0, 0); /*gyro_x*/
	input_set_abs_params(ginput_dev, ABS_TILT_Y, -3000000, 3000000, 0, 0); /*gyro_y*/
	input_set_abs_params(ginput_dev, ABS_TOOL_WIDTH, -3000000, 3000000, 0, 0); /*gyro_z*/

	res = input_register_device(ginput_dev);
	if (res < 0) {
		pr_err("%s: could not register input device\n", __func__);
		};

	/*---------------------------------------------------------------------------
		init. sysfs
	---------------------------------------------------------------------------*/
//	if ((res = sysfs_create_group(&client->dev.kobj, &sensors_attribute_group)))
	if ((res = sysfs_create_group(&ginput_dev->dev.kobj, &sensors_attribute_group)))
	{
		pr_err("%s: could not create sysfs group\n", __func__);
	//	goto err_sysfs_create;
	}
	pdata = (struct mpu3050_platform_data *) client->dev.platform_data;
	if (!pdata) {
		printk("Warning no platform data for mpu3050\n");
	} else {
		mpu->mldl_cfg.pdata = pdata;

#ifdef CONFIG_SENSORS_MPU3050_MODULE
		pdata->accel.get_slave_descr = get_accel_slave_descr;
		pdata->compass.get_slave_descr = get_compass_slave_descr;
#endif

		if (pdata->accel.get_slave_descr) {
			mpu->mldl_cfg.accel =
			pdata->accel.get_slave_descr();
			printk("MPU3050: +%s\n",mpu->mldl_cfg.accel->name);
		} else {
			printk("MPU3050: No Accel Present\n");
		}

		if (pdata->compass.get_slave_descr) {
			mpu->mldl_cfg.compass =pdata->compass.get_slave_descr();
			printk("MPU3050: +%s\n",mpu->mldl_cfg.compass->name);
		} else {
			printk("MPU3050: No Compass Present\n");
		}
	}

	mpu->mldl_cfg.addr = client->addr;
	res = mpu3050_open(&mpu->mldl_cfg, client->adapter);

	if (res) {
		printk("Unable to open MPU3050 %d\n", res);
		res = -ENODEV;
		goto out_whoami_failed;
	}
#ifdef CONFIG_HAS_EARLYSUSPEND
	mpu->early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 12;
	mpu->early_suspend.suspend = mpu3050_early_suspend;
	mpu->early_suspend.resume = mpu3050_rate_resume;
	register_early_suspend(&mpu->early_suspend);
#endif

	res = misc_register(&i2c_mpu_device);
	if (res < 0) {
		printk("ERROR: misc_register returned %d\n", res);
		goto out_misc_register_failed;
	}

	return res;

      out_misc_register_failed:
#ifdef CONFIG_HAS_EARLYSUSPEND
	unregister_early_suspend(&mpu->early_suspend);
#endif
      out_whoami_failed:
	kfree(mpu);
      out_alloc_data_failed:
      out_check_functionality_failed:
	printk(KERN_ERR "%s failed %d\n", __func__, res);
	return res;
}

static int mpu3050_remove(struct i2c_client *client)
{
	struct mpu_private_data *mpu = i2c_get_clientdata(client);
	printk("%s\n", __func__);

	misc_deregister(&i2c_mpu_device);
#ifdef CONFIG_HAS_EARLYSUSPEND
	unregister_early_suspend(&mpu->early_suspend);
#endif
	kfree(mpu);

	return 0;
}

static const struct i2c_device_id mpu3050_id[] = {
	{"mpu3050", 0},
	{}
};

MODULE_DEVICE_TABLE(i2c, mpu3050_id);

static struct i2c_driver mpu3050_driver = {
	.class = I2C_CLASS_HWMON,
	.probe = mpu3050_probe,
	.remove = mpu3050_remove,
	.id_table = mpu3050_id,
	.driver = {
		.owner = THIS_MODULE,
		.name = "mpu3050",
		},
	.address_list = normal_i2c,
//	.shutdown = mpu_shutdown,/* optional */
	.suspend = mpu_suspend,/* optional */
	.resume = mpu_resume,/* optional */
};

static int __init mpu_init(void)
{
	int res=0;
	printk("%s\n", __func__);
	res = i2c_add_driver(&mpu3050_driver);
	if (res) {
		printk("%s failed\n", __func__);
	}
	return res;
}

static void __exit mpu_exit(void)
{
	printk("%s\n", __func__);
	i2c_del_driver(&mpu3050_driver);
}

module_init(mpu_init);
module_exit(mpu_exit);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("MPU3050 Sensor Driver for LGE STAR Tablet");
MODULE_LICENSE("GPL");
MODULE_ALIAS("mpu3050");
