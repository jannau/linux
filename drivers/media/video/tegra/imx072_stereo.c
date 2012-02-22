/*
* imx072_main.c - sony imx072 5M Bayer type sensor driver
*
* Copyright (C) 2010 LG Inc.
*
* Contributors:
*      Hyunsu Choi <hyunsu.choi@lge.com>
*
* Leverage OV9640.c
*
* This file is licensed under the terms of the GNU General Public License
* version 2. This program is licensed "as is" without any warranty of any
* kind, whether express or implied.
*/

#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/i2c.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <media/imx072_main.h>
#include <mach/gpio-names.h>
#include <mach/hardware.h>
#include <linux/gpio.h>
#include "../../../startablet/star_cam_pmic.h"


struct imx072_stereo_info {
	int mode;
	struct i2c_client *i2c_client_left;
	struct i2c_client *i2c_client_right;
	struct imx072_platform_data *pdata;
};

#define imx072_TABLE_WAIT_MS 0
#define imx072_TABLE_END 1
#define imx072_MAX_RETRIES 3

static struct imx072_reg mode_start[] = {
	{0x0103,	0x01},  //reset 0
	{imx072_TABLE_WAIT_MS, 0x0001},

//	Stand-by OFF Sequence
//		Power ON
//		Input INCK
//		XCLR OFF
//		PLL Setting	INCK=24MHz,27times
//			Address	value
	{0x0307,	0x1B},
	{0x302B,	0x4B},
//	Global Setting
	{0x0101,	0x00},
	{0x300A,	0x80},
	{0x3014,	0x08},
	{0x3015,	0x37},
	{0x3017,	0x40},
	{0x301C,	0x01},
	{0x3031,	0x28},
	{0x3040,	0x00},
	{0x3041,	0x60},
	{0x3051,	0x24},
	{0x3053,	0x34},
	{0x3055,	0x3B},
	{0x3057,	0xC0},
	{0x3060,	0x30},
	{0x3065,	0x00},
	{0x30AA,	0x88},
	{0x30AB,	0x1C},
	{0x30B0,	0x32},
	{0x30B2,	0x83},
	{0x30D3,	0x04},
	{0x310E,	0xDD},
	{0x31A4,	0xD8},
	{0x31A6,	0x17},
	{0x31AC,	0xCF},
	{0x31AE,	0xF1},
	{0x31B4,	0xD8},
	{0x31B6,	0x17},

	{0x3304,	0x03},
	{0x3305,	0x03},
	{0x3306,	0x0A},
	{0x3307,	0x02},
	{0x3308,	0x11},
	{0x3309,	0x04},
	{0x330A,	0x05},
	{0x330B,	0x04},
	{0x330C,	0x05},
	{0x330D,	0x04},
	{0x330E,	0x01},

	{imx072_TABLE_END, 0x0000}
};

static struct imx072_reg mode_2592x1944[] = {

//		FULL
//		Mipi 2Lane	Address	value
//	{0x0340,	0x07},
//	{0x0341,	0xEE},
	{0x0104,	0x01},
	{0x034C,	0x0A},
	{0x034D,	0x30},
	{0x034E,	0x07},
	{0x034F,	0xA8},
	{0x0381,	0x01},
	{0x0383,	0x01},
	{0x0385,	0x01},
	{0x0387,	0x01},
	{0x3016,	0x06},
	{0x30E8,	0x06},
	{0x3301,	0x01},
	{0x0104,	0x00},
	//{0x0100 ,	0x01},

	{imx072_TABLE_END, 0x0000}

};

static struct imx072_reg mode_1296x972[] = {

	{0x034C,	0x05},//X SIZE
	{0x034D,	0x18},

	{0x034E,	0x03},// Y SIZE
	{0x034F,	0xD4},

	{0x0381,	0x01},
	{0x0383,	0x03},
	{0x0385,	0x01},
	{0x0387,	0x03},
	{0x3016,	0x06},
	{0x30E8,	0x06},
	{0x3301,	0x41},
	{imx072_TABLE_END, 0x0000}
};

