/* bh1721_als.c
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


#define LED_LIGHT_REMOVAL
//#define CAM_ON_CHECK
#define VER_ALS_MAJ	0
#define VER_ALS_MIN	6

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <linux/input.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/earlysuspend.h>
#include <mach/gpio-names.h>
#include "bh1721_als.h"
/* TODO: not used, so delete it */
#define LIGHTSENSOR_IOCTL_MAGIC		'l'
#define LIGHTSENSOR_IOCTL_GET_ENABLED	_IOR(LIGHTSENSOR_IOCTL_MAGIC, 1, int *)
#define LIGHTSENSOR_IOCTL_ENABLE	_IOW(LIGHTSENSOR_IOCTL_MAGIC, 2, int *)

static int als_read_lux(int *lux);
#define D(x...)		do{ if (dbg) { pr_err("bh1721: " x); } }while(0)
//#define D(x...)
#define DERR(x...)	pr_err("bh1721: " x)

/* HW revision dependent defines */
#if defined (CONFIG_MACH_STARTABLET_EVB)
#define DVI_GPIO_PIN	TEGRA_GPIO_PS7
#elif defined (CONFIG_MACH_STARTABLET_REV_A) || \
	defined (CONFIG_MACH_STARTABLET_REV_A1)
#define DVI_GPIO_PIN	TEGRA_GPIO_PI7
#else
#define DVI_GPIO_PIN	TEGRA_GPIO_PD0
#endif

#if defined (CAM_ON_CHECK)
#if defined (CONFIG_MACH_STARTABLET_EVB)
#define CAM_EN_PIN	TEGRA_GPIO_PR6
#elif defined(CONFIG_MACH_STARTABLET_REV_A) || \
	defined(CONFIG_MACH_STARTABLET_REV_A1) || \
	defined(CONFIG_MACH_STARTABLET_REV_C)
#define CAM_EN_PIN	TEGRA_GPIO_PQ1
#else
#define CAM_EN_PIN	TEGRA_GPIO_PS5
#endif //CONFIG_MACH_STARTABLET
#endif //CAM_ON_CHECK

#define ALS_POLL_TIME			1000	/* ms */

/* scail = 4.17 = DLFT_SCAL_M / DFLT_SCAIL_N */
#define DFLT_SCAIL_M		656
#define DFLT_SCAIL_N		100

#define DFLT_RESOL		50
#define MIN_ALS_VAL		0
#define MAX_ALS_VAL		65528
#define DFLT_RESMODE		AUTO_RES_MODE

#define I2C_PWDOWN		0x0
#define I2C_PWON		0x01
#define I2C_AUTO_RES_MODE	0x10
#define I2C_HIGH_RES_MODE	0x12
#define I2C_LOW_RES_MODE	0x13

#define SET_STAT(x)	(the_data.b_status |= 1<<x)
#define CLR_STAT(x) 	(the_data.b_status &= ~(1<<x))
#define GET_STAT(x) 	(!!(the_data.b_status & 1<<x))

static int dbg = 0;
#if defined (CAM_ON_CHECK)
static int cam_on = 0;
#endif
static int enter = 0;

#if defined (LED_LIGHT_REMOVAL)
#define ALS_POLL_TIME_LED_BLINK		200	/* ms */
#define BLINK_POLL_COUNT 5
#endif
enum {
	PS = 0,	/* power supply */
	LG,	/* dvi pin */
	CA,	/* chip active state */
	IR,	/* input report poll */
};

static struct bh1721_data {
	struct	input_dev *input_dev;
	struct	i2c_client *client;
	char	phys_name[32];
	int	enabled;
	int	lux;
	int	test_lux;
	/* timer data */
	int	polling;
	struct	work_struct work;
	struct	timer_list timer;
	int	b_status;
	struct	semaphore als_sem;
	int	resmode;
	int	resol;
	int	scail_m;
	int	scail_n;
	struct	early_suspend	early_suspend;
#if defined (LED_LIGHT_REMOVAL)
	int	blink_on;
	int	led_value;
	int	lux_min;
	int	lux_blink;
	unsigned int	blink_c;
#endif
} the_data;

static int misc_opened;

