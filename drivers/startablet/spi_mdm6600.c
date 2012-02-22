/*
 * spi_mdm6600.c -- Serial peheripheral interface framing layer for MDM6600 modem.
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
#include <linux/tty.h>
#include <linux/tty_driver.h>
#include <linux/tty_flip.h>
#include <linux/interrupt.h>
#include <linux/completion.h>
#include <linux/spi/spi.h>
#include <linux/workqueue.h>
#include <mach/gpio-names.h>
#include <mach/spi.h>
#include <mach/hardware.h>
#include <linux/hrtimer.h>
#include <linux/delay.h>
#include <linux/err.h>

#ifdef CONFIG_HAS_WAKELOCK
#include <linux/wakelock.h>

#define SPI_WAKELOCK_TIMEOUT 3
#endif

#define TX_BUFFER_QUEUE
#define MDM6600_SPI_HEADER
#define SPI_STATISTICS_CHECK //create dummy traffic using a polling method with a timer setting of 10 sec to release the AP from an AT command pending state

#ifndef MDM6600_SPI_HEADER
#include <linux/spi/ifx_n721_spi.h>
#else
#define IFX_SPI_FRAME_SIZE  (2048*2)
#define IFX_SPI_HEADER_SIZE 8
#define IFX_SPI_MAX_BUF_SIZE (IFX_SPI_FRAME_SIZE - IFX_SPI_HEADER_SIZE)
#define IFX_SPI_DEFAULT_BUF_SIZE (IFX_SPI_FRAME_SIZE - IFX_SPI_HEADER_SIZE)
#define IFX_SPI_MAJOR			153	/* assigned */
#define IFX_N_SPI_MINORS		4	/* ... up to 256 */
#endif

/* define for spi1, spi2 port gpio information */
static int SPI1_SRDY;
static int SPI1_MRDY;
static int SPI2_SRDY;
static int SPI2_MRDY;

#define GPIO_CP_STATE         TEGRA_GPIO_PO6    // GPIO_RESERVED_1
#define GPIO_IFX_RESET_1V8_N  TEGRA_GPIO_PV0
#define GPIO_IFX_PWRON_1V8    TEGRA_GPIO_PV1

//#define TTYSPI_DEBUG
#ifdef TTYSPI_DEBUG
#define TTYSPI_DEBUG_PRINT(format, args...) printk(KERN_INFO format , ## args)
#else
#define TTYSPI_DEBUG_PRINT(format, args...)
#endif

/* ################################################################################################################ */

struct spi_device        *global_spi; //for time out callback function

/* Structure used to store private data */
struct mdm_spi_data {
    dev_t                    devt;
    spinlock_t               spi_lock;
    struct spi_device        *spi;
    struct list_head         device_entry;
    struct completion        mdm_read_write_completion;
    struct tty_struct        *mdm_tty;
    unsigned int             index;

#ifdef TX_BUFFER_QUEUE
    bool is_waiting;
#endif

    /* buffer is NULL unless this device driver is open (users > 0) */
    struct mutex             buf_lock;
    unsigned                 users;
    unsigned int             throttle;
    struct work_struct       mdm_work;
    struct workqueue_struct  *mdm_wq;

    /* For dual spi */
    /* Tx/Rx Buffer */
    unsigned char            *mdm_tx_buffer;
    unsigned char            *mdm_rx_buffer;
    unsigned int             mdm_master_initiated_transfer;

    /* Buffer management */
    unsigned int             mdm_sender_buf_size;
    unsigned int             mdm_receiver_buf_size;
    unsigned int             mdm_current_frame_size;
    unsigned int             mdm_valid_frame_size;
    unsigned int             mdm_ret_count;
    unsigned int             mdm_spi_count;
    const unsigned char      *mdm_spi_buf;

    struct hrtimer  timer; /* for timeout callback function */

#ifdef CONFIG_HAS_WAKELOCK
	struct wake_lock  spi_wakelock;
#endif

    int is_suspended;

    atomic_t is_syncing;
};

union mdm_spi_frame_header{
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
	}mdm_spi_header;
#ifdef MDM6600_SPI_HEADER
    unsigned char framesbytes[4];
    unsigned int framecount;
#else
    unsigned char framesbytes[IFX_SPI_HEADER_SIZE];
#endif
};

struct mdm_spi_data *gspi_data[2];
struct tty_driver   *mdm_spi_tty_driver;
unsigned int        assign_num = 0;

struct allocation_table {
    unsigned short allocated;
	atomic_t in_use;
};

struct allocation_table spi_table[4];


/* ################################################################################################################ */
/* Global Declarations */

/* Function Declarations */
static void mdm_spi_set_header_info(unsigned char *header_buffer, unsigned int curr_buf_size, unsigned int next_buf_size);
static int mdm_spi_get_header_info(struct mdm_spi_data *spi_data, unsigned int *valid_buf_size);
static void mdm_spi_set_srdy_signal(s16 bus_num, int value);
static irqreturn_t mdm_spi_handle_mrdy_irq(int irq, void *handle);
#ifndef TX_BUFFER_QUEUE
static void mdm_spi_setup_transmission(struct mdm_spi_data *spi_data);
#endif
static void mdm_spi_send_and_receive_data(struct mdm_spi_data *spi_data, int tx_pending);
#ifndef TX_BUFFER_QUEUE
static int mdm_spi_get_next_frame_size(int count);
#endif
static int mdm_spi_allocate_frame_memory(struct mdm_spi_data *spi_data, unsigned int memory_size);
static void mdm_spi_buffer_initialization(struct mdm_spi_data *spi_data);
static unsigned int mdm_spi_sync_read_write(struct mdm_spi_data *spi_data, unsigned int len);
static void mdm_spi_handle_work(struct work_struct *work);
static int mdm_spi_callback(void *client_data);
#ifdef TX_BUFFER_QUEUE
int get_tx_pending_data(struct mdm_spi_data *spi_data, int *anymore);
#endif

int is_modem_communicating(void);

#ifdef MDM6600_SPI_HEADER
static unsigned int tx_count[2] = {0, 0};
static unsigned int rx_count[2] = {0, 0};
#endif

unsigned char rx_dummy[]={0xff,0xff,0xff,0xff};

#define USE_SRDY
#ifndef USE_SRDY
#define ENABLE_SRDY_IRQ(irq)	enable_irq(irq);
#define DISABLE_SRDY_IRQ(irq)	disable_irq(irq);
#else
#define ENABLE_SRDY_IRQ(irq)
#define DISABLE_SRDY_IRQ(irq)
#endif

/* ################################################################################################################ */

static LIST_HEAD(device_list);
static DEFINE_MUTEX(device_list_lock);

#ifdef TX_BUFFER_QUEUE
#define MAX_SPI_DATA_QUEUE 200

struct spi_data_send_struct;
struct spi_data_send_struct {
    struct spi_data_send_struct *curr;
    struct spi_data_send_struct *next;
    struct spi_data_send_struct *prev;
    u8  count;
    u8 *data;
    int size;
    u8 *bkp_data;
    int bkp_size;
};
static struct spi_data_send_struct *spi_data_send_pending = NULL;
static int queue_first_time = 1;

static spinlock_t spi_nodes_lock = SPIN_LOCK_UNLOCKED;
static unsigned long spi_lock_flag;
static atomic_t next_transfer_flag = ATOMIC_INIT(0);
//static int timeout_count = 0;
#endif

extern void disable_mdm_irq(void);

/* ################################################################################################################ */

/* MDM SPI Operations */

static enum hrtimer_restart spi_timeout_cb_func(struct hrtimer *timer)
{
    struct spi_device *temp_spi = (struct spi_device *)global_spi;
	TTYSPI_DEBUG_PRINT("%s called ...........................................................\n", __func__);
    spi_tegra_abort_transfer(temp_spi);
    return HRTIMER_NORESTART;
}

#ifdef SPI_STATISTICS_CHECK
#define SPI_STAT_POLL_PERIOD (10)    // sec
static struct  timer_list  spi_statistics_timer;
static int statistics_count = 0;
static int dummy_data_flag = 0;

typedef enum {
    SPI_STAT_STATE_MIN=0,
    SPI_STAT_STATE_TXRX_NOT_CHG,
    SPI_STAT_STATE_RX_ONLY_NOT_CHG,
    SPI_STAT_STATE_STILL_NOT_CHG = SPI_STAT_STATE_RX_ONLY_NOT_CHG,
    SPI_STAT_STATE_DUMMY_TRANSACTION,
    SPI_STAT_STATE_DUMMY_TRANSACTION2,
    SPI_STAT_STATE_NO_TRANSACTION,
    SPI_STAT_STATE_MAX,
}spi_statistics_state_type;

static spi_statistics_state_type spi_s_state = SPI_STAT_STATE_MIN;