static struct imx072_reg mode_1920x1088[] = {

//		1920x1080 (30fps)
//		Mipi 2Lane	Address	value
//	{0x0340,	0x06},//	1550	frame_length
//	{0x0341,	0x0E},//		frame_length
	{0x0344,	0x01},//	336	x_addr_start
	{0x0345,	0x50},//		x_addr_start
//	{0x0346,	0x01},//	432	y_addr_start
//	{0x0347,	0xB0},//		y_addr_start
	{0x0348,	0x08},//	2271	x_addr_end
	{0x0349,	0xDF},//		x_addr_end
//	{0x034A,	0x05},//	1527	y_addr_end
//	{0x034B,	0xF7},//		y_addr_end
	{0x034C,	0x07},//	1936	x_out_size
	{0x034D,	0x90},//		x_out_size
	{0x034E,	0x04},//	1096	y_out_size
	{0x034F,	0x48},//		y_out_size
	{0x0346,	0x01},
	{0x0347,	0xB3},// FOR RGB ORDER 
	{0x034A,	0x05},
	{0x034B,	0xFa},

	{0x0381,	0x01},//		x_even_inc
	{0x0383,	0x01},//		x_odd_inc
	{0x0385,	0x01},//		y_even_inc
	{0x0387,	0x01},//		y_odd_inc
	{0x3016,	0x06},//		VMODEADD
	{0x30E8,	0x06},//		HADDAVE
	{0x3301,	0x01},//		RGLANESEL,RGPXL_REPETITION,RGSRC2DIV
	{0x0100,	0x01},

	{imx072_TABLE_END, 0x0000}
};

static struct imx072_reg mode_1296X736[]=
{

	{0x0340,	0x03},
	{0x0341,	0xF7},
	{0x034C,	0x05},// X SIZE
	{0x034D,	0x18},
	{0x034E,	0x02},// Y SIZE
	{0x034F,	0xE8},

	{0x0346,	0x00},//		y_addr_start	
	{0x0347,	0x81},//		y_addr_start	
	{0x034A,	0x06},//		y_addr_end	
	{0x034B,	0x50},//		y_addr_end	

	{0x0381,	0x01},
	{0x0383,	0x03},
	{0x0385,	0x01},
	{0x0387,	0x03},
	{0x3016,	0x46},
	{0x30E8,	0x86},
	{0x3301,	0x01},
	{0x0100, 0x00},

	{imx072_TABLE_END, 0x0000, 1}

};

static struct imx072_reg mode_end[] = {

	{0x0100, 0x01}, /* Data Stream On*/
	/*	{FAST_SETMODE_END, 0}, */
	{imx072_TABLE_END, 0x0000}
};

enum {
//	imx072_MODE_2592x1944,
	imx072_MODE_1296x736,
//	imx072_MODE_1920x1088,
};

static struct imx072_reg *mode_table[] = {
//	[imx072_MODE_2592x1944] = mode_2592x1944,
	[imx072_MODE_1296x736] = mode_1296X736,
//	[imx072_MODE_1920x1088] = mode_1920x1088,
};

/* 2 regs to program frame length */
static inline void imx072_get_frame_length_regs(struct imx072_reg *regs,
						u32 frame_length)
{
	regs->addr = 0x0340;
	regs->val = (frame_length >> 8) & 0xff;
	(regs + 1)->addr = 0x0341;
	(regs + 1)->val = (frame_length) & 0xff;
}

/* 2 regs to program coarse time */
static inline void imx072_get_coarse_time_regs(struct imx072_reg *regs,
						u32 coarse_time)
{
	regs->addr = 0x0202;
	regs->val = (coarse_time >> 8) & 0xff;
	(regs + 1)->addr = 0x0203;
	(regs + 1)->val = (coarse_time) & 0xff;
}

/* 1 reg to program gain */
static inline void imx072_get_gain_reg(struct imx072_reg *regs, u16 gain)
{
	regs->addr = 0x0205;
	regs->val = gain;
}

static int imx072_write_reg(struct i2c_client *client, u16 addr, u8 val)
{
	int err;
	struct i2c_msg msg;
	unsigned char data[3];
	int retry = 0;

	if (!client->adapter)
		return -ENODEV;

	data[0] = (u8) (addr >> 8);;
	data[1] = (u8) (addr & 0xff);
	data[2] = (u8) (val & 0xff);

	msg.addr = client->addr;
	msg.flags = 0;
	msg.len = 3;
	msg.buf = data;
//s32 i2c_write_block_data(client, u8 addr_flags, 1, addr, &val)
	do {
		err = i2c_transfer(client->adapter, &msg, 1);
		if (err >0 )
			return 0;
		retry++;
		pr_err("imx072: i2c transfer failed, retrying %x %x\n",
			addr, val);
		msleep(3);
	} while (retry <imx072_MAX_RETRIES);

	return err;
}

