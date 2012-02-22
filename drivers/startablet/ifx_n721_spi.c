/*
 * ifx_n721_spi.c -- Serial peheripheral interface framing layer for IFX modem.
 *
 * Copyright (C) 2009 Texas Instruments
 * Authors:	Umesh Bysani <bysani@ti.com> and
 *		Shreekanth D.H <sdh@ti.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/smp_lock.h>
#include <asm/uaccess.h>
#include <linux/irq.h>
#include <mach/gpio.h>
#include <mach/hardware.h>
#include <linux/tty.h>
#include <linux/tty_driver.h>
#include <linux/tty_flip.h>
#include <linux/interrupt.h>
#include <linux/completion.h>
#include <linux/spi/spi.h>
#include <linux/workqueue.h>

#include <mach/iomap.h>
#include <linux/io.h>

#include <linux/delay.h>
#include <linux/spi/ifx_n721_spi.h>

//20100927-1, syblue.lee@lge.com, Hold wake-lock for cp interrupt [START]
#define WAKE_LOCK_RESUME
#ifdef WAKE_LOCK_RESUME
#include <linux/wakelock.h>
#endif
//20100927, syblue.lee@lge.com, Hold wake-lock for cp interrupt [END]

#include <mach/gpio-names.h>

//20100701-1, syblue.lee@lge.com, delay time until CP can be ready again [START]
#include <linux/delay.h>
#define MRDY_DELAY_TIME	800	//20101127-1, syblue.lee@lge.com, Change delay time for transcation : 1000us -> 400us
//#define MRDY_DELAY_TIME	500	//20101127-1, syblue.lee@lge.com, Change delay time for transcation : 1000us -> 400us
//20100701-1, syblue.lee@lge.com, delay time until CP can be ready again [END]
#define SAFE_GUARD		100
#define SPI_CLOCK_24M (4*24*1000000)
#define SPI_CLOCK_12M (4*12*1000000)
#define SPI_CLOCK_6M  (4*6*1000000)

#define SPI_CLOCK SPI_CLOCK_24M

/* ################################################################################################################ */

#define USE_SPINLOCK 0
#if USE_SPINLOCK
#define LOCK_T          spinlock_t
#define CREATELOCK(_l)  spin_lock_init((_l))
#define DELETELOCK(_l)
#define LOCK(_l)        spin_lock((_l))
#define UNLOCK(_l)      spin_unlock((_l))
#define ATOMIC(_l,_f)   spin_lock_irqsave((_l),(_f))
#define UNATOMIC(_l,_f) spin_unlock_irqrestore((_l),(_f))
#else
#define LOCK_T          struct mutex
#define CREATELOCK(_l)  mutex_init((_l))
#define DELETELOCK(_l)
#define LOCK(_l)        mutex_lock((_l))
#define UNLOCK(_l)      mutex_unlock((_l))
#define ATOMIC(_l,_f)   local_irq_save((_f))
#define UNATOMIC(_l,_f) local_irq_restore((_f))
#endif

#define GPIO_CP_STATE         TEGRA_GPIO_PO6    // GPIO_RESERVED_1
#define GPIO_IFX_RESET_1V8_N  TEGRA_GPIO_PV0
#define GPIO_IFX_PWRON_1V8    TEGRA_GPIO_PV1

/* Structure used to store private data */
struct ifx_spi_data {
	dev_t			devt;
	spinlock_t		spi_lock;
	struct spi_device	*spi;
	struct list_head	device_entry;
    struct completion   ifx_read_write_completion;
    struct tty_struct   *ifx_tty;

	/* buffer is NULL unless this device is open (users > 0) */
	struct mutex		buf_lock;
	unsigned			users;
    unsigned int		throttle;
    struct work_struct       ifx_work;
    struct work_queue_struct *ifx_wq;

	//20100927-1, syblue.lee@lge.com, Hold wake-lock for cp interrupt [START]
#ifdef WAKE_LOCK_RESUME
	struct wake_lock wake_lock;
	unsigned int		wake_lock_flag;
#endif
//20100927, syblue.lee@lge.com, Hold wake-lock for cp interrupt [END]
	volatile int is_suspend ;
	volatile int packet_count ;
};

union ifx_spi_frame_header{
	struct{
		unsigned int curr_data_size:12;
		unsigned int more:1;
		unsigned int res1:1;
		unsigned int res2:2;
		unsigned int next_data_size:12;
		unsigned int ri:1;
		unsigned int dcd:1;
		unsigned int cts_rts:1;
		unsigned int dsr_dtr:1;
	}ifx_spi_header;
	unsigned char framesbytes[IFX_SPI_HEADER_SIZE];
};

struct ifx_spi_data	*gspi_data;
struct tty_driver 	*ifx_spi_tty_driver;

/* ################################################################################################################ */
/* Global Declarations */
unsigned long		minors[IFX_N_SPI_MINORS / BITS_PER_LONG];
unsigned int		ifx_master_initiated_transfer = 0;
unsigned int		ifx_spi_count;
unsigned int		ifx_sender_buf_size;
unsigned int		ifx_receiver_buf_size;
unsigned int		ifx_current_frame_size;
unsigned int		ifx_valid_frame_size;
unsigned int		ifx_ret_count;
const unsigned char 	*ifx_spi_buf;
unsigned char		*ifx_tx_buffer;
unsigned char           *ifx_rx_buffer;

