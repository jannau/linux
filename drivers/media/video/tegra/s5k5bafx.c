/**
	@brief		 S5K5BAFX -samsung s5k5bafx 2M CMOS sensor driver
	@author		 hyungmoo.huh@lge.com
	@date		 2010.11.19
*/


#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/i2c.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include "s5k5bafx.h"
#include <mach/gpio-names.h>
#include <mach/hardware.h>
#include <linux/gpio.h>
#include "../../../startablet/star_cam_pmic.h"


extern i2c_cam_reg16 S5k5bafx_init_176x144[];
extern i2c_cam_reg16 S5k5bafx_init_320x240[];
extern i2c_cam_reg16 S5k5bafx_init_352x288[];
extern i2c_cam_reg16 S5k5bafx_init_640x480[];
extern i2c_cam_reg16 S5k5bafx_init_800x600[];
extern i2c_cam_reg16 S5k5bafx_init_1280x720[];
extern i2c_cam_reg16 S5k5bafx_init_640x480_30fps[];
extern i2c_cam_reg16 S5k5bafx_init_1280x720_30fps[];
extern i2c_cam_reg16 S5k5bafx_change_from_crop_to_full[];
extern i2c_cam_reg16 S5k5bafx_Preview[];
extern i2c_cam_reg16 S5k5bafx_Capture[];

extern i2c_cam_reg16 S5k5bafx_Color_Effect_None[];
extern i2c_cam_reg16 S5k5bafx_Color_Effect_MONO[];
extern i2c_cam_reg16 S5k5bafx_Color_Effect_NEGATIVE[];
extern i2c_cam_reg16 S5k5bafx_Color_Effect_SEPIA[];
extern i2c_cam_reg16 S5k5bafx_Color_Effect_Aqua[];
extern i2c_cam_reg16 S5k5bafx_Color_Effect_Sketch[];

extern i2c_cam_reg16 S5k5bafx_MWB_Auto[];
extern i2c_cam_reg16 S5k5bafx_MWB_Incandescent[];
extern i2c_cam_reg16 S5k5bafx_MWB_Fluorescent[];
extern i2c_cam_reg16 S5k5bafx_MWB_Daylight[];
extern i2c_cam_reg16 S5k5bafx_MWB_CloudyDaylight[];

extern i2c_cam_reg16 S5k5bafx_Exposure_Minus_2[];
extern i2c_cam_reg16 S5k5bafx_Exposure_Minus_1[];
extern i2c_cam_reg16 S5k5bafx_Exposure_Normal[];
extern i2c_cam_reg16 S5k5bafx_Exposure_Plus_1[];
extern i2c_cam_reg16 S5k5bafx_Exposure_Plus_2[];

static struct cam_yuv_info *info;
static int s5k5bafx_reset;
static int s5k5bafx_pwrdn ;

static i2c_cam_reg16 *s5k5bafx_SetModeSequenceList[] =
{
	S5k5bafx_init_176x144,
	S5k5bafx_init_320x240,
	S5k5bafx_init_352x288,
	S5k5bafx_init_640x480,
	S5k5bafx_init_800x600,
//	S5k5bafx_init_1280x720,
	S5k5bafx_Capture,
	S5k5bafx_Capture,
	S5k5bafx_change_from_crop_to_full
};


static int s5k5bafx_read_reg(struct i2c_client *client, u32 addr, u16 *val)
{
	int err = 0;
	struct i2c_msg msg[2];
	unsigned char data[6];

	memset (data, 0, 6*sizeof(unsigned char));
	if (!client->adapter)
		return -ENODEV;


	msg[0].addr = client->addr;
	msg[0].flags = 0;
	msg[0].len = 4;
	msg[0].buf = data;
	/* high byte goes out first */
	data[0] = (u8) (addr >> 24) & 0xff;
	data[1] = (u8) (addr >> 16 ) & 0xff;
	data[2] = (u8) (addr >>8) & 0xff;
	data[3] = (u8) (addr & 0xff);

	msg[1].addr = client->addr;
	msg[1].flags = I2C_M_RD;
	msg[1].len = 2;
	msg[1].buf = data + 4;

	err = i2c_transfer(client->adapter, msg, 2);

	*val = data[4];

	return err ;
}