static int imx072_write_table(struct i2c_client *client,
			const struct imx072_reg table[],
			const struct imx072_reg override_list[],
			int num_override_regs)
{
	int err;
	const struct imx072_reg *next;
	int i;
	u16 val;

	for (next = table; next->addr != imx072_TABLE_END; next++) {
		if (next->addr == imx072_TABLE_WAIT_MS) {
			msleep(next->val);
			continue;
		}

		val = next->val;

		/* When an override list is passed in, replace the reg	*/
		/* value to write if the reg is in the list		*/
		if (override_list) {
			for (i = 0; i < num_override_regs; i++) {
				if (next->addr == override_list[i].addr) {
					val = override_list[i].val;
					break;
				}
			}
		}

		err = imx072_write_reg(client, next->addr, val);
		if (err)
			return err;
	}
	return 0;
}

static int imx072_set_mode(struct imx072_stereo_info *info, struct imx072_mode *mode)
{
	int sensor_mode;
	int err;
	struct imx072_reg reg_list[5];

	pr_info("%s: xres %u yres %u framelength %u coarsetime %u gain %u\n",
		__func__, mode->xres, mode->yres, mode->frame_length,
		mode->coarse_time, mode->gain);
	if (mode->xres == 1296 && mode->yres == 736)
		sensor_mode = imx072_MODE_1296x736;
	else {
		pr_err("%s: invalid resolution supplied to set mode %d %d\n",
			func__, mode->xres, mode->yres);
		return -EINVAL;
	}

	/* get a list of override regs for the asking frame length,	*/
	/* coarse integration time, and gain.				*/
	imx072_get_frame_length_regs(reg_list, mode->frame_length);
		pr_info("%s: 1\n",__func__);
	imx072_get_coarse_time_regs(reg_list + 2, mode->coarse_time);
		pr_info("%s: 2\n",__func__);
	imx072_get_gain_reg(reg_list + 4, mode->gain);
	pr_info("%s: 2\n",__func__);
	err = imx072_write_table(info->i2c_client_left, mode_start, NULL, 0);
	pr_info("%s: 4\n",__func__);
	if (err)
		return err;
	err = imx072_write_table(info->i2c_client_right, mode_start, NULL, 0);
	pr_info("%s: 5\n",__func__);
	if (err)
		return err;
	err = imx072_write_table(info->i2c_client_left, mode_table[sensor_mode], reg_list, 5);
	if (err)
		return err;
	err = imx072_write_table(info->i2c_client_right, mode_table[sensor_mode], reg_list, 5);
	if (err)
		return err;
	err = imx072_write_table(info->i2c_client_left, mode_end, NULL, 0);
	if (err)
		return err;
	err = imx072_write_table(info->i2c_client_right, mode_end, NULL, 0);
	if (err)
		return err;
	info->mode = sensor_mode;
	return 0;
}

static int imx072_set_frame_length(struct imx072_stereo_info *info, u32 frame_length)
{
	struct imx072_reg reg_list[2];
	int i = 0;
	int ret;

	imx072_get_frame_length_regs(reg_list, frame_length);
	ret = imx072_write_reg(info->i2c_client_left, 0x0104, 0x01);
	if (ret)
		return ret;
	ret = imx072_write_reg(info->i2c_client_right, 0x0104, 0x01);
	if (ret)
		return ret;
	
	for (i = 0; i < 2; i++)	{
		ret = imx072_write_reg(info->i2c_client_left, reg_list[i].addr,
			reg_list[i].val);
		if (ret)
			return ret;
		ret = imx072_write_reg(info->i2c_client_right, reg_list[i].addr,
			reg_list[i].val);
		if (ret)
			return ret;
	}
	ret = imx072_write_reg(info->i2c_client_left, 0x0104, 0x00);
	if (ret)
		return ret;
	ret = imx072_write_reg(info->i2c_client_right, 0x0104, 0x00);
	if (ret)
		return ret;
	return 0;
}

static int imx072_set_coarse_time(struct imx072_stereo_info *info, u32 coarse_time)
{
	int ret;

	struct imx072_reg reg_list[2];
	int i = 0;

	imx072_get_coarse_time_regs(reg_list, coarse_time);

	ret = imx072_write_reg(info->i2c_client_left, 0x0104, 0x01);
	if (ret)
		return ret;
	ret = imx072_write_reg(info->i2c_client_right, 0x0104, 0x01);
	if (ret)
		return ret;
	for (i = 0; i < 2; i++)	{
		ret = imx072_write_reg(info->i2c_client_left, reg_list[i].addr,
			reg_list[i].val);
		if (ret)
			return ret;
		ret = imx072_write_reg(info->i2c_client_right, reg_list[i].addr,
			reg_list[i].val);
		if (ret)
			return ret;
	}

	ret = imx072_write_reg(info->i2c_client_left, 0x0104, 0x00);
	if (ret)
		return ret;

	ret = imx072_write_reg(info->i2c_client_right, 0x0104, 0x00);
	if (ret)
		return ret;
	
	return 0;
}