/* Function Declarations */
static void ifx_spi_set_header_info(unsigned char *header_buffer, unsigned int curr_buf_size, unsigned int next_buf_size);
static int ifx_spi_get_header_info(unsigned int *valid_buf_size);
static void ifx_spi_set_mrdy_signal(int value);
static void ifx_spi_setup_transmission(void);
static void ifx_spi_send_and_receive_data(struct ifx_spi_data *spi_data);
static int ifx_spi_get_next_frame_size(int count);
static int ifx_spi_allocate_frame_memory(unsigned int memory_size);
static void ifx_spi_buffer_initialization(void);
static unsigned int ifx_spi_sync_read_write(struct ifx_spi_data *spi_data, unsigned int len);
static irqreturn_t ifx_spi_handle_srdy_irq(int irq, void *handle);
static void ifx_spi_handle_work(struct work_struct *work);
int ifx_spi_cp_resume(void);

// hgahn
unsigned char rx_dummy1[]={0xff,0xff,0xff,0xff};
unsigned char rx_dummy2[]={0xee,0xee,0xee,0xee};

/* ################################################################################################################ */

static LIST_HEAD(device_list);
static DEFINE_MUTEX(device_list_lock);

// static DECLARE_MUTEX(spi_sync_mutex);
LOCK_T spi_sync_mutex ;

static void __iomem *tmrus_base = IO_ADDRESS(TEGRA_TMRUS_BASE);
#define TIMERUS_CNTR_1US 0x0
#define TIMERUS_USEC_CFG 0x4
#define TIMERUS_CNTR_FREEZE 0x3C

int my_udelay(long delay)
{
	volatile unsigned long start ;
	volatile unsigned long end ;
	volatile unsigned long diff ;
	volatile unsigned long count ;
	volatile unsigned long last ;
	volatile unsigned long freez ;

	count = 0 ; // if timer register does not increase
	start = readl(tmrus_base + TIMERUS_CNTR_1US);
	last = start ;

	while(1){
		end = readl(tmrus_base + TIMERUS_CNTR_1US);
		if(end >= start) diff = end - start ;
		else diff = 0xFFFFFFFFU - start + end ;

		if(diff >= delay)
			break;

		if(end == last){
			count++ ;
			if(count > 10000)
				break ;
		}
		else {
			count=0 ;
			last = end ;
		}
	}

	return diff ;
}

int my_check_diff_time(struct timeval tv_start, struct timeval tv_end)
{
	volatile unsigned long time_diff =0;

	if((unsigned long)tv_end.tv_sec == (unsigned long)(tv_start.tv_sec + 1)){
		time_diff = 1000000 + (tv_end.tv_usec - tv_start.tv_usec) ;
	}
	else if (tv_end.tv_sec == tv_start.tv_sec){
		time_diff = (tv_end.tv_usec - tv_start.tv_usec) ;
	}

	return time_diff ;
}

int my_check_delay(long delay)
{
	struct timeval tv_start ;
	struct timeval tv_end ;
	volatile unsigned long time_diff ;
	int result ;
	int max_try = 3;
	do_gettimeofday(&tv_start) ;
	while(1){
		time_diff = my_udelay(delay) ;
		do_gettimeofday(&tv_end) ;
		time_diff = my_check_diff_time(tv_start, tv_end);
		if(time_diff >= delay || max_try-- <= 0)
			break ;
	}
	if( (time_diff > (delay + delay)) || (time_diff < (delay - delay/2))){
		printk("delay routine err req %d actual %d \n", delay, time_diff) ;
		return -1 ;
	}

	return 0 ;
}

int my_check_delay2(long delay)
{
	struct timeval tv_start ;
	struct timeval tv_end ;
	volatile unsigned long time_diff ;
	int result ;
	int max_try = 3;

	do_gettimeofday(&tv_start) ;
	while(1){
		udelay(delay);
		do_gettimeofday(&tv_end) ;
		time_diff = my_check_diff_time(tv_start, tv_end);
		if(time_diff >= delay || max_try-- <= 0)
			break ;
	}
	if( (time_diff > (delay + delay/2)) || (time_diff < (delay - delay/2))){
		printk("delay routine err req %d actual %d \n", delay, time_diff) ;
		return -1 ;
	}

	return 0 ;
}

void my_check_cfg(void)
{
	volatile unsigned long cfg ;
	volatile unsigned long freez ;
	cfg = readl(tmrus_base + TIMERUS_USEC_CFG);
	if( (cfg & 0x0000ffff) != 0x000b) // 12MHz
		printk(" %s cfg reg 0x%08x\n", __FUNCTION__, cfg) ;

	freez = readl(tmrus_base + TIMERUS_CNTR_FREEZE);
	printk(" %s freez reg 0x%08x\n", __FUNCTION__, freez) ;
}


/* ################################################################################################################ */

/* IFX SPI Operations */

/*
 * Function opens a tty device when called from user space
 */
static int
ifx_spi_open(struct tty_struct *tty, struct file *filp)
{
	int status = 0;

	struct ifx_spi_data *spi_data = gspi_data;
	spi_data->ifx_tty = tty;
	tty->driver_data = spi_data;

// hgahn
//	spi_data->ifx_spi_lock =0;

	ifx_spi_buffer_initialization();
	spi_data->throttle = 0;
	printk("**** %s status %d \n",__FUNCTION__, status);

	return status;
}