static int s5k5bafx_write_reg(struct i2c_client *client, u16 addr, u16 val)
{
	int err = 0;
	struct i2c_msg msg;
	unsigned char data[4];
	int retry = 0;

	memset (data, 0, 4*sizeof(unsigned char));

	if (!client->adapter)
		return -ENODEV;

	data[0] = (u8) ((addr >> 8) & 0xff);
	data[1] = (u8) (addr & 0xff);
	data[2] = (u8) ((val >>8) & 0xff);
	data[3] = (u8) (val  & 0xff);

	msg.addr = client->addr;
	msg.flags = 0;
	msg.len = 4;
	msg.buf = data;

//s32 i2c_write_block_data(client, u8 addr_flags, 1, addr, &val)
	do {
		err = i2c_transfer(client->adapter, &msg, 1);
		if (err >0 )
			return 0;
		retry++;
		pr_err("S5k5bafx: i2c transfer failed, retrying Add: 0x%x Val: 0x%x\n",
			addr, val);
		msleep(3);
	} while (retry <5);

	return err;
}


static int s5k5bafx_write_table(struct i2c_client *client,
			const i2c_cam_reg16 table[],
			const i2c_cam_reg16 override_list[],
			int num_override_regs)
{
	int err = 0;
	const i2c_cam_reg16 *next;
	int i;
	u16 val;

	for (next = table; next->addr != SEQUENCE_END; next++) {
		if (next->addr == SEQUENCE_WAIT_MS) {
			msleep(next->val);
			continue;
		}

		val = next->val;

		/* When an override list is passed in, replace the reg */
		/* value to write if the reg is in the list            */
		if (override_list) {
			for (i = 0; i < num_override_regs; i++) {
				if (next->addr == override_list[i].addr) {
					val = override_list[i].val;
					break;
				}
			}
		}

		err = s5k5bafx_write_reg(client, next->addr, val);
		if (err !=0)
			return err;
	}
	return err;
}

static int s5k5bafx_set_mode(struct cam_yuv_info *info, struct cam_yuv_mode *mode)
{
	int err = 0, set_index;
	set_index = mode->index;
	pr_info("%s : index: %d\n", __func__,set_index);

	err = s5k5bafx_write_table(info->i2c_client, s5k5bafx_SetModeSequenceList[set_index],NULL,0);
	if (err !=0)
		pr_info("%s : error : %d\n", __func__,err);

	return err;
}
static int s5k5bafx_set_color_effect(struct cam_yuv_info *info, unsigned int color_effect)
{
	int err = 0;
	pr_info("%s : %d\n", __func__, color_effect);

	switch(color_effect)
		{
			case YUVCamColorEffect_None :
				err = s5k5bafx_write_table(info->i2c_client, S5k5bafx_Color_Effect_None,NULL,0);
				break;

			case YUVCamColorEffect_Negative :
				err = s5k5bafx_write_table(info->i2c_client, S5k5bafx_Color_Effect_NEGATIVE,NULL,0);

				break;

			case YUVCamColorEffect_Aqua :
				err = s5k5bafx_write_table(info->i2c_client, S5k5bafx_Color_Effect_Aqua,NULL,0);

				break;

			case YUVCamColorEffect_Posterize :
				err = s5k5bafx_write_table(info->i2c_client, S5k5bafx_Color_Effect_Sketch,NULL,0);

				break;

			case YUVCamColorEffect_Sepia :
				err = s5k5bafx_write_table(info->i2c_client, S5k5bafx_Color_Effect_SEPIA,NULL,0);

				break;

			case YUVCamColorEffect_Mono :
				err = s5k5bafx_write_table(info->i2c_client, S5k5bafx_Color_Effect_MONO,NULL,0);

				break;

			default :
				err = s5k5bafx_write_table(info->i2c_client, S5k5bafx_Color_Effect_None,NULL,0);

				break;
		}
	if (err !=0)
		pr_info("%s : Color Effect : %d,  error : %d\n", __func__, color_effect, err);

	return err;
}