static int imx072_set_gain(struct imx072_stereo_info *info, u16 gain)
{
	int ret;
	struct imx072_reg reg_list;

	imx072_get_gain_reg(&reg_list, gain);
	ret = imx072_write_reg(info->i2c_client_left, 0x0104, 0x01);
	if (ret)
		return ret;
	ret = imx072_write_reg(info->i2c_client_right, 0x0104, 0x01);
	if (ret)
		return ret;

	ret = imx072_write_reg(info->i2c_client_left, reg_list.addr, reg_list.val);
	if (ret)
		return ret;
	ret = imx072_write_reg(info->i2c_client_right, reg_list.addr, reg_list.val);
	if (ret)
		return ret;
		
	ret = imx072_write_reg(info->i2c_client_left, 0x0104, 0x00);
	if (ret)
		return ret;
	ret = imx072_write_reg(info->i2c_client_right, 0x0104, 0x00);
	if (ret)
		return ret;
	return 0;
}

static int imx072_get_status(struct imx072_stereo_info *info, u8 *status)
{
	int err=0;

	*status = 0;
//	err = imx072_read_reg(info->i2c_client, 0x002, status);  // which status do you want?
	pr_info("%s: %u %d\n", __func__, *status, err);
	return err;
}


static int imx072_ioctl(struct inode *inode, struct file *file,
			unsigned int cmd, unsigned long arg)
{
	int err;
	struct imx072_stereo_info *info = file->private_data;

	pr_info("\nimx072_ioctl : cmd = %d\n", cmd);

	switch (cmd) {
	case IMX072_IOCTL_SET_MODE:
	{
		struct imx072_mode mode;
		if (copy_from_user(&mode,
				   (const void __user *)arg,
				   sizeof(struct imx072_mode))) {
			pr_info("%s %d\n", __func__, __LINE__);
			return -EFAULT;
		}

		return imx072_set_mode(info, &mode);
	}
	case IMX072_IOCTL_SET_FRAME_LENGTH:
		return imx072_set_frame_length(info, (u32)arg);
	case IMX072_IOCTL_SET_COARSE_TIME:
		return imx072_set_coarse_time(info, (u32)arg);
	case IMX072_IOCTL_SET_GAIN:
		return imx072_set_gain(info, (u16)arg);
	case IMX072_IOCTL_GET_STATUS:
	{
		u8 status;

		err = imx072_get_status(info, &status);
		if (err)
			return err;
		if (copy_to_user((void __user *)arg, &status,
				 2)) {
			pr_info("%s %d\n", __func__, __LINE__);
			return -EFAULT;
		}
		return 0;
	}
	default:
		return -EINVAL;
	}
	return 0;
}

static struct imx072_stereo_info *info=NULL;
static int Imx072L_RESET;
static int Imx072L_PWRDN;
static int Imx072R_RESET;
static int Imx072R_PWRDN;
static int imx072_open(struct inode *inode, struct file *file)
{
	u8 status;
	
	pr_info("%s:++++\n", __func__);

	Imx072L_RESET = TEGRA_GPIO_PD2;
	Imx072L_PWRDN = TEGRA_GPIO_PBB5;
	Imx072R_RESET = TEGRA_GPIO_PT4;
	Imx072R_PWRDN =TEGRA_GPIO_PT3 ;
	
	star_cam_Main_power_on();
	
	gpio_request(Imx072L_RESET, "CamLeft_Reset");
	gpio_request(Imx072L_PWRDN, "CamLeft_Power_Down");
	gpio_request(Imx072R_RESET, "CamRight_Reset");	
	gpio_request(Imx072R_PWRDN, "CamRight_Power_Down");
	tegra_gpio_enable(Imx072L_RESET);
	tegra_gpio_enable(Imx072L_PWRDN);
	tegra_gpio_enable(Imx072R_RESET);
	tegra_gpio_enable(Imx072R_PWRDN);
	gpio_direction_output(Imx072L_RESET,1);
	gpio_direction_output(Imx072L_PWRDN,1);
	gpio_direction_output(Imx072R_RESET,1);
	gpio_direction_output(Imx072R_PWRDN,1);
	gpio_set_value(Imx072L_RESET,0);
	gpio_set_value(Imx072L_PWRDN,0);
	gpio_set_value(Imx072R_RESET,0);
	gpio_set_value(Imx072R_PWRDN,0);
	mdelay(10);
	gpio_set_value(Imx072L_RESET,1);
	gpio_set_value(Imx072R_RESET,1);
	mdelay(10);
	gpio_set_value(Imx072L_PWRDN,1);
	gpio_set_value(Imx072R_PWRDN,1);
	mdelay(10);

	file->private_data = info;
	pr_info("%s:---\n", __func__);
/*	if (info->pdata && info->pdata->power_on)
		info->pdata->power_on();
*/
//	imx072_get_status(info, &status);
	return 0;
}