/* dvi gpio control */
static int dvi_high(int on)
{
	if (on != 0 && on != 1) {
		DERR("%s : on value : %dis not valid \n", __func__, on);
		return -1;
	}

	if(!GET_STAT(IR))
		msleep(2);

	if(on)
		SET_STAT(LG);
	else
		CLR_STAT(LG);

	gpio_set_value(DVI_GPIO_PIN, on);
	return 0;

}

/* read data */
static int bh1721_chip_i2c_read_raw_lux(int *raw_lux)
{
	int status = 0;
	char buf[2];
	D("enter %s \n", __func__);
	status = i2c_master_recv(the_data.client, buf, 2);
	if(status == 2) {
		*raw_lux = (int)buf[0];
		*raw_lux <<= 8;
		*raw_lux += (int)buf[1];
		D("raw lux =  %d \n", *raw_lux);
		return 0;
	} else {
		DERR("%s read failed\n", __func__);
		return status;
	}
}

static int bh1721_chip_i2c_write_cmd(char cmd)
{
	int status = 0;
	D("enter %s \n", __func__);
	status = i2c_master_send(the_data.client, &cmd, 1);
	if (status == 1) {
		status = 0;
	} else {
		DERR("%s wirte failed\n", __func__);
		status = -1;
	}
	if (cmd == I2C_PWON && status == 0)
		SET_STAT(CA);
	else if (cmd == I2C_PWDOWN && status == 0)
		CLR_STAT(CA);
	return status;
}

/* timer : first half work */
static void bh1721_poll_timer(unsigned long data)
{
	D("enter %s \n", __func__);
	if(!GET_STAT(IR)) {
		DERR("Enter %s during polling disabled\n", __func__);
		return;
	}
	schedule_work(&the_data.work);
}

/* timer work : real work is done here */
static void bh1721_poll_timer_work(struct work_struct *work)
{
	D("enter %s \n", __func__);
	als_cmd(REPORT, REPORT_WORK);
}

/* timer start : timer register */
static void bh1721_poll_timer_start(void)
{
	D("enter %s \n", __func__);
	if(GET_STAT(IR)) {
		DERR("Enter %s during poll enabled\n", __func__);
		return;
	}
	SET_STAT(IR);
	setup_timer(&the_data.timer, bh1721_poll_timer,
						(unsigned long)&the_data);
	INIT_WORK(&the_data.work, bh1721_poll_timer_work);
	schedule_work(&the_data.work);
}

/* timer stop : timer unregister */
static void bh1721_poll_timer_stop(void)
{
	D("enter %s \n", __func__);
	if(!GET_STAT(IR)) {
		DERR("Enter %s during poll disabled\n", __func__);
		return;
	}
	del_timer_sync(&the_data.timer);
	flush_work(&the_data.work);
	CLR_STAT(IR);
}

static int als_read_lux(int *lux)
{
	int status = 0;
	int i = 0;
	D("enter %s \n", __func__);

	status = bh1721_chip_i2c_read_raw_lux(lux);
	if (status) {
		DERR("%s : REPORT_WORK error \n", __func__);
		return status;
	}

	D("%s raw lux value = %d\n", __func__, *lux);

#if defined (LED_LIGHT_REMOVAL)
		if(!the_data.blink_on)
			*lux -= the_data.led_value;
#endif
	D("%s raw lux value - led_value = %d\n", __func__, *lux);
	/* compensation in low luminance */
	if (*lux < 8)		/* X 1 */
		*lux *= 2;
	else if (*lux < 24) {	/* X 1.5 */
		*lux *= 3;
		*lux /= 2;
	}


	if(*lux < 0) {
		DERR("%s : lux is minus %d \n", __func__, *lux);
		*lux = 0;
	}

	*lux *= the_data.scail_m;
	if((*lux % the_data.scail_n) * 2 >= the_data.scail_n)
		i = 1;
	else
		i = 0;
	*lux =	*lux / the_data.scail_n + i;
	if((*lux % the_data.resol) * 2 >= the_data.resol)
		i=1;
	else
		i=0;
	*lux = *lux / the_data.resol + i;
	*lux *= the_data.resol;
	return status;
}