/*
 * Function closes a opened a tty device when called from user space
 */
static void
ifx_spi_close(struct tty_struct *tty, struct file *filp)
{
	//struct ifx_spi_data *spi_data = (struct ifx_spi_data *)tty->driver_data;
	//spi_data->ifx_tty = NULL;
	//tty->driver_data = NULL;
}

/*
 * Function is called from user space to send data to MODEM, it setups the transmission, enable MRDY signal and
 * waits for SRDY signal HIGH from MDOEM. Then starts transmission and reception of data to and from MODEM.
 * Once data read from MODEM is transferred to TTY core flip buffers, then "ifx_read_write_completion" is set
 * and this function returns number of bytes sent to MODEM
 */
static int
ifx_spi_write(struct tty_struct *tty, const unsigned char *buf, int count)
{
	struct ifx_spi_data *spi_data = (struct ifx_spi_data *)tty->driver_data;
	int i=0;

	if( !buf ){
		printk("File: ifx_n721_spi.c\tFunction: int ifx_spi_write()\t Buffer NULL\n");
		return 0;
	}
	if(!count){
		printk("File: ifx_n721_spi.c\tFunction: int ifx_spi_write()\t Count is ZERO\n");
		return 0;
	}
	//printk("%s \n", __FUNCTION__) ;
	LOCK(&spi_sync_mutex);

	ifx_ret_count = 0;
	spi_data->ifx_tty = tty;
	spi_data->ifx_tty->low_latency = 1;

	ifx_spi_buf = buf;
	ifx_spi_count = count;

	ifx_master_initiated_transfer = 1;
	if(!gpio_get_value(TEGRA_GPIO_PO0))
		ifx_spi_set_mrdy_signal(1);

	UNLOCK(&spi_sync_mutex);

	// wait_for_completion(&spi_data->ifx_read_write_completion);
	wait_for_completion_timeout(&spi_data->ifx_read_write_completion, 2*HZ);
	if(ifx_ret_count==0){
		ifx_spi_set_mrdy_signal(0);
		printk("%s - timeout!! \n", __FUNCTION__);
	}

	init_completion(&spi_data->ifx_read_write_completion);

	return ifx_ret_count; /* Number of bytes sent to the device */
}

/* This function should return number of free bytes left in the write buffer in this case always return 2048 */

static int
ifx_spi_write_room(struct tty_struct *tty)
{
	return (IFX_SPI_DEFAULT_BUF_SIZE + IFX_SPI_HEADER_SIZE);
}


/* ################################################################################################################ */
/* These two functions are to be used in future to implement flow control (RTS & CTS)*/
/*static void
ifx_spi_throttle(struct tty_struct *tty)
{
	unsigned int flags;
	struct ifx_spi_data *spi_data = (struct ifx_spi_data *)tty->driver_data;
	spi_data->ifx_tty = tty;
	spin_lock_irqsave(&spi_data->spi_lock, flags);
	spi_data->throttle = 1;
	spin_unlock_irqrestore(&spi_data->spi_lock, flags);
}

static void
ifx_spi_unthrottle(struct tty_struct *tty)
{
	unsigned int flags;
	struct ifx_spi_data *spi_data = (struct ifx_spi_data *)tty->driver_data;
	spi_data->ifx_tty = tty;
	spin_lock_irqsave(&spi_data->spi_lock, flags);
	spi_data->throttle = 0;
	if( ifx_rx_buffer != NULL ){
	     tty_insert_flip_string(spi_data->ifx_tty, ifx_rx_buffer, valid_buffer_count);
	}
	spin_unlock_irqrestore(&spi_data->spi_lock, flags);
}*/
/* ################################################################################################################ */

/* End of IFX SPI Operations */

/* ################################################################################################################ */

/* TTY - SPI driver Operations */

