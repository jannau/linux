/*
* dw9716.c focuser driver.
*
* Copyright (C) 2010 LGE Inc.
*
* This file is licensed under the terms of the GNU General Public License
* version 2. This program is licensed "as is" without any warranty of any
* kind, whether express or implied.
*/

#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/i2c.h>
#include <linux/miscdevice.h>
#include <linux/regulator/consumer.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <media/dw9716.h>


#define POS_LOW 50
#define POS_HIGH 1000
#define SETTLETIME_MS 100
#define FOCAL_LENGTH (3.5f)
#define FNUMBER (2.6f)
#define FPOS_COUNT 1024
DEFINE_MUTEX(star_focuser_lock);
#define DW9716_MAX_RETRIES (3)

struct dw9716_info {
	struct i2c_client *i2c_client;
	struct i2c_client *i2c_client_right;
	struct regulator *regulator;
	struct dw9716_config config;
};

enum StereoCameraMode{
	Main = 0,
	/// Sets the stereo camera to stereo mode.
	Stereo = 1,
	/// Only the sensor on the left is on.
	LeftOnly,
	/// Only the sensor on the right is on.
	RightOnly,
	/// Ignore -- Forces compilers to make 32-bit enums.
	StereoCameraMode_Force32 = 0x7FFFFFFF
};
static enum StereoCameraMode camera_mode;

static int dw9716_write(struct i2c_client *client, u16 value)
{
	int count;
	struct i2c_msg msg[1];
	unsigned char data[2];
	int retry = 0;

	if (!client->adapter)
		return -ENODEV;

	data[0] = (u8) ((value >> 4) & 0x3F);
	data[1] = (u8) ((value & 0xF) << 4);
	data[1] = (data[1] &0xF0) |0x05;		// Slew rate control (8 steps, 50us)

	msg[0].addr = client->addr;
	msg[0].flags = 0;
	msg[0].len = ARRAY_SIZE(data);
	msg[0].buf = data;
//	pr_info("%s: focuser set position = %d, 0x%x\n", __func__, value, *(u16*)data);
	do {
		count = i2c_transfer(client->adapter, msg, ARRAY_SIZE(msg));
		if (count == ARRAY_SIZE(msg))
			return 0;
		retry++;
		pr_err("dw9716: i2c transfer failed, retrying %x\n",
				value);
		msleep(3);
	}while (retry <= DW9716_MAX_RETRIES);
	return -EIO;
}
static int dw9716_write_helper(struct dw9716_info *info, u16 value)
{
	int ret;
	switch(camera_mode){
		case Main:
		case LeftOnly:
			ret = dw9716_write(info->i2c_client,  value);
			break;
		case Stereo:
			ret = dw9716_write(info->i2c_client,  value);
			ret = dw9716_write(info->i2c_client_right,  value);
			break;
		case RightOnly:
			ret = dw9716_write(info->i2c_client_right,  value);
			break;
		default :
			return -1;
	}
	return ret;
}
static int dw9716_set_position(struct dw9716_info *info, u32 position)
{
	if (position < info->config.pos_low ||
		position > info->config.pos_high)
		return -EINVAL;

	return dw9716_write(info->i2c_client, position);
}

