/* drivers/startablet/star_touch.c
 *
 * maXTouch TSP driver.
 *
 * Copyright (C) 2011 LG Electronics Co. Ltd.
 *
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/hrtimer.h>
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/i2c.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/string.h>
#include <linux/proc_fs.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/input.h>
#include <linux/module.h>
#include <linux/random.h>
#include <linux/major.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <linux/poll.h>
#include <linux/device.h>
#include <linux/mutex.h>
#include <linux/rcupdate.h>
#include <linux/smp_lock.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/workqueue.h>
#include <linux/wakelock.h>
#include <linux/freezer.h>
#include <linux/i2c.h>
#include <linux/gpio.h>

#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif
#include <mach/gpio-names.h>
#include <mach/hardware.h>
#include "star_touch.h"

//static int debug = DEBUG_TRACE;
static int debug = DEBUG_INFO;

static int TOUCH_INT;
static int TOUCH_MAIN_PWR;
static int TOUCH_IO_PWR;
static int TOUCH_RESET;

#define NO_RESET_FUNCTION

u8 mxt_suspend_flag = 0;
u8 without_touch_moudle = 0;

atomic_t mxt_first_boot_flag;

u8	*object_type_name[MXT_MAX_OBJECT_TYPES]	= {
	[0]	= "Reserved",
	[2]	= "T2 - Obsolete",
	[3]	= "T3 - Obsolete",
	[5]	= "MXT_GEN_MESSAGEPROCESSOR_T5",
	[6]	= "MXT_GEN_COMMANDPROCESSOR_T6",
	[7]	= "MXT_GEN_POWERCONFIG_T7",
	[8]	= "MXT_GEN_ACQUIRECONFIG_T8",
	[9]	= "TOUCH_MULTITOUCHSCREEN_T9",
	[15]	= "TOUCH_KEYARRAY_T15",
	[19]	= "SPT_GPIOPWM_T19",
	[22]	= "PROCG_NOISESUPPRESSION_T22",
	[23]	= "TOUCH_PROXIMITY_T23",
	[24]	= "PROCI_ONETOUCHGESTUREPROCESSOR_T24",
	[25]	= "SPT_SELFTEST_T25",
	[26]	= "T26 - Obsolete",
	[27]	= "PROCI_TWOTOUCHGESTUREPROCESSOR_T27",
	[28]	= "SPT_CTECONFIG_T28",
	[37]	= "DEBUG_DIAGNOSTICS_T37",
	[38]	= "USER_INFO_T38",
	[40]	= "PROCI_GRIPSUPPRESSION_T40",
	[41]	= "PROCI_PALMSUPPRESSION_T41",
	[43]	= "SPT_DIGITIZER_T43",
	[44]	= "SPT_MESSAGECOUNT_T44",
};

module_param(debug, int, 0644);
MODULE_PARM_DESC(debug, "Activate debugging output");

#ifdef CONFIG_HAS_EARLYSUSPEND
static void mxt_early_suspend(struct early_suspend *h)
{
	struct mxt_data *pmxt;
	u8 status;
	u8 i;
	u8 force_release_flag = 0;

	printk("### touch early suspend###\n");

	pmxt = container_of(h, struct mxt_data, early_suspend);


	for(i = 0; i < MAX_USING_FINGER_NUM; i++){
		if (fingerinfo[i].pressure != -1) {
			force_release_flag = 1;
			input_report_abs(pmxt->input, ABS_MT_TRACKING_ID, i);
			input_report_abs(pmxt->input, ABS_MT_TOUCH_MAJOR, 0);
			input_report_abs(pmxt->input, ABS_MT_POSITION_X, fingerinfo[i].x);
			input_report_abs(pmxt->input, ABS_MT_POSITION_Y, fingerinfo[i].y);
			input_mt_sync(pmxt->input);
		}
	}

	input_sync(pmxt->input);

	if(force_release_flag)
		printk("mxT1386 : froce release for suspend\n");

	status = mxt_suspend(pmxt->client);

	if(status!=0){
		printk("mxt_touch_suspend error  <<---\n");
	}
}

static void  mxt_late_resume(struct early_suspend *h)
{
	struct mxt_data *pmxt;
	u8 status;

	printk("### touch late resume###\n");

	pmxt = container_of(h, struct mxt_data, early_suspend);

	status=mxt_resume(pmxt->client);

	if(status!=0){
		printk("mxt_touch_resume error  <<---\n");
	}
}
#endif

u32 CRC_24(u32 crc, u8 byte1, u8 byte2)
{
	static const u32 crcpoly = 0x80001B;
	u32 result;
	u16 data_word;

	data_word = (u16) ((u16) (byte2 << 8u) | byte1);
	result = ((crc << 1u) ^ (u32) data_word);
	if (result & 0x1000000)
		result ^= crcpoly;
	return result;
}

u16 get_object_address(uint8_t object_type, uint8_t instance, struct mxt_object *object_table, int max_objs)
{
	uint8_t object_table_index = 0;
	uint8_t address_found = 0;
	uint16_t address = 0;

	struct mxt_object obj;

	while ((object_table_index < max_objs) && !address_found) {

		obj = object_table[object_table_index];

		if (obj.type == object_type) {

			address_found = 1;

			if (obj.instances >= instance) {
				address = obj.chip_addr + (obj.size + 1) * instance;
			}
			else {
				return 0;
			}
		}

		object_table_index++;
	}

	return address;
}

int mxt_read_block(struct i2c_client *client, u16 addr, u16 length, u8 *value)
{
	struct i2c_adapter *adapter = client->adapter;
	struct i2c_msg msg[2];
	__le16	le_addr;
	struct mxt_data *mxt;

	mxt = i2c_get_clientdata(client);

	if (mxt != NULL) {

		if ((mxt->last_read_addr == addr) && (addr == mxt->msg_proc_addr)) {
			if  (i2c_master_recv(client, value, length) == length)
				return 0;
			else
				return -EIO;
		}
		else {

			mxt->last_read_addr = addr;
		}
	}

	le_addr = cpu_to_le16(addr);
	msg[0].addr  = client->addr;
	msg[0].flags = 0x00;
	msg[0].len   = 2;
	msg[0].buf   = (u8 *) &le_addr;

	msg[1].addr  = client->addr;
	msg[1].flags = I2C_M_RD;
	msg[1].len   = length;
	msg[1].buf   = (u8 *) value;

	if  (i2c_transfer(adapter, msg, 2) == 2)
		return 0;
	else
		return -EIO;

}

int mxt_read_block_wo_addr(struct i2c_client *client, u16 length, u8 *value)
{

	if  (i2c_master_recv(client, value, length) == length) {

		printk(KERN_INFO "read ok\n");
		return length;
	}
	else {

		printk(KERN_WARNING "read failed\n");
		return -EIO;
	}

}

int mxt_write_block(struct i2c_client *client, u16 addr, u16 length, u8 *value)
{
	int i;
	struct {
	__le16	le_addr;
	u8	data[256];

	} i2c_block_transfer;

	struct mxt_data *mxt;

	if (length > 256)
		return -EINVAL;

	mxt = i2c_get_clientdata(client);

	if (mxt != NULL)
		mxt->last_read_addr = -1;

	for (i = 0; i < length; i++)
		i2c_block_transfer.data[i] = *value++;

	i2c_block_transfer.le_addr = cpu_to_le16(addr);

	i = i2c_master_send(client, (u8 *) &i2c_block_transfer, length + 2);

	if (i == (length + 2))
		return length;
	else
		return -EIO;
}

static int mxt_reset(struct i2c_client *client)
{

	u32 pinValue, count = 0;
	u8	i,j;
	u8 tmpbuf[9];

	printk("### mxt_reset ###\n");

	gpio_set_value(TOUCH_MAIN_PWR,0);
	gpio_set_value(TOUCH_IO_PWR,0);
	gpio_set_value(TOUCH_RESET,0);
	mdelay(5);
	gpio_set_value(TOUCH_MAIN_PWR,1);
	mdelay(10);
	gpio_set_value(TOUCH_IO_PWR,1);
	gpio_set_value(TOUCH_RESET,1);
	mdelay(200);

	do{
		pinValue = gpio_get_value(TOUCH_INT);

		if(debug >= DEBUG_INFO)
			printk("Touch INT still High in %s\n",__FUNCTION__);

		mdelay(3);

		count++;

		if(count > 500){
			printk("Reset Fail in %s, %d!!!\n",__FUNCTION__, __LINE__);
			return 1;
		}

	}while(pinValue == 1);

	mdelay(100);

	count = 0;

	do {
		mxt_read_block(client, 249, 7,tmpbuf);

		printk("message: ");

		for (j = 0; j < 9; j++)
			printk("%d ", tmpbuf[j]);

		printk("\n");

		count++;

		if(count > 100){
			printk("mXT : Initial Message Read Fail in %s, %d!!!\n",__FUNCTION__, __LINE__);
			break;
		}
	} while((tmpbuf[0]!=0xFF)&&(tmpbuf[0]!=0));

	for(i = 0; i < MAX_USING_FINGER_NUM; i++){

		fingerinfo[i].size_id = 0;
		fingerinfo[i].pressure = -1;
		fingerinfo[i].x = 0;
		fingerinfo[i].y = 0;
	}

	return 0;
}

int calculate_infoblock_crc(u32 *crc_result, struct mxt_data *mxt)
{
	u32 crc = 0;
	u16 crc_area_size;
	u8 *mem;
	int i;

	int error;
	struct i2c_client *client;

	client = mxt->client;

	crc_area_size = MXT_ID_BLOCK_SIZE + mxt->device_info.num_objs * MXT_OBJECT_TABLE_ELEMENT_SIZE;

	mem = kmalloc(crc_area_size, GFP_KERNEL);

	if (mem == NULL) {
		dev_err(&client->dev, "Error allocating memory\n");
		return -ENOMEM;
	}

	error = mxt_read_block(client, 0, crc_area_size, mem);
	if (error < 0) {
		kfree(mem);
		return error;
	}

	for (i = 0; i < (crc_area_size - 1); i = i + 2)
		crc = CRC_24(crc, *(mem + i), *(mem + i + 1));

	if (crc_area_size & 0x0001)
		crc = CRC_24(crc, *(mem + i), 0);

	kfree(mem);

	*crc_result = (crc & 0x00FFFFFF);

	return 1;

}

u8 process_error_message(u8 *message, struct mxt_data *mxt)
{
	struct i2c_client *client;
	u8 status;
	u8 need_to_reset_flag = 0;

	client = mxt->client;
	status = message[1];

	if (status & MXT_MSGB_T6_COMSERR){
		printk("maXTouch checksum error\n");
		need_to_reset_flag = 1;
	}
	if (status & MXT_MSGB_T6_CFGERR)
		printk("maXTouch configuration error\n");
	if (status & MXT_MSGB_T6_CAL)
		printk("maXTouch calibration in progress\n");
	if (status & MXT_MSGB_T6_SIGERR){
		printk("maXTouch acquisition error\n");
		need_to_reset_flag = 1;
	}
	if (status & MXT_MSGB_T6_OFL)
		printk("maXTouch cycle overflow\n");
	if (status & MXT_MSGB_T6_RESET)
		printk("maXTouch chip reset\n");

	return need_to_reset_flag;
}

static void mxt_worker(struct work_struct *work)
{
	struct	mxt_data *mxt;
	struct	i2c_client *client;

	u8	*message;
	u16	message_length;
	u16	message_addr;
	u8	report_id;
	u8	object;
	int	error;
	int	i;
	uint8_t id;
	u8	status[10] = {0,};
	u8	touch_size = 255;
	u16	xpos = 0xFFFF;
	u16	ypos = 0xFFFF;

	u16	object_address_1, object_address_2;
	u8 temp[2];
	u8	object_size;
	u16 ret;
	u8	readed_message[10];
	message = NULL;
	mxt = container_of(work, struct mxt_data, dwork.work);
	client = mxt->client;
	message_addr = mxt->msg_proc_addr;
	message_length = mxt->message_size;

	if( atomic_read( &mxt_first_boot_flag ) == 1 ){ // noise_filtering_level_change

		printk("mxt_noise_filtering_level_change_start !!\n");

		object_address_1 = mxt->object_table[MXT_PROCG_NOISESUPPRESSION_T22].chip_addr + 10;
		object_address_2 = mxt->object_table[MXT_GEN_COMMANDPROCESSOR_T6].chip_addr + 1;

		object_size = 1;

		/* Frequency Hopping Scale Change */

		temp[0] = MXT_T22_FREQHOPSCALE_IDLE;

		ret = mXT1386_i2c_write(client, object_address_1, temp, object_size);

		/* Back_up Config */

		temp[1] = 0x55;

		ret = mXT1386_i2c_write(client, object_address_2, temp + 1 , object_size);

		for(i = 0; i < 10; i++)
			mxt_read_block(client, 155, 9, readed_message);

		printk("mXT1386 : noise_filtering_level_change success !!!\n");

		atomic_set( &mxt_first_boot_flag, 0 );

	}

	if (message_length < 256) {
		message = kmalloc(message_length, GFP_KERNEL);
		if (message == NULL) {
			dev_err(&client->dev, "Error allocating memory\n");
			return;
		}
	}
	else {
		dev_err(&client->dev,
			"Message length larger than 256 bytes not supported\n");
	}

	memset(message, 0x0, sizeof(message));

	do {
		mxt->message_counter++;

		for (i = 1; i < I2C_RETRY_COUNT; i++) {

			error = mxt_read_block(client,
						message_addr,
						message_length,
						message);

			if(message[0] == 1 && process_error_message(message, mxt)){
				printk("maxTouch : Command_Processor Error Detected !!!!\n");

				if(mxt_reset(client))
					printk("maxTouch : Reset Failure in %s ,%d !!!!\n",__FUNCTION__,__LINE__);

				error = -EIO;
			}

			if (error >= 0)
				break;

			if (mxt_suspend_flag == 1){
					printk("mXT : worker out -> Suspend flag \n");
					enable_irq(mxt->irq);
					kfree(message);
					return;
			}

			mxt->read_fail_counter++;

			printk(KERN_ALERT "mXT: message read failed!\n");

			dev_err(&client->dev,"Failure reading maxTouch device\n");

			if (i == 9){
				mxt_reset(client);
				printk("maxTouch : Reset Failure in %s ,%d !!!!\n",__FUNCTION__,__LINE__);
				printk("mXT : worker out -> I2C_RETRY_COUNT Exceed\n");
				enable_irq(mxt->irq);
				kfree(message);
				return;
			}
		}

		report_id = message[0];

		if ((report_id != MXT_END_OF_MESSAGES) && (report_id != 0)) {

			object = mxt->rid_map[report_id].object;

			ypos = message[MXT_MSG_T9_XPOSMSB] * 16 +((message[MXT_MSG_T9_XYPOSLSB] >> 4) & 0xF);
			xpos = message[MXT_MSG_T9_YPOSMSB] * 16 +((message[MXT_MSG_T9_XYPOSLSB] >> 0) & 0xF);

			ypos >>= 2;

			id = message[MXT_MSG_REPORTID] -mxt->rid_map[report_id].first_rid;

			status[id] = message[MXT_MSG_T9_STATUS];

			if (status[id] & MXT_MSGB_T9_DETECT) {
				touch_size = message[MXT_MSG_T9_TCHAREA];
				touch_size = touch_size >> 2;

				if (!touch_size)
					touch_size = 1;

				if (status[id] & MXT_MSGB_T9_PRESS) {
					fingerinfo[id].x = xpos;
					fingerinfo[id].y = ypos;
					fingerinfo[id].pressure = touch_size;
					fingerinfo_send = 1;
				}

				else if (status[id] & MXT_MSGB_T9_MOVE) {
					fingerinfo[id].x = xpos;
					fingerinfo[id].y = ypos;
					fingerinfo[id].pressure = touch_size;
					fingerinfo_send = 1;
				}

				if ((status[id] & MXT_MSGB_T9_RELEASE) || (status[id] & MXT_MSGB_T9_SUPPRESS)) {
					fingerinfo[id].x = xpos;
					fingerinfo[id].y = ypos;
					fingerinfo[id].pressure = 0;
					fingerinfo_send = 1;
					printk("mxT1386 : press & release occur sametime !!\n");
				}
			}
			else if ((status[id] & MXT_MSGB_T9_RELEASE) || (status[id] & MXT_MSGB_T9_SUPPRESS)) {
					fingerinfo[id].x = xpos;
					fingerinfo[id].y = ypos;
					fingerinfo[id].pressure = 0;
					fingerinfo_send = 1;
			}
		}
	}while (gpio_get_value(TOUCH_INT) == 0);

	if (fingerinfo_send) {

		for (i=0; i< MAX_USING_FINGER_NUM; i++) {

			if ( fingerinfo[i].pressure == -1 )
				continue;

			REPORT_MT(i, fingerinfo[i].x, fingerinfo[i].y, fingerinfo[i].pressure,
					  fingerinfo[i].size_id, fingerinfo[i].size_id ? 1 : 0);

			if(status[i] & MXT_MSGB_T9_PRESS)
				printk("Touch Press - [%2d] (%4d, %4d)\n", i+1, fingerinfo[i].x,fingerinfo[i].y);

			if(status[i] & MXT_MSGB_T9_RELEASE)
				printk("Touch Release - [%2d] (%4d, %4d)\n", i+1, fingerinfo[i].x,fingerinfo[i].y);

			if(status[i] & MXT_MSGB_T9_SUPPRESS)
				printk("Touch Suppress - [%2d] (%4d, %4d)\n", i+1, fingerinfo[i].x,fingerinfo[i].y);

			if ( fingerinfo[i].pressure == 0 )
				fingerinfo[i].pressure= -1;
		}

		input_sync(mxt->input);

		fingerinfo_send = 0;
	}

	enable_irq(mxt->irq);

	kfree(message);
}

