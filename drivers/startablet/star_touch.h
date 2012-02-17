/* drivers/startablet/star_touch.h
 *
 * maXTouch TSP driver.
 *
 * Copyright (C) 2010 LG Electronics Co. Ltd.
 *
 */

#define		DEBUG_INFO					1
#define		DEBUG_VERBOSE					2
#define		DEBUG_MESSAGES					5
#define		DEBUG_RAW					8
#define		DEBUG_TRACE					10

typedef struct {
	int version;
	char* fw_ver_name;
} touch_fw_rev_info;

static touch_fw_rev_info star_touch_fw_rev_value[] = {
	{0x08, "0.8"},
	{0x09, "0.9"},
	{0x10, "1.0"},
	{0x11, "1.1.01"},
	{0x11, "1.1.AA"},
	{0x12, "1.2"},
	{0x00, "Unknown"},
};

struct mxt_device_info {
	u8	family_id;
	u8	variant_id;
	u8	major;
	u8	minor;
	u8	build;
	u8	num_objs;
	u8	x_size;
	u8	y_size;
	u8	family[16];
	u8	variant[16];
	u16	num_nodes;
};

struct mxt_object {
	u8	type;
	u16	chip_addr;
	u8	size;
	u8	instances;
	u8	num_report_ids;
	u16	info_addr;
	u16	first_report_id;
	u16	index;
};

struct report_id_map {
	u8  object;
	u8  instance;
	u8  first_rid;
};

struct delayed_work  noise_change_work;

struct mxt_data {
	struct i2c_client *client;
	struct input_dev  *input;
	char phys_name[32];
	int irq;

	u16 last_read_addr;
	bool new_msgs;
	u8 *last_message;

	wait_queue_head_t msg_queue;
	struct semaphore  msg_sem;

	int valid_irq_counter;
	int invalid_irq_counter;
	int irq_counter;
	int message_counter;
	int read_fail_counter;

	int bytes_to_read;

	spinlock_t           lock;
	struct delayed_work  dwork;
	//struct delayed_work  cwork;
	u8 xpos_format;
	u8 ypos_format;

	u8 numtouch;

	struct mxt_device_info	device_info;

	u32 info_block_crc;
	u32 configuration_crc;
	u16 report_id_count;
	struct report_id_map *rid_map;
	struct mxt_object *object_table;

	u16 msg_proc_addr;
	u8 message_size;

	s16 *delta;
	u16 *reference;
	u16 *cte;

	u16 max_x_val;
	u16 max_y_val;

	void (*init_hw)(void);
	void (*exit_hw)(void);

	u8 mxt_i2c_address;

	struct early_suspend    early_suspend;

};

const u8 *maxtouch_family = "maXTouch";
const u8 *mxt1386_variant  = "mXT1386";

static u16 address_pointer;

#define		MAX_USING_FINGER_NUM 10

typedef struct{
	u16 size_id;
	int pressure;
	int x;
	int y;
} report_finger_info_t;

static report_finger_info_t fingerinfo[MAX_USING_FINGER_NUM];
static u8 fingerinfo_send = 0;

#define		MAXTOUCH_FAMILYID				0x80
#define		MXT1386_CAL_VARIANTID				0x01
#define		MXT1386_UNCAL_VARIANTID				0x00

#define		MXT_MAX_REPORTED_WIDTH				255
#define		MXT_MAX_REPORTED_PRESSURE			255

#define		MXT_MAX_TOUCH_SIZE				255
#define		MXT_MAX_NUM_TOUCHES				10

#define		I2C_RETRY_COUNT 				10
#define		INIT_RETRY_COUNT 				3

#define		mxt_valid_interrupt()				1

#define		I2C_PAYLOAD_SIZE 				254

#define		READ_MEM_OK					0
#define		READ_MEM_FAILED					1
#define		WRITE_MEM_FAILED				2
#define		WRITE_MEM_OK					3
#define		CONNECT_OK					4
#define		FIRMWARE_UP_SUCCESS				5
#define		FIRMWARE_OUT					6
#define		NEED_NOT_FIRM_UP				7

