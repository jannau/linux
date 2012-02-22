/*
* imx072_eeprom.c EEPROM driver.
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
#include <media/imx072_eeprom.h>


#define EEPROM_MAX_RETRIES (3)

struct eeprom_info {
	struct i2c_client *i2c_client;
	struct regulator *regulator;
};

static u32 BuildBuffer(u8 *pBuf, u8 Width, u32 Value)
{
    u32 count = 0;
    switch (Width)
    {
		case 0:
		// possibly no address, some focusers use this
		break;

		// cascading switch
		case 32:
			pBuf[count++] = (u8)((Value>>24) & 0xFF);
		//     printf("pBuf[%d] : %d \n", count-1 , pBuf[count-1]);
		case 24:
			pBuf[count++] = (u8)((Value>>16) & 0xFF);
		//	printf("pBuf[%d] : %d \n", count-1 , pBuf[count-1]);
		case 16:
			pBuf[count++] = (u8)((Value>>8) & 0xFF);
		//	printf("pBuf[%d] : %d \n", count-1 , pBuf[count-1]);
		case 8:
			pBuf[count++] = (u8)(Value & 0xFF);
		//      printf("pBuf[%d] : %d \n", count-1 , pBuf[count-1]);
			break;

		default:
			pr_info("Unsupported Bit Width %d\n", Width);
		break;
    }
    return count;
}


static u32 RebuildValue(u8 *pBuf, u8 Width)
{
	u32 Value = 0;
	u32 count = 0;
	switch (Width)
	{
		case 0:
			break;
		case 32:
			Value |= pBuf[count++] << 24;
		case 24:
			Value |= pBuf[count++] << 16;
		case 16:
			Value |= pBuf[count++] << 8;
		case 8:
			Value |= pBuf[count++];
			break;
	}
	return Value;
}


static int eeprom_write(struct i2c_client *client, u16 addr, u32 value)
{
	int count;
	struct i2c_msg msg[1];
	unsigned char data[6];
	int retry = 0;
	pr_info("%s %d\n", __func__, __LINE__);

	if (!client->adapter)
		return -ENODEV;

	pr_info("%s %d\n", __func__, __LINE__);

	data[0] = (u8) (addr >> 8);
	data[1] = (u8) (addr & 0xff);

	data[2] = (u8) (value >> 24);
	data[3] = (u8) ((value >> 16)&0xff);
	data[4] = (u8) ((value >> 8)&0xff);
	data[5] = (u8) (value &0xff);

	msg[0].addr = client->addr;
	msg[0].flags = 0;
	msg[0].len = ARRAY_SIZE(data);
	msg[0].buf = data;
	pr_info("%s %d\n", __func__, __LINE__);
	do {
		count = i2c_transfer(client->adapter, msg, ARRAY_SIZE(msg));

		pr_info("[%s] i2c_transfer return value = %d\n",__func__, count);

		if (count == ARRAY_SIZE(msg))
			return 0;
		retry++;
		pr_err("[%s]: i2c transfer failed, retrying %x\n",__func__, value);
		msleep(3);
	}while (retry <= EEPROM_MAX_RETRIES);
	return -EIO;
}

static int eeprom_read(struct i2c_client *client, u16 addr, u32 *val)
{
	int err;
	struct i2c_msg msg[2];
	unsigned char data[6];

	//u32 temp_data = 0;

	if (!client->adapter)
		return -ENODEV;

	msg[0].addr = client->addr;
	msg[0].flags = 0;
	msg[0].len = 2;
	msg[0].buf = data;

	/* high byte goes out first */
	data[0] = (u8) (addr >> 8);
	data[1] = (u8) (addr & 0xff);

	msg[1].addr = client->addr;
	msg[1].flags = I2C_M_RD;
	msg[1].len = 4;
	msg[1].buf = data + 2;
	err = i2c_transfer(client->adapter, msg, 2);
	pr_info("[%s] i2c_transfer return value = %d\n",__func__, err);
	if (err != 2){
		pr_err("data read error %d",err);
		return -EINVAL;
	}
	*val =  RebuildValue(&(data[2]), 32);