static irqreturn_t mxt_irq_handler(int irq, void *_mxt)
{
	struct	mxt_data *mxt = _mxt;
	unsigned long	flags;

	disable_irq_nosync(mxt->irq);
	mxt->irq_counter++;
	spin_lock_irqsave(&mxt->lock, flags);

	if (mxt_valid_interrupt()) {

		cancel_delayed_work(&mxt->dwork);
		schedule_delayed_work(&mxt->dwork, 0);
		mxt->valid_irq_counter++;
	}
	else {
		mxt->invalid_irq_counter++;
	}
	spin_unlock_irqrestore(&mxt->lock, flags);

	return IRQ_HANDLED;
}

u16 mxt_firmware[] = {
	#include "mXT1386_V11_AA.h"
};

static int __devinit mxt_identify(struct i2c_client *client,  struct mxt_data *mxt)
{
	u8 buf[7];
	int error;
	int identified;

	identified = 0;

	error = mxt_read_block(client, MXT_ADDR_INFO_BLOCK, 7, (u8 *) buf);

	if (error < 0) {
		mxt->read_fail_counter++;
		dev_err(&client->dev, "Failure accessing maXTouch device\n");
		return -EIO;
	}

	mxt->device_info.family_id  = buf[0];
	mxt->device_info.variant_id = buf[1];
	mxt->device_info.major	    = ((buf[2] >> 4) & 0x0F);
	mxt->device_info.minor      = (buf[2] & 0x0F);
	mxt->device_info.build	    = buf[3];
	mxt->device_info.x_size	    = buf[4];
	mxt->device_info.y_size	    = buf[5];
	mxt->device_info.num_objs   = buf[6];
	mxt->device_info.num_nodes  = mxt->device_info.x_size * mxt->device_info.y_size;

	if (mxt->device_info.family_id == MAXTOUCH_FAMILYID) {
		strcpy(mxt->device_info.family, maxtouch_family);
	}
	else {
		dev_err(&client->dev, "maXTouch Family ID [%d] not supported\n", mxt->device_info.family_id);
		identified = 0;
	}

	if ((mxt->device_info.variant_id == MXT1386_CAL_VARIANTID) ||(mxt->device_info.variant_id == MXT1386_UNCAL_VARIANTID)){
		strcpy(mxt->device_info.variant, mxt1386_variant);
	}
	else {
		dev_err(&client->dev, "maXTouch Variant ID [%d] not supported\n", mxt->device_info.variant_id);
		identified = -ENXIO;
	}

	dev_info(	&client->dev, 	"Atmel %s.%s Firmware version [%d.%d] Build 0x%02X\n",
		mxt->device_info.family,
		mxt->device_info.variant,
		mxt->device_info.major,
		mxt->device_info.minor,
		mxt->device_info.build
	);

	dev_info(
		&client->dev, "Atmel %s.%s Configuration [X: %d] x [Y: %d]\n",
		mxt->device_info.family,
		mxt->device_info.variant,
		mxt->device_info.x_size,
		mxt->device_info.y_size
	);

	return identified;

}