static void spi_statistic_cb_func(unsigned long unused)
{
    static unsigned int spi_tx_count = -1;
    static unsigned int spi_rx_count = -1;
    struct mdm_spi_data *spi_data = (struct mdm_spi_data *)gspi_data[0];
    int ret;
    int tx_chg_flg = (spi_tx_count == tx_count[0])?0:1;
    int rx_chg_flg = (spi_rx_count == rx_count[0])?0:1;

    statistics_count++;

    TTYSPI_DEBUG_PRINT("%s: statistics_count=%d, spi_h_state=%d\n", __func__, statistics_count, spi_s_state);

    if(tx_chg_flg || rx_chg_flg) {
        spi_s_state = SPI_STAT_STATE_MIN;
    } else {
        if((tx_chg_flg == 1)&&(rx_chg_flg==0)) {
            spi_s_state=SPI_STAT_STATE_RX_ONLY_NOT_CHG;
        }
        switch(spi_s_state) {
            case SPI_STAT_STATE_MIN:
                spi_s_state = SPI_STAT_STATE_TXRX_NOT_CHG;
            break;
            case SPI_STAT_STATE_TXRX_NOT_CHG:
                spi_s_state = SPI_STAT_STATE_STILL_NOT_CHG;
            break;
            case SPI_STAT_STATE_STILL_NOT_CHG:
                spi_s_state = SPI_STAT_STATE_DUMMY_TRANSACTION;
            break;
            case SPI_STAT_STATE_DUMMY_TRANSACTION:
                spi_s_state = SPI_STAT_STATE_DUMMY_TRANSACTION2;
            break;
            case SPI_STAT_STATE_DUMMY_TRANSACTION2:
            case SPI_STAT_STATE_NO_TRANSACTION:
                spi_s_state = SPI_STAT_STATE_NO_TRANSACTION;
            break;
            case SPI_STAT_STATE_MAX:
            default:
                printk(KERN_ERR "%s Unknown SPI statistics State %d \n", __func__ ,  spi_s_state);
                spi_s_state = SPI_STAT_STATE_NO_TRANSACTION;
            break;
        }
    }

    if(spi_data->is_suspended) {
        if(spi_s_state != SPI_STAT_STATE_NO_TRANSACTION) {
            spi_s_state = SPI_STAT_STATE_MIN;
        }
    }

    TTYSPI_DEBUG_PRINT("spi Tx : prev %d curr %d flg=%d\n", spi_tx_count, tx_count[0], tx_chg_flg);
    TTYSPI_DEBUG_PRINT("spi Rx : prev %d curr %d flg=%d spi_h_state=%d\n", spi_rx_count, rx_count[0], rx_chg_flg, spi_s_state);

    if((spi_s_state == SPI_STAT_STATE_DUMMY_TRANSACTION)
      ||(spi_s_state == SPI_STAT_STATE_DUMMY_TRANSACTION2)) {
        dummy_data_flag++;
        printk(KERN_INFO "spi dummy transaction dummy_data_flag=%d, spi_s_state=%d \n", dummy_data_flag, spi_s_state);

        //prevent spi transmission being registered as workqueue event
		if(atomic_read(&spi_table[spi_data->index].in_use) == 1) {
            queue_work(spi_data->mdm_wq, &spi_data->mdm_work);
		}
    }

    spi_tx_count = tx_count[0];
    spi_rx_count = rx_count[0];

    ret = mod_timer(&(spi_statistics_timer), round_jiffies(jiffies + (SPI_STAT_POLL_PERIOD*HZ)));
    if (ret) printk(KERN_ERR "%s:: Error in mod_timer\n", __func__);

    TTYSPI_DEBUG_PRINT("%s ended ...(%d, %d)\n", __func__ , statistics_count, spi_s_state);

}
#endif

/*
 * Function opens a tty device when called from user space
 */
static int
mdm_spi_open(struct tty_struct *tty, struct file *filp)
{
	int status = 0;
	struct mdm_spi_data *spi_data;
	int i;
    bool bfound = false;

    //prevent duplicated open of this driver by mux
    if(atomic_read(&spi_table[tty->index].in_use) == 1) {
        printk(KERN_ERR "%s: spi already open, Open failed!!!\n", __func__);
        return -1;
    }

    //process only when 'spi_table[tty->index].allocated' is set to 1 in  mdm_spi_probe
	if(spi_table[tty->index].allocated)
	{
		//increase assign_num only in mdm_spi_probe
        for (i=0; i<assign_num; i++) {
            if (gspi_data[i]->index == tty->index) {
                bfound = true;
                break;
            }
        }

        if (bfound) {
		    spi_data = gspi_data[i];
		    spi_data->mdm_tty = tty;
		    tty->driver_data = spi_data;
		    mdm_spi_buffer_initialization(spi_data);
		    spi_data->throttle = 0;

            atomic_set(&spi_table[tty->index].in_use, 1);

			printk(KERN_ERR "%s: spi Open success!!!\n", __func__);

		    TTYSPI_DEBUG_PRINT("%s: tty->index=%d, spi_data->index=:%d\n", __func__, tty->index, spi_data->index);

			//set spi transfer wait timeout function: intialize timer, register the timeout callback function, assign global_spi to spi_data->spi
            hrtimer_init(&(spi_data->timer), CLOCK_MONOTONIC, HRTIMER_MODE_REL);
            spi_data->timer.function = spi_timeout_cb_func;
            global_spi = spi_data->spi;
        }
        else {
		    printk(KERN_ERR "%s: failed - no matching spi found!!\n", __func__);
			status=-1;
        }

	}
	else
	{
		printk(KERN_ERR "%s: failed!!\n", __func__);
		status=-1;
	}

#ifdef SPI_STATISTICS_CHECK
    spi_s_state = SPI_STAT_STATE_NO_TRANSACTION;
#endif

	return status;
}

/*
 * Function closes an opened tty device driver when called from user space
 */
static void
mdm_spi_close(struct tty_struct *tty, struct file *filp)
{
#ifdef TX_BUFFER_QUEUE
	struct mdm_spi_data *spi_data = (struct mdm_spi_data *)tty->driver_data;
	int tx_anymore = 0;
    int queue_count = 0;
#endif

	//spi_data->mdm_tty = NULL;
	//tty->driver_data = NULL;

	TTYSPI_DEBUG_PRINT("%s\n",__func__);

    //assign the value 0 to 'in_use', inactivating it and blocking additional write requests occuring from the workqueue
	atomic_set(&spi_table[tty->index].in_use, 0);

#ifdef SPI_STATISTICS_CHECK
    spi_s_state = SPI_STAT_STATE_NO_TRANSACTION;
#endif

    //wait until spi_sync is finished
    while(atomic_read(&spi_data->is_syncing) == 1) {
		printk(KERN_INFO "%s: is spi_syncing \n", __func__);
        msleep(500);
    }

    //prevent calling NULL timer callback function after processing 'mdm_spi_close'
    do
    {
        //when the timer is currently active apply 1 ms of sleep
        if (hrtimer_try_to_cancel(&(spi_data->timer))==-1) {
			//actual scheduling occurs through the execution of msleep(1)
            msleep(1);
        } else {
            TTYSPI_DEBUG_PRINT("%s: hrtimer_try_to_cancel success!!!\n", __func__);
            break;
        }
    }while(1);

    //this log is for checking whether 'flush_workqueue' is finished
    TTYSPI_DEBUG_PRINT("%s: flush_workqueue()++ \n",__func__);

    //flush all threads currently queued in the workqueue
	flush_workqueue(spi_data->mdm_wq);

    //this log is for checking whether 'flush_workqueue' is finished
	TTYSPI_DEBUG_PRINT("%s: flush_workqueue()-- \n",__func__);

#ifdef TX_BUFFER_QUEUE
    while( (0 != get_tx_pending_data(spi_data, &tx_anymore)) && (queue_count < MAX_SPI_DATA_QUEUE) ) {
		queue_count++;
		TTYSPI_DEBUG_PRINT("%s: tx_buffer_queue_count:%d", __func__, queue_count);
    }
#endif
}

/*
 * Function is called from user space to send data to MODEM, it setups the transmission, enable MRDY signal and
 * waits for SRDY signal HIGH from MDOEM. Then starts transmission and reception of data to and from MODEM.
 * Once data read from MODEM is transferred to TTY core flip buffers, then "mdm_read_write_completion" is set
 * and this function returns number of bytes sent to MODEM
 */