static int
ifx_spi_probe(struct spi_device *spi)
{
	int status;
	struct ifx_spi_data *spi_data;
	/* Allocate SPI driver data */
	spi_data = (struct ifx_spi_data*)kmalloc(sizeof(struct ifx_spi_data), GFP_KERNEL);
	memset(spi_data, 0, sizeof(struct ifx_spi_data)) ;

	if (!spi_data){
		return -ENOMEM;
    }

    status = ifx_spi_allocate_frame_memory(IFX_SPI_MAX_BUF_SIZE + IFX_SPI_HEADER_SIZE);
    if(status != 0){
	printk("File: ifx_n721_spi.c\tFunction: int ifx_spi_probe\tFailed to allocate memory for buffers\n");
	return -ENOMEM;
    }

    dev_set_drvdata(&spi->dev,spi_data);
    spin_lock_init(&spi_data->spi_lock);

    spi_data->ifx_wq = create_singlethread_workqueue("ifxn721");
    if(!spi_data->ifx_wq){
		printk("Failed to setup workqueue - ifx_wq \n");
    }
	INIT_WORK(&spi_data->ifx_work,ifx_spi_handle_work);

	init_completion(&spi_data->ifx_read_write_completion);

        /* Configure SPI */

	spi_data->spi = spi;
#if 1
	// spi->mode = SPI_MODE_1 | SPI_CS_HIGH | SPI_NO_CS;
	spi->mode = SPI_MODE_1 | SPI_CS_HIGH | SPI_NO_CS;
	// spi->mode = SPI_MODE_0 | SPI_CS_HIGH ;
    // spi->bits_per_word = 8;
    spi->bits_per_word = 8;
	spi->max_speed_hz = SPI_CLOCK;
	spi->chip_select = 0 ;
	//spi->irq = gpio_to_irq(TEGRA_GPIO_PO5);//0;
	memcpy(spi->modalias, "ifxn721", sizeof("ifxn721")) ;
//
#endif
	status = spi_setup(spi);
    if(status < 0){
		printk("Failed to setup SPI \n");
    }

// hgahn
//	spi_data->ifx_spi_lock =1;

	/* Enable SRDY Interrupt request - If the SRDY signal is high then ifx_spi_handle_srdy_irq() is called */
	//status = request_irq(spi->irq, ifx_spi_handle_srdy_irq,  IRQF_TRIGGER_RISING, spi->dev.driver->name, spi_data);

	gpio_request(TEGRA_GPIO_PO0, "ifx_mrdy");
 	tegra_gpio_enable(TEGRA_GPIO_PO0);

	gpio_request(TEGRA_GPIO_PO5, "ifx_srdy");
 	tegra_gpio_enable(TEGRA_GPIO_PO5);

	// gpio_set_value(TEGRA_GPIO_PO0, 0);
	gpio_direction_output(TEGRA_GPIO_PO0, 0);

	gpio_direction_input(TEGRA_GPIO_PO5);
	spi->irq = gpio_to_irq(TEGRA_GPIO_PO5) ;

    // enable gpios for modem shutdown
    //tegra_gpio_enable(GPIO_IFX_RESET_1V8_N);
    //tegra_gpio_enable(GPIO_IFX_PWRON_1V8);

    //gpio_request_one(GPIO_IFX_RESET_1V8_N, GPIOF_IN, "modem_reset_n");
    //gpio_request_one(GPIO_IFX_PWRON_1V8, GPIOF_OUT_INIT_HIGH, "modem_pwron");

	spi_data->is_suspend = 0 ;
	spi_data->packet_count = 0 ;
	status = request_irq(spi->irq, ifx_spi_handle_srdy_irq,IRQF_TRIGGER_RISING ,spi->dev.driver->name, spi_data);
	if(is_modem_connected()){
		set_irq_wake(spi->irq, 1);
	}
	else {
		disable_irq(spi->irq);
	}

//20100927-1, syblue.lee@lge.com, Hold wake-lock for cp interrupt [START]
#ifdef WAKE_LOCK_RESUME
	wake_lock_init(&spi_data->wake_lock, WAKE_LOCK_SUSPEND, "mspi_wake");
	spi_data->wake_lock_flag = 0;
#endif
//20100927-1, syblue.lee@lge.com, Hold wake-lock for cp interrupt [END]

	if (status != 0){
		printk(KERN_ERR "Failed to request IRQ for SRDY\n");
		printk(KERN_ERR "IFX SPI Probe Failed\n");
		if(ifx_tx_buffer){
			kfree(ifx_tx_buffer);
		}
		if(ifx_rx_buffer){
			kfree(ifx_rx_buffer);
		}
		if(spi_data){
			kfree(spi_data);
		}
	}
	else{
		gspi_data = spi_data;
	}
/////////////////////////////////////////////////////////////////////////////////////////////
// jeehp
//        enable_irq_wake(spi->irq);
////////////////////////////////////////////////////////////////////////////////////////////
	printk("**** %s \n",__FUNCTION__);
	return status;
}

static int
ifx_spi_remove(struct spi_device *spi)
{
	struct ifx_spi_data *spi_data;
	spi_data = spi_get_drvdata(spi);
	spin_lock_irq(&spi_data->spi_lock);
	spi_data->spi = NULL;
	spi_set_drvdata(spi, NULL);
	spin_unlock_irq(&spi_data->spi_lock);

        if(ifx_tx_buffer){
		kfree(ifx_tx_buffer);
	}
        if(ifx_rx_buffer){
		kfree(ifx_rx_buffer);
	}
        if(spi_data){
		kfree(spi_data);
        }
        return 0;
}

/*
 * Power down modem(CP)
 * return 0 : success to power down modem
 */
static int modem_powerdown(void)
{
    int level;

    // set power on high for a while to simulate power key pressed at modem
    gpio_set_value(GPIO_IFX_PWRON_1V8, 0);
    mdelay(500);
    gpio_set_value(GPIO_IFX_PWRON_1V8, 1);

    // check CP state if modem(CP) is turned off
    level = gpio_get_value(GPIO_CP_STATE);

    printk(KERN_INFO "%s: CP STATE pin is %d\n", __func__, level);

    return (0 == level? 0: -1);
}

/* reset modem */
void modem_reset(void)
{
    printk("%s is called. \n", __func__);
    gpio_set_value(GPIO_IFX_RESET_1V8_N, 1);
    mdelay(30);
    gpio_set_value(GPIO_IFX_RESET_1V8_N, 0);
    mdelay(30);
    gpio_set_value(GPIO_IFX_RESET_1V8_N, 1);
}
EXPORT_SYMBOL_GPL(modem_reset);

static int ifx_spi_shutdown(struct spi_device *spi)
{
    //printk("%s()\n", __func__);

	// if modem is not connected, return
	if (!is_modem_connected())
		return 0;

    // power down modem by gpio
    if (modem_powerdown()) // if fail to power down, retry
    {
        modem_reset();
        mdelay(6000); // wait until CP reboot
        modem_powerdown();
    }
    return 0;
}