static int __devinit mxt_read_object_table(struct i2c_client *client, struct mxt_data *mxt)
{
	u16	report_id_count;
	u8	buf[MXT_OBJECT_TABLE_ELEMENT_SIZE];
	u8	object_type;
	u16	object_address;
	u16	object_size;
	u8	object_instances;
	u8	object_report_ids;
	u16	object_info_address;
	u32	crc;
	u32	crc_calculated;
	int	i;
	int	error;
	bool	incremental;

	u8 object_order[MXT_MAX_OBJECT_TYPES] = {0,};

	u8	object_instance;
	u8	object_report_id;
	u8	report_id;
	int	first_report_id;

	struct mxt_object *object_table;

	printk("mxt_read_object_table +\n");

	if (debug >= DEBUG_TRACE)
		printk(KERN_INFO "maXTouch driver get configuration\n");

	mxt->object_table	=	object_table
				=	kzalloc(sizeof(struct mxt_object) * MXT_MAX_OBJECT_TYPES, GFP_KERNEL);

	if (object_table == NULL) {
		printk(KERN_WARNING "maXTouch: Memory allocation failed in %s,#%d!\n",__FUNCTION__, __LINE__);
		return -ENOMEM;
	}

	if (debug >= DEBUG_TRACE)
		printk(KERN_INFO "maXTouch driver Memory allocated\n");

	incremental = false;

	object_info_address = MXT_ADDR_OBJECT_TABLE;

	report_id_count = 0;

	for (i = 0; i < mxt->device_info.num_objs; i++) {

		if (debug >= DEBUG_TRACE)
			printk(KERN_INFO "Reading maXTouch at [0x%04x]: ", object_info_address);

		error = mxt_read_block(client, object_info_address, MXT_OBJECT_TABLE_ELEMENT_SIZE, buf);

		if (error < 0) {
			mxt->read_fail_counter++;
			dev_err(&client->dev, "maXTouch Object %d could not be read\n", i);
			return -EIO;
		}

		object_type		=  buf[0];
		object_address		= (buf[2] << 8) + buf[1];
		object_size		=  buf[3] + 1;
		object_instances	=  buf[4] + 1;
		object_report_ids	=  buf[5];

		if (debug >= DEBUG_TRACE)
			printk(KERN_INFO "Type=%03d, Address=0x%04x, "
				"Size=0x%02x, %d instances, %d report id's\n",
				object_type,
				object_address,
				object_size,
				object_instances,
				object_report_ids
		);

		if (object_type > MXT_MAX_OBJECT_TYPES) {

			dev_err(&client->dev, "maXTouch object type [%d] not recognized\n", object_type);
			return -ENXIO;

		}
		else if (object_table[object_type].chip_addr != 0) {

			if ((object_type == 2) || (object_type == 3)) {
				incremental = true;
			} else {

				printk("Multiple maXTouch objects of the same type [%d]\n",object_type);

			}
		}

		if (object_type == MXT_GEN_MESSAGEPROCESSOR_T5) {
			mxt->msg_proc_addr = object_address;
			mxt->message_size = 7;
		}

		if (incremental) {
			object_table[object_type].size	+= object_size;
			incremental = false;
		}
		else{

			object_table[object_type].info_addr = object_info_address;
			object_table[object_type].index	= i;
			object_order[i] = object_type;
			object_table[object_type].type		= object_type;
			object_table[object_type].chip_addr	= object_address;
			object_table[object_type].size		= object_size;
			object_table[object_type].instances	= object_instances;
			object_table[object_type].num_report_ids	= object_report_ids;
			object_table[object_type].first_report_id = report_id_count + 1;
			report_id_count += object_instances * object_report_ids;

			if (debug >= DEBUG_TRACE){
				printk("object_table[%d] : type = %d, chip_addr = %04x, size = %d, instances = %d, num_report_ids = %d\n",
					object_type ,
					object_table[object_type].type ,
					object_table[object_type].chip_addr,
					object_table[object_type].size,
					object_table[object_type].instances,
					object_table[object_type].num_report_ids);
			}
		}

		object_info_address += MXT_OBJECT_TABLE_ELEMENT_SIZE;
	}

	mxt->rid_map =
		kzalloc(sizeof(struct report_id_map) * (report_id_count + 1), GFP_KERNEL);

	if (mxt->rid_map == NULL) {
		printk(KERN_WARNING "maXTouch: Memory allocation failed in %s,#%d!\n",__FUNCTION__, __LINE__);
		return -ENOMEM;
	}

	mxt->last_message = kzalloc(mxt->message_size, GFP_KERNEL);
	if (mxt->last_message == NULL) {
		printk(KERN_WARNING "maXTouch: Memory allocation failed in %s,#%d!\n",__FUNCTION__, __LINE__);
		return -ENOMEM;
	}

	mxt->report_id_count = report_id_count;
	if (report_id_count > 254) {
			dev_err(&client->dev, "Too many maXTouch report id's [%d]\n",	report_id_count);
			return -ENXIO;
	}

	report_id = 1;

	for (i = 0; i < mxt->device_info.num_objs; i++) {

		object_type	= object_order[i];
		object_address	= object_table[object_type].chip_addr;
		object_size	= object_table[object_type].size;

		for (object_instance = 0; object_instance < object_table[object_type].instances; object_instance++){

			first_report_id = report_id;

			for (object_report_id = 0; object_report_id < object_table[object_type].num_report_ids; object_report_id++) {

				mxt->rid_map[report_id].object = object_type;
				mxt->rid_map[report_id].instance = object_instance;
				mxt->rid_map[report_id].first_rid = first_report_id;
				report_id++;
			}
		}
	}

	error = mxt_read_block(client, object_info_address, 3, buf);
	if (error < 0) {
		mxt->read_fail_counter++;
		dev_err(&client->dev, "Error reading CRC\n");
	}

	crc = (buf[2] << 16) | (buf[1] << 8) | buf[0];

	calculate_infoblock_crc(&crc_calculated, mxt);

	if (debug >= DEBUG_TRACE) {
		printk(KERN_INFO "Reported info block CRC = 0x%6X\n\n", crc);
		printk(KERN_INFO "Calculated info block CRC = 0x%6X\n\n", crc_calculated);
	}

	if (crc == crc_calculated) {
		mxt->info_block_crc = crc;
	}
	else {
		mxt->info_block_crc = 0;
		printk(KERN_WARNING "maXTouch: info block CRC invalid!\n");
	}

	mxt->delta	= NULL;
	mxt->reference	= NULL;
	mxt->cte	= NULL;

	if (debug >= DEBUG_VERBOSE) {

		dev_info(&client->dev, "maXTouch: %d Objects\n", mxt->device_info.num_objs);

		for (i = 0; i < mxt->device_info.num_objs; i++) {

			dev_info(&client->dev, "Type:\t\t\t[%d]: %s\n",
				object_table[i].type,
				object_type_name[object_table[i].type]);

			dev_info(&client->dev, "\tAddress:\t0x%04X\n",
				object_table[i].chip_addr);

			dev_info(&client->dev, "\tSize:\t\t%d Bytes\n",
				object_table[i].size);

			dev_info(&client->dev, "\tInstances:\t%d\n",
				object_table[i].instances);

			dev_info(&client->dev, "\tReport Id's:\t%d\n",
				object_table[i].num_report_ids);
		}
	}

	printk("mxt_read_object_table -\n");

	return 0;
}