static int s5k5bafx_set_white_balance(struct cam_yuv_info *info, unsigned int wb_mode)
{
	int err = 0;
	pr_info("%s : %d\n", __func__, wb_mode);

	switch(wb_mode)
		{
			case YUVCamWhitebalance_Auto :
				err = s5k5bafx_write_table(info->i2c_client, S5k5bafx_MWB_Auto,NULL,0);
				break;

			case YUVCamWhitebalance_Incandescent :
				err = s5k5bafx_write_table(info->i2c_client, S5k5bafx_MWB_Incandescent,NULL,0);

				break;

			case YUVCamWhitebalance_SunLight :
				err = s5k5bafx_write_table(info->i2c_client, S5k5bafx_MWB_Daylight,NULL,0);

				break;

			case YUVCamWhitebalance_Fluorescent :
				err = s5k5bafx_write_table(info->i2c_client, S5k5bafx_MWB_Fluorescent,NULL,0);

				break;

			default :
				err = s5k5bafx_write_table(info->i2c_client, S5k5bafx_MWB_Auto,NULL,0);

				break;
		}
	if (err !=0)
		pr_info("%s : White Balance : %d,  error : %d\n", __func__, wb_mode, err);

	return err;
}

static int s5k5bafx_set_exposure(struct cam_yuv_info *info, int exposure)
{
	int err = 0;
	pr_info(" %s : %d \n", __func__,exposure);

	exposure = (int) exposure / 0x10000;
	switch(exposure)
		{
			case 	-2 :
				err = s5k5bafx_write_table(info->i2c_client, S5k5bafx_Exposure_Minus_2,NULL,0);
				break;

			case 	-1 :
				err = s5k5bafx_write_table(info->i2c_client, S5k5bafx_Exposure_Minus_1,NULL,0);

				break;

			case 	0 :
				err = s5k5bafx_write_table(info->i2c_client, S5k5bafx_Exposure_Normal,NULL,0);

				break;

			case 	1 :
				err = s5k5bafx_write_table(info->i2c_client, S5k5bafx_Exposure_Plus_1,NULL,0);

				break;

			case 	2 :
				err = s5k5bafx_write_table(info->i2c_client, S5k5bafx_Exposure_Plus_2,NULL,0);

				break;


			default :
				err = s5k5bafx_write_table(info->i2c_client, S5k5bafx_Exposure_Normal,NULL,0);

				break;
		}
	if (err !=0)
		pr_info("%s  : %d,  error : %d\n", __func__, exposure, err);

	return err;
}

static long s5k5bafx_ioctl(struct file *file,
			unsigned int cmd, unsigned long arg)
{
	struct cam_yuv_info *info = file->private_data;

	pr_info("%s, cmd : %d\n", __func__, cmd);

	switch (cmd) {
	case S5K5BAFX_IOCTL_SET_MODE:
	{
		struct cam_yuv_mode mode;
		if (copy_from_user(&mode,
					(const void __user *)arg,
					sizeof(struct cam_yuv_mode))) {
			return -EFAULT;
		}

		return s5k5bafx_set_mode(info, &mode);
	}
	case S5K5BAFX_IOCTL_SET_COLOR_EFFECT :
	{
		unsigned int color_effect;

		if (copy_from_user(&color_effect,
					(const void __user *)arg,
					sizeof(unsigned int))) {
			return -EFAULT;
		}

		return s5k5bafx_set_color_effect(info, color_effect);

	}
	case S5K5BAFX_IOCTL_SET_WHITE_BALANCE :
	{
		unsigned int white_balance;

		if (copy_from_user(&white_balance,
					(const void __user *)arg,
					sizeof(unsigned int))) {
			return -EFAULT;
		}
		return s5k5bafx_set_white_balance(info, white_balance);
	}

	case S5K5BAFX_IOCTL_SET_EXPOSURE:
	{
		int exposure;

		if (copy_from_user(&exposure,
					(const void __user *)arg,
					sizeof(int))) {
			return -EFAULT;
		}
		return s5k5bafx_set_exposure(info, exposure);
	}

	default:
		return -EINVAL;
	}
	return 0;
}