//	pr_info("+++++[%s] i2c_transfer read value = %d+++++\n",__func__, *val);

	return 0;
}


static long eeprom_ioctl(struct file *file,
			unsigned int cmd, unsigned long arg)
{
	struct eeprom_info *info = file->private_data;
	mdelay(10); // for test
	switch (cmd) {
	case EEPROM_IOCTL_WRITE_REG:
	{
		struct eeprom_reg reg;
		if (copy_from_user(&reg,
					(const void __user *)arg,
					sizeof(struct eeprom_reg))) {
			pr_info("%s %d\n", __func__, __LINE__);
			return -EFAULT;
		}
		return eeprom_write(info->i2c_client, reg.addr, reg.val);
	}
	case EEPROM_IOCTL_READ_REG:
	{
		int err;
		struct eeprom_reg reg;
		if (copy_from_user(&reg,
					(const void __user *)arg,
					sizeof(struct eeprom_reg))) {
			pr_info("%s %d\n", __func__, __LINE__);
			return -EFAULT;
		}
		pr_info("%s %d\n", __func__, __LINE__);
		err = eeprom_read(info->i2c_client, reg.addr, &(reg.val));
		if(err<0)
			return -EFAULT;
		pr_info("%s %d\n", __func__, __LINE__);
		if (copy_to_user((void __user *)arg, &reg, sizeof(struct eeprom_reg))) {
			pr_info("%s %d\n", __func__, __LINE__);
			return -EFAULT;
		}
//		pr_info("%s %d\n", __func__, __LINE__);

	}
	break;
	default:
		return -EINVAL;
	}

	return 0;
}

static struct eeprom_info *info = NULL;

static int eeprom_open(struct inode *inode, struct file *file)
{
	file->private_data = info;
//	if (info->regulator)
//		regulator_enable(info->regulator);
	return 0;
}

int eeprom_release(struct inode *inode, struct file *file)
{
//	if (info->regulator)
//		regulator_disable(info->regulator);
	file->private_data = NULL;
	return 0;
}


static const struct file_operations eeprom_fileops = {
	.owner = THIS_MODULE,
	.open = eeprom_open,
	.unlocked_ioctl = eeprom_ioctl,
	.release = eeprom_release,
};

static struct miscdevice eeprom_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "imx072_eeprom",
	.fops = &eeprom_fileops,
};

static int eeprom_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	int err;

	pr_info("eeprom_probe: probing sensor.\n");

	info = kzalloc(sizeof(struct eeprom_info), GFP_KERNEL);
	if (!info) {
		pr_err("eeprom_probe: Unable to allocate memory!\n");
		return -ENOMEM;
	}

	err = misc_register(&eeprom_device);
	if (err) {
		pr_err("eeprom_probe: Unable to register misc device!\n");
		kfree(info);
		return err;
	}

	info->i2c_client = client;
	i2c_set_clientdata(client, info);
	return 0;
}

static int eeprom_remove(struct i2c_client *client)
{
	struct eeprom_info *info;
	info = i2c_get_clientdata(client);
	misc_deregister(&eeprom_device);
	kfree(info);
	return 0;
}

static const struct i2c_device_id eeprom_id[] = {
	{ "imx072_eeprom", 0 },
	{ },
};


MODULE_DEVICE_TABLE(i2c, eeprom_id);

static struct i2c_driver eeprom_i2c_driver = {
	.driver = {
		.name = "imx072_eeprom",
		.owner = THIS_MODULE,
	},
	.probe = eeprom_probe,
	.remove = eeprom_remove,
	.id_table = eeprom_id,
};


static int __init eeprom_init(void)
{
	pr_info("dw9716 sensor driver loading\n");
	return i2c_add_driver(&eeprom_i2c_driver);
}

static void __exit eeprom_exit(void)
{
	i2c_del_driver(&eeprom_i2c_driver);
}

module_init(eeprom_init);
module_exit(eeprom_exit);