static int imx072_release(struct inode *inode, struct file *file)
{
	pr_info("%s\n", __func__);
//	if (info->pdata && info->pdata->power_off)
//		info->pdata->power_off();
	file->private_data = NULL;
	gpio_free(Imx072L_RESET);
	gpio_free(Imx072L_PWRDN);
	gpio_free(Imx072R_RESET);
	gpio_free(Imx072R_PWRDN);

	star_cam_power_off();
	return 0;
}


static const struct file_operations imx072_stereo_fileops = {
	.owner = THIS_MODULE,
	.open = imx072_open,
	.ioctl = imx072_ioctl,
	.release = imx072_release,
};

static struct miscdevice imx072_stereo_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "imx072_stereo",
	.fops = &imx072_stereo_fileops,
};

static int imx072L_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	int err;
	pr_info("imx072L: probing sensor.\n");

	if(!info)
	{
		info = kzalloc(sizeof(struct imx072_stereo_info), GFP_KERNEL);
		if (!info) {
			pr_err("imx072: Unable to allocate memory!\n");
			return -ENOMEM;
		}
	}
	err = misc_register(&imx072_stereo_device);
	if (err) {
		pr_err("imx072L: Unable to register misc device!\n");
		kfree(info);
		return err;
	}

	info->pdata = client->dev.platform_data;
	info->i2c_client_left = client;

//	i2c_set_clientdata(client,info->i2c_client_left);

	return 0;
}

static int imx072R_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	int err;
	pr_info("imx072R: probing sensor.\n");
	if(!info)
	{
		info = kzalloc(sizeof(struct imx072_stereo_info), GFP_KERNEL);
		if (!info) {
			pr_err("imx072: Unable to allocate memory!\n");
			return -ENOMEM;
		}
	}
	pr_info("imx072R: probing sensor2.\n");
	info->i2c_client_right = client;

//	i2c_set_clientdata(client, info->i2c_client_right);

	return 0;
}

static int imx072L_remove(struct i2c_client *client)
{
	struct imx072_stereo_info *info;
//	info = i2c_get_clientdata(client);
	misc_deregister(&imx072_stereo_device);
	if(!info){
		kfree(info);
		info = NULL;
	}
	return 0;
}
static int imx072R_remove(struct i2c_client *client)
{
	struct imx072_info *info;
//	info = i2c_get_clientdata(client);
//	misc_deregister(&imx072_device);
	if(!info){
		kfree(info);
		info = NULL;
	}
	return 0;
}
static const struct i2c_device_id imx072_stereo_left_id[] = {
	{ "imx072L", 0 },
	{ },
};
static const struct i2c_device_id imx072_stereo_right_id[] = {
	{ "imx072R", 0 },	
	{ },
};
MODULE_DEVICE_TABLE(i2c, imx072_stereo_id);

static struct i2c_driver imx072L_i2c_driver = {
	.probe = imx072L_probe,
	.remove = imx072L_remove,
	.id_table = imx072_stereo_left_id,
	.driver = {
		.name = "imx072L",
		.owner = THIS_MODULE,
	},
};

static struct i2c_driver imx072R_i2c_driver = {
	.probe = imx072R_probe,
	.remove = imx072R_remove,
	.id_table = imx072_stereo_right_id,
	.driver = {
		.name = "imx072R",
		.owner = THIS_MODULE,
	},
};

static int __init imx072_stereo_init(void)
{
	pr_info("imx072 Stereo sensor driver loading\n");
	i2c_add_driver(&imx072L_i2c_driver);
	i2c_add_driver(&imx072R_i2c_driver);
}

static void __exit imx072_stereo_exit(void)
{
	i2c_del_driver(&imx072L_i2c_driver);
	i2c_del_driver(&imx072R_i2c_driver);
}

module_init(imx072_stereo_init);
module_exit(imx072_stereo_exit);