#define		I2C_APPL_ADDR_0 				0x5B
#define		I2C_APPL_ADDR_1 				0x5B
#define		I2C_BOOT_ADDR_0					0x35
#define		I2C_BOOT_ADDR_1					0x6A

#define		MXT_WAITING_BOOTLOAD_COMMAND			0xC0
#define		MXT_APP_CRC_FAIL 				0x03
#define		MXT_WAITING_FRAME_DATA				0X80
#define		MXT_FRAME_CRC_CHECK				0x02
#define		MXT_FRAME_CRC_PASS				0x04
#define		MXT_FRAME_CRC_FAIL				0x03

#define		I2C_MAX_SEND_LENGTH				533

#define		MXT_ADDR_INFO_BLOCK				0
#define		MXT_ADDR_OBJECT_TABLE				7
#define		MXT_ID_BLOCK_SIZE				7
#define		MXT_OBJECT_TABLE_ELEMENT_SIZE			6

#define		MXT_MAX_OBJECT_TYPES				100
#define		MXT_END_OF_MESSAGES				0xFF

#define		MXT_MSGB_T6_COMSERR				0x04
#define		MXT_MSGB_T6_CFGERR				0x08
#define		MXT_MSGB_T6_CAL					0x10
#define		MXT_MSGB_T6_SIGERR				0x20
#define		MXT_MSGB_T6_OFL					0x40
#define		MXT_MSGB_T6_RESET				0x80

int mxt_read__block(struct i2c_client *client, __u16 addr, __u16 length, __u8 *value);
int mxt_write_block(struct i2c_client *client, __u16 addr, __u16 length, __u8 *value);
bool mxt_mem_clear(struct i2c_client *client);
u8 boot_reset(struct i2c_client *client, struct mxt_data *mxt);
u8 boot_reset_reset_toggling(struct i2c_client *client, struct mxt_data *mxt);
u8 boot_unlock(struct i2c_client *client);
u8 address_slave(struct i2c_client *client, u8* data);
u8 mxt_boot(struct i2c_client *client, struct mxt_data *mxt);
u8 firmware_update(struct i2c_client *client, struct mxt_data *mxt);
static int mxt_suspend(struct i2c_client *client);
static int mxt_resume(struct i2c_client *client);
u16 get_object_address(uint8_t object_type, uint8_t instance, struct mxt_object *object_table, int max_objs);
int mXT1386_i2c_write(struct i2c_client *client, u16 reg, u8 *read_val, unsigned int len);

#define		MXT_BASE_ADDR(object_type) \
get_object_address(object_type, 0, mxt->object_table, mxt->device_info.num_objs)

#define		REPORT_ID_TO_OBJECT(rid) \
(((rid) == 0xff) ? 0 : mxt->rid_map[rid].object)

#define	T6_REG(x) (MXT_BASE_ADDR(MXT_GEN_COMMANDPROCESSOR_T6) + (x))
#define	T37_REG(x) (MXT_BASE_ADDR(MXT_DEBUG_DIAGNOSTICS_T37) +  (x))

#define REPORT_MT(touch_number, x, y, amplitude, size, key)         \
do {                                                                \
   input_report_abs(mxt->input, ABS_MT_TRACKING_ID, touch_number);  \
   input_report_abs(mxt->input, ABS_MT_POSITION_X, x);              \
   input_report_abs(mxt->input, ABS_MT_POSITION_Y, y);              \
   input_report_abs(mxt->input, ABS_MT_TOUCH_MAJOR, amplitude);     \
   input_report_abs(mxt->input, ABS_MT_WIDTH_MAJOR, size);          \
   input_report_abs(mxt->input, ABS_MT_PRESSURE, amplitude);        \
   input_report_key(mxt->input, BTN_TOUCH, key); 					\
   input_mt_sync(mxt->input);                                       \
} while (0)