static int s5k5bafx_open(struct inode *inode, struct file *file)
{

	pr_info("%s\n", __func__);

	s5k5bafx_reset = TEGRA_GPIO_PBB1;
	s5k5bafx_pwrdn = TEGRA_GPIO_PD5;
	star_cam_VT_power_on();
	mdelay(10);
	gpio_request(s5k5bafx_reset, "s5k5bafx_Reset");
	gpio_request(s5k5bafx_pwrdn, "s5k5bafx_Power_Down");
	tegra_gpio_enable(s5k5bafx_reset);
	tegra_gpio_enable(s5k5bafx_pwrdn);
	gpio_direction_output(s5k5bafx_reset,0);
	gpio_direction_output(s5k5bafx_pwrdn,0);
	mdelay(1);
	gpio_set_value(s5k5bafx_pwrdn,1);
	mdelay(1);
	gpio_set_value(s5k5bafx_reset,1);
	mdelay(10);

	file->private_data = info;

//	pr_info("%s:----\n", __func__);

	return 0;
}

static int s5k5bafx_release(struct inode *inode, struct file *file)
{
	pr_info("%s\n", __func__);

	file->private_data = NULL;
	gpio_set_value(s5k5bafx_reset,0);
	gpio_set_value(s5k5bafx_pwrdn,0);
	gpio_free(s5k5bafx_reset);
	gpio_free(s5k5bafx_pwrdn);
	star_cam_power_off();
	return 0;
}

static const struct file_operations s5k5bafx_fileops = {
	.owner = THIS_MODULE,
	.open = s5k5bafx_open,
	.unlocked_ioctl = s5k5bafx_ioctl,
	.release = s5k5bafx_release,
};

static struct miscdevice s5k5bafx_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "s5k5bafx",
	.fops = &s5k5bafx_fileops,
	.mode = S_IRWXUGO
};

/****************************************************************************
			I2C Client & Driver
 ****************************************************************************/

static int s5k5bafx_probe(struct i2c_client *client,
			 const struct i2c_device_id *id)
{
	int err;
	pr_info("%s\n", __func__);


	info = kzalloc(sizeof(struct cam_yuv_info), GFP_KERNEL);
	if (!info) {
		pr_err("s5k5bafx: Unable to allocate memory!\n");
		return -ENOMEM;
	}
	err = misc_register(&s5k5bafx_device);
	if (err) {
		pr_err("s5k5bafx: Unable to register misc device!\n");
		kfree(info);
		return err;
	}

	info->pdata = client->dev.platform_data;
	info->i2c_client = client;

	i2c_set_clientdata(client, info);

	return 0;
}

static int s5k5bafx_remove(struct i2c_client *client)
{
	info = i2c_get_clientdata(client);
	misc_deregister(&s5k5bafx_device);
	kfree(info);
	return 0;
}

/* ----------------------------------------------------------------------- */

static const struct i2c_device_id s5k5bafx_id[] = {
	{ "s5k5bafx", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, s5k5bafx_id);

static struct i2c_driver s5k5bafx_i2c_driver = {
	.probe = s5k5bafx_probe,
	.remove = s5k5bafx_remove,
	.id_table = s5k5bafx_id,
	.driver = {
		.name = "s5k5bafx",
		.owner = THIS_MODULE,
	},
};


static int __init s5k5bafx_init(void)
{
	pr_info("s5k5bafx sensor driver loading\n");
	return i2c_add_driver(&s5k5bafx_i2c_driver);
}

static void __exit s5k5bafx_exit(void)
{
	i2c_del_driver(&s5k5bafx_i2c_driver);
}

module_init(s5k5bafx_init);
module_exit(s5k5bafx_exit);