//20100908 cs77.ha deepsleep wakeup issue [START]
#define PMC_WAKE_STATUS 0x14
#define WAKEUP_IFX_SRDY_MASK    (1 << 0)     // Wake Event 0 - IFX_SRDY

static void __iomem *pmc_base = IO_ADDRESS(TEGRA_PMC_BASE);

static int ifx_spi_suspend(struct platform_device *dev, pm_message_t state)
{
    unsigned long reg;

    reg = readl(pmc_base + PMC_WAKE_STATUS);

    // Clear power key wakeup pad bit.
    if (reg & WAKEUP_IFX_SRDY_MASK)
    {
        //printk("[IFX_SRDY] %s() wakeup pad : 0x%lx\n", __func__, reg);
        writel(WAKEUP_IFX_SRDY_MASK, pmc_base + PMC_WAKE_STATUS);
    }

	if(gspi_data)
		gspi_data->is_suspend = 1;
    return 0;
}

static int ifx_spi_resume(struct platform_device *dev)
{
    unsigned long pinValue;
    unsigned long reg;

	if(gspi_data)
		gspi_data->is_suspend = 0;

    reg = readl(pmc_base + PMC_WAKE_STATUS);

    if (reg & WAKEUP_IFX_SRDY_MASK) {
	//20100927-1, syblue.lee@lge.com, Hold wake-lock for cp interrupt [START]
	#ifdef	WAKE_LOCK_RESUME
		//printk("[IFX_SRDY] %s() wake lock : 0x%lx\n", __func__, &gspi_data->wake_lock);
		if(&gspi_data->wake_lock)
			wake_lock_timeout(&gspi_data->wake_lock, 50);	//20101203-1, syblue.lee@lge.com, change 3 to 1 for power consumption
	#endif
	//20100927-1, syblue.lee@lge.com, Hold wake-lock for cp interrupt [END]
	    printk("%s() wakeup pad : 0x%lx\n", __func__, reg);

		gspi_data->wake_lock_flag = 1;
        // queue_work(gspi_data->ifx_wq, &gspi_data->ifx_work);
    }

    return 0;
}

int ifx_spi_cp_resume(void)
{
	volatile unsigned long reg;

	reg = readl(pmc_base + PMC_WAKE_STATUS);
	if (reg != 0)
		return 1 ;

	return 0 ;
}

//20100908 cs77.ha deepsleep wakeup issue [END]

/* End of TTY - SPI driver Operations */
/* ################################################################################################################ */

static const struct spi_device_id star_spi_ids[] = {
	{ "ifxn721", 0 },
	{ /* end of list */ },
};

static struct spi_driver ifx_spi_driver = {
	.probe = ifx_spi_probe,
	.remove = __devexit_p(ifx_spi_remove),
	//.shutdown = ifx_spi_shutdown,

	 //.id_table  = star_spi_ids,
	.driver = {
		.name = "ifxn721",
        .bus = &spi_bus_type,
		.owner = THIS_MODULE,
	},

	//20100908 cs77.ha deepsleep wakeup issue
	.suspend = ifx_spi_suspend,
	.resume = ifx_spi_resume,
};

/*
 * Structure to specify tty core about tty driver operations supported in TTY SPI driver.
 */
static const struct tty_operations ifx_spi_ops = {
    .open = ifx_spi_open,
    .close = ifx_spi_close,
    .write = ifx_spi_write,
    .write_room = ifx_spi_write_room,
    //.throttle = ifx_spi_throttle,
    //.unthrottle = ifx_spi_unthrottle,
    //.set_termios = ifx_spi_set_termios,
};

/* ################################################################################################################ */

/*
 * Intialize frame sizes as "IFX_SPI_DEFAULT_BUF_SIZE"(128) bytes for first SPI frame transfer
 */
static void
ifx_spi_buffer_initialization(void)
{
	ifx_sender_buf_size = IFX_SPI_DEFAULT_BUF_SIZE;
        ifx_receiver_buf_size = IFX_SPI_DEFAULT_BUF_SIZE;
}

/*
 * Allocate memeory for TX_BUFFER and RX_BUFFER
 */
static int
ifx_spi_allocate_frame_memory(unsigned int memory_size)
{
	int status = 0;
	ifx_rx_buffer = kmalloc(memory_size+IFX_SPI_HEADER_SIZE, GFP_KERNEL);
	if (!ifx_rx_buffer){
		printk("Open Failed ENOMEM\n");
		status = -ENOMEM;
	}
	ifx_tx_buffer = kmalloc(memory_size+IFX_SPI_HEADER_SIZE, GFP_KERNEL);
	if (!ifx_tx_buffer){
		printk("Open Failed ENOMEM\n");
		status = -ENOMEM;
	}
	if(status == -ENOMEM){
		if(ifx_tx_buffer){
			kfree(ifx_tx_buffer);
		}
		if(ifx_rx_buffer){
			kfree(ifx_rx_buffer);
		}
	}
	return status;
}

/*
 * Function to set header information according to IFX SPI framing protocol specification
 */