#define		MXT_GEN_MESSAGEPROCESSOR_T5			5
#define		MXT_GEN_COMMANDPROCESSOR_T6			6
#define		MXT_GEN_POWERCONFIG_T7				7
#define		MXT_GEN_ACQUIRECONFIG_T8			8
#define		MXT_TOUCH_MULTITOUCHSCREEN_T9			9
#define		MXT_TOUCH_KEYARRAY_T15				15
#define		MXT_SPT_COMMSCONFIG_T18				18
#define		MXT_PROCG_NOISESUPPRESSION_T22			22
#define		MXT_PROCI_ONETOUCHGESTUREPROCESSOR_T24		24
#define		MXT_SPT_SELFTEST_T25				25
#define		MXT_PROCI_TWOTOUCHGESTUREPROCESSOR_T27		27
#define		MXT_SPT_CTECONFIG_T28				28
#define		MXT_DEBUG_DIAGNOSTICS_T37			37
#define		MXT_USERDATA_T38				38
#define		MXT_PROCI_GRIPSUPPRESSION_T40			40
#define		MXT_PROCI_PALMSUPPRESSION_T41			41
#define		MXT_SPT_DIGITIZER_T43				43
#define		MXT_SPT_MESSAGECOUNT_T44			44

#define		MXT_MSG_REPORTID				0x00

#define		MXT_MSG_T9_STATUS				0x01
#define		MXT_MSGB_T9_SUPPRESS				0x02
#define		MXT_MSGB_T9_AMP					0x04
#define		MXT_MSGB_T9_VECTOR				0x08
#define		MXT_MSGB_T9_MOVE				0x10
#define		MXT_MSGB_T9_RELEASE				0x20
#define		MXT_MSGB_T9_PRESS				0x40
#define		MXT_MSGB_T9_DETECT				0x80

#define		MXT_MSG_T9_XPOSMSB				0x02
#define		MXT_MSG_T9_YPOSMSB				0x03
#define		MXT_MSG_T9_XYPOSLSB				0x04
#define		MXT_MSG_T9_TCHAREA				0x05
#define		MXT_MSG_T9_TCHAMPLITUDE				0x06
#define		MXT_MSG_T9_TCHVECTOR				0x07

#define		MXT_T7_IDLEACQINT_INIT				32//16
#define		MXT_T7_ACTVACQINT_INIT				255
#define		MXT_T7_ACTV2IDLETO_INIT				20

#define		MXT_T8_CHARGTIME_INIT				10
#define		MXT_T8_RESERVED_INIT				0
#define		MXT_T8_TCHDRIFT_INIT				20
#define		MXT_T8_DRIFTST_INIT				20
#define		MXT_T8_TCHAUTOCAL_INIT				0
#define		MXT_T8_ATCHCALST_INIT				5
#define		MXT_T8_ATCHCALSTHR_INIT				0//40
#define		MXT_T8_ATCHCALFRCTHR_INIT			10//50//40//50
#define		MXT_T8_ATCHCALFRCRATIO_INIT			215//25//30

#define		MXT_T9_CTRL_INIT				131
#define		MXT_T9_XORIGIN_INIT				0
#define		MXT_T9_YORIGIN_INIT				0
#define		MXT_T9_XSIZE_INIT				27
#define		MXT_T9_YSIZE_INIT				42
#define		MXT_T9_AKSCFG_INIT				0
#define		MXT_T9_BLEN_INIT				32
#define		MXT_T9_TCHTHR_INIT				80
#define		MXT_T9_TCHDI_INIT				3
#define		MXT_T9_ORIENT_INIT				0
#define		MXT_T9_ORIENT_INVERT				2
#define		MXT_T9_MRGTIMEOUT_INIT				0
#define		MXT_T9_MOVHYSTI_INIT				3
#define		MXT_T9_MOVHYSTN_INIT				1
#define		MXT_T9_MOVFILTER_INIT				0x21//32
#define		MXT_T9_NUMTOUCH_INIT				10
#define		MXT_T9_MRGHYST_INIT				40
#define		MXT_T9_MRGTHR_INIT				40
#define		MXT_T9_AMPHYST_INIT				10
#define		MXT_T9_XRANGE_INIT				0x02FF
#define		MXT_T9_YRANGE_INIT				0x04FF
#define		MXT_T9_XLOCLIP_INIT				3//2
#define		MXT_T9_XHICLIP_INIT				3//6
#define		MXT_T9_YLOCLIP_INIT				4//6
#define		MXT_T9_YHICLIP_INIT				4//5
#define		MXT_T9_XEDGECTRL_INIT				0x8A//0x88
#define		MXT_T9_XEDGEDIST_INIT				30//20
#define		MXT_T9_YEDGECTRL_INIT				0x80//0x88
#define		MXT_T9_YEDGEDIST_INIT				0//15
#define		MXT_T9_JUMPLIMIT_INIT				25
#define		MXT_T9_TCHHYST_INIT				10
#define		MXT_T9_XPITCH_INIT				0
#define		MXT_T9_YPITCH_INIT				0