static long dw9716_ioctl(struct file *file,
			unsigned int cmd, unsigned long arg)
{
	struct dw9716_info *info = file->private_data;
	int ret;
	switch (cmd) {
	case DW9716_IOCTL_GET_CONFIG:
	{
		if (copy_to_user((void __user *) arg,
				 &info->config,
				 sizeof(info->config))) {
			pr_err("%s: 0x%x\n", __func__, __LINE__);
			return -EFAULT;
		}

		break;
	}
	case DW9716_IOCTL_SET_POSITION:
		mutex_lock(&star_focuser_lock);
		ret = dw9716_set_position(info, (u32) arg);
		mutex_unlock(&star_focuser_lock);
		return ret;
	case DW9716_IOCTL_SET_MODE:
		camera_mode =(enum StereoCameraMode)arg;
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static struct dw9716_info *info = NULL;

static int dw9716_open(struct inode *inode, struct file *file)
{
	file->private_data = info;
	if (info->regulator)
		regulator_enable(info->regulator);
	return 0;
}

int dw9716_release(struct inode *inode, struct file *file)
{
	if (info->regulator)
		regulator_disable(info->regulator);
	file->private_data = NULL;
	return 0;
}


static const struct file_operations dw9716_fileops = {
	.owner = THIS_MODULE,
	.open = dw9716_open,
	.unlocked_ioctl = dw9716_ioctl,
	.release = dw9716_release,
};

static struct miscdevice dw9716_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "dw9716",
	.fops = &dw9716_fileops,
};

static int dw9716_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	int err;

	pr_info("dw9716: probing sensor.\n");

	info = kzalloc(sizeof(struct dw9716_info), GFP_KERNEL);
	if (!info) {
		pr_err("dw9716: Unable to allocate memory!\n");
		return -ENOMEM;
	}

	err = misc_register(&dw9716_device);
	if (err) {
		pr_err("dw9716: Unable to register misc device!\n");
		kfree(info);
		return err;
	}
//WBT#196353 : don'use the regulator. the power turn on when camera turn on.
/*
	info->regulator = regulator_get(&client->dev, "p_cam_avdd");
	if (IS_ERR_OR_NULL(info->regulator)) {
		dev_err(&client->dev, "unable to get regulator %s\n",
			dev_name(&client->dev));
		info->regulator = NULL;
	} else {
		regulator_enable(info->regulator);
	}
*/
	info->regulator = 0;
	info->i2c_client = client;
	info->config.settle_time = SETTLETIME_MS;
	info->config.focal_length = FOCAL_LENGTH;
	info->config.fnumber = FNUMBER;
	info->config.pos_low = POS_LOW;
	info->config.pos_high = POS_HIGH;
	i2c_set_clientdata(client, info);
	return 0;
}

static int dw9716_remove(struct i2c_client *client)
{
	struct dw9716_info *info;
	info = i2c_get_clientdata(client);
	misc_deregister(&dw9716_device);
	kfree(info);
	return 0;
}

static int dw9716R_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	//int err;

	pr_info("dw9716R: probing sensor.\n");
	if(!info){
		info = kzalloc(sizeof(struct dw9716_info), GFP_KERNEL);
		if (!info) {
			pr_err("dw9716: Unable to allocate memory!\n");
			return -ENOMEM;
		}
	}
/*
	err = misc_register(&dw9716_device);
	if (err) {
		pr_err("dw9716: Unable to register misc device!\n");
		kfree(info);
		return err;
	}

	info->regulator = regulator_get(&client->dev, "p_cam_avdd");
	if (IS_ERR_OR_NULL(info->regulator)) {
		dev_err(&client->dev, "unable to get regulator %s\n",
			dev_name(&client->dev));
		info->regulator = NULL;
	} else {
		regulator_enable(info->regulator);
	}
*/
	info->i2c_client_right = client;
//	i2c_set_clientdata(client, info);
	return 0;
}

static int dw9716R_remove(struct i2c_client *client)
{
//WBT#196352
//	struct dw9716_info *info;
	if(!info){
		kfree(info);
		info = NULL;
	}
	return 0;
}



static const struct i2c_device_id dw9716_id[] = {
	{ "dw9716", 0 },
	{ },
};
static const struct i2c_device_id dw9716R_id[] = {
	{ "dw9716R", 0 },
	{ },
};

MODULE_DEVICE_TABLE(i2c, dw9716_id);

static struct i2c_driver dw9716_i2c_driver = {
	.driver = {
		.name = "dw9716",
		.owner = THIS_MODULE,
	},
	.probe = dw9716_probe,
	.remove = dw9716_remove,
	.id_table = dw9716_id,
};
static struct i2c_driver dw9716R_i2c_driver = {
	.driver = {
		.name = "dw9716R",
		.owner = THIS_MODULE,
	},
	.probe = dw9716R_probe,
	.remove = dw9716R_remove,
	.id_table = dw9716R_id,
};

static int __init dw9716_init(void)
{
	pr_info("dw9716 sensor driver loading\n");
	i2c_add_driver(&dw9716_i2c_driver);
	i2c_add_driver(&dw9716R_i2c_driver);

	return 0;
}

static void __exit dw9716_exit(void)
{
	i2c_del_driver(&dw9716_i2c_driver);
	i2c_del_driver(&dw9716R_i2c_driver);
}

module_init(dw9716_init);
module_exit(dw9716_exit);