int mXT1386_i2c_write_firmware(struct i2c_client *client, u16 *read_val, unsigned int len)
{

	struct i2c_msg wmsg;
	unsigned char data[I2C_MAX_SEND_LENGTH];
	int ret,i;

	if(len > I2C_MAX_SEND_LENGTH){

		printk("[TSP][ERROR] %s() data length error\n", __FUNCTION__);
		return -ENODEV;
	}

	wmsg.addr = client->addr;
	wmsg.flags = 0;
	wmsg.len = len;
	wmsg.buf = data;

	for (i = 0; i < len; i++){

		data[i] = *(read_val+i);
	}

	ret = i2c_transfer(client->adapter, &wmsg, 1);

	if (ret == 1)
		return WRITE_MEM_OK;

	else
		return WRITE_MEM_FAILED;

}

int mXT1386_i2c_read_firmware(struct i2c_client *client, u8 *read_val, unsigned int len)
{

	struct i2c_msg wmsg;

	int ret;

	wmsg.addr = client->addr;
	wmsg.flags = I2C_M_RD;
	wmsg.len = len;
	wmsg.buf = (u8 *)read_val;

	ret = i2c_transfer(client->adapter, &wmsg, 1);

	if (ret == 1)
		return READ_MEM_OK;
	else
		return READ_MEM_FAILED;

}

u8 boot_read_mem(struct i2c_client *client, u16 start, u8 size, u8 *mem)
{
	struct i2c_msg rmsg;
	int ret;

	rmsg.addr = I2C_BOOT_ADDR_0;
	rmsg.flags = I2C_M_RD;
	rmsg.len = size;
	rmsg.buf = mem;

	ret = i2c_transfer(client->adapter, &rmsg, 1);

	if (ret == 1)
		return READ_MEM_OK;
	else
		return READ_MEM_FAILED;

}

int mXT1386_i2c_write(struct i2c_client *client, u16 reg, u8 *read_val, unsigned int len)
{

	struct i2c_msg wmsg;
	unsigned char data[I2C_MAX_SEND_LENGTH];
	int ret,i;

	address_pointer = reg;

	if(len+2 > I2C_MAX_SEND_LENGTH){

		printk("[ERROR] %s() data length error\n", __FUNCTION__);
		return -ENODEV;
	}

	wmsg.addr = client->addr;
	wmsg.flags = 0;
	wmsg.len = len + 2;
	wmsg.buf = data;

	data[0] = reg & 0x00ff;
	data[1] = reg >> 8;

	for (i = 0; i < len; i++){

		data[i+2] = *(read_val+i);
	}

	ret = i2c_transfer(client->adapter, &wmsg, 1);

	return ret;
}

u8 write_power_config(struct i2c_client *client, struct mxt_data *mxt)
{
	u16	object_address;
	u8*	tmp;
	u8 	object_size;
	u16 ret;

	object_address = mxt->object_table[MXT_GEN_POWERCONFIG_T7].chip_addr;
	object_size = mxt->object_table[MXT_GEN_POWERCONFIG_T7].size;

	printk("write_power_config -- Object addr : 0x%04x,  Object_size : %d\n", object_address, object_size);

	tmp = (uint8_t *) kmalloc(object_size, GFP_KERNEL);

	if (tmp == NULL) {
		printk(KERN_WARNING "maXTouch: Memory allocation failed in %s ,#%d!\n", __FUNCTION__, __LINE__);
		return -ENOMEM;
	}

	*(tmp + 0) = MXT_T7_IDLEACQINT_INIT;
	*(tmp + 1) = MXT_T7_ACTVACQINT_INIT;
	*(tmp + 2) = MXT_T7_ACTV2IDLETO_INIT;

	ret = mXT1386_i2c_write(client, object_address, tmp, object_size);

	kfree(tmp);

	return ret;
}

u8 write_acquisition_config(struct i2c_client *client, struct mxt_data *mxt)
{
	u16	object_address;
	u8*	tmp;
	u8 	object_size;
	u16 ret;

	object_address = mxt->object_table[MXT_GEN_ACQUIRECONFIG_T8].chip_addr;
	object_size = mxt->object_table[MXT_GEN_ACQUIRECONFIG_T8].size;

	printk("write_acquisition_config -- Object addr : 0x%04x,  Object_size : %d\n", object_address, object_size);

	tmp = (uint8_t *) kmalloc(object_size, GFP_KERNEL);

	if (tmp == NULL) {
		printk(KERN_WARNING "maXTouch: Memory allocation failed in %s ,#%d!\n", __FUNCTION__, __LINE__);
		return -ENOMEM;
	}

	*(tmp + 0) = MXT_T8_CHARGTIME_INIT;
	*(tmp + 1) = MXT_T8_RESERVED_INIT;
	*(tmp + 2) = MXT_T8_TCHDRIFT_INIT;
	*(tmp + 3) = MXT_T8_DRIFTST_INIT;
	*(tmp + 4) = MXT_T8_TCHAUTOCAL_INIT;
	*(tmp + 5) = MXT_T8_RESERVED_INIT;
	*(tmp + 6) = MXT_T8_ATCHCALST_INIT;
	*(tmp + 7) = MXT_T8_ATCHCALSTHR_INIT;
	*(tmp + 8) = MXT_T8_ATCHCALFRCTHR_INIT;
	*(tmp + 9) = MXT_T8_ATCHCALFRCRATIO_INIT;

	ret = mXT1386_i2c_write(client, object_address, tmp, object_size);

	kfree(tmp);

	return ret;

}

u8 write_multitouchscreen_config(struct i2c_client *client, struct mxt_data *mxt)
{
	u16	object_address;
	u8*	tmp;
	u8	object_size;
	u16 ret;

	object_address = mxt->object_table[MXT_TOUCH_MULTITOUCHSCREEN_T9].chip_addr;
	object_size = mxt->object_table[MXT_TOUCH_MULTITOUCHSCREEN_T9].size;

	printk("write_multitouchscreen_config -- Object addr : 0x%04x,  Object_size : %d\n", object_address, object_size);

	tmp = (uint8_t *) kmalloc(object_size, GFP_KERNEL);

	if (tmp == NULL) {
		printk(KERN_WARNING "maXTouch: Memory allocation failed in %s ,#%d!\n", __FUNCTION__, __LINE__);
		return -ENOMEM;
	}

	*(tmp + 0) = MXT_T9_CTRL_INIT;
	*(tmp + 1) = MXT_T9_XORIGIN_INIT;
	*(tmp + 2) = MXT_T9_YORIGIN_INIT;
	*(tmp + 3) = MXT_T9_XSIZE_INIT;
	*(tmp + 4) = MXT_T9_YSIZE_INIT;
	*(tmp + 5) = MXT_T9_AKSCFG_INIT;
	*(tmp + 6) = MXT_T9_BLEN_INIT;
	*(tmp + 7) = MXT_T9_TCHTHR_INIT;
	*(tmp + 8) = MXT_T9_TCHDI_INIT;

	if (get_hw_rev() >= REV_G)
		*(tmp + 9) = MXT_T9_ORIENT_INVERT;
	else
		*(tmp + 9) = MXT_T9_ORIENT_INIT;

	*(tmp + 10) = MXT_T9_MRGTIMEOUT_INIT;
	*(tmp + 11) = MXT_T9_MOVHYSTI_INIT;
	*(tmp + 12) = MXT_T9_MOVHYSTN_INIT;
	*(tmp + 13) = MXT_T9_MOVFILTER_INIT;
	*(tmp + 14) = MXT_T9_NUMTOUCH_INIT;
	*(tmp + 15) = MXT_T9_MRGHYST_INIT;
	*(tmp + 16) = MXT_T9_MRGTHR_INIT;
	*(tmp + 17) = MXT_T9_AMPHYST_INIT;

	*(tmp + 18) = (uint8_t) (MXT_T9_XRANGE_INIT &  0xFF);
	*(tmp + 19) = (uint8_t) (MXT_T9_XRANGE_INIT >> 8);

	*(tmp + 20) = (uint8_t) (MXT_T9_YRANGE_INIT  &  0xFF);
	*(tmp + 21) = (uint8_t) (MXT_T9_YRANGE_INIT  >> 8);

	*(tmp + 22) = MXT_T9_XLOCLIP_INIT;
	*(tmp + 23) = MXT_T9_XHICLIP_INIT;
	*(tmp + 24) = MXT_T9_YLOCLIP_INIT;
	*(tmp + 25) = MXT_T9_YHICLIP_INIT;
	*(tmp + 26) = MXT_T9_XEDGECTRL_INIT;
	*(tmp + 27) = MXT_T9_XEDGEDIST_INIT;
	*(tmp + 28) = MXT_T9_YEDGECTRL_INIT;
	*(tmp + 29) = MXT_T9_YEDGEDIST_INIT;
	*(tmp + 30) = MXT_T9_JUMPLIMIT_INIT;
	*(tmp + 31) = MXT_T9_TCHHYST_INIT;
	*(tmp + 32) = MXT_T9_XPITCH_INIT;
	*(tmp + 33) = MXT_T9_YPITCH_INIT;

	ret = mXT1386_i2c_write(client, object_address, tmp, object_size);

	kfree(tmp);

	return ret;

}