/*
 *	static int als_cmd(int cmd, int arg)
 *	: this function changes global variables, so only one control path can
 *	enter it.
 *
 *	Return 0 if success
 */
int als_cmd(int cmd, int arg)
{
	int ret = 0;
	int mtv = 0;
	int prev_lux = 0;

	D("calling %s(%d,%d)\n", __func__, cmd, arg);

	ret = down_interruptible(&the_data.als_sem);
	D("enter als_sem mutex %d \n", ++enter);
	if(ret) {
		DERR("%s : als semaphore error \n", __func__);
		return -1;
	}

	switch (cmd) {
	case PW:
		switch(arg) {
		case 1:
			ret = bh1721_chip_i2c_write_cmd(I2C_PWON);
			break;
		case 0:
			ret = bh1721_chip_i2c_write_cmd(I2C_PWDOWN);
			break;
		default:
			ret = -1;
			break;
		}
		break;
	case RES_MODE:
		switch(arg) {
		case HIGH_RES_MODE:
			ret =
			bh1721_chip_i2c_write_cmd(I2C_HIGH_RES_MODE);
			the_data.resmode = HIGH_RES_MODE;
			break;
		case LOW_RES_MODE:
			ret = bh1721_chip_i2c_write_cmd(I2C_LOW_RES_MODE);
			the_data.resmode = LOW_RES_MODE;
			break;
		case AUTO_RES_MODE:
			ret = bh1721_chip_i2c_write_cmd(I2C_AUTO_RES_MODE);
			the_data.resmode = AUTO_RES_MODE;
			break;
		default:
			ret = -1;
			break;
		}
		break;
	case READ_LUX:
		ret = als_read_lux(&the_data.lux);
		break;
	case SET_MGT:
		mtv = (((300 * arg) / 100) & 0x3f0) + 0xc;
		D("%s MT will be changed to :  %d percent \n"
						, __func__, (mtv * 100) / 300);
		ret = bh1721_chip_i2c_write_cmd((mtv >> 5) | 0x40);
		ret |= bh1721_chip_i2c_write_cmd(0x60 | ((mtv << 1) & 0x10));
		break;
	case DVI:
		switch(arg) {
			case 1:
				ret = dvi_high(1);
				break;
			case 0:
				ret = dvi_high(0);
				break;
			default:
				ret = -1;
				break;
		}
		break;
	case REPORT:
		switch(arg) {
		case REPORT_ON:
			the_data.lux = -1;
			bh1721_poll_timer_start();
			break;
		case REPORT_OFF:
			bh1721_poll_timer_stop();
			break;
		case REPORT_WORK:
			D("%s : enter REPORT_WORK \n", __func__);

#if defined (CAM_ON_CHECK)
			cam_on = gpio_get_value(CAM_EN_PIN);
			D("cam_on = %d\n", cam_on);

			if(!cam_on) {
#endif
#if defined (LED_LIGHT_REMOVAL)
				if(the_data.blink_on) {
					ret = als_read_lux(&the_data.lux_blink);
					if (ret) {
						DERR("%s : als_read_lux error\n"
							, __func__);
						break;
					}
					if(the_data.lux_blink <= the_data.lux_min)
						the_data.lux_min = the_data.lux_blink;

					mod_timer(&the_data.timer, jiffies +
						msecs_to_jiffies(the_data.polling));
					if(the_data.blink_c++ % BLINK_POLL_COUNT != 0)
						break;
					if (the_data.lux != the_data.lux_min) {
						the_data.lux = the_data.lux_min;
						D("%s : report lux = %d\n", __func__,
									the_data.lux);
						input_report_abs(the_data.input_dev,
							ABS_PRESSURE, the_data.lux);
						input_sync(the_data.input_dev);
					}
					the_data.lux_min = 0x7ffffff;
					break;
				}
#endif

				prev_lux = the_data.lux;
				ret = als_read_lux(&the_data.lux);
				if (ret) {
					DERR("%s : als_read_lux error\n"
						, __func__);
					break;
				}
				if (prev_lux != the_data.lux) {
					D("%s : report lux = %d\n", __func__,
								the_data.lux);
					input_report_abs(the_data.input_dev,
						ABS_PRESSURE, the_data.lux);
					input_sync(the_data.input_dev);
				}
#if defined (CAM_ON_CHECK)
			}
#endif
			mod_timer(&the_data.timer, jiffies +
					msecs_to_jiffies(the_data.polling));
		break;
		default:
			ret = -1;
			break;
		}
		break;
	case RESOL:
		the_data.resol = arg;
		break;
	case SCAIL_M:
		the_data.scail_m = arg;
		break;
	case SCAIL_N:
		the_data.scail_n = arg;
		break;
	case POLLTIME:
		the_data.polling = arg;
		break;
	case ALS_TEST_READ:
		ret = dvi_high(1);
		ret |= bh1721_chip_i2c_write_cmd(I2C_PWON);
		switch(the_data.resmode) {
		case HIGH_RES_MODE:
			ret |= bh1721_chip_i2c_write_cmd(I2C_HIGH_RES_MODE);
			break;
		case LOW_RES_MODE:
			ret |= bh1721_chip_i2c_write_cmd(I2C_LOW_RES_MODE);
			break;
		case AUTO_RES_MODE:
			ret |= bh1721_chip_i2c_write_cmd(I2C_AUTO_RES_MODE);
			break;
		default:
			ret = -1;
			break;
		}
		ret |= als_read_lux(&the_data.test_lux);
		break;
	case ALS_ON:
		ret = dvi_high(1);
		ret |= bh1721_chip_i2c_write_cmd(I2C_PWON);
		switch(the_data.resmode) {
		case HIGH_RES_MODE:
			ret |= bh1721_chip_i2c_write_cmd(I2C_HIGH_RES_MODE);
			break;
		case LOW_RES_MODE:
			ret |= bh1721_chip_i2c_write_cmd(I2C_LOW_RES_MODE);
			break;
		case AUTO_RES_MODE:
			ret |= bh1721_chip_i2c_write_cmd(I2C_AUTO_RES_MODE);
			break;
		default:
			ret = -1;
			break;
		}
		the_data.lux = -1;
		D("%s : report lux = %d\n", __func__,-1);
		input_report_abs(the_data.input_dev, ABS_PRESSURE, -1);
		input_sync(the_data.input_dev);
		bh1721_poll_timer_start();
		the_data.enabled = 1;
		break;
	case ALS_OFF:
		if (the_data.enabled == 0) {
			ret = 0;
			break;
		}
		bh1721_poll_timer_stop();
		ret = bh1721_chip_i2c_write_cmd(I2C_PWDOWN);
		ret |= dvi_high(0);
		the_data.enabled = 0;
		break;

#if defined (LED_LIGHT_REMOVAL)
	case ALS_LED_BLINK:
		the_data.blink_on = arg;
		if(arg)
			the_data.polling = ALS_POLL_TIME_LED_BLINK;
		else
			the_data.polling = ALS_POLL_TIME;
		break;
	case ALS_LED_VALUE:
		the_data.led_value = arg;
		break;
#endif

	default:
		ret = -1;
		D("%s invalid cmd \n", __func__);
		break;
	}

	D("leave %s  with ret %d \n", __func__, ret);
	if (ret) {
		DERR("%s error : ret =%d, cmd = %d\n", __func__, ret, cmd);
	}
	D("leave als_sem mutex %d \n", --enter);
	up(&the_data.als_sem);
	return ret;
}
EXPORT_SYMBOL(als_cmd);