#ifdef TX_BUFFER_QUEUE
static int
mdm_spi_write(struct tty_struct *tty, const unsigned char *buf, int count)
{
	struct mdm_spi_data *spi_data = (struct mdm_spi_data *)tty->driver_data;

    struct spi_data_send_struct *pSpiToSend;
    struct spi_data_send_struct *curr_ptr;

    int queue_size=0;
    u8 *send = NULL;

    if(spi_data==NULL){
	    printk(KERN_ERR "%s: no spi handle\n", __func__);
	    return 0;
	}

	//do not execute mdm_spi_write when mdm_spi_close has been processed
	if (atomic_read(&spi_table[spi_data->index].in_use) == 0) {
        printk(KERN_ERR "%s: open ttyspi first(#%d)\n", __func__, spi_data->index);
        return 0;
    }

	//tty can be changable because of dual spi
	spi_data->mdm_tty = tty;
	spi_data->mdm_tty->low_latency = 1;

	if( !buf ){
	    printk(KERN_ERR "%s: Buffer is NULL\n", __func__);
	    return 0;
	}
	if(!count){
	    printk(KERN_ERR "%s: Count is ZERO\n", __func__);
	    return 0;
	}

    curr_ptr = spi_data_send_pending;
    TTYSPI_DEBUG_PRINT("mdm_spi_write()++ curr_ptr=0x%x, index %d, tx_len= %d\n", (uint)curr_ptr, spi_data->index, count);

    if((curr_ptr!=NULL)
        &&(curr_ptr->count>MAX_SPI_DATA_QUEUE)){

		//wait until completely processing "mdm_read_write" so that the buffer queue is filled
        spi_data->is_waiting = true;
        TTYSPI_DEBUG_PRINT("%s waiting start\n",__func__);
        wait_for_completion(&spi_data->mdm_read_write_completion);
        TTYSPI_DEBUG_PRINT("%s waiting end\n",__func__);
        init_completion(&spi_data->mdm_read_write_completion);

        //printk(KERN_ERR "mdm_spi_write()-- ERROR curr_ptr=0x%x count=%d FULL", (uint)curr_ptr, curr_ptr->count);
        //return 0;
    }

    send = kmalloc(count, GFP_ATOMIC);

    //WBT #196463
    if(NULL == send) {
		printk(KERN_ERR "%s: memory allocation for 'send' failed!\n", __func__);
		return 0;
    }

    pSpiToSend = kmalloc(sizeof(struct spi_data_send_struct), GFP_ATOMIC);

    //WBT #196462, #196464
    if(NULL == pSpiToSend) {
		printk(KERN_ERR "%s: memory allocation for 'pSpiToSend' failed!\n", __func__);
        //WBT #219191
        if(send) {
            kfree(send);
        }
		return 0;
    }

    memset((void*)pSpiToSend, 0, sizeof(struct spi_data_send_struct));
    memcpy(send, buf, count);

    pSpiToSend->data = send;
    pSpiToSend->size = count;

    spin_lock_irqsave(&spi_nodes_lock, spi_lock_flag);
    pSpiToSend->next = spi_data_send_pending;

    if(spi_data_send_pending!=NULL){
        spi_data_send_pending->prev = pSpiToSend;
        queue_size = spi_data_send_pending->count;
    }
    else{
        queue_size = 0;
    }

    spi_data_send_pending = pSpiToSend;
    spi_data_send_pending->count = queue_size+1;
    spin_unlock_irqrestore(&spi_nodes_lock, spi_lock_flag);

    curr_ptr = spi_data_send_pending;

    TTYSPI_DEBUG_PRINT("mdm_spi_write()::  curr_ptr=0x%x count=%d \n", (uint)curr_ptr, curr_ptr->count);

    //prevent spi transmission being registered as workqueue event
	if(atomic_read(&spi_table[spi_data->index].in_use) == 1) {
	    queue_work(spi_data->mdm_wq, &spi_data->mdm_work);
	}

    return count;
}
#else // TX_BUFFER_QUEUE
static int
mdm_spi_write(struct tty_struct *tty, const unsigned char *buf, int count)
{
	struct mdm_spi_data *spi_data = (struct mdm_spi_data *)tty->driver_data;

	TTYSPI_DEBUG_PRINT("%s(index=%d, %d size)\n", __func__, spi_data->index, count);

    spi_data->mdm_ret_count = 0;

	if(spi_data==NULL)
	{
		printk(KERN_ERR "%s: no spi handle\n", __func__);
		return 0;
	}
	spi_data->mdm_tty = tty;
	spi_data->mdm_tty->low_latency = 1;
	if( !buf ){
		printk(KERN_ERR "%s: buffer is NULL\n", __func__);
		return spi_data->mdm_ret_count;
	}
	if(!count){
		printk(KERN_ERR "%s: count is ZERO\n", __func__);
		return spi_data->mdm_ret_count;
	}
	spi_data->mdm_master_initiated_transfer = 1;
	spi_data->mdm_spi_buf = buf;
	spi_data->mdm_spi_count = count;

	DISABLE_SRDY_IRQ(spi_data->spi->irq);
	//mdm_spi_set_srdy_signal(spi_data->spi->master->bus_num, 1);
#ifdef USE_SRDY
    //prevent spi transmission being registered as workqueue event
    if(atomic_read(&spi_table[spi_data->index].in_use) == 1) {
	    queue_work(spi_data->mdm_wq, &spi_data->mdm_work);
    }
#endif
	wait_for_completion(&spi_data->mdm_read_write_completion);

	ENABLE_SRDY_IRQ(spi_data->spi->irq);
	init_completion(&spi_data->mdm_read_write_completion);
	return spi_data->mdm_ret_count; /* Number of bytes sent to the device */
}
#endif // TX_BUFFER_QUEUE


/* This function should return number of free bytes left in the write buffer, in this case always return 2048 */
static int
mdm_spi_write_room(struct tty_struct *tty)
{
	return IFX_SPI_MAX_BUF_SIZE;
}


/* ################################################################################################################ */
/* These two functions are to be used in future to implement flow control (RTS & CTS)*/
/*static void
mdm_spi_throttle(struct tty_struct *tty)
{
	unsigned int flags;
	struct mdm_spi_data *spi_data = (struct mdm_spi_data *)tty->driver_data;
	spi_data->mdm_tty = tty;
	spin_lock_irqsave(&spi_data->spi_lock, flags);
	spi_data->throttle = 1;
	spin_unlock_irqrestore(&spi_data->spi_lock, flags);
}

static void
mdm_spi_unthrottle(struct tty_struct *tty)
{
	unsigned int flags;
	struct mdm_spi_data *spi_data = (struct mdm_spi_data *)tty->driver_data;
	spi_data->mdm_tty = tty;
	spin_lock_irqsave(&spi_data->spi_lock, flags);
	spi_data->throttle = 0;
	if( mdm_rx_buffer != NULL ){
	     tty_insert_flip_string(spi_data->mdm_tty, mdm_rx_buffer, valid_buffer_count);
	}
	spin_unlock_irqrestore(&spi_data->spi_lock, flags);
}*/
/* ################################################################################################################ */

/* End of MDM SPI Operations */

/* ################################################################################################################ */

/* TTY - SPI driver Operations */
static int
mdm_spi_probe(struct spi_device *spi)
{
	int status;
	struct mdm_spi_data *spi_data;

    TTYSPI_DEBUG_PRINT("%s: (assign_num=%d)\n", __func__, assign_num);

	/* memory allocation for SPI driver data */
	spi_data = (struct mdm_spi_data*)kmalloc(sizeof(struct mdm_spi_data), GFP_KERNEL);
	if (!spi_data){
	    return -ENOMEM;
    }

	//spi_data->mdm_tty = NULL;
    //initialize all member variables of mdm_spi_data
	memset((void*)spi_data, 0, sizeof(struct mdm_spi_data));

    status = mdm_spi_allocate_frame_memory(spi_data, IFX_SPI_FRAME_SIZE);
    if(status != 0) {
		printk(KERN_ERR "%s: Failed to allocate memory for buffers\n", __func__);

		//WBT #196465
		if(spi_data) {
			kfree(spi_data);
		}

		return -ENOMEM;
    }

    dev_set_drvdata(&spi->dev,spi_data);
    spin_lock_init(&spi_data->spi_lock);
    INIT_WORK(&spi_data->mdm_work, mdm_spi_handle_work);

    //workqueue thread must be created in the probe function, because of the mrdy interrupt coming from cp
    spi_data->mdm_wq = create_singlethread_workqueue("mdm6600");
    if(!spi_data->mdm_wq){
		printk(KERN_ERR "Failed to setup workqueue - mdm_wq \n");
    }

	init_completion(&spi_data->mdm_read_write_completion);

    #ifdef TX_BUFFER_QUEUE
    spi_data->is_waiting = false;
    #endif

    //config srdy,mrdy gpio value according to hw revision
    if (get_hw_rev() < REV_F) {
        SPI1_SRDY = TEGRA_GPIO_PO0;
        SPI1_MRDY = TEGRA_GPIO_PO5;
        SPI2_SRDY = TEGRA_GPIO_PO4;
        SPI2_MRDY = TEGRA_GPIO_PO3;
    } else {
        SPI1_SRDY = TEGRA_GPIO_PS0;
        SPI1_MRDY = TEGRA_GPIO_PO5; //TEGRA_GPIO_PS1;
        SPI2_SRDY = TEGRA_GPIO_PS1; //TEGRA_GPIO_PO5;
        SPI2_MRDY = TEGRA_GPIO_PO0;
    }

    /* configure SPI controller */
    spi_data->spi = spi;
    spi->mode = SPI_MODE_1 | SPI_CS_HIGH;
    spi->bits_per_word = 32;

    /* get the spi bus id : ex) spi0.0 */
    spi_data->index = spi->master->bus_num;

    /* configure SPI chip_select */
    switch (spi_data->index) {
    	case 0:
    	case 1:
        	spi->chip_select = 0;
        	break;
    	case 2:
        	spi->chip_select = 2;
        	break;
    	default:
        	spi->chip_select = 0;
        	break;
    }

    //must be set to 4 times the speed of the actual running clock
	spi->max_speed_hz = 4*24*1000*1000;

    status = spi_setup(spi);

	printk(KERN_ERR "%s: (bus_num=%d)\n", __func__, spi->master->bus_num);
    spi_tegra_register_callback(spi, mdm_spi_callback, (void*)&spi->master->bus_num);
    if(status < 0){
		printk(KERN_ERR "%s: Failed to setup SPI \n", __func__);
    }

#ifdef CONFIG_DUAL_SPI //disables spi secondary port
	if(0 == spi_data->index) {
#endif
		/* SPI1_SRDY */
		gpio_request(SPI1_SRDY, "ipc1_srdy");
		tegra_gpio_enable(SPI1_SRDY);
		gpio_direction_output(SPI1_SRDY, 0);

		/* SPI1_MRDY */
		gpio_request(SPI1_MRDY, "ipc1_mrdy");
		tegra_gpio_enable(SPI1_MRDY);
		gpio_direction_input(SPI1_MRDY);
		spi->irq = gpio_to_irq(SPI1_MRDY);

		TTYSPI_DEBUG_PRINT("irq = %d, name = %s\n", spi->irq, spi->dev.driver->name);
		status = request_irq(spi->irq, mdm_spi_handle_mrdy_irq,  IRQF_TRIGGER_RISING, spi->dev.driver->name, spi_data);

		// set irq_wake only when modem is connected
		set_irq_wake(spi->irq, !!is_modem_connected());

		// set the SPI2_MRDY gpio pin as an ap_suspend_state gpio pin  for checking the suspend state of the ap
		gpio_request(SPI2_MRDY, "ap_suspend_state");
		tegra_gpio_enable(SPI2_MRDY);
		gpio_direction_output(SPI2_MRDY, 0);

#ifdef CONFIG_DUAL_SPI //disables spi secondary port
	} else {
		/* SPI2_SRDY */
		gpio_request(SPI2_SRDY, "ipc2_srdy");
		tegra_gpio_enable(SPI2_SRDY);
		gpio_direction_output(SPI2_SRDY, 0);

		/* SPI2_MRDY */
		gpio_request(SPI2_MRDY, "ipc2_mrdy");
		tegra_gpio_enable(SPI2_MRDY);
		gpio_direction_input(SPI2_MRDY);
		spi->irq = gpio_to_irq(SPI2_MRDY);

		TTYSPI_DEBUG_PRINT("irq = %d, name = %s\n", spi->irq, spi->dev.driver->name);
    	status = request_irq(spi->irq, mdm_spi_handle_mrdy_irq,  IRQF_TRIGGER_RISING, spi->dev.driver->name, spi_data);
	}
#endif

    if (status != 0) {
	    printk(KERN_ERR "Failed to request IRQ for MRDY\n");
	    printk(KERN_ERR "%s: Failed\n", __func__);
	    if(spi_data->mdm_tx_buffer) {
		    kfree(spi_data->mdm_tx_buffer);
	    }
	    if(spi_data->mdm_rx_buffer) {
		    kfree(spi_data->mdm_rx_buffer);
	    }
	    if(spi_data) {
		    kfree(spi_data);
	    }
	    return status;
    }
    spi_data->mdm_master_initiated_transfer = 0;

    gspi_data[assign_num] = spi_data;

	//mdm_spi_sync_read_write(spi_data, IFX_SPI_HEADER_SIZE); /* 4 bytes for header */
    spi_table[spi_data->index].allocated = 1;
    /* increase assigned number of spi driver */
    assign_num++;

    // enable gpios for modem shutdown
    tegra_gpio_enable(GPIO_CP_STATE);
    tegra_gpio_enable(GPIO_IFX_RESET_1V8_N);
    tegra_gpio_enable(GPIO_IFX_PWRON_1V8);

    gpio_request_one(GPIO_CP_STATE, GPIOF_IN, "cp_state");
    gpio_request_one(GPIO_IFX_RESET_1V8_N, GPIOF_OUT_INIT_LOW, "modem_reset_n");
    gpio_request_one(GPIO_IFX_PWRON_1V8, GPIOF_OUT_INIT_LOW, "modem_pwron");

#ifdef CONFIG_HAS_WAKELOCK
    wake_lock_init(&spi_data->spi_wakelock, WAKE_LOCK_SUSPEND, "spi_wakelock");
#endif

#ifdef SPI_STATISTICS_CHECK
    if(status == 0) {
        int ret = 0;
        TTYSPI_DEBUG_PRINT("%s: statistics timer setting !!\n", __func__);
        setup_timer(&spi_statistics_timer, spi_statistic_cb_func, 0);
        ret = mod_timer( &spi_statistics_timer, round_jiffies(jiffies + SPI_STAT_POLL_PERIOD*HZ));
        if (ret) printk(KERN_ERR "%s: Error in mod_timer !!\n", __func__);
    }
#endif
    TTYSPI_DEBUG_PRINT(KERN_INFO "%s: end\n", __func__);

	return status;
}