u8 write_communications_config(struct i2c_client *client, struct mxt_data *mxt)
{
	u16	object_address;
	u8*	tmp;
	u8 	object_size;
	u16 ret;

	object_address = mxt->object_table[MXT_SPT_COMMSCONFIG_T18].chip_addr;
	object_size = mxt->object_table[MXT_SPT_COMMSCONFIG_T18].size;

	printk("write_communications_config -- Object addr : 0x%04x,  Object_size : %d\n", object_address, object_size);

	tmp = (uint8_t *) kmalloc(object_size, GFP_KERNEL);

	if (tmp == NULL) {
		printk(KERN_WARNING "maXTouch: Memory allocation failed in %s ,#%d!\n", __FUNCTION__, __LINE__);
		return -ENOMEM;
	}

	*(tmp + 0) = MXT_T18_CTRL_INIT;
	*(tmp + 1) = MXT_T18_CMD_INIT;

	ret = mXT1386_i2c_write(client, object_address, tmp, object_size);

	kfree(tmp);

	return ret;

}

u8 write_noisesupression_config(struct i2c_client *client, struct mxt_data *mxt)
{
	u16	object_address;
	u8*	tmp;
	u8	object_size;
	u16 ret;

	object_address = mxt->object_table[MXT_PROCG_NOISESUPPRESSION_T22].chip_addr;
	object_size = mxt->object_table[MXT_PROCG_NOISESUPPRESSION_T22].size;

	printk("write_noisesupression_config -- Object addr : 0x%04x,  Object_size : %d\n", object_address, object_size);

	tmp = (uint8_t *) kmalloc(object_size, GFP_KERNEL);

	if (tmp == NULL) {
		printk(KERN_WARNING "maXTouch: Memory allocation failed in %s ,#%d!\n", __FUNCTION__, __LINE__);
		return -ENOMEM;
	}

	*(tmp + 0) = MXT_T22_CTRL_INIT;
	*(tmp + 1) = MXT_T22_RESERVED_INIT;
	*(tmp + 2) = MXT_T22_RESERVED_INIT;
	*(tmp + 3) = MXT_T22_RESERVED_INIT;
	*(tmp + 4) = MXT_T22_RESERVED_INIT;
	*(tmp + 5) = MXT_T22_RESERVED_INIT;
	*(tmp + 6) = MXT_T22_RESERVED_INIT;
	*(tmp + 7) = MXT_T22_RESERVED_INIT;
	*(tmp + 8) = MXT_T22_NOISETHR_INIT;
	*(tmp + 9) = MXT_T22_RESERVED_INIT;
	*(tmp + 10) = MXT_T22_FREQHOPSCALE_INIT;
	*(tmp + 11) = MXT_T22_FREQ_0_INIT;
	*(tmp + 12) = MXT_T22_FREQ_1_INIT;
	*(tmp + 13) = MXT_T22_FREQ_2_INIT;
	*(tmp + 14) = MXT_T22_FREQ_3_INIT;
	*(tmp + 15) = MXT_T22_FREQ_4_INIT;
	*(tmp + 16) = MXT_T22_RESERVED_INIT;

	ret = mXT1386_i2c_write(client, object_address, tmp, object_size);

	kfree(tmp);

	return ret;

}

u8 write_cte_config(struct i2c_client *client, struct mxt_data *mxt)
{
	u16	object_address;
	u8*	tmp;
	u8	object_size;
	u16 ret;

	object_address = mxt->object_table[MXT_SPT_CTECONFIG_T28].chip_addr;
	object_size = mxt->object_table[MXT_SPT_CTECONFIG_T28].size;

	printk("write_cte_config -- Object addr : 0x%04x,  Object_size : %d\n", object_address, object_size);

	tmp = (uint8_t *) kmalloc(object_size, GFP_KERNEL);

	if (tmp == NULL) {
		printk(KERN_WARNING "maXTouch: Memory allocation failed in %s ,#%d!\n", __FUNCTION__, __LINE__);
		return -ENOMEM;
	}

	*(tmp + 0) = MXT_T28_CTRL_INIT;
	*(tmp + 1) = MXT_T28_CMD_INIT;
	*(tmp + 2) = MXT_T28_RESERVED_INIT;
	*(tmp + 3) = MXT_T28_IDLEGCAFDEPTH_INIT;
	*(tmp + 4) = MXT_T28_ACTVGCAFDEPTH_INIT;
	*(tmp + 5) = MXT_T28_VOLTAGE_INIT;

	ret = mXT1386_i2c_write(client, object_address, tmp, object_size);

	kfree(tmp);

	return ret;

}

u8 write_gripsuppression_config(struct i2c_client *client, struct mxt_data *mxt)
{
	u16	object_address;
	u8*	tmp;
	u8	object_size;
	u16 ret;

	object_address = mxt->object_table[MXT_PROCI_GRIPSUPPRESSION_T40].chip_addr;
	object_size = mxt->object_table[MXT_PROCI_GRIPSUPPRESSION_T40].size;

	printk("write_gripsuppression_config -- Object addr : 0x%04x,  Object_size : %d\n", object_address, object_size);

	tmp = (uint8_t *) kmalloc(object_size, GFP_KERNEL);

	if (tmp == NULL) {
		printk(KERN_WARNING "maXTouch: Memory allocation failed in %s ,#%d!\n", __FUNCTION__, __LINE__);
		return -ENOMEM;
	}

	*(tmp + 0) = MXT_T40_CTRL_INIT;
	*(tmp + 1) = MXT_T40_XLOGRIP_INIT;
	*(tmp + 2) = MXT_T40_XHIGRIP_INIT;
	*(tmp + 3) = MXT_T40_YLOGRIP_INIT;
	*(tmp + 4) = MXT_T40_YHIGRIP_INIT;

	ret = mXT1386_i2c_write(client, object_address, tmp, object_size);

	kfree(tmp);

	return ret;

}

u8 write_palmsuppression_config(struct i2c_client *client, struct mxt_data *mxt)
{
	u16	object_address;
	u8*	tmp;
	u8	object_size;
	u16 ret;

	object_address = mxt->object_table[MXT_PROCI_PALMSUPPRESSION_T41].chip_addr;
	object_size = mxt->object_table[MXT_PROCI_PALMSUPPRESSION_T41].size;

	printk("write_palmsuppression_config -- Object addr : 0x%04x,  Object_size : %d\n", object_address, object_size);

	tmp = (uint8_t *) kmalloc(object_size, GFP_KERNEL);

	if (tmp == NULL) {
		printk(KERN_WARNING "maXTouch: Memory allocation failed in %s ,#%d!\n", __FUNCTION__, __LINE__);
		return -ENOMEM;
	}

	*(tmp + 0) = MXT_T41_CTRL_INIT;
	*(tmp + 1) = MXT_T41_RESERVED_0_INIT;
	*(tmp + 2) = MXT_T41_RESERVED_1_INIT;
	*(tmp + 3) = MXT_T41_LARGEOBJTHR_INIT;
	*(tmp + 4) = MXT_T41_DISTANCE_INIT;
	*(tmp + 5) = MXT_T41_SUPEXTTO_INIT;
	*(tmp + 6) = MXT_T41_STRENGTH_INIT;

	ret = mXT1386_i2c_write(client, object_address, tmp, object_size);

	kfree(tmp);

	return ret;

}

u8 write_backup_config(struct i2c_client *client, struct mxt_data *mxt)
{
	u16	object_address;
	u8*	tmp;
	u8	object_size;
	u16 ret;

	object_address = mxt->object_table[MXT_GEN_COMMANDPROCESSOR_T6].chip_addr + 1;
	object_size = 1;

	tmp = (uint8_t *) kmalloc(object_size, GFP_KERNEL);

	if (tmp == NULL) {
		printk(KERN_WARNING "maXTouch: Memory allocation failed in %s ,#%d!\n", __FUNCTION__, __LINE__);
		return -ENOMEM;
	}

	*(tmp + 0)  = 0x55;//back up

	ret = mXT1386_i2c_write(client, object_address, tmp, object_size);

	mdelay(150);

	kfree(tmp);

	return ret;

}

u8 write_config_data(struct i2c_client *client, struct mxt_data *mxt)
{

	printk("mxt_config_data +\n");

	if(write_power_config(client, mxt) < 0)
		printk("write_power_config error\n");
	if(write_acquisition_config(client, mxt) < 0)
		printk("write_acquisition_config error\n");
	if(write_multitouchscreen_config(client, mxt) < 0)
		printk("write_multitouchscreen_config error\n");
	if(write_communications_config(client, mxt) < 0)
		printk("write_communications_config error\n");
	if(write_noisesupression_config(client, mxt) < 0)
		printk("write_noisesupression_config error\n");
	if(write_cte_config(client, mxt) < 0)
		printk("write_cte_config error\n");
	if(write_gripsuppression_config(client, mxt) < 0)
		printk("write_gripsuppression_config error\n");
	if(write_palmsuppression_config(client, mxt) < 0)
		printk("write_palmsuppression_config error\n");
	if(write_backup_config(client, mxt) < 0)
		printk("write_backup_config error\n");

	printk("mxt_config_data -\n");

	return 0;

}