#define		MXT_T15_CTRL_INIT				0
#define		MXT_T15_XORIGIN_INIT				0
#define		MXT_T15_YORIGIN_INIT				0
#define		MXT_T15_XSIZE_INIT				0
#define		MXT_T15_YSIZE_INIT				0
#define		MXT_T15_AKSCFG_INIT				0
#define		MXT_T15_BLEN_INIT				0
#define		MXT_T15_TCHTHR_INIT				0
#define		MXT_T15_TCHDI_INIT				0
#define		MXT_T15_RESERVED_INIT				0

#define		MXT_T18_CTRL_INIT				0x04
#define		MXT_T18_CMD_INIT				0

#define		MXT_T22_CTRL_INIT				5
#define		MXT_T22_RESERVED_INIT				0
#define		MXT_T22_NOISETHR_INIT				50
#define		MXT_T22_FREQHOPSCALE_INIT			3
#define		MXT_T22_FREQHOPSCALE_IDLE			0
#define		MXT_T22_FREQ_0_INIT				8//0
#define		MXT_T22_FREQ_1_INIT				15//10
#define		MXT_T22_FREQ_2_INIT				25//30
#define		MXT_T22_FREQ_3_INIT				35
#define		MXT_T22_FREQ_4_INIT				54

#define		MXT_T24_CTRL_INIT				0
#define		MXT_T24_NUMGEST_INIT				0
#define		MXT_T24_GESTEN_INIT				0
#define		MXT_T24_PROCESS_INIT				0
#define		MXT_T24_TAPTO_INIT				0
#define		MXT_T24_FLICKTO_INIT				0
#define		MXT_T24_DRAGTO_INIT				0
#define		MXT_T24_SPRESSTO_INIT				0
#define		MXT_T24_LPRESSTO_INIT				0
#define		MXT_T24_REPPRESSTO_INIT				0
#define		MXT_T24_FLICKTHR_INIT				0
#define		MXT_T24_DRAGTHR_INIT				0
#define		MXT_T24_TAPTHR_INIT				0
#define		MXT_T24_THROWTHR_INIT				0

#define		MXT_T27_CTRL_INIT				0
#define		MXT_T27_NUMGEST_INIT				0
#define		MXT_T27_RESERVED_INIT				0
#define		MXT_T27_GESTEN_INIT				0
#define		MXT_T27_ROTATETHR_INIT				0
#define		MXT_T27_ZOOMTHR_INIT				0

#define		MXT_T28_CTRL_INIT				0
#define		MXT_T28_CMD_INIT				0
#define		MXT_T28_RESERVED_INIT				0
#define		MXT_T28_IDLEGCAFDEPTH_INIT			16
#define		MXT_T28_ACTVGCAFDEPTH_INIT			32//8
#define		MXT_T28_VOLTAGE_INIT				60

#define		MXT_T40_CTRL_INIT				0x11
#define		MXT_T40_XLOGRIP_INIT				13
#define		MXT_T40_XHIGRIP_INIT				0
#define		MXT_T40_YLOGRIP_INIT				0
#define		MXT_T40_YHIGRIP_INIT				0

#define		MXT_T41_CTRL_INIT				1
#define		MXT_T41_RESERVED_0_INIT				0
#define		MXT_T41_RESERVED_1_INIT				0
#define		MXT_T41_LARGEOBJTHR_INIT			100
#define		MXT_T41_DISTANCE_INIT				5
#define		MXT_T41_SUPEXTTO_INIT				5
#define		MXT_T41_STRENGTH_INIT				178

#define		MXT_T43_CTRL_INIT				0
#define		MXT_T43_HIDIDLERATE_INIT			0
#define		MXT_T43_XLENGTH_INIT				0
#define		MXT_T43_YLENGTH_INIT				0

#define		MXT_T44_COUNT_INIT				0