static int
mdm_spi_remove(struct spi_device *spi)
{
	struct mdm_spi_data *spi_data;

    //WBT #219190
	spi_data = spi_get_drvdata(spi);

#ifdef CONFIG_HAS_WAKELOCK
	wake_lock_destroy(&spi_data->spi_wakelock);
#endif

	spin_lock_irq(&spi_data->spi_lock);
	spi_data->spi = NULL;
	spi_set_drvdata(spi, NULL);
	spin_unlock_irq(&spi_data->spi_lock);

    if(spi_data->mdm_tx_buffer) {
		kfree(spi_data->mdm_tx_buffer);
	}
    if(spi_data->mdm_rx_buffer) {
		kfree(spi_data->mdm_rx_buffer);
	}
    if(spi_data) {
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
    gpio_set_value(GPIO_IFX_PWRON_1V8, 1);
    mdelay(2300);
    gpio_set_value(GPIO_IFX_PWRON_1V8, 0);

    // check CP state if modem(CP) is turned off
    mdelay(6000);
    level = gpio_get_value(GPIO_CP_STATE);

    printk(KERN_INFO "%s: CP STATE pin is %d\n", __func__, level);

    return (0 == level? 0: -1);
}

/* reset modem */
// LGE_UPDATE_S CP Reset & Response halt Recovery
#ifdef LGE_FEATURE_CP_HALT_DETECTION
void modem_reset(void)
{
    gpio_set_value(GPIO_IFX_PWRON_1V8, 1);
    mdelay(500);
    gpio_set_value(GPIO_IFX_PWRON_1V8, 0);
    mdelay(10);
    gpio_set_value(GPIO_IFX_RESET_1V8_N, 1);
    mdelay(500);
    gpio_set_value(GPIO_IFX_RESET_1V8_N, 0);
}
EXPORT_SYMBOL_GPL(modem_reset);
#else
static void modem_reset(void)
{
    gpio_set_value(GPIO_IFX_PWRON_1V8, 1);
    mdelay(500);
    gpio_set_value(GPIO_IFX_PWRON_1V8, 0);
    mdelay(10);
    gpio_set_value(GPIO_IFX_RESET_1V8_N, 1);
    mdelay(500);
    gpio_set_value(GPIO_IFX_RESET_1V8_N, 0);
}
#endif
// LGE_UPDATE_E CP Reset & Response halt Recovery

static int mdm_spi_shutdown(struct spi_device *spi)
{
	int retry = 3; // retry count
	//TTYSPI_DEBUG_PRINT("%s()\n", __func__);

	//When there is unknonw modem state, this GPIO would make leakage voltage to CPU.
	//Because of the leakage voltage, the system can not boot up by POWER Button.
	//To prevent this issue, the SPI1_SRDY pin will be changed from OUTPUT to INPUT at SHUTDOWN.
	gpio_direction_input(SPI1_SRDY);

	// if modem is not connected, return
	if (!is_modem_connected())
		return 0;

	// power down modem by gpio
	disable_mdm_irq();
	while (0 != modem_powerdown() && retry > 0)
	{
		retry--;
		modem_reset();
		mdelay(6000); // wait until CP reboot
	}
	return 0;
}

#ifdef CONFIG_PM
static int mdm_spi_suspend(struct spi_device *spi, pm_message_t mesg)
{
	struct mdm_spi_data *spi_data;
	unsigned long flags;

	TTYSPI_DEBUG_PRINT("%s()\n", __func__);

	spi_data = spi_get_drvdata(spi);

	//prevent duplicated access to 'spi_data->is_suspended'
	spin_lock_irqsave(&spi_data->spi_lock, flags);
	spi_data->is_suspended = 1;
	spin_unlock_irqrestore(&spi_data->spi_lock, flags);

	TTYSPI_DEBUG_PRINT("%s: (is_suspend=%d)\n", __func__, spi_data->is_suspended);

    //if modem is connected, give info 'ap_suspend_state' to cp
    if(is_modem_connected()) {
	    //set 'ap_suspend_state' gpio value to 'true'
	    gpio_set_value(SPI2_MRDY, spi_data->is_suspended);
	    TTYSPI_DEBUG_PRINT("%s: (gpio=%d, ap_suspend_state=%d)\n", __func__, SPI2_MRDY, gpio_get_value(SPI2_MRDY));
    }

#if 0 //remove 'flush_workqueue' to fix pending of AT CMD on "SCREEN OFF"
    //if modem is connected and communicating, flush all threads currently queued
    if(is_modem_connected() && is_modem_communicating()) {
        //flush all threads currently queued in the workqueue
	    //flush_workqueue(spi_data->mdm_wq);
    }
#endif

    //if modem is removed, disable ap wakeup
	if (!is_modem_connected())
		set_irq_wake(spi->irq, 0);

	return 0;
}

static int mdm_spi_resume(struct spi_device *spi)
{
	struct mdm_spi_data *spi_data;
	unsigned long flags;

	TTYSPI_DEBUG_PRINT("%s()\n", __func__);

	spi_data = spi_get_drvdata(spi);

	//prevent duplicated access to 'spi_data->is_suspended'
	spin_lock_irqsave(&spi_data->spi_lock, flags);
	spi_data->is_suspended = 0;
	spin_unlock_irqrestore(&spi_data->spi_lock, flags);

	TTYSPI_DEBUG_PRINT("%s: (is_suspend=%d)\n", __func__, spi_data->is_suspended);

    //if modem is connected, give info 'ap_suspend_state' to cp
    if(is_modem_connected()) {
	    //set ap_suspend state gpio value to 'false'
	    gpio_set_value(SPI2_MRDY, spi_data->is_suspended);
	    TTYSPI_DEBUG_PRINT("%s: (gpio=%d, ap_suspend_state=%d)\n", __func__, SPI2_MRDY, gpio_get_value(SPI2_MRDY));
    }

	return 0;
}
#endif

/* End of TTY - SPI driver Operations */

/* ################################################################################################################ */

static struct spi_driver mdm_spi_driver = {
	.driver = {
		.name = "mdm6600",
        .bus = &spi_bus_type,
		.owner = THIS_MODULE,
	},
	.probe = mdm_spi_probe,
	.remove = __devexit_p(mdm_spi_remove),
	.shutdown = mdm_spi_shutdown,
#ifdef CONFIG_PM
	.suspend = mdm_spi_suspend,
	.resume = mdm_spi_resume,
#endif
};

/*
 * Structure to specify info for the tty core about tty driver operations supported in TTY SPI driver.
 */
static const struct tty_operations mdm_spi_ops = {
    .open = mdm_spi_open,
    .close = mdm_spi_close,
    .write = mdm_spi_write,
    .write_room = mdm_spi_write_room,
    //.throttle = mdm_spi_throttle,
    //.unthrottle = mdm_spi_unthrottle,
    //.set_termios = mdm_spi_set_termios,
};

/* ################################################################################################################ */

/*
 * Intialize frame sizes as "IFX_SPI_DEFAULT_BUF_SIZE"(2044) bytes for first SPI frame transfer
 */
static void
mdm_spi_buffer_initialization(struct mdm_spi_data *spi_data)
{
	spi_data->mdm_sender_buf_size = IFX_SPI_DEFAULT_BUF_SIZE;
    spi_data->mdm_receiver_buf_size = IFX_SPI_DEFAULT_BUF_SIZE;

	spi_data->mdm_spi_buf = NULL;
	spi_data->mdm_spi_count = 0;
}

/*
 * Allocate memory for TX_BUFFER and RX_BUFFER
 */
static int
mdm_spi_allocate_frame_memory(struct mdm_spi_data *spi_data, unsigned int memory_size)
{
	int status = 0;

	TTYSPI_DEBUG_PRINT("%s: (assign_num=%d)\n", __func__, assign_num);

	spi_data->mdm_rx_buffer = kmalloc(memory_size+IFX_SPI_HEADER_SIZE, GFP_KERNEL);
	if (!spi_data->mdm_rx_buffer) {
		printk(KERN_ERR "Open Failed ENOMEM\n");
		status = -ENOMEM;
	}
	spi_data->mdm_tx_buffer = kmalloc(memory_size+IFX_SPI_HEADER_SIZE, GFP_KERNEL);
	if (!spi_data->mdm_tx_buffer) {
		printk(KERN_ERR "Open Failed ENOMEM\n");
		status = -ENOMEM;
	}
	if(status == -ENOMEM){
		if(spi_data->mdm_tx_buffer){
			kfree(spi_data->mdm_tx_buffer);
		}
		if(spi_data->mdm_rx_buffer){
			kfree(spi_data->mdm_rx_buffer);
		}
	}
	return status;
}

/*
 * Function to set header information according to IFX SPI framing protocol specification
 */
static void
mdm_spi_set_header_info(unsigned char *header_buffer, unsigned int curr_buf_size, unsigned int next_buf_size)
{
	int i;
	union mdm_spi_frame_header header;

	for(i=0; i<4; i++){
		header.framesbytes[i] = 0;
	}

	header.mdm_spi_header.curr_data_size = curr_buf_size;
	if(next_buf_size){
		header.mdm_spi_header.more=1;
		header.mdm_spi_header.next_data_size = next_buf_size;
	}
	else{
		header.mdm_spi_header.more=0;
		header.mdm_spi_header.next_data_size = 128;
	}

	for(i=3; i>=0; i--){
		header_buffer[i] = header.framesbytes[i];
	}
}

/*
 * Function to get header information according to IFX SPI framing protocol specification
 */
static int
mdm_spi_get_header_info(struct mdm_spi_data *spi_data, unsigned int *valid_buf_size)
{
	int i;
	union mdm_spi_frame_header header;

	for(i=0; i<4; i++){
		header.framesbytes[i] = 0;
	}

	for(i=3; i>=0; i--){
		header.framesbytes[i] = spi_data->mdm_rx_buffer[i];
	}

	if(header.mdm_spi_header.curr_data_size<=IFX_SPI_DEFAULT_BUF_SIZE)
	{
		// check rx size
	    *valid_buf_size = header.mdm_spi_header.curr_data_size;
	}
	else
	{
		*valid_buf_size = 0;
		printk(KERN_ERR "%s: rx data exceed buffer size\n", __func__);
	}

	if(header.mdm_spi_header.more){
		// check next rx size
		if(header.mdm_spi_header.next_data_size<=IFX_SPI_DEFAULT_BUF_SIZE){
			return header.mdm_spi_header.next_data_size;
		}
	}
	return 0;
}


static void
mdm_spi_clear_header_info(unsigned int *hdr_ptr)
{
    *(hdr_ptr) = 0;
    *(hdr_ptr+1) = 0;
}

#ifdef MDM6600_SPI_HEADER
static void
mdm_spi_set_tx_frame_count(unsigned int *hdr_ptr, unsigned int id)
{
    *(hdr_ptr+1) = ++(tx_count[id]);
    if(*(hdr_ptr+1)==0) *(hdr_ptr+1) = ++(tx_count[id]);
    TTYSPI_DEBUG_PRINT(KERN_ERR "Tx Count %d\n", tx_count[id]);
}

static unsigned int
mdm_spi_get_rx_frame_count(unsigned int *hdr_ptr)
{
    return *(hdr_ptr+1);
}

static int
mdm_spi_get_remote_chk(unsigned long *hdr_ptr)
{
    if(*(hdr_ptr) & (0x00002000)){
	    return 1;
	}
	return 0;
}
#endif

/*
 * Function to set/unset SRDY signal
 */
static void
mdm_spi_set_srdy_signal(s16 bus_num, int value)
{
#if 1//ndef USE_SRDY
	int gpio_num = 0;
	TTYSPI_DEBUG_PRINT("%s: bus_num=%d\n", __func__, bus_num);

#ifdef CONFIG_DUAL_SPI //disables spi secondary port
	if(0 == bus_num) {
#endif
		gpio_set_value(SPI1_SRDY, value);
		gpio_num = SPI1_SRDY;
#ifdef CONFIG_DUAL_SPI //disables spi secondary port
	} else if(1 == bus_num) {
		gpio_set_value(SPI2_SRDY, value);
		gpio_num = SPI2_SRDY;
	}
#endif

	TTYSPI_DEBUG_PRINT("%s: gpio=%d, srdy=%d\n", __func__, gpio_num, value);
#endif
}


static int
mdm_spi_get_mrdy_signal(s16 bus_num)
{
	int gpio_num = 0;
    int value = 0;

	TTYSPI_DEBUG_PRINT("%s: bus_num=%d\n", __func__, bus_num);

#ifdef CONFIG_DUAL_SPI //disables spi secondary port
	if(0 == bus_num) {
#endif
		value = gpio_get_value(SPI1_MRDY);
		gpio_num = SPI1_MRDY;
#ifdef CONFIG_DUAL_SPI //disables spi secondary port
	} else if(1 == bus_num) {
		value = gpio_get_value(SPI2_MRDY);
		gpio_num = SPI2_MRDY;
	}
#endif

	TTYSPI_DEBUG_PRINT("%s: gpio=%d, srdy=%d\n", __func__, gpio_num, value);

    return value;
}


int	mdm_spi_callback(void *client_data)
{
	s16 *bus_num;
	bus_num = (s16*)client_data;

	TTYSPI_DEBUG_PRINT("%s called",__func__);
	TTYSPI_DEBUG_PRINT("%s bus_num=%d",__func__, *bus_num);
	mdm_spi_set_srdy_signal(*bus_num, 1);
	return 0;
}

#ifndef TX_BUFFER_QUEUE
/*
 * Function to calculate next_frame_size required for filling in SPI frame Header
 */
static int
mdm_spi_get_next_frame_size(int count)
{
	if(count > IFX_SPI_MAX_BUF_SIZE){
		return IFX_SPI_MAX_BUF_SIZE;
	}
	else{
		return count;
	}
}

/*
 * Function to setup transmission and reception. It implements a logic to find out the mdm_current_frame_size,
 * valid_frame_size and sender_next_frame_size to set in SPI header frame. Copys the data to be transferred from
 * user space to TX buffer and set MRDY signal to HIGH to indicate Master is ready to transfer data.
 */
static void
mdm_spi_setup_transmission(struct mdm_spi_data *spi_data)
{
    if( (spi_data->mdm_sender_buf_size != 0) || (spi_data->mdm_receiver_buf_size != 0) ){

        // current_frame_size must be IFX_SPI_MAX_BUF_SIZE
		spi_data->mdm_current_frame_size = IFX_SPI_MAX_BUF_SIZE;

		if(spi_data->mdm_spi_count > 0){
			if(spi_data->mdm_spi_count > spi_data->mdm_current_frame_size){
				spi_data->mdm_valid_frame_size = spi_data->mdm_current_frame_size;
				spi_data->mdm_spi_count = spi_data->mdm_spi_count - spi_data->mdm_current_frame_size;
			}
			else{
				spi_data->mdm_valid_frame_size = spi_data->mdm_spi_count;
				spi_data->mdm_spi_count = 0;
			}
        }
		else{
			spi_data->mdm_valid_frame_size = 0;
			spi_data->mdm_sender_buf_size = 0;
		}
		spi_data->mdm_sender_buf_size = mdm_spi_get_next_frame_size(spi_data->mdm_spi_count);

		/* memset buffers to 0 */
		memset(spi_data->mdm_tx_buffer, 0, IFX_SPI_MAX_BUF_SIZE+IFX_SPI_HEADER_SIZE);
		memset(spi_data->mdm_rx_buffer, 0, IFX_SPI_MAX_BUF_SIZE+IFX_SPI_HEADER_SIZE);

		/* Set header information */
		mdm_spi_set_header_info(spi_data->mdm_tx_buffer, spi_data->mdm_valid_frame_size, spi_data->mdm_sender_buf_size);
		if( spi_data->mdm_valid_frame_size > 0 ){

#ifdef MDM6600_SPI_HEADER
            mdm_spi_set_tx_frame_count((unsigned int *)spi_data->mdm_tx_buffer, spi_data->index);
#endif

			memcpy(spi_data->mdm_tx_buffer+IFX_SPI_HEADER_SIZE, spi_data->mdm_spi_buf, spi_data->mdm_valid_frame_size);
			spi_data->mdm_spi_buf = spi_data->mdm_spi_buf + spi_data->mdm_valid_frame_size;
		}
	}
}
#endif

#define MORE_DATA   0x00001000
#define CURR_DATA   0x00000FFF
#define NEXT_DATA   0x0FFF0000

static int
check_valid_rx_frame_header(unsigned int *hdr_ptr, int index)
{
  int retval=1;
  if(*(hdr_ptr) == 0xFFFFFFFF){
    retval = 0;
  }
  else if(*(hdr_ptr) == 0){
    retval = 0;
  }
  else if( (*(hdr_ptr)&(~(0x0FFF3FFF))) != 0 ){
    retval = 0;
  }
  else if((*(hdr_ptr+1)==0)){
    if( *(hdr_ptr) == 0x00800000 ) retval = 1;
    else if( *(hdr_ptr) == 0x00802000 ) retval = 1;
    else retval = 0;
  }
  else if( (*(hdr_ptr+1)!=0) ){
    if(( (*(hdr_ptr)&MORE_DATA) != 0 )&&( (*(hdr_ptr)&((NEXT_DATA))) == 0 )) retval = 0;
    else if(( (*(hdr_ptr)&MORE_DATA) == 0 )&&( (*(hdr_ptr)&((NEXT_DATA))) != 0x00800000 )) retval = 0;
    else if( *(hdr_ptr+1) == rx_count[index]) retval = 0;
    else retval = 1;
  }
  else{
    retval = 1;
  }
  TTYSPI_DEBUG_PRINT("\n check_valid_rx_frame_header *(hdr_ptr)=0x%x retval=%d \n", *(hdr_ptr), retval);
  return(retval);
}

/*
 * Function starts Read and write operation and transfers received data to TTY core. It pulls down MRDY signal
 * in case of single frame transfer then sets "mdm_read_write_completion" to indicate transfer complete.
 */
#define MAX_RETRY_COUNT 3
#ifdef TX_BUFFER_QUEUE
static void
mdm_spi_send_and_receive_data(struct mdm_spi_data *spi_data, int tx_pending)
{
	unsigned int rx_valid_buf_size;
	int status = 0;
	int retry_count = 0;

	//WBT #196460
    unsigned int prev_rx_frame = 0;
    int mReTransFlag=0;
    int mValidRxFrame=0;

	if(atomic_read(&spi_table[spi_data->index].in_use)==0) {
		return;
	}

    status = mdm_spi_sync_read_write(spi_data, IFX_SPI_FRAME_SIZE); /* 4 bytes for header */

    //resend tx buffer when spi transfer timeout happens and max_retry_count is smaller than MAX_RETRY_COUNT
#if 0
	while((status == -EAGAIN)
		&& (retry_count < MAX_RETRY_COUNT)
		&& (tx_pending != 0)
		&& (check_valid_rx_frame_header((unsigned int *)spi_data->mdm_rx_buffer) == 0) )
	{
		TTYSPI_DEBUG_PRINT("\n SPI Transaction Error : status = %d, retry_count=%d tx_pending=%d \n", status, retry_count, tx_pending);
	    mdelay(10);
		status = mdm_spi_sync_read_write(spi_data, IFX_SPI_FRAME_SIZE);
	    retry_count++;
	}
#else
    do{
      mReTransFlag = 0;

      mValidRxFrame = check_valid_rx_frame_header((unsigned int *)spi_data->mdm_rx_buffer, spi_data->index);

      if((status == -EAGAIN)
          &&(tx_pending != 0)
          &&(mValidRxFrame == 0)){  // TX Frame & spi internal error checking
        TTYSPI_DEBUG_PRINT("\n SPI Transaction Tx Error : status = %d, retry_count=%d tx_pending=%d \n", status, retry_count, tx_pending);
        mReTransFlag = 1;
        mdelay(10);
      }
      else{ // Rx frame checking
        if(mValidRxFrame == 0){
          TTYSPI_DEBUG_PRINT("\n SPI Transaction Rx Error : status = %d, retry_count=%d tx_pending=%d \n", status, retry_count, tx_pending);
          mReTransFlag = 1;
          mdelay(50);
        }
        else{
          mReTransFlag = 0;
        }
      }

      if(mReTransFlag == 1){
        status = mdm_spi_sync_read_write(spi_data, IFX_SPI_FRAME_SIZE);
        retry_count++;
      }
    }while((mReTransFlag == 1)&&(retry_count<MAX_RETRY_COUNT));
#endif

	if(memcmp(rx_dummy, spi_data->mdm_rx_buffer, IFX_SPI_HEADER_SIZE) ==0) {
	    spi_data->mdm_receiver_buf_size = 0;
	    return;
	}

	/* Handling Received data */
	spi_data->mdm_receiver_buf_size = mdm_spi_get_header_info(spi_data, &rx_valid_buf_size);

#ifdef MDM6600_SPI_HEADER
    if(rx_valid_buf_size !=0 ){
        prev_rx_frame = rx_count[spi_data->index];
        rx_count[spi_data->index] = mdm_spi_get_rx_frame_count((unsigned int *)spi_data->mdm_rx_buffer);

        TTYSPI_DEBUG_PRINT("\n Rx Count : prev_rx_frame = %d, rx_count=%d \n", prev_rx_frame, rx_count[spi_data->index]);

        if(rx_count[spi_data->index] == prev_rx_frame){
            printk(KERN_ERR "\n SPI RX Error : duplicated : drop the frame :: prev_rx_frame = %d, rx_count=%d \n", prev_rx_frame, rx_count[spi_data->index]);
            rx_valid_buf_size = 0;   // drop
            rx_count[spi_data->index] = prev_rx_frame;
        }
        else if(tx_count[spi_data->index] == 0){
            printk(KERN_ERR "\n SPI RX Error : NO Tx count rx bytes :: prev_rx_frame = %d, rx_count=%d \n", prev_rx_frame, rx_count[spi_data->index]);
        }
        else if(rx_count[spi_data->index] != (prev_rx_frame+1)){
            printk(KERN_ERR "\n SPI RX Error : prev_rx_frame = %d, rx_count=%d \n", prev_rx_frame, rx_count[spi_data->index]);
        }
    }
#endif

    //below statement needs to be disabled for spi echo command test, but needs to be enabled for uploading data from spi driver to ttyspi driver.
    if((spi_data->throttle == 0)
		&& (rx_valid_buf_size != 0)
		&& (spi_data->mdm_tty!=NULL)
		&& (atomic_read(&spi_table[spi_data->index].in_use) != 0)) //do not send tty layer after process mdm_spi_close
	{
		tty_insert_flip_string(spi_data->mdm_tty, spi_data->mdm_rx_buffer+IFX_SPI_HEADER_SIZE, rx_valid_buf_size);
		tty_flip_buffer_push(spi_data->mdm_tty);
	}

#ifdef MDM6600_SPI_HEADER
    status = mdm_spi_get_remote_chk((unsigned long *)spi_data->mdm_rx_buffer);
    if(status != 0 ){
        printk(KERN_ERR "\n SPI Remote CHK : prev_rx_frame = %d, rx_count=%d \n", prev_rx_frame, rx_count[spi_data->index]);
    }
#endif
}
#else // TX_BUFFER_QUEUE
static void
mdm_spi_send_and_receive_data(struct mdm_spi_data *spi_data)
{
	unsigned int rx_valid_buf_size;
	int status = 0;

    TTYSPI_DEBUG_PRINT("%s: start\n", __func__);

	//status = mdm_spi_sync_read_write(spi_data, spi_data->mdm_current_frame_size+IFX_SPI_HEADER_SIZE); /* 4 bytes for header */
    status = mdm_spi_sync_read_write(spi_data, IFX_SPI_MAX_BUF_SIZE+IFX_SPI_HEADER_SIZE); /* 4 bytes for header */
	if(status > 0){
		memset(spi_data->mdm_tx_buffer, 0, IFX_SPI_MAX_BUF_SIZE+IFX_SPI_HEADER_SIZE);
		spi_data->mdm_ret_count = spi_data->mdm_ret_count + spi_data->mdm_valid_frame_size;
	}
    TTYSPI_DEBUG_PRINT("%s: mdm_ret_count = %d\n", __func__, spi_data->mdm_ret_count);

	if(memcmp(rx_dummy, spi_data->mdm_rx_buffer, IFX_SPI_HEADER_SIZE) ==0) {
		spi_data->mdm_receiver_buf_size = 0;
		return;
	}

	/* Handling Received data */
	spi_data->mdm_receiver_buf_size = mdm_spi_get_header_info(spi_data, &rx_valid_buf_size);
    TTYSPI_DEBUG_PRINT("%s: mdm_receiver_buf_size = %d, rx_valid_buf_size = %d\n", __func__, spi_data->mdm_receiver_buf_size, rx_valid_buf_size);

	//if(!rx_valid_buf_size)
	//{
		//rx_valid_buf_size = 6;
	//}

    //below statement needs to be disabled for spi echo command test, but needs to be enabled for uploading data from spi driver to ttyspi driver.
	if((spi_data->throttle == 0) && (rx_valid_buf_size != 0) && (spi_data->mdm_tty!=NULL) && (atomic_read(&spi_table[spi_data->index].in_use) != 0))
	{
		tty_insert_flip_string(spi_data->mdm_tty, spi_data->mdm_rx_buffer+IFX_SPI_HEADER_SIZE, rx_valid_buf_size);
		tty_flip_buffer_push(spi_data->mdm_tty);
	}
	/*else
  	{
	handle RTS and CTS in SPI flow control
	Reject the packet as of now
	}*/

    TTYSPI_DEBUG_PRINT("%s: end\n", __func__);
}
#endif // TX_BUFFER_QUEUE


/*
 * Function copies the TX_BUFFER and RX_BUFFER pointer to a spi_transfer structure and add it to SPI tasks.
 * And calls SPI Driver function "spi_sync" to start data transmission and reception to from MODEM
 */

#define MAX_TRANSFER_FAILED 5
static int count_transfer_failed = 0;

//check communication with modem
int
is_modem_communicating(void)
{
	int ret = 1;

    if(count_transfer_failed >= MAX_TRANSFER_FAILED) {
		ret = 0;
    }

	return ret;
}


//#define SPI_TIMEDOUT_NSEC     20000000     //20ms
#define SPI_TIMEDOUT_SEC      2     //2s

static unsigned int
mdm_spi_sync_read_write(struct mdm_spi_data *spi_data, unsigned int len)
{
	int status;
	struct spi_message	m;
	struct spi_transfer	t = {
        .tx_buf		= spi_data->mdm_tx_buffer,
   		.rx_buf		= spi_data->mdm_rx_buffer,
        .len		= len,
    };
#if 0
    int i;
#endif

    TTYSPI_DEBUG_PRINT(KERN_INFO "%s: start\n", __func__);

	spi_message_init(&m);
	spi_message_add_tail(&t, &m);

    //condition added for checking the 'in_use' variable in order to prevent spi transmitting after 'mdm_spi_close'
	if((NULL == spi_data) || (NULL == spi_data->spi) || (atomic_read(&spi_table[spi_data->index].in_use)==0)) {
		//WBT #196461
		//status = -ESHUTDOWN;
		return -ESHUTDOWN;
	}
	else {
        //hrtimer_start(&(spi_data->timer), ktime_set(0, SPI_TIMEDOUT_NSEC), HRTIMER_MODE_REL);
        hrtimer_start(&(spi_data->timer), ktime_set(SPI_TIMEDOUT_SEC,0), HRTIMER_MODE_REL);

		//check if 'spi_sync' is running(when running, is_syncing is 1, when finished, is_syncing is 0)
		atomic_set(&spi_data->is_syncing, 1);

		status = spi_sync(spi_data->spi, &m);

		atomic_set(&spi_data->is_syncing, 0);

        do
		{
			//when the timer is currently active apply 1 ms of sleep
			if (hrtimer_try_to_cancel(&(spi_data->timer))==-1) {
				msleep(1);
			}
			else break;
		}while(1);

	}

	mdm_spi_set_srdy_signal(spi_data->spi->master->bus_num, 0);

    TTYSPI_DEBUG_PRINT("%s: status = %d\n", __func__, status);

	if (status == 0){
		status = m.status;
		if (status == 0)
			status = m.actual_length;

        //reset 'count_transfer_failed' to zero, if spi transter succeeds at least one out of five times
        count_transfer_failed = 0;
	}
    else {
		if(0 == is_modem_communicating()) {
            printk(KERN_ERR "%s: transmission UNsuccessful, status:%d, count_Transfer_Failed:%d\n", __func__, status, count_transfer_failed);
		}

		//increase 'count_transfer_failed', when spi transter fails
		count_transfer_failed++;
    }

#if 0
    //for debug perpose only print ttyspi layer tx/rx buffer value
    if(1 == spi_data->is_suspended) {
        printk("[spi tx] ");
        for(i=0;i<24;i++)
        {
        	printk("%02x ",spi_data->mdm_tx_buffer[i]);
        }
        printk("\n");

        printk("[spi rx] ");
        for(i=0;i<24;i++)		{
        printk("%02x ",spi_data->mdm_rx_buffer[i]);
        }
        printk(", status 0x%x \n", status);
    }
#endif

	return status;
}

/*
 * Function is a Interrupt service routine, is called when MRDY signal goes HIGH. It set up srdy pin when ready and
 * reception if it is a Master initiated data transfer. For both the cases Master intiated/Slave intiated
 * transfer it starts data transfer.
 */
static irqreturn_t
mdm_spi_handle_mrdy_irq(int irq, void *handle)
{
	struct mdm_spi_data *spi_data = (struct mdm_spi_data *)handle;
    int value;

    value = mdm_spi_get_mrdy_signal(spi_data->index);

    if(value == 0){
        TTYSPI_DEBUG_PRINT(KERN_ERR "%s in the mrdy irq value(=%d) --> ignore \n", __func__, value);
        return IRQ_HANDLED;
    }

#ifdef CONFIG_HAS_WAKELOCK
    wake_lock_timeout(&spi_data->spi_wakelock, SPI_WAKELOCK_TIMEOUT * HZ);
#endif

#ifdef TX_BUFFER_QUEUE
    if(atomic_read(&next_transfer_flag) == 0)
#else
	if(!spi_data->mdm_master_initiated_transfer)
#endif
	{
		//prevent spi transmission being registered as workqueue event
		if(atomic_read(&spi_table[spi_data->index].in_use) == 1) {
		    queue_work(spi_data->mdm_wq, &spi_data->mdm_work);
			TTYSPI_DEBUG_PRINT("%s: queue_work executed!!!\n", __func__);
		}
	}

	return IRQ_HANDLED;
}

#ifdef TX_BUFFER_QUEUE
int get_tx_pending_data(struct mdm_spi_data *spi_data, int *anymore)
{
    int cumu_data_size = 0;
    int data_size = 0;
    u8 *data_ptr;
    int tx_data_found = 0;
    int remain_count =0;
    int queue_count = 0;
    int initial_queue_count = 0;
    int cumu_queue_count = 0;

    struct spi_data_send_struct *prev_ptr;
    struct spi_data_send_struct **ppMS;
    struct spi_data_send_struct *curr_ptr=spi_data_send_pending;

    TTYSPI_DEBUG_PRINT("get_tx_pending_data()++ spi_data_send_pending 0x%x\n", (uint)curr_ptr);

    spin_lock_irqsave(&spi_nodes_lock, spi_lock_flag);

    if(spi_data_send_pending == NULL){
        spin_unlock_irqrestore(&spi_nodes_lock, spi_lock_flag);
        return 0;
    }

    initial_queue_count = spi_data_send_pending->count;

    do{
        prev_ptr = NULL;

		//fixed the queue boundary check routine for when ONLY ONE queue remains
        for(ppMS = &spi_data_send_pending, queue_count = 0;
            (((*ppMS)->next !=NULL) && (queue_count<=spi_data_send_pending->count));
            ppMS = &((*ppMS)->next), queue_count++);

        if(*ppMS == NULL) {
            printk(KERN_ERR "Never Hit this case\n");
            return 0;
        }
        else if(((*ppMS)->next != NULL)){
            printk(KERN_ERR "Never Hit this case *ppMS->next 0x%x\n", (uint)(*ppMS)->next);
            return 0;
        }
        else{
            // Good Queue
        }

        if(((*ppMS)->size+cumu_data_size) <= IFX_SPI_MAX_BUF_SIZE ){
            memcpy(&(spi_data->mdm_tx_buffer[IFX_SPI_HEADER_SIZE+cumu_data_size]),(*ppMS)->data,(*ppMS)->size);

            if((*ppMS)->bkp_data!=NULL){
                data_ptr = (*ppMS)->bkp_data;
                data_size = (*ppMS)->bkp_size;
                (*ppMS)->bkp_data = NULL;
                (*ppMS)->bkp_size = 0;
            }
            else {
                data_ptr = (*ppMS)->data;
                data_size = (*ppMS)->size;
            }

            cumu_data_size = cumu_data_size + (*ppMS)->size;

            (*ppMS)->size = 0;
            (*ppMS)->data = NULL;

            prev_ptr = (*ppMS)->prev;
            (*ppMS)->next = NULL;
            (*ppMS)->prev = NULL;

            spi_data_send_pending->count--;
            tx_data_found = 1;
            kfree(data_ptr);
            kfree((*ppMS));

            if(prev_ptr == NULL){
                spi_data_send_pending = NULL;
                break;
            }
            else{
                prev_ptr->next = NULL;
            }
        }
        else{
            remain_count = ((*ppMS)->size+cumu_data_size)-IFX_SPI_MAX_BUF_SIZE;
            memcpy(&(spi_data->mdm_tx_buffer[IFX_SPI_HEADER_SIZE+cumu_data_size]),(*ppMS)->data, ((*ppMS)->size-remain_count));
            cumu_data_size += ((*ppMS)->size-remain_count);

            (*ppMS)->bkp_data = (*ppMS)->data;
            (*ppMS)->bkp_size = (*ppMS)->size;
            (*ppMS)->data = (*ppMS)->data+((*ppMS)->size-remain_count);
            (*ppMS)->size = remain_count;

            *anymore = 1;
            break;
        }

        if(prev_ptr == NULL){
            printk(KERN_ERR "Never Hit this case prev_ptr=0x%x\n", (uint)prev_ptr);
            break;
        }
    cumu_queue_count++;
    }while((cumu_data_size <= IFX_SPI_MAX_BUF_SIZE) && (cumu_queue_count<initial_queue_count));

    curr_ptr=spi_data_send_pending;
    spin_unlock_irqrestore(&spi_nodes_lock, spi_lock_flag);

    TTYSPI_DEBUG_PRINT("get_tx_pending_data()-- curr_ptr=0x%x cumu_data_size=%d anymore=%d\n", (uint)curr_ptr, cumu_data_size, *anymore);

    return cumu_data_size;
}
#endif


#ifdef TX_BUFFER_QUEUE
static void
mdm_spi_handle_work(struct work_struct *work)
{
    struct mdm_spi_data *spi_data;
    int tx_pending = 0;
    int tx_anymore = 0;

    int pm_off_count;
    //unsigned int tx_starttime;
    //unsigned int tx_endtime;

    TTYSPI_DEBUG_PRINT("%s()++ %d\n", __func__, atomic_read(&next_transfer_flag));
    atomic_set(&next_transfer_flag, 0);
    do{
        spi_data = container_of(work, struct mdm_spi_data, mdm_work);

        if(spi_data==NULL) {
            printk(KERN_ERR "%s error spi_data(0x%x) is NULL \n", __func__, (int)spi_data);
            break;
        }

		if (atomic_read(&spi_table[spi_data->index].in_use) == 0) {
            printk(KERN_ERR "%s: open ttyspi first(#%d)\n", __func__, spi_data->index);
            return;
        }

        //replace to the header cleaning.
        //memset(spi_data->mdm_tx_buffer, 0, IFX_SPI_FRAME_SIZE);
        //memset(spi_data->mdm_rx_buffer, 0, IFX_SPI_FRAME_SIZE);

        tx_anymore =0;
        tx_pending = get_tx_pending_data(spi_data, &tx_anymore);

        pm_off_count = 0;

        //need to wait transferring tx/rx data because ap is in a suspended state
        if(1 == spi_data->is_suspended){
            pm_off_count = 1;
            TTYSPI_DEBUG_PRINT("mdm_spi_handle_work INFO is_suspended is (0x%x)\n", (int)spi_data->is_suspended);

            //wait for ap to return to resume state with a worst case scenario of 5sec
            do{
                mdelay(5);
                pm_off_count++;
            }while((1 == spi_data->is_suspended) && (pm_off_count<(5*200)));

            TTYSPI_DEBUG_PRINT("mdm_spi_handle_work INFO EXIT is_suspend = 0x%x pm_off_count=%d\n", (int)spi_data->is_suspended, pm_off_count);
            if(1 == spi_data->is_suspended) {
               // To Do how to handle the PM OFF state during 1sec
               TTYSPI_DEBUG_PRINT("mdm_spi_handle_work error is_suspended is (0x%x)\n", (int)spi_data->is_suspended);
            }
        }

#if 0 // Sometimes, the AP response time takes a long time, so a AT command pending happens. --> allow the null traffic because the CP seem to be ready to send.
        if((tx_pending == 0)
          && (atomic_read(&next_transfer_flag) == 0)
#ifdef SPI_STATISTICS_CHECK
          && (dummy_data_flag == 0)
#endif
          && (spi_data->mdm_receiver_buf_size == 0)
          && (mdm_spi_get_mrdy_signal(spi_data->index) == 0)
          && (pm_off_count == 0) ) {
            TTYSPI_DEBUG_PRINT("NULL Transfer will be triggered...(%d %d) %d %d %d %d %d\n",
            tx_count[0], rx_count[0], tx_pending, atomic_read(&next_transfer_flag), spi_data->mdm_receiver_buf_size,
            mdm_spi_get_mrdy_signal(spi_data->index), pm_off_count);
            break; // just return;
        }
#endif

        spi_data->mdm_receiver_buf_size = 0;  // next rx frame size

        //0. Frame Setup
        mdm_spi_clear_header_info((unsigned int *)spi_data->mdm_tx_buffer);

        if(tx_anymore) mdm_spi_set_header_info(spi_data->mdm_tx_buffer, tx_pending, IFX_SPI_MAX_BUF_SIZE);
        else mdm_spi_set_header_info(spi_data->mdm_tx_buffer, tx_pending, 0);

        if(tx_pending !=0) mdm_spi_set_tx_frame_count((unsigned int *)spi_data->mdm_tx_buffer, spi_data->index);

        mdm_spi_clear_header_info((unsigned int *)spi_data->mdm_rx_buffer);

        // 1. SPI Transmit
        //tx_starttime= NvOsGetTimeMS();
        mdm_spi_send_and_receive_data(spi_data, tx_pending);
        //tx_endtime = NvOsGetTimeMS();

#if 0
        if((tx_endtime - tx_starttime) > 1500) {
            printk(KERN_ERR "\n SPI TX Error : tx_endtime = %d, tx_starttime=%d (diff=%d) \n", tx_endtime, tx_starttime, (tx_endtime - tx_starttime));
            //if(timeout_count++>5) print_spi_log(3);
            timeout_count++;
        }
        else {
            timeout_count = 0;
        }
#endif

        if(tx_anymore || spi_data->mdm_receiver_buf_size) {
            TTYSPI_DEBUG_PRINT("mdm_spi_handle_work :: %d  %d \n", tx_anymore, spi_data->mdm_receiver_buf_size);
			atomic_set(&next_transfer_flag, 1);
        }
        else {
			atomic_set(&next_transfer_flag, 0);
        }

        if(spi_data->is_waiting == true)
        {
            complete(&spi_data->mdm_read_write_completion);
			spi_data->is_waiting = false;
        }

    }while(atomic_read(&next_transfer_flag) != 0);

    TTYSPI_DEBUG_PRINT("%s()-- %d\n", __func__, atomic_read(&next_transfer_flag));
	atomic_set(&next_transfer_flag, 0);
#ifdef SPI_STATISTICS_CHECK
    dummy_data_flag = 0;
#endif
}
#else
static void
mdm_spi_handle_work(struct work_struct *work)
{
	struct mdm_spi_data *spi_data = container_of(work, struct mdm_spi_data, mdm_work);
	s16 bus_num = spi_data->spi->master->bus_num;

    TTYSPI_DEBUG_PRINT("%s: start\n", __func__);
	TTYSPI_DEBUG_PRINT("%s: bus_num=%d\n", __func__, bus_num);

	if (atomic_read(&spi_table[spi_data->index].in_use) == 0) {
        printk(KERN_ERR "open ttyspi first(#%d)\n", spi_data->index);
        return;
    }

	/* read condition */
	if (!spi_data->mdm_master_initiated_transfer){
		TTYSPI_DEBUG_PRINT("[spi_mdm6600 #%d]mdm_spi_handle_work came from mdm6600\n", spi_data->index);
		mdm_spi_setup_transmission(spi_data);
		//mdm_spi_set_srdy_signal(bus_num, 1);
		mdm_spi_send_and_receive_data(spi_data);
		/* Once data transmission is completed, the MRDY signal is lowered */
		if((spi_data->mdm_sender_buf_size == 0)  && (spi_data->mdm_receiver_buf_size == 0)) {
			//mdm_spi_set_srdy_signal(bus_num, 0);

            //  Sometimes spi_tx_buf is set to NULL
            spi_data->mdm_sender_buf_size = IFX_SPI_DEFAULT_BUF_SIZE;
            spi_data->mdm_receiver_buf_size = IFX_SPI_DEFAULT_BUF_SIZE;
  	    }

		/* We are processing the slave initiated transfer in the mean time Mux has requested master initiated data transfer */
		/* Once Slave initiated transfer is complete then start Master initiated transfer */
		if(spi_data->mdm_master_initiated_transfer == 1){
		/* It is a condition where Slave has initiated data transfer and both SRDY and MRDY are high and at the end of data transfer
	 	* MUX has some data to transfer. MUX initiates Master initiated transfer rising MRDY high, which will not be detected at Slave-MODEM.
	 	* So it was required to rise MRDY high again */
            //mdm_spi_set_srdy_signal(bus_num, 1);
		}
	}
	/* write condition */
	else{
		TTYSPI_DEBUG_PRINT("[spi_mdm6600 #%d]mdm_spi_handle_work came from t20\n", spi_data->index);
		mdm_spi_setup_transmission(spi_data);
		mdm_spi_send_and_receive_data(spi_data);
		/* Once data transmission is completed, the MRDY signal is lowered */
		if(spi_data->mdm_sender_buf_size == 0){
			if(spi_data->mdm_receiver_buf_size == 0){
				//mdm_spi_set_srdy_signal(bus_num, 0);
#if 1
			    spi_data->mdm_sender_buf_size = IFX_SPI_DEFAULT_BUF_SIZE;
			    spi_data->mdm_receiver_buf_size = IFX_SPI_DEFAULT_BUF_SIZE;
#else
			    mdm_spi_buffer_initialization(spi_data);
#endif
			}
			spi_data->mdm_master_initiated_transfer = 0;

            //  fix RIL holding problem - in case of  spi_data->mdm_sender_buf_size > 0, must send complete also.
			//complete(&spi_data->mdm_read_write_completion);
		}
		//spi_data->mdm_master_initiated_transfer = 0;
		complete(&spi_data->mdm_read_write_completion);
        // fix RIL holding problem - in case of  spi_data->mdm_sender_buf_size > 0, must send complete also.
	}

}
#endif

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
__init mdm_spi_init(void)
{
	int status = 0;
	TTYSPI_DEBUG_PRINT("%s: start\n", __func__);
    memset(spi_table, 0x0, sizeof(struct allocation_table)*4);

	/* Allocate and Register a TTY device */
	mdm_spi_tty_driver = alloc_tty_driver(IFX_N_SPI_MINORS);
	if (!mdm_spi_tty_driver){
		printk(KERN_ERR "Fail to allocate TTY Driver\n");
		return -ENOMEM;
	}

	/* initialize the tty driver */
	mdm_spi_tty_driver->owner = THIS_MODULE;
	mdm_spi_tty_driver->driver_name = "tty_mdm6600";
	mdm_spi_tty_driver->name = "ttyspi";
	mdm_spi_tty_driver->major = IFX_SPI_MAJOR;
	mdm_spi_tty_driver->minor_start = 0;
	mdm_spi_tty_driver->type = TTY_DRIVER_TYPE_SERIAL;
	mdm_spi_tty_driver->subtype = SERIAL_TYPE_NORMAL;
	mdm_spi_tty_driver->flags = TTY_DRIVER_REAL_RAW;
	mdm_spi_tty_driver->init_termios = tty_std_termios;
	mdm_spi_tty_driver->init_termios.c_cflag = B921600 | CS8 | CREAD | HUPCL | CLOCAL;
	tty_set_operations(mdm_spi_tty_driver, &mdm_spi_ops);

	status = tty_register_driver(mdm_spi_tty_driver);
	if (status){
		printk(KERN_ERR "Failed to register mdm_spi_tty_driver");
		put_tty_driver(mdm_spi_tty_driver);
		return status;
	}
    else {
        TTYSPI_DEBUG_PRINT("register mdm_spi_tty_driver\n");
    }

	/* Register SPI Driver */
	status = spi_register_driver(&mdm_spi_driver);
	if (status < 0){
		printk(KERN_ERR "Failed to register SPI device");
		tty_unregister_driver(mdm_spi_tty_driver);
		put_tty_driver(mdm_spi_tty_driver);
		return status;
	}
    else {
        TTYSPI_DEBUG_PRINT("register mdm_spi_driver\n");
    }

#ifdef TX_BUFFER_QUEUE
    if(queue_first_time == 1){
        spin_lock_init(&spi_nodes_lock);
        spi_data_send_pending = NULL;
        queue_first_time = 0;
    }
#endif

	TTYSPI_DEBUG_PRINT("%s end\n", __func__);
	return status;
}

module_init(mdm_spi_init);


/*
 * Exit function to unregister SPI driver and tty SPI driver
 */
static void
__exit mdm_spi_exit(void)
{
    TTYSPI_DEBUG_PRINT("%s\n", __func__);
    spi_unregister_driver(&mdm_spi_driver);
    tty_unregister_driver(mdm_spi_tty_driver);
    put_tty_driver(mdm_spi_tty_driver);
}

module_exit(mdm_spi_exit);

/* End of Initialization Functions */

/* ################################################################################################################ */

MODULE_DESCRIPTION("MDM6600 SPI framing layer for dual spi");
MODULE_LICENSE("GPL");