static void noise_filtering_level_change_worker(struct work_struct *work)
{

	printk("mxt_noise_filtering_level_change_worker\n");

	atomic_set( &mxt_first_boot_flag, 1);

}

static int __devinit mxt_probe(struct i2c_client *client,
				const struct i2c_device_id *id)
{
	struct mxt_data	*mxt;
	struct input_dev	*input;

	int error = 0;
	int count = 0;

	u8	i,j;
	u8	message[10];
	u8	identify_flag;
	u16	message_length=9;
	u32 pinValue;
	char fw_ver[10];
	u8 tmpbuf[9];

	struct proc_dir_entry *touch_firmware_version_proc_fp = NULL;

	printk("##### mXT1386 : Probe Start !!! #####\n");
	atomic_set( &mxt_first_boot_flag, 0);

	if (get_hw_rev() == REV_C){

		TOUCH_INT = TEGRA_GPIO_PB4;
		TOUCH_MAIN_PWR = TEGRA_GPIO_PK4;
		TOUCH_IO_PWR = TEGRA_GPIO_PK3;
		TOUCH_RESET = TEGRA_GPIO_PD1;
	}
	else if (get_hw_rev() == REV_E)
	{
		TOUCH_INT = TEGRA_GPIO_PB4;
		TOUCH_MAIN_PWR = TEGRA_GPIO_PK4;
		TOUCH_IO_PWR = TEGRA_GPIO_PP3;
		TOUCH_RESET = TEGRA_GPIO_PD1;
	}
	else{
		TOUCH_INT = TEGRA_GPIO_PW0;
		TOUCH_MAIN_PWR = TEGRA_GPIO_PK4;
		TOUCH_IO_PWR = TEGRA_GPIO_PP3;
		TOUCH_RESET = TEGRA_GPIO_PZ3;
	}

		gpio_request(TOUCH_INT, "touch_int_n");
		tegra_gpio_enable(TOUCH_INT);
		gpio_direction_input(TOUCH_INT);

		gpio_request(TOUCH_RESET, "touch_reset");
		tegra_gpio_enable(TOUCH_RESET);
		gpio_direction_output(TOUCH_RESET,0);

		gpio_request(TOUCH_MAIN_PWR, "touch_main_pwr");
		tegra_gpio_enable(TOUCH_MAIN_PWR);
		gpio_direction_output(TOUCH_MAIN_PWR,0);

		gpio_request(TOUCH_IO_PWR, "touch_io_pwr");
		tegra_gpio_enable(TOUCH_IO_PWR);
		gpio_direction_output(TOUCH_IO_PWR,0);

		mdelay(5);
		gpio_set_value(TOUCH_MAIN_PWR,1);
		mdelay(10);
		gpio_set_value(TOUCH_IO_PWR,1);
		gpio_set_value(TOUCH_RESET,1);
		mdelay(200);

		do{
			pinValue = gpio_get_value(TOUCH_INT);

			if(debug >= DEBUG_INFO)
				printk("Touch INT sitill High in %s\n",__FUNCTION__);

			count++;

			if(count > 500){
				without_touch_moudle = 1;
				break;
			}

			mdelay(3);

		}while(pinValue == 1);

		if(without_touch_moudle == 0){
			count = 0;
			do {
				mxt_read_block(client, 249, 7,tmpbuf);

				printk("message: ");

				for (j = 0; j < 9; j++)
					printk("%d ", tmpbuf[j]);

				printk("\n");

				count++;

				if(count > 100){
					printk("mXT : Initial Message Read Fail in %s, %d!!!\n",__FUNCTION__, __LINE__);
					break;
				}
			} while((tmpbuf[0]!=0xFF)&&(tmpbuf[0]!=0));
		}

	if(without_touch_moudle == 1){
		printk("mXT1386 : Touch module connection check !!!\n");
		gpio_set_value(TOUCH_IO_PWR,0);
		gpio_set_value(TOUCH_RESET,0);
		mdelay(5);
		gpio_set_value(TOUCH_MAIN_PWR,0);
		error = -ENXIO;
		goto err_without_module;
	}

	mdelay(100);

	printk(KERN_INFO "mXT1386: mxt_probe\n");

	if (client == NULL)
		printk(KERN_INFO "maXTouch: client == NULL\n");
	else if (client->adapter == NULL)
		printk(KERN_INFO "maXTouch: client->adapter == NULL\n");
	else if (&client->dev == NULL)
		printk(KERN_INFO "maXTouch: client->dev == NULL\n");
	else if (&client->adapter->dev == NULL)
		printk(KERN_INFO "maXTouch: client->adapter->dev == NULL\n");
	else if (id == NULL)
		printk(KERN_INFO "maXTouch: id == NULL\n");
	else
		goto param_check_ok;
	return	-EINVAL;

param_check_ok:
	if (debug >= DEBUG_INFO) {
		printk(KERN_INFO "maXTouch driver\n");
		printk(KERN_INFO "\t \"%s\"\n",		client->name);
		printk(KERN_INFO "\taddr:\t0x%04x\n",	client->addr);
		printk(KERN_INFO "\tirq:\t%d\n",	client->irq);
		printk(KERN_INFO "\tflags:\t0x%04x\n",	client->flags);
		printk(KERN_INFO "\tadapter:\"%s\"\n",	client->adapter->name);
		printk(KERN_INFO "\tdevice:\t\"%s\"\n",	client->dev.init_name);
	}
	if (debug >= DEBUG_TRACE)
		printk(KERN_INFO "Parameters OK\n");

	mxt = kzalloc(sizeof(struct mxt_data), GFP_KERNEL);
	input = input_allocate_device();
	if (!mxt || !input){
		dev_err(&client->dev, "insufficient memory in %s,#%d\n",__FUNCTION__, __LINE__);
		error = -ENOMEM;
		goto err_free_mem;
	}

	mxt->read_fail_counter	= 0;
	mxt->message_counter	= 0;
	mxt->max_x_val		= 1279;
	mxt->max_y_val		= 767;

	if (debug >= DEBUG_TRACE)
		printk(KERN_INFO "maXTouch driver identifying chip\n");

	identify_flag = 0;

	if (mxt_identify(client, mxt) < 0){
		dev_err(&client->dev, "Chip could not be identified\n");
		identify_flag = 1;
	}

	if (debug >= DEBUG_TRACE)
		printk(KERN_INFO "maXTouch driver allocating input device\n");

	mxt->client = client;
	mxt->input  = input;

	INIT_DELAYED_WORK(&mxt->dwork, mxt_worker);
	if (debug >= DEBUG_TRACE)
		printk(KERN_INFO "maXTouch driver init spinlock\n");

	init_waitqueue_head(&mxt->msg_queue);
	init_MUTEX(&mxt->msg_sem);

	spin_lock_init(&mxt->lock);

	if (debug >= DEBUG_TRACE)
		printk(KERN_INFO "maXTouch driver creating device name\n");

	snprintf(
		mxt->phys_name,
		sizeof(mxt->phys_name),
		"%s/input0",
		dev_name(&client->dev)
	);
	input->name = "atmel-maxtouch";
	input->phys = mxt->phys_name;
	input->id.bustype = BUS_I2C;
	input->dev.parent = &client->dev;

	if (debug >= DEBUG_INFO) {
		printk(KERN_INFO "maXTouch name: \"%s\"\n", input->name);
		printk(KERN_INFO "maXTouch phys: \"%s\"\n", input->phys);
		printk(KERN_INFO "maXTouch driver setting abs parameters\n");
	}

	__set_bit(EV_ABS, input->evbit);

	input_set_abs_params(input, ABS_X, 0, mxt->max_x_val, 0, 0);
	input_set_abs_params(input, ABS_Y, 0, mxt->max_y_val, 0, 0);
	input_set_abs_params(input, ABS_MT_PRESSURE, 0, 255, 0, 0);
	input_set_abs_params(input, ABS_TOOL_WIDTH, 0, MXT_MAX_REPORTED_WIDTH, 0, 0);

	input_set_abs_params(input, ABS_MT_POSITION_X, 0, mxt->max_x_val, 0, 0);
	input_set_abs_params(input, ABS_MT_POSITION_Y, 0, mxt->max_y_val, 0, 0);
	input_set_abs_params(input, ABS_MT_TOUCH_MAJOR, 0, 255, 0, 0);
	input_set_abs_params(input, ABS_MT_WIDTH_MAJOR, 0, 30, 0, 0);
	input_set_abs_params(input, ABS_MT_TRACKING_ID, 0, MXT_MAX_NUM_TOUCHES, 0, 0);

	if (debug >= DEBUG_TRACE)
		printk(KERN_INFO "maXTouch driver setting client data\n");

	i2c_set_clientdata(client, mxt);
	if (debug >= DEBUG_TRACE)
		printk(KERN_INFO "maXTouch driver setting drv data\n");

	input_set_drvdata(input, mxt);

	if (debug >= DEBUG_TRACE)
		printk(KERN_INFO "maXTouch driver input register device\n");

	error = input_register_device(mxt->input);
	if (error < 0) {
		dev_err(&client->dev,
			"Failed to register input device\n");
		goto err_free_irq;
	}
	if (debug >= DEBUG_TRACE)
		printk(KERN_INFO "maXTouch driver allocate interrupt\n");

	mxt->irq = client->irq;
	mxt->valid_irq_counter = 0;
	mxt->invalid_irq_counter = 0;
	mxt->irq_counter = 0;

	if(firmware_update(client, mxt)== FIRMWARE_UP_SUCCESS){
		client->addr = I2C_APPL_ADDR_0;
		identify_flag = 1;

		if(mxt_reset(client)){
			printk("maxTouch : Reset Failure in %d ,%s !!!!\n",__LINE__, __FUNCTION__);
			goto err_free_irq;
		}
	}

	if(identify_flag == 1){
		if (mxt_identify(client, mxt) < 0){
			dev_err(&client->dev, "Chip could not be identified\n");
			return -ENODEV;
		}
	}

	switch(mxt->device_info.major){
		case 0:

			switch(mxt->device_info.minor){
				case 8:
					strlcpy(fw_ver, star_touch_fw_rev_value[0].fw_ver_name, 8);
					break;
				case 9:
					strlcpy(fw_ver, star_touch_fw_rev_value[1].fw_ver_name, 8);
					break;
				default:
					printk("mXT1386 : touch ic firmware AT_Comm write fail !! \n");
					strlcpy(fw_ver, star_touch_fw_rev_value[5].fw_ver_name, 8);
					break;
			}

			break;

		case 1:

			switch(mxt->device_info.minor){
				case 0:
					strlcpy(fw_ver, star_touch_fw_rev_value[2].fw_ver_name, 8);
					break;
				case 1:
					if(mxt->device_info.build == 0xAA)
						strlcpy(fw_ver, star_touch_fw_rev_value[4].fw_ver_name, 8);
					else
						strlcpy(fw_ver, star_touch_fw_rev_value[3].fw_ver_name, 8);
					break;
				case 2:
					strlcpy(fw_ver, star_touch_fw_rev_value[5].fw_ver_name, 8);
					break;
				default:
					printk("mXT1386 : touch ic firmware AT_Comm write fail !! \n");
					strlcpy(fw_ver, star_touch_fw_rev_value[6].fw_ver_name, 8);
					break;
			}

			break;

		default:
			printk("mXT1386 : touch ic firmware AT_Comm write fail !! \n");
			strlcpy(fw_ver, star_touch_fw_rev_value[5].fw_ver_name, sizeof(star_touch_fw_rev_value[5].fw_ver_name));
			break;
	}

	touch_firmware_version_proc_fp = proc_mkdir("touchfwver", 0);

	create_proc_entry(fw_ver, S_IFREG | S_IRWXU, touch_firmware_version_proc_fp);

	error = mxt_read_object_table(client, mxt);

	if (error < 0){
		printk("mxt_read_object_table ERROR\n");
		goto err_free_irq;
	}

	write_config_data(client, mxt);

	printk("after mxt_read_object_table \n");
	for(i=0;i<10;i++){
		error = mxt_read_block(client,
					   155,
					   message_length,
					   message);
	}
	printk("after mxt_read_block \n");

	if (error < 0) {
		dev_err(&client->dev, "failed to allocate irq %d\n", mxt->irq);
		goto err_free_device;
	}

	if (debug > DEBUG_INFO)
		dev_info(&client->dev, "touchscreen, irq %d\n", mxt->irq);

	if (error)
		goto err_free_irq;

	pinValue = gpio_get_value(TOUCH_INT);
	printk(" touch interrupt1 =%d \n",pinValue);

	mxt->irq = gpio_to_irq(TOUCH_INT);
	client->irq = gpio_to_irq(TOUCH_INT);
	error = request_irq(gpio_to_irq(TOUCH_INT),mxt_irq_handler,IRQF_TRIGGER_LOW,"touch_int_n", mxt);

	pinValue = gpio_get_value(TOUCH_INT);
	printk(" touch interrupt2 =%d \n",pinValue);

#ifdef CONFIG_HAS_EARLYSUSPEND
	mxt->early_suspend.level =  EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 17;
	mxt->early_suspend.suspend = mxt_early_suspend;
	mxt->early_suspend.resume = mxt_late_resume;
	register_early_suspend(&(mxt->early_suspend));
	printk("#### Touchscreen Early Suspend ON ##### \n");
#endif

	for(i = 0; i < MAX_USING_FINGER_NUM; i++){
		fingerinfo[i].size_id = 0;
		fingerinfo[i].pressure = -1;
		fingerinfo[i].x = 0;
		fingerinfo[i].y = 0;
	}

	INIT_DELAYED_WORK(&noise_change_work, noise_filtering_level_change_worker);

	schedule_delayed_work( &noise_change_work, 2000 );

	printk(KERN_WARNING "mxt probe ok\n");
	return 0;

err_free_irq:
	if (mxt->irq)
		free_irq(mxt->irq, mxt);
err_free_device:
	input_free_device(input);
err_free_mem:
	if (mxt != NULL) {
		kfree(mxt->rid_map);
		kfree(mxt->delta);
		kfree(mxt->reference);
		kfree(mxt->cte);
		kfree(mxt->object_table);
		kfree(mxt->last_message);
		mxt->exit_hw();
	}
	kfree(mxt);
	kfree(input);
err_without_module:
	return error;
}