static void
ifx_spi_set_header_info(unsigned char *header_buffer, unsigned int curr_buf_size, unsigned int next_buf_size)
{
	int i;
	union ifx_spi_frame_header header;
	for(i=0; i<4; i++){
		header.framesbytes[i] = 0;
	}

	header.ifx_spi_header.curr_data_size = curr_buf_size;
	if(next_buf_size){
		header.ifx_spi_header.more=1;
		header.ifx_spi_header.next_data_size = next_buf_size;
	}
	else{
		header.ifx_spi_header.more=0;
		header.ifx_spi_header.next_data_size = 128;
	}

	for(i=3; i>=0; i--){
	header_buffer[i] = header.framesbytes[/*3-*/i];
	}
}

/*
 * Function to get header information according to IFX SPI framing protocol specification
 */
static int
ifx_spi_get_header_info(unsigned int *valid_buf_size)
{
	int i;
	union ifx_spi_frame_header header;

	for(i=0; i<4; i++){
		header.framesbytes[i] = 0;
	}

	for(i=3; i>=0; i--){
		header.framesbytes[i] = ifx_rx_buffer[/*3-*/i];
	}

	*valid_buf_size = header.ifx_spi_header.curr_data_size;
	//printk("*** header sz:%d mr:%d nsz:%d ri:%d dcd:%d cts:%d dsr:%d \n", header.ifx_spi_header.curr_data_size, header.ifx_spi_header.more, \
	//	header.ifx_spi_header.next_data_size, header.ifx_spi_header.ri, header.ifx_spi_header.dcd,header.ifx_spi_header.cts_rts,header.ifx_spi_header.dsr_dtr) ;

	if(header.ifx_spi_header.more){
		return header.ifx_spi_header.next_data_size;
	}
	return 0;
}

/*
 * Function to set/reset MRDY signal
 */
static void
ifx_spi_set_mrdy_signal(int value)
{
	gpio_set_value(TEGRA_GPIO_PO0, value);
	// gpio_direction_output(TEGRA_GPIO_PO0, value);
}

/*
 * Function to calculate next_frame_size required for filling in SPI frame Header
 */
static int
ifx_spi_get_next_frame_size(int count)
{
	if(count > IFX_SPI_MAX_BUF_SIZE){
		return IFX_SPI_MAX_BUF_SIZE;
	}
	else{
		return count;
	}
}

/*
 * Function to setup transmission and reception. It implements a logic to find out the ifx_current_frame_size,
 * valid_frame_size and sender_next_frame_size to set in SPI header frame. Copys the data to be transferred from
 * user space to TX buffer and set MRDY signal to HIGH to indicate Master is ready to transfer data.
 */

unsigned char dummy_tx[128] = {
	0x3, 0x0, 0x80, 0x0, 0x41, 0x54, 0xd, 0x0,
};


static void
ifx_spi_setup_transmission(void)
{
	if( (ifx_sender_buf_size != 0) || (ifx_receiver_buf_size != 0) ){
		if(ifx_sender_buf_size > ifx_receiver_buf_size){
			ifx_current_frame_size = ifx_sender_buf_size;
		}
		else{
			ifx_current_frame_size = ifx_receiver_buf_size;
		}
		if(ifx_spi_count > 0){
			if(ifx_spi_count > ifx_current_frame_size){
				ifx_valid_frame_size = ifx_current_frame_size;
				ifx_spi_count = ifx_spi_count - ifx_current_frame_size;
			}
			else{
				ifx_valid_frame_size = ifx_spi_count;
				ifx_spi_count = 0;
			}
                }
		else{
			ifx_valid_frame_size = 0;
			ifx_sender_buf_size = 0;
		}
		ifx_sender_buf_size = ifx_spi_get_next_frame_size(ifx_spi_count);

		/* memset buffers to 0 */
		memset(ifx_tx_buffer,0,IFX_SPI_MAX_BUF_SIZE+IFX_SPI_HEADER_SIZE);
		memset(ifx_rx_buffer,0,IFX_SPI_MAX_BUF_SIZE+IFX_SPI_HEADER_SIZE);

		/* Set header information */
		ifx_spi_set_header_info(ifx_tx_buffer, ifx_valid_frame_size, ifx_sender_buf_size);
		if( ifx_valid_frame_size > 0 ){
			memcpy(ifx_tx_buffer+IFX_SPI_HEADER_SIZE, ifx_spi_buf, ifx_valid_frame_size);
			ifx_spi_buf = ifx_spi_buf + ifx_valid_frame_size;
		}

		// memcpy(ifx_tx_buffer, dummy_tx, 8) ;
	}
}


/*
 * Function starts Read and write operation and transfers received data to TTY core. It pulls down MRDY signal
 * in case of single frame transfer then sets "ifx_read_write_completion" to indicate transfer complete.
 */