static ssize_t als_control_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	int cmd = 0;
	int arg = 0;
	sscanf(buf, "%d %d", &cmd, &arg);
	if(als_cmd(cmd, arg))
		return 0;
	return size;
}

static ssize_t als_control_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	return sprintf(buf,
			" lux		= %4d, \t test_lux	= %4d\n"
			" poll time(ms)	= %4d,\n"
			" resmode	= %4d, \t resol		= %4d\n"
			" scail_m	= %4d, \t scail_n	= %4d\n"
			" blink_on	= %4d, \t led_value 	= %4d\n"
			" lux_min	= %4d, \t lux_blink	= %4d\n"
			" blink_c	= %4d, \t\n"
			" PS		= %4d, \t LG		= %4d\n"
			" CA		= %4d, \t IR		= %4d\n",
			the_data.lux, 		the_data.test_lux,
			the_data.polling,
			the_data.resmode, 	the_data.resol,
			the_data.scail_m, 	the_data.scail_n,
			the_data.blink_on,	the_data.led_value,
			the_data.lux_min,	the_data.lux_blink,
			the_data.blink_c,
			GET_STAT(PS),		GET_STAT(LG),
			GET_STAT(CA),		GET_STAT(IR));
}
static ssize_t als_data_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
    return size;
}

static ssize_t als_data_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	//als_cmd(ALS_TEST_READ, 0);
	return sprintf(buf, "%d\n", the_data.lux);
}
static ssize_t als_debug_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	sscanf(buf, "%d", &dbg);
	return size;
}