static int __devexit mxt_remove(struct i2c_client *client)
{
	struct mxt_data *mxt;

	mxt = i2c_get_clientdata(client);

#ifdef CONFIG_HAS_EARLYSUSPEND
	unregister_early_suspend(&mxt->early_suspend);
#endif

	if (mxt->irq)
		free_irq(mxt->irq, mxt);

	cancel_delayed_work_sync(&mxt->dwork);

	input_unregister_device(mxt->input);

	if (mxt != NULL) {
		kfree(mxt->rid_map);
		kfree(mxt->delta);
		kfree(mxt->reference);
		kfree(mxt->cte);
		kfree(mxt->object_table);
		kfree(mxt->last_message);
	}

	kfree(mxt);

	i2c_set_clientdata(client, NULL);
	if (debug >= DEBUG_TRACE)
		dev_info(&client->dev, "Touchscreen unregistered\n");

	return 0;
}

static int mxt_suspend(struct i2c_client *client)
{
	mxt_suspend_flag = 1;

	printk("### mxt_suspend_start!!! ### \n");

	disable_irq(client->irq);

	gpio_set_value(TOUCH_IO_PWR,0x0);
	gpio_set_value(TOUCH_RESET,0x0);
	mdelay(10);
	gpio_set_value(TOUCH_MAIN_PWR,0x0);

	printk("### mxt_suspend_success!!! ### \n");
	return 0;
}

static int mxt_resume(struct i2c_client *client)
{

	printk("### mxt_resume_start!!! ###\n");

	if(mxt_reset(client)){
		printk("maxTouch : Reset Failure in %s !!!!\n",__FUNCTION__);
		enable_irq(client->irq);
		return 1;
	}

	enable_irq(client->irq);

	mxt_suspend_flag = 0;

	printk("### mxt_resume_success!!! ###\n");

	return 0;
}

static struct i2c_device_id mxt_idtable[] ={
	{"mXT1386", 0,},
	{ }
};

MODULE_DEVICE_TABLE(i2c, mxt_idtable);

static const struct dev_pm_ops mxt_pm_ops = {
	.suspend	= mxt_suspend,
	.resume	= mxt_resume,
};

static struct i2c_driver mxt_driver = {
	.driver = {
		.name	= "mXT1386",
		.owner  = THIS_MODULE,
		#ifndef CONFIG_HAS_EARLYSUSPEND
		.pm		= &mxt_pm_ops,
		#endif
	},

	.id_table	= mxt_idtable,
	.probe		= mxt_probe,
	.remove		= __devexit_p(mxt_remove),

};

static int __init mxt_init(void)
{
	int err;
	err = i2c_add_driver(&mxt_driver);

	if(without_touch_moudle)
		err = ENXIO;

	if (err) {
		printk(KERN_WARNING "Adding mXT1386 driver failed "
		       "(errno = %d)\n", err);
	} else {
		printk(KERN_INFO "Successfully added driver %s\n",
		       mxt_driver.driver.name);
	}
	return err;
}

static void __exit mxt_cleanup(void)
{
	i2c_del_driver(&mxt_driver);
}


bool mxt_mem_clear(struct i2c_client *client)
{
	u16 address;
	u8 *tmp;
	u16 object_size;

	address = 0;
	object_size = 0x200;

	tmp = (u8 *) kmalloc(object_size,GFP_KERNEL);

	memset(tmp,0,object_size);

	if(mXT1386_i2c_write(client, address, tmp, object_size) < 0){
		kfree(tmp);
		return WRITE_MEM_FAILED;
	}

	kfree(tmp);

	return WRITE_MEM_OK;
}