static void
ifx_spi_send_and_receive_data(struct ifx_spi_data *spi_data)
{
	unsigned int rx_valid_buf_size;
	int status = 0;
	int recieve_copied ;

	//printk("ifx_spi_sync_read_write %d \n ", ifx_current_frame_size+IFX_SPI_HEADER_SIZE) ;
	status = ifx_spi_sync_read_write(spi_data, ifx_current_frame_size+IFX_SPI_HEADER_SIZE); /* 4 bytes for header */
	if(status > 0){
		memset(ifx_tx_buffer,0,IFX_SPI_MAX_BUF_SIZE+IFX_SPI_HEADER_SIZE);
		ifx_ret_count = ifx_ret_count + ifx_valid_frame_size;
	}

	// hgahn
	if(memcmp(rx_dummy1, ifx_rx_buffer, IFX_SPI_HEADER_SIZE) ==0) {
		ifx_receiver_buf_size = 0;
		if (!ifx_master_initiated_transfer)
			printk(">> spi dummy \n") ;
		return;
	}else if(memcmp(rx_dummy2, ifx_rx_buffer, IFX_SPI_HEADER_SIZE) ==0) {
		ifx_receiver_buf_size = 0;
		// printk("received dummy2  err") ;
		return;
	}

	/* Handling Received data */
	ifx_receiver_buf_size = ifx_spi_get_header_info(&rx_valid_buf_size);


// hgahn
	if((spi_data->throttle == 0) && (rx_valid_buf_size != 0)){ //&& !(spi_data->ifx_spi_lock)){
		if(spi_data->ifx_tty){
			recieve_copied = tty_insert_flip_string(spi_data->ifx_tty, ifx_rx_buffer+IFX_SPI_HEADER_SIZE, rx_valid_buf_size);

			// if(recieve_copied)
				tty_flip_buffer_push(spi_data->ifx_tty);
			//else
			//	printk("tty_insert_flip_string err") ;
		}
		else {
			printk("no tty err \n") ;
		}
	}
	/*else
  	{
	handle RTS and CTS in SPI flow control
	Reject the packet as of now
	}*/
}

/*
 * Function copies the TX_BUFFER and RX_BUFFER pointer to a spi_transfer structure and add it to SPI tasks.
 * And calls SPI Driver function "spi_sync" to start data transmission and reception to from MODEM
 */
#if 1
static unsigned int
ifx_spi_sync_read_write(struct ifx_spi_data *spi_data, unsigned int len)
{

	int i=0;
	int j=0;
	int status;
	struct spi_message	m;
	struct spi_transfer	t = {
					.tx_buf		= ifx_tx_buffer,
                    .rx_buf		= ifx_rx_buffer,
					.len		= len,
					.speed_hz    = SPI_CLOCK,
				    };

	spi_message_init(&m);

	spi_message_add_tail(&t, &m);


	if (spi_data->spi == NULL)
		status = -ESHUTDOWN;
	else
		status = spi_sync(spi_data->spi, &m);

	if (status == 0){
		status = m.status;
		if (status == 0)
			status = m.actual_length;
	}
    else{
		printk("%s status %d m.status %d master %d \n", __FUNCTION__,status, m.status, ifx_master_initiated_transfer) ;
	}
#if 0
printk("spi tx:: ");
for(i=0;i<16;i++){
	printk( "%02x ",ifx_tx_buffer[i]);
}
printk("\n");

printk("spi rx:: ");
for(i=0;i<16;i++){
	printk( "%02x ",ifx_rx_buffer[i]);
}
printk("\n");
#endif
	return status;
}
#else
extern int spi_tegra_start_transfer(struct spi_device *spi, struct spi_transfer *t);
static unsigned int
ifx_spi_sync_read_write(struct ifx_spi_data *spi_data, unsigned int len)
{

	int i=0;
	int j=0;
	int status;
	struct spi_message	m;
	struct spi_transfer t = {
						.tx_buf 	= ifx_tx_buffer,
						.rx_buf 	= ifx_rx_buffer,
						.len		= len,
						.speed_hz	 = SPI_CLOCK,
						};

	status = spi_tegra_start_transfer(spi_data->spi, &t) ;


	printk("spi tx:: ");
	for(i=0;i<16;i++){
		printk( "%02x ",ifx_tx_buffer[i]);
	}
	printk("\n");

	printk("spi rx:: ");
	for(i=0;i<16;i++){
		printk( "%02x ",ifx_rx_buffer[i]);
	}
	printk("\n");

	if(!status)
		return len ;
	else
		return -1 ;
}
#endif

/*
 * Function is a Interrupt service routine, is called when SRDY signal goes HIGH. It set up transmission and
 * reception if it is a Slave initiated data transfer. For both the cases Master intiated/Slave intiated
 * transfer it starts data transfer.
 */
static irqreturn_t
ifx_spi_handle_srdy_irq(int irq, void *handle)
{
	struct ifx_spi_data *spi_data = (struct ifx_spi_data *)handle;
	int work_inqueue ;
	work_inqueue = queue_work(spi_data->ifx_wq, &spi_data->ifx_work);
	if(!work_inqueue)
		printk("---------------^-^------------- \n") ;
	return IRQ_HANDLED;
}