static ssize_t als_debug_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	return sprintf(buf,
			"debug on off status	: %d\n"
			"als driver version	: %d.%d\n",
			dbg, VER_ALS_MAJ, VER_ALS_MIN);
}

#if defined (LED_LIGHT_REMOVAL)
static ssize_t als_ledstate_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
//	sscanf(buf, "%d", &led_on);
	return size;
}

static ssize_t als_ledstate_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d", the_data.blink_on);
}
#endif

static DEVICE_ATTR(control, S_IRUGO | S_IWUGO, als_control_show,
			als_control_store);
static DEVICE_ATTR(data, S_IRUGO | S_IWUGO, als_data_show, als_data_store);
static DEVICE_ATTR(debug, S_IRUGO | S_IWUGO, als_debug_show, als_debug_store);
#if defined (LED_LIGHT_REMOVAL)
static DEVICE_ATTR(ledstate, S_IRUGO | S_IWUGO, als_ledstate_show,
							als_ledstate_store);
#endif

static int bh1721_dev_fops_open(struct inode *inode, struct file *file)
{
	D("%s\n", __func__);
	if (misc_opened)
		return -EBUSY;
	misc_opened = 1;
	return 0;
}

static int bh1721_dev_fops_release(struct inode *inode, struct file *file)
{
	D("%s\n", __func__);
	misc_opened = 0;
	return 0;
}

static long bh1721_dev_fops_ioctl(struct file *file, unsigned int cmd,
							unsigned long arg)
{
	int on_off;
	long status = 0;
	D("%s cmd %d\n", __func__, _IOC_NR(cmd));
	switch (cmd) {
	case LIGHTSENSOR_IOCTL_ENABLE:
		if (get_user(on_off, (int __user *)arg))
			return -EFAULT;
		D("%s : enable= %d\n", __func__,on_off);
		if (on_off)
			status = als_cmd(ALS_ON,0);
		else
			status = als_cmd(ALS_OFF,0);
		return (status == 0) ? 0 : -EINVAL;
	case LIGHTSENSOR_IOCTL_GET_ENABLED:
		return put_user(the_data.enabled, (int __user *)arg);
	default:
		DERR("%s: invalid cmd %d\n", __func__, _IOC_NR(cmd));
		return -EINVAL;
	}
}

static struct file_operations bh1721_dev_fops = {
	.owner = THIS_MODULE,
	.open = bh1721_dev_fops_open,
	.release = bh1721_dev_fops_release,
	.unlocked_ioctl = bh1721_dev_fops_ioctl
};

struct miscdevice bh1721_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "bh1721",
	.fops = &bh1721_dev_fops
};
#ifdef CONFIG_HAS_EARLYSUSPEND
static void bh1721_drv_early_suspend(struct early_suspend *es)
{
	D("enter %s \n", __func__);
	/*	als_cmd(ALS_OFF, 0); */
}

static void bh1721_drv_late_resume(struct early_suspend *es)
{
	D("enter %s \n", __func__);
	/*	als_cmd(ALS_ON, 0); */
}
#else //CONFIG_HAS_EARLYSUSPEND

int bh1721_drv_suspend(struct i2c_client *i2c_clnt, pm_message_t mesg)
{
	D("enter %s \n", __func__);
	return 0;
}