u8 boot_reset(struct i2c_client *client, struct mxt_data *mxt)
{

	u8 *tmp;
	u16 reg;

	printk("boot_reset +\n");

	tmp = (u8 *) kmalloc(1,GFP_KERNEL);

	if (tmp == NULL) {
		printk("maXTouch: Memory allocation failed in %s ,#%d!\n", __FUNCTION__, __LINE__);
		return -ENOMEM;
	}

	if(mxt->device_info.major == 0 && mxt->device_info.minor == 7)
		reg=0x00EF;
	else
		reg=0x0102;

	*(tmp + 0)  = 0xA5;

	if(mXT1386_i2c_write(client, reg, tmp, 1) < 0){
		printk("maXTouch: i2c_write failed in %s ,#%d!\n", __FUNCTION__, __LINE__);
		kfree(tmp);
		return WRITE_MEM_FAILED;
	}

	kfree(tmp);

	printk("boot_reset -\n");

	return WRITE_MEM_OK;
}

u8 boot_reset_using_toggling(struct i2c_client *client, struct mxt_data *mxt)
{
	u8 i;

	printk("boot_reset_using_toggling +\n");

	gpio_set_value(TOUCH_RESET,1);
	mdelay(50);

	for(i = 0;i < 10;i++){
		gpio_set_value(TOUCH_RESET,0);
		mdelay(5);
		gpio_set_value(TOUCH_RESET,1);
		mdelay(80);
	}
	mdelay(20);

	return WRITE_MEM_OK;
}

u8 boot_unlock(struct i2c_client *client)
{
	u16 data[2];
	u8 status;

	printk("boot_unlock +\n");

	data[0] = 0xDC;
	data[1] = 0xAA;

	status = mXT1386_i2c_write_firmware(client, data, 2);

	printk("boot_unlock -\n");

	return status;
}

u8 address_slave(struct i2c_client *client, u8* data)
{
	printk("address_slave +\n");

	if(client->addr == I2C_BOOT_ADDR_0){
		if((mXT1386_i2c_read_firmware(client, data, 2))!= READ_MEM_OK){
			return READ_MEM_FAILED;
		}
	}

	else{
		client->addr = I2C_BOOT_ADDR_0;
		if((mXT1386_i2c_read_firmware(client, data, 2))!= READ_MEM_OK){
			client->addr = I2C_APPL_ADDR_0;
			return READ_MEM_FAILED;
		}
	}
	printk("address_slave -\n");

	return CONNECT_OK;
}

u8 mxt_boot(struct i2c_client *client, struct mxt_data *mxt)
{
	u8 retry_cnt;
	u32 character_position = 0;
	u16 frame_size = 0;
	u16 next_frame = 0;
	u16 crc_error_count = 0;
	u16 size1,size2;
	u32 status;
	u8 boot_status[3];
	u32 f_size;
	u16  *firmware_data ;
	u16 i = 0, j = 0;
	u8 firm_write_end_flag = 0;
	u8 read_status_flag = 0;

	printk("mxt_boot +\n");

	firmware_data = mxt_firmware;

	f_size = sizeof(firmware_data);

	firmware_data +=2;

	status = 0;

	if((mxt->mxt_i2c_address == I2C_APPL_ADDR_0) || (mxt->mxt_i2c_address == I2C_APPL_ADDR_1)){

		//status = boot_reset(client, mxt);
		status = boot_reset_using_toggling(client, mxt);

		if(status == WRITE_MEM_OK){

			printk("Boot reset ok \r\n");

			if(mxt->mxt_i2c_address == I2C_APPL_ADDR_0){
			mxt->mxt_i2c_address = I2C_BOOT_ADDR_0;
			}
			else if(mxt->mxt_i2c_address == I2C_APPL_ADDR_1){
			mxt->mxt_i2c_address = I2C_BOOT_ADDR_1;
			}
		}

		else{
			return status;
		}

	mdelay(1000);
	}

	switch(status){

		case WRITE_MEM_OK:
			status = CONNECT_OK;
			client->addr = I2C_BOOT_ADDR_0;
			break;

		default :
			break;
	}

	if(status == CONNECT_OK){
		printk("Boot Update Start !!!!\n\r");
		printk("Boot I2C Address = 0x%x \r\n",client->addr);
	}
	else{
		return (WRITE_MEM_FAILED);
	}

	for(retry_cnt = 0; retry_cnt < 5 ; retry_cnt++){

			mdelay(60);

			if ((boot_read_mem(client, 0, 3, boot_status)) == READ_MEM_OK){
				printk("Bootloader Ver %d !!\r\n",boot_status[1]&0x3F);
				crc_error_count = 0;
				character_position = 0;
				next_frame = 0;

				while(1){

					for(j = 0; j<5; j++){
						if(boot_read_mem(client, 0, 1, boot_status) == READ_MEM_OK){
							read_status_flag = 1;
							break;
						}

						else
							read_status_flag = 0;
					}

					if(read_status_flag == 1){

						if((boot_status[0] & 0xC0) == MXT_WAITING_BOOTLOAD_COMMAND){

							if(firm_write_end_flag == 1)
								return 0;

							if(boot_unlock(client) == WRITE_MEM_OK){

								mdelay(10);

								printk(" Boot Unlock OK !!\r\n");

								firm_write_end_flag = 1;
							}
							else{
								printk(" %s - boot_unlock fail!!%d\n", __func__, __LINE__ );
							}
						}else if((boot_status[0] & 0xC0) == MXT_WAITING_FRAME_DATA){
								printk(" %s, sending packet %dth\n", __func__,i);
								size1 =  *(firmware_data+character_position);
								size2 =  *(firmware_data+character_position+1)+2;
								frame_size = (size1<<8) + size2;
								printk(" firmware_data : %p   charcter_position : %d  \n\r", firmware_data, character_position);
								printk(" size1 : %d   size2 : %d   frame size  : %d  \n\r", size1, size2, frame_size);
								printk(" boot write data(firmware_data+character_position) : %p\n", firmware_data+character_position);

								if(frame_size == 0){
									if(mxt->mxt_i2c_address == I2C_BOOT_ADDR_0)
									{
										mxt->mxt_i2c_address = I2C_APPL_ADDR_0;
									}
									else if(mxt->mxt_i2c_address == I2C_BOOT_ADDR_1)
									{
										mxt->mxt_i2c_address = I2C_APPL_ADDR_1;
									}
									printk("0 == frame_size\n");

									return FIRMWARE_OUT;
								}

								next_frame = 1;
								status = mXT1386_i2c_write_firmware(client, firmware_data + character_position, frame_size);
								i++;

								mdelay(20);

								if(status != WRITE_MEM_OK){
									printk(" boot_write_mem fail \n\r !!!!");
									next_frame = 0;
								}
						}

						else if(boot_status[0] == MXT_FRAME_CRC_CHECK){

							printk("  --> ");
						}

						else if(boot_status[0] == MXT_FRAME_CRC_PASS){
							if( next_frame == 1){

								printk("ok \n\r");
								character_position += frame_size;
								next_frame = 0;
								}
						}

						else if(boot_status[0]  == MXT_FRAME_CRC_FAIL){
							printk("CRC fail(%d) \n\r",crc_error_count);
							crc_error_count++;
							next_frame = 0;

							if(crc_error_count > 10){
								return WRITE_MEM_FAILED;
							}
						}

						else if((boot_status[0] & 0xC0) == MXT_APP_CRC_FAIL){
							printk(" Boot Application CRC Fail!! \r\n");
							crc_error_count++;
						}

						if(crc_error_count > 10){
							printk(" %s, crc fail %d times! firmware update fail!!!!\n", __func__, crc_error_count );
							return FIRMWARE_OUT;
						}

					}
					else
						return FIRMWARE_OUT;
				}
			}
	}

	printk("mxt_boot -\n");

	return FIRMWARE_UP_SUCCESS;

}

u8 firmware_update(struct i2c_client *client, struct mxt_data *mxt)
{
	u8 version, build;
	u16  * fptr;
	u8 status;
	u32 pinValue;

	printk("firmware_update +\n");

	mxt->mxt_i2c_address = I2C_BOOT_ADDR_0;

	status = mxt_boot(client, mxt);

	if( status == 0 ){

		do{
			pinValue = gpio_get_value(TOUCH_INT);
		}while(pinValue == 1);

		mxt->mxt_i2c_address = I2C_APPL_ADDR_0;

		goto done;

	}

	fptr = mxt_firmware;

	version = (mxt->device_info.major << 4) + mxt->device_info.minor;

	build = mxt->device_info.build;

	if((version != *fptr) ||(build != *(fptr+1))){

		mxt_mem_clear(client);

		mxt->mxt_i2c_address = I2C_APPL_ADDR_0;

		status = mxt_boot(client, mxt);

		do{
			pinValue = gpio_get_value(TOUCH_INT);
		}while(pinValue == 1);

		mxt->mxt_i2c_address = I2C_APPL_ADDR_0;
	}
	else{
		printk("firmware_update NEED_NOT_FIRM_UP -\n");

		return NEED_NOT_FIRM_UP;
	}

	done :
		printk("firmware_update FIRMWARE_UP_SUCCESS -\n");

	return FIRMWARE_UP_SUCCESS;

}

module_init(mxt_init);
module_exit(mxt_cleanup);

MODULE_AUTHOR("LGE_Software_Platform_Lab");
MODULE_DESCRIPTION("Driver for Atmel maXTouch Touchscreen Controller");
MODULE_LICENSE("GPL");