static void
ifx_spi_handle_work(struct work_struct *work)
{
	struct ifx_spi_data *spi_data = container_of(work, struct ifx_spi_data, ifx_work);
	int timer_result;
	volatile int check_resume ;
	volatile int check_suspend ;
	volatile int check_count ;

	if(spi_data->packet_count < SAFE_GUARD)
		spi_data->packet_count++;

	check_suspend = spi_data->is_suspend ;
	check_count = 0 ;
	while(1){
		if(!check_suspend)
			break ;

		msleep(1) ;
		check_suspend = spi_data->is_suspend ;
		check_count++ ;
		if(check_count > 1000)
			break ;
	}
	if(check_count != 0)
		printk("%s check count %d \n", __FUNCTION__, check_count) ;

	if (!ifx_master_initiated_transfer){
		// printk( "slave start\n");
		LOCK(&spi_sync_mutex);
		ifx_spi_setup_transmission();
		if(!gpio_get_value(TEGRA_GPIO_PO0))
			ifx_spi_set_mrdy_signal(1);
		ifx_spi_send_and_receive_data(spi_data);
		/* Once data transmission is completed, the MRDY signal is lowered */
		if((ifx_sender_buf_size == 0)  && (ifx_receiver_buf_size == 0)){
			ifx_spi_set_mrdy_signal(0);
			ifx_spi_buffer_initialization();
			if(spi_data->packet_count < SAFE_GUARD)
				my_check_delay(2*MRDY_DELAY_TIME) ;
			else
				my_check_delay(MRDY_DELAY_TIME) ;
			// udelay(2*MRDY_DELAY_TIME) ;
		}
		UNLOCK(&spi_sync_mutex);
		/* We are processing the slave initiated transfer in the mean time Mux has requested master initiated data transfer */
		/* Once Slave initiated transfer is complete then start Master initiated transfer */
		if(ifx_master_initiated_transfer == 1){
		/* It is a condition where Slave has initiated data transfer and both SRDY and MRDY are high and at the end of data transfer
	 	* MUX has some data to transfer. MUX initiates Master initiated transfer rising MRDY high, which will not be detected at Slave-MODEM.
	 	* So it was required to rise MRDY high again */
			if(!gpio_get_value(TEGRA_GPIO_PO0))
				ifx_spi_set_mrdy_signal(1);
		}
		//printk( "slave end\n");
	}
	else{
		//printk( "master start\n");
		ifx_spi_setup_transmission();
		ifx_spi_send_and_receive_data(spi_data);
		/* Once data transmission is completed, the MRDY signal is lowered */
		if(ifx_sender_buf_size == 0){
			if(ifx_receiver_buf_size == 0){
				ifx_spi_set_mrdy_signal(0);
				ifx_spi_buffer_initialization();
				if(spi_data->packet_count < SAFE_GUARD)
					my_check_delay(2*MRDY_DELAY_TIME) ;
				else
					my_check_delay(MRDY_DELAY_TIME) ;
				//udelay(2*MRDY_DELAY_TIME) ;
			}
			ifx_master_initiated_transfer = 0;
			complete(&spi_data->ifx_read_write_completion);
		}
		//printk( "master end\n");
	}
	//printk( "tegra_spi_workerthread / End\n");
}


/* ################################################################################################################ */


/* ################################################################################################################ */

/* Initialization Functions */

/*
 * Initialization function which allocates and set different parameters for TTY SPI driver. Registers the tty driver
 * with TTY core and register SPI driver with the Kernel. It validates the GPIO pins for MRDY and then request an IRQ
 * on SRDY GPIO pin for SRDY signal going HIGH. In case of failure of SPI driver register cases it unregister tty driver
 * from tty core.
 */
static int
__init ifx_spi_init(void)
{
int status = 0;

	/* Allocate and Register a TTY device */
	ifx_spi_tty_driver = alloc_tty_driver(2);//(IFX_N_SPI_MINORS);
	if (!ifx_spi_tty_driver){
		printk(KERN_ERR "Fail to allocate TTY Driver\n");
		return -ENOMEM;
	}
	my_check_cfg() ;
	/* initialize the tty driver */
	ifx_spi_tty_driver->owner = THIS_MODULE;
	ifx_spi_tty_driver->driver_name = "ifxn721";
	ifx_spi_tty_driver->name = "ttyspi";
	ifx_spi_tty_driver->major = IFX_SPI_MAJOR;
	ifx_spi_tty_driver->minor_start = 0;
	ifx_spi_tty_driver->type = TTY_DRIVER_TYPE_SERIAL;
	ifx_spi_tty_driver->subtype = SERIAL_TYPE_NORMAL;
	ifx_spi_tty_driver->flags = TTY_DRIVER_REAL_RAW;
	ifx_spi_tty_driver->init_termios = tty_std_termios;
	ifx_spi_tty_driver->init_termios.c_cflag = B9600 | CS8 | CREAD | HUPCL | CLOCAL;
	tty_set_operations(ifx_spi_tty_driver, &ifx_spi_ops);

	status = tty_register_driver(ifx_spi_tty_driver);
	if (status){
		printk(KERN_ERR "Failed to register IFX SPI tty driver");
		put_tty_driver(ifx_spi_tty_driver);
		return status;
	}

	/* Register SPI Driver */

	status = spi_register_driver(&ifx_spi_driver);
	if (status < 0){
		printk("Failed to register SPI device");
		tty_unregister_driver(ifx_spi_tty_driver);
		put_tty_driver(ifx_spi_tty_driver);
		return status;
	}

	CREATELOCK(&spi_sync_mutex) ;
	printk("**** %s \n",__FUNCTION__);
	return status;
}

module_init(ifx_spi_init);


/*
 * Exit function to unregister SPI driver and tty SPI driver
 */
static void
__exit ifx_spi_exit(void)
{
	spi_unregister_driver(&ifx_spi_driver);
	tty_unregister_driver(ifx_spi_tty_driver);
        put_tty_driver(ifx_spi_tty_driver);
}

module_exit(ifx_spi_exit);

/* End of Initialization Functions */

/* ################################################################################################################ */

MODULE_AUTHOR("Umesh Bysani and Shreekanth D.H, <bysani@ti.com> <sdh@ti.com>");
MODULE_DESCRIPTION("IFX SPI Framing Layer");
MODULE_LICENSE("GPL");