int bh1721_drv_resume(struct i2c_client *i2c_clnt)
{
	D("enter %s \n", __func__);
	return 0;
}
#endif //CONFIG_HAS_EARLYSUSPEND

static int bh1721_drv_probe(struct i2c_client *i2c_clnt,
					const struct i2c_device_id *i2c_id)
{
	int rc = 0;
	struct input_dev *inpdev;

	D(" enter %s  \n", __func__);

	/* setup i2c device data */
	i2c_set_clientdata(i2c_clnt, &the_data);
	the_data.client = i2c_clnt;

	/* setup gpio */
	gpio_request(DVI_GPIO_PIN, "ambient_rst");
	tegra_gpio_enable(DVI_GPIO_PIN);
	gpio_direction_output(DVI_GPIO_PIN, 0);

	/* setup input device */
	inpdev = input_allocate_device();
	if(!inpdev) {
		DERR(" %s input alloc failed!\n", __func__);
		rc = -ENOMEM;
		goto err1;
	}
	inpdev->name = "ambient_light";
	set_bit(EV_ABS, inpdev->evbit);
	set_bit(EV_SYN, inpdev->evbit);

	snprintf(the_data.phys_name, sizeof(the_data.phys_name),
					"%s/input0", dev_name(&i2c_clnt->dev));
	inpdev->phys = the_data.phys_name;
	inpdev->id.bustype = BUS_I2C;
	inpdev->dev.parent = &i2c_clnt->dev;

	input_set_abs_params(inpdev, ABS_PRESSURE, MIN_ALS_VAL,
				MAX_ALS_VAL, 0, 0);
	input_set_drvdata(inpdev, &the_data);
	the_data.input_dev = inpdev;
	the_data.polling = ALS_POLL_TIME;  /* ms */

	rc = input_register_device(inpdev);
	if (rc < 0) {
		DERR(" %s input register failed!\n", __func__);
		goto err2;
	}
	rc = misc_register(&bh1721_dev);
	if (rc) {
		DERR("%s: could not register misc device\n", __func__);
		goto err2;
	}
	rc = device_create_file(&i2c_clnt->dev, &dev_attr_control);
	rc = device_create_file(&i2c_clnt->dev, &dev_attr_data);
	rc = device_create_file(&i2c_clnt->dev, &dev_attr_debug);
#if defined (LED_LIGHT_REMOVAL)
	rc = device_create_file(&i2c_clnt->dev, &dev_attr_ledstate);
#endif
	if (rc) {
		DERR("%s: could not create file\n", __func__);
		goto err3;
	}

	init_MUTEX(&the_data.als_sem);

	the_data.resmode = DFLT_RESMODE;
	the_data.resol = DFLT_RESOL;
	the_data.scail_m = DFLT_SCAIL_M;
	the_data.scail_n = DFLT_SCAIL_N;
	the_data.b_status = 0;
	the_data.enabled = 0;
	/* setup power-on timging */
	als_cmd(DVI,0);

#ifdef CONFIG_HAS_EARLYSUSPEND
	the_data.early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 15;
	the_data.early_suspend.suspend = bh1721_drv_early_suspend;
	the_data.early_suspend.resume = bh1721_drv_late_resume;
	register_early_suspend(&the_data.early_suspend);
#endif
	return rc;

/* handle errrors */
err3:
	input_unregister_device(inpdev);
err2:
	input_free_device(inpdev);
err1:
	return rc;
}


static struct i2c_device_id bh1721_idtable[] = {
	{"bh1721_als", 0,},
	{ /* end of list */ },
};

static struct i2c_driver bh1721_drv = {

	.driver = {
		.name	= "bh1721_als",
		.owner	= THIS_MODULE
	},
	.id_table	= bh1721_idtable,
	.probe		= bh1721_drv_probe,
/*  .remove		= __devexit_p(bh1721_drv_remove), */
#ifndef CONFIG_HAS_EARLYSUSPEND
	.suspend	= bh1721_drv_suspend,
	.resume		= bh1721_drv_resume,
#endif
};

static int __devinit bh1721_init(void)
{
	return i2c_add_driver(&bh1721_drv);
}

device_initcall(bh1721_init);
