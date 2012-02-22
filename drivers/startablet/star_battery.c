/*
 * drivers/power/star_battery.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/debugfs.h>
#include <linux/power_supply.h>
#include <linux/wakelock.h>
#include <linux/kthread.h>
#include <linux/freezer.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/mfd/tps6586x.h>

#include <mach/hardware.h>
#include <mach/gpio-names.h>
#include <mach/iomap.h>

#define USE_CHARGER_MAX8903
//#define CHECK_CHARGE_STATUS     // check CHARGE_STATUS_N gpio to decide if charge done
#define BATTERY_CUSTOM_MODEL    // for 6400mAh Battery
//#define USE_USB_CHARGING  // Use usb charging at power-supply event

#define GPIO_CHARGE_ENABLE_N  TEGRA_GPIO_PR0
#define GPIO_USB_SUSPEND      TEGRA_GPIO_PS3
#define GPIO_CHARGE_STATUS_N  TEGRA_GPIO_PQ3
#define GPIO_CHARGE_FAULT_N   TEGRA_GPIO_PI3
#define GPIO_AC_DETECT        TEGRA_GPIO_PU5
#define GPIO_USB_DETECT_N     TEGRA_GPIO_PQ6
#define GPIO_USB1_ID_INT      TEGRA_GPIO_PI4
#define GPIO_THM_VTG_SEL      TEGRA_GPIO_PQ0

//Add Charging Done & Fault Detection by CHG_STAT_N pin and CHG_FAULT_N pin
#define CHARGING_DONE           1
#define CHARGING_FAULT_DETECT   0

//#define DEBUG_INFO
//#define DEBUG_FUNCTION

#ifdef DEBUG_INFO
#define D(fmt, args...) printk(KERN_INFO fmt, ##args)
#else
#define D(fmt, args...) do {} while (0)
#endif

#ifdef DEBUG_FUNCTION
#define D_FUNC(fmt, args...) printk(KERN_INFO fmt, ##args)
#else
#define D_FUNC(fmt, args...) do {} while (0)
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define BATTERY_STATUS_HIGH               0x01
#define BATTERY_STATUS_LOW                0x02
#define BATTERY_STATUS_CRITICAL           0x04
#define BATTERY_STATUS_CHARGING           0x08
#define BATTERY_STATUS_DISCHARGING        0x10
#define BATTERY_STATUS_IDLE               0x20
#define BATTERY_STATUS_VERY_CRITICAL      0x40
#define BATTERY_STATUS_NO_BATTERY         0x80
#define BATTERY_STATUS_UNKNOWN            0xFF

#define MAX17040_VCELL_MSB  0x02
#define MAX17040_VCELL_LSB  0x03
#define MAX17040_SOC_MSB    0x04
#define MAX17040_SOC_LSB    0x05
#define MAX17040_MODE_MSB   0x06
#define MAX17040_MODE_LSB   0x07
#define MAX17040_VER_MSB    0x08
#define MAX17040_VER_LSB    0x09
#define MAX17040_RCOMP_MSB  0x0C
#define MAX17040_RCOMP_LSB  0x0D
#define MAX17040_CMD_MSB    0xFE
#define MAX17040_CMD_LSB    0xFF

//For Star Tablet 6400 mAh battery,
// Add MAX17040 Custom Model Setting [Start]
#define MAX17040_UNLOCK_MSB 0x3E	//Unlock Model Access address
#define MAX17040_OCV_MSB    0x0E	//OCV register address
#define MAX17040_MODEL_1    0x40	//MODEL register(1st 16 byte ) address
#define MAX17040_MODEL_2    0x50	//MODEL register(2nd 16 byte ) address
#define MAX17040_MODEL_3    0x60	//MODEL register(3rd 16 byte ) address
#define MAX17040_MODEL_4    0x70	//MODEL register(4th 16 byte ) address

#define BATTERY_POLLING_INTERVAL      (60 * 1000)   // 60s
#define BATTERY_POLLING_INTERVAL_LOW  (10 * 1000)   // 10s
#define BATTERY_POLLING_IMMEDIATELY   (300)         // 300ms
#define BATTERY_DATA_UNKNOWN          0x7FFFFFFF

//T20 CPU register control
#define RTC_SECONDS_COUNTDOWN_ALARM0  0x020
#define RTC_INTR_STATUS_0             0x02c
#define RTC_INTR_MASK_0               0x028
#define RTC_BUSY_0                    0x004

#define SEC_CDN_ALARM_INTR_SET    (1 << 3)     // SEC_CDN_ALARM_INTR_SET - bit 3

/** extern functions **/
void battery_at_fuelrst(void);

/** enum **/
typedef enum {
    Charger_Type_Battery = 0,
  #ifdef USE_USB_CHARGING
    Charger_Type_USB,
  #endif
    Charger_Type_AC,
    Charger_Type_Num,
    Charger_Type_Force32 = 0x7FFFFFFF
} Charger_Type;

typedef enum {
    Charge_Control_Charging_Disable = 0,
    Charge_Control_Charging_Enable,
    Charge_Control_Num,
    Charge_Control_Force32 = 0x7FFFFFFF
} Charge_Control;

typedef enum
{
    ChargerGpio_ChargeEnable_N = 0,
    ChargerGpio_UsbSuspend,
    ChargerGpio_ChargeStatus_N,
    ChargerGpio_ChargeFault_N,
    ChargerGpio_AcDetect_N,
    ChargerGpio_UsbDetect_N,
    ChargerGpio_Num,
} NvOdmChargerGpio;

typedef enum
{
    charger_offline,
    charger_ac_online,
    charger_usb_online,
    //charger_backup_battery,
    charger_max
} max17040_charge_line;

static enum power_supply_property star_battery_properties[] = {
    POWER_SUPPLY_PROP_STATUS,
    POWER_SUPPLY_PROP_HEALTH,
    POWER_SUPPLY_PROP_PRESENT,
    POWER_SUPPLY_PROP_TECHNOLOGY,
    POWER_SUPPLY_PROP_CAPACITY,
    POWER_SUPPLY_PROP_VOLTAGE_NOW,
    POWER_SUPPLY_PROP_TEMP
};

static enum power_supply_property star_power_properties[] = {
    POWER_SUPPLY_PROP_ONLINE,
};

//static char *supply_list[] = {
//    "battery",
//};

typedef struct battery_data_rec
{
    uint batteryLifePercent;
    uint batteryVoltage;
    int  batteryTemperature;
} battery_data;

typedef struct star_battery_dev {
    struct  i2c_client  *client;
    struct  timer_list  battery_poll_timer;
    struct  work_struct  work;
#ifdef CONFIG_HAS_WAKELOCK
    struct wake_lock  wlock;
#endif

    uint    batt_vol;           /* voltage from ADC */
    int     batt_temp;          /* temperature (degrees C) */
    uint    batt_soc;

    uint    batt_status_poll_period;
    bool    present;

#ifdef BATTERY_CUSTOM_MODEL
    bool    load_custom_model;  /* true : custom model is loaded*/
#endif
} star_battery_dev;

typedef struct star_battery_test {
    int     at_charge_mode;         // set at%charge value
    bool    test_mode_polling;      // if test mode for battery polling is set
    bool    test_mode_temperature;  // if test mode for battery temperature is set
    int     test_batt_temperature;  // battery temperature which set by test mode
    bool    test_mode_soc;          // if test mode for battery soc is set
    int     test_batt_soc;          // battery soc(level) which set by test mode
} star_battery_test;

/** Local functions **/
static int star_battery_get_data(battery_data *battery);
static void charger_control(bool main_chg, bool usb_chg);
static void charger_status(max17040_charge_line* pStatus, int set_charger);
static int star_power_get_property(struct power_supply *psy,
    enum power_supply_property psp, union power_supply_propval *val);
static int star_battery_get_property(struct power_supply *psy,
    enum power_supply_property psp, union power_supply_propval *val);

#ifdef CONFIG_USB_TEGRA_OTG
extern int muic_is_host_mode(void);
#endif

/** Local variables **/
static void __iomem *rtc_base = IO_ADDRESS(TEGRA_RTC_BASE);
static star_battery_dev *batt_dev;
static star_battery_test *batt_test;

// /sys/class/power_supply/battery/device
static struct power_supply star_supplies[] = {
    {
        .name = "battery",
        .type = POWER_SUPPLY_TYPE_BATTERY,
        .properties = star_battery_properties,
        .num_properties = ARRAY_SIZE(star_battery_properties),
        .get_property = star_battery_get_property,
    },
#ifdef USE_USB_CHARGING
    {
        .name = "usb",
        .type = POWER_SUPPLY_TYPE_USB,
        //.supplied_to = supply_list,
        //.num_supplicants = ARRAY_SIZE(supply_list),
        .properties = star_power_properties,
        .num_properties = ARRAY_SIZE(star_power_properties),
        .get_property = star_power_get_property,
    },
#endif
    {
        .name = "ac",
        .type = POWER_SUPPLY_TYPE_MAINS,
        //.supplied_to = supply_list,
        //.num_supplicants = ARRAY_SIZE(supply_list),
        .properties = star_power_properties,
        .num_properties = ARRAY_SIZE(star_power_properties),
        .get_property = star_power_get_property,
    },
};

static ssize_t star_battery_show_poll_period(
        struct device *dev,
        struct device_attribute *attr,
        char *buf)
{
    return sprintf(buf, "%d\n", batt_dev->batt_status_poll_period);
}

static ssize_t star_battery_store_poll_period(
        struct device *dev,
        struct device_attribute *attr,
        const char *buf,
        size_t count)
{
    unsigned int value = 0;

    value = simple_strtoul(buf, NULL, 0);
    batt_dev->batt_status_poll_period = value;
    batt_test->test_mode_polling = (value == 0 ? FALSE : TRUE);
    mod_timer(&(batt_dev->battery_poll_timer),
        jiffies + msecs_to_jiffies(batt_dev->batt_status_poll_period));
    return count;
}

static ssize_t star_battery_show_test_temperature(
        struct device *dev,
        struct device_attribute *attr,
        char *buf)
{
    return sprintf(buf, "%d\n", batt_test->test_batt_temperature);
}

/*
 * Set temperature for battery driver test
 * To terminate test mode, enter value over 100
 */
static ssize_t star_battery_store_test_temperature(
        struct device *dev,
        struct device_attribute *attr,
        const char *buf,
        size_t count)
{
    int value = 0;

    value = simple_strtol(buf, NULL, 0);
    batt_test->test_batt_temperature = value;
    batt_test->test_mode_temperature = (value > 99 ? FALSE : TRUE);
    mod_timer(&(batt_dev->battery_poll_timer), jiffies + msecs_to_jiffies(BATTERY_POLLING_IMMEDIATELY));
    return count;
}

/*
 * Set soc for battery driver test
 * To terminate test mode, enter value over 100
 */
static ssize_t star_battery_store_test_soc(
        struct device *dev,
        struct device_attribute *attr,
        const char *buf,
        size_t count)
{
    int value = 0;

    value = simple_strtol(buf, NULL, 0);
    batt_test->test_batt_soc = value;
    batt_test->test_mode_soc = (value > 100 ? FALSE : TRUE);
    mod_timer(&(batt_dev->battery_poll_timer), jiffies + msecs_to_jiffies(BATTERY_POLLING_IMMEDIATELY));
    return count;
}

/*
<item android:maxLevel="4" android:drawable="@android:drawable/stat_sys_battery_0" />
<item android:maxLevel="15" android:drawable="@android:drawable/stat_sys_battery_15" />
<item android:maxLevel="35" android:drawable="@android:drawable/stat_sys_battery_28" />
<item android:maxLevel="49" android:drawable="@android:drawable/stat_sys_battery_43" />
<item android:maxLevel="60" android:drawable="@android:drawable/stat_sys_battery_57" />
<item android:maxLevel="75" android:drawable="@android:drawable/stat_sys_battery_71" />
<item android:maxLevel="90" android:drawable="@android:drawable/stat_sys_battery_85" />
<item android:maxLevel="100" android:drawable="@android:drawable/stat_sys_battery_100" />
*/
static ssize_t star_battery_show_indicator(
        struct device *dev,
        struct device_attribute *attr,
        char *buf)
{
    int indi;

    if (batt_dev->batt_soc <= 4)
        indi = 1;
    else if (batt_dev->batt_soc <= 15)
        indi = 2;
    else if (batt_dev->batt_soc <= 35)
        indi = 3;
    else if (batt_dev->batt_soc <= 49)
        indi = 4;
    else if (batt_dev->batt_soc <= 60)
        indi = 5;
    else if (batt_dev->batt_soc <= 75)
        indi = 6;
    else if (batt_dev->batt_soc <= 90)
        indi = 7;
    else
        indi = 8;

    return sprintf(buf, "%d\n", indi);
}

static struct device_attribute star_battery_attr[] = {
    {
        .attr = { .name = "status_poll_period", .mode = S_IRUGO | S_IWUSR },
        .show = star_battery_show_poll_period,
        .store = star_battery_store_poll_period,
    },
    {
        .attr = { .name = "battery_temperature", .mode = S_IRUGO | S_IWUSR },
        .show = star_battery_show_test_temperature,
        .store = star_battery_store_test_temperature,
    },
    {
        .attr = { .name = "battery_soc", .mode = S_IWUSR },
        .show = NULL,
        .store = star_battery_store_test_soc,
    },
    {
        .attr = { .name = "indicator", .mode = S_IRUGO },
        .show = star_battery_show_indicator,
    },
};

/*
 * I2C functions
 */
static int max17040_write_reg(struct i2c_client *client, int reg, u8 value)
{
	int ret;

	ret = i2c_smbus_write_byte_data(client, reg, value);
	if (ret < 0) {
		dev_err(&client->dev, "[BAT] failed writing 0x%02x to 0x%02x\n",
				value, reg);
		return ret;
	}

	//printk(KERN_INFO "[BAT] %s() reg 0x%2x = 0x%2x(%d)\n", __func__, reg, value, value);
	return 0;
}

static int max17040_read_reg(struct i2c_client *client, int reg, u8 *value)
{
	int ret;

	ret = i2c_smbus_read_byte_data(client, reg);
	if (ret < 0) {
		dev_err(&client->dev, "[BAT] failed reading at 0x%02x\n", reg);
		return ret;
	}

	*value = (uint8_t)ret;

	//printk(KERN_INFO "[BAT] %s() reg 0x%2x = 0x%2x(%d)\n", __func__, reg, *value, *value);
	return 0;
}

static int max17040_write_regs(struct i2c_client *client, int reg, u16 value)
{
	int ret;

	ret = i2c_smbus_write_word_data(client, reg, (u16)(value >> 8 | value << 8));
	if (ret < 0) {
		dev_err(&client->dev, "[BAT] failed writing 0x%02x to 0x%02x\n",
				value, reg);
		return ret;
	}

	//printk(KERN_INFO "[BAT] %s() reg 0x%2x = 0x%2x(%d)\n", __func__, reg, value, value);
	return 0;
}

static int max17040_read_regs(struct i2c_client *client, int reg, u16 *value)
{
	int ret;

	ret = i2c_smbus_read_word_data(client, reg);
	if (ret < 0) {
		dev_err(&client->dev, "[BAT] failed reading at 0x%02x\n", reg);
		return ret;
	}

	*value = (uint16_t)(ret >> 8 | ret << 8 );

	//printk(KERN_INFO "[BAT] %s() reg 0x%2x = 0x%2x(%d)\n", __func__, reg, *value, *value);
	return 0;
}

#if 0
static void max17040_reset(struct i2c_client *client)
{
    max17040_write_reg(client, MAX17040_CMD_MSB, 0x54);
    max17040_write_reg(client, MAX17040_CMD_LSB, 0x00);
}
#endif

static void max17040_quickstart(struct i2c_client *client)
{
    printk(KERN_INFO "[BAT] %s()\n", __func__);
    max17040_write_regs(client, MAX17040_MODE_MSB, 0x4000);
}

static uint max17040_get_vcell(struct i2c_client *client)
{
    u8 msb = 0, lsb = 0;
    int ret1, ret2;
    uint volt;

    ret1 = max17040_read_reg(client, MAX17040_VCELL_MSB, &msb);
    ret2 = max17040_read_reg(client, MAX17040_VCELL_LSB, &lsb);

    if (ret1 < 0 || ret2 < 0)
        return BATTERY_DATA_UNKNOWN;

    //volt = (msb << 4) + (lsb >> 4);
    volt = ((msb << 8 | lsb) >> 4) * 5 /4; // * 1.25

    return volt;
}

static uint max17040_get_soc(struct i2c_client *client)
{
    u8 msb = 0, lsb = 0;
    int ret1, ret2;
    uint soc;

    ret1 = max17040_read_reg(client, MAX17040_SOC_MSB, &msb);
    ret2 = max17040_read_reg(client, MAX17040_SOC_LSB, &lsb);

    if (ret1 < 0 || ret2 < 0)
        return BATTERY_DATA_UNKNOWN;

#ifdef BATTERY_CUSTOM_MODEL
    // For Star Tablet 6400 mAh battery,
    // Use special calculation for Custom Model (Star tablet)
    soc = ( (msb * 256 ) + lsb ) / 256; // 0~200 scale
    soc = soc * 100 / 98 / 2;
#else
    soc = msb * 100 + (lsb * 100 / 256);
    soc = soc / 96;
#endif

    D("[BAT] %s() 0x%02x:%02x --> soc:%d%% (ret:%d,%d)\n", __func__, msb, lsb, soc, ret1, ret2);
    if (soc > 100)
        soc = 100;

    return soc;
}

static uint max17040_get_version(struct i2c_client *client)
{
    u16 data = 0;
    int ret;

    ret = max17040_read_regs(client, MAX17040_VER_MSB, &data);
    if (ret < 0)
        return BATTERY_DATA_UNKNOWN;

    dev_info(&client->dev, "MAX17040 Fuel-Gauge Ver 0x%04x\n", data);
    return data;
}

// RCOMP setting for Battery Gauging Compensation
static uint max17040_set_rcomp(struct i2c_client *client, int temp)
{
#define CUSTOMER_RCOMP  0xBD    // Customer's RCOMP value at 20'C = 189(0xBD)
#define STANDARD_TEMP   20
#define TEMP_CO_HOT     (-1400) // RCOMP change per degree for every degree above 20'C = -1.4
#define TEMP_CO_COLD    (-6975) // RCOMP change per degree for every degree below 20'C = -6.975

    u16 data;
    u16 new_rcomp;

    if (BATTERY_DATA_UNKNOWN == temp)
        temp = STANDARD_TEMP;

    if (temp >= STANDARD_TEMP)
        new_rcomp = (CUSTOMER_RCOMP + ((temp - STANDARD_TEMP) * TEMP_CO_HOT / 1000));
    else
        new_rcomp = (CUSTOMER_RCOMP + ((temp - STANDARD_TEMP) * TEMP_CO_COLD / 1000));

    new_rcomp = (new_rcomp > 255) ? 255 : new_rcomp;
    new_rcomp = (new_rcomp < 0) ? 0 : new_rcomp;
    data = (u16)(new_rcomp << 8);
    D("[BAT] %s() new_rcomp:0x%x(%d), temp:%d'C\n", __func__, new_rcomp, new_rcomp, temp);

    return max17040_write_regs(client, MAX17040_RCOMP_MSB, data);
}

#define LEVEL_UPPER     50
#define LEVEL_LOWER     50

typedef struct {
    uint mVolt;
    uint soc;
} battery_level;

static battery_level battery_only[] = {
    { 4200, 100 }, { 4150, 98 }, { 4100, 93 }, { 4050, 88 }, { 4000, 81 },
    { 3950, 74 },  { 3900, 65 }, { 3850, 55 }, { 3800, 45 }, { 3790, 43 },
    { 3780, 38 },  { 3770, 30 }, { 3760, 23 }, { 3750, 21 }, { 3730, 18 },
    { 3710, 17 },  { 3700, 14 }, { 3680, 11 }, { 3670, 6 },  { 3660, 3 },
    { 3650, 0 },   { 0, 0 },
};

static void init_level_check(void)
{
    max17040_charge_line charger;
    battery_data battery;
    uint theorical_soc, ref_mVolt;
    uint slope, constant;
    uint i;

    D_FUNC("[BAT] %s()\n", __func__);

    charger_status(&charger, 0);

    // Disable charger to check level-voltage
    // Quickstart need to be charger turned off
    charger_control(FALSE, FALSE);
    msleep(500);

    star_battery_get_data(&battery);

    ref_mVolt = (charger_ac_online == charger) ? (battery.batteryVoltage - 100) : battery.batteryVoltage;

    for (i = 0; i < ARRAY_SIZE(battery_only); i++)
    {
        if (ref_mVolt < battery_only[i].mVolt)
            continue;

        // if voltage is within tables
        if (0 == i)
        {
            //theorical_soc = battery_only[i].soc;
            slope = 0;
            constant = battery_only[i].soc;
            break;
        }

        slope = ((battery_only[i-1].soc - battery_only[i].soc) * 1000 ) / (battery_only[i-1].mVolt - battery_only[i].mVolt);
        constant = battery_only[i-1].soc - slope * battery_only[i-1].mVolt / 1000;
        break;
    }

    theorical_soc = slope * ref_mVolt / 1000 + constant;

    printk("[BAT] battery - %d%%(theorical) = (%d / 1000) x %dmV + (%d) (TA:%d)\n",
        theorical_soc, slope, battery.batteryVoltage, constant, charger);

    // if read soc is over tolerenct(+-20%), quick start fuel gauging ic
    if (battery.batteryLifePercent > ((uint)theorical_soc + LEVEL_UPPER)
        || battery.batteryLifePercent < ((uint)theorical_soc > LEVEL_LOWER ? (uint)theorical_soc - LEVEL_LOWER : 0))
    {
        max17040_quickstart(batt_dev->client);
    }
}

// For Star Tablet 6400 mAh battery, MAX17040 setting should be changed to Custom Model [Start]
#ifdef BATTERY_CUSTOM_MODEL
static int max17040_set_model_data(struct i2c_client *client, u8 reg)
{
    int ret;
    int array_idx = -1;
    u8 model_data[4][16] =
    {
        //Write the Data to Buffer
        {0x9A, 0x80, 0xB6, 0x40, 0xB7, 0xC0, 0xB9, 0xF0, 0xBB, 0x00, 0xBC, 0x20, 0xBC, 0x80, 0xBC, 0xE0},
        {0xBD, 0x10, 0xBE, 0x00, 0xBE, 0xD0, 0xC1, 0x10, 0xC3, 0x10, 0xC6, 0xE0, 0xCB, 0x90, 0xD0, 0x70},
        {0x00, 0x20, 0x30, 0x50, 0x1B, 0x20, 0x1B, 0xF0, 0x1A, 0x60, 0x7F, 0x10, 0x7F, 0x10, 0xA1, 0x00},
        {0x3E, 0x70, 0x46, 0xF0, 0x00, 0xF0, 0x2D, 0xB0, 0x18, 0xE0, 0x12, 0xE0, 0x11, 0xD0, 0x11, 0xD0},
    };

    D_FUNC("[BAT] %s(reg:0x%02x)\n", __func__, reg);

    reg &= 0xFF;
    switch (reg)
    {
    case 0x40:  array_idx = 0;  break;
    case 0x50:  array_idx = 1;  break;
    case 0x60:  array_idx = 2;  break;
    case 0x70:  array_idx = 3;  break;
    default:    // Error
        pr_err("[BAT] %s() reg(0x%x) is not for custom model\n", __func__, reg);
        return -1;
    }

    ret = i2c_write_block_data(client, DEVICE_I2C_ADDRESS_SIZE_1BYTE, 16, reg, model_data[array_idx]);
    if (ret < 0)
    {
        dev_err(&client->dev, "[BAT] failed writing model data to 0x%02x\n", reg);
        return ret;
    }

    return 0;
}

#define CUSTOM_MODEL_RETRY  3
static int max17040_set_custom_model(struct i2c_client *client)
{
    u16 value16;
    u8  value8;
    u16 original_rcomp, original_ocv;
    int retry;
    int ret = 0;

    // -----------Start to make MAX17040 to Custom Model for Star Tablet 6400 mAh battery
    mdelay(200);

    //1.Unlock Model Access
    if (max17040_write_regs(client, MAX17040_UNLOCK_MSB, 0x4A57))
        return -1;

    //2. Read RCOMP and OCV
    if (max17040_read_regs(client, MAX17040_RCOMP_MSB, &value16))
        return -1;
    original_rcomp = value16;

    if (max17040_read_regs(client, MAX17040_OCV_MSB, &value16))
        return -1;
    original_ocv = value16;
    D("[BAT] %s() originl value - RCOMP : 0x%04x, OCV : 0x%04x \n", __func__, original_rcomp, original_ocv);

    //3. Write OCV
    if (max17040_write_regs(client, MAX17040_OCV_MSB, 0xDA70))
    {
        ret = -1;
        goto lock_model;
    }

    //4. Write RCOMP
    if (max17040_write_regs(client, MAX17040_RCOMP_MSB, 0xFF00))
    {
        ret = -1;
        goto rollback_setting;
    }

    //5. Write the Model data - 64 byte (Addr 0x40 - 0x70 )
    max17040_set_model_data(client, MAX17040_MODEL_1);
    max17040_set_model_data(client, MAX17040_MODEL_2);
    max17040_set_model_data(client, MAX17040_MODEL_3);
    max17040_set_model_data(client, MAX17040_MODEL_4);

    //6. Delay at least 150 mS
    mdelay(160);

    //7. Write OCV
    if (max17040_write_regs(client, MAX17040_OCV_MSB, 0xDA70))
    {
        ret = -1;
        goto rollback_setting;
    }

    //8. Delay between 150 mS and 600 mS
    mdelay(300);

    //9. Read SOC
    if (max17040_read_regs(client, MAX17040_SOC_MSB, &value16))
    {
        ret = -1;
        goto rollback_setting;
    }

    value8 = value16 >> 8;
    if ((value8 >= 0xF3) && (value8 <= 0xF5))
    {
        printk(KERN_INFO "[BAT] Custom model was loaded successful!! soc_msb = 0x%02x(0x%02x)\n", value8, value16 & 0xff);
    }
    else
    {
        printk(KERN_INFO "[BAT] Custom model was NOT loaded successful!! soc_msb = 0x%02x(0x%02x)\n", value8, value16 & 0xff);
    }

rollback_setting:
    //10. Write Original RCOMP
    if (max17040_write_regs(client, MAX17040_RCOMP_MSB, original_rcomp))
        printk(KERN_WARNING "[BAT] Restoring RCOMP failed\n");

    //11. Write Original OCV
    retry = CUSTOM_MODEL_RETRY;
    while (max17040_write_regs(client, MAX17040_OCV_MSB, original_ocv) && retry > 0)
    {
        printk(KERN_WARNING "[BAT] Restoring OCV failed\n");
        retry--;
    }

lock_model:
    //12.lock Model Access
    retry = CUSTOM_MODEL_RETRY;
    while (max17040_write_regs(client, MAX17040_UNLOCK_MSB, 0x0000) && retry > 0)
    {
        printk(KERN_WARNING "[BAT] Lock model failed\n");
        retry--;
    }
    mdelay(300);

    batt_dev->load_custom_model = true;
    //printk(KERN_INFO "[BAT] Done\n");

    return ret;
}
#endif // BATTERY_CUSTOM_MODEL

#define MEASURE_INTERVAL    10  // second
#define MEASURE_DIFFERENCE  2   // degree C
static int star_get_battery_temperature(void)
{
    int temp;
    static int old_temp = BATTERY_DATA_UNKNOWN;
    static u64 last_gauging_time = 0;
    static int temp_limit_high = 0;
    static int temp_limit_low = 0;

    D_FUNC("[BAT] %s()\n", __func__);

    if (batt_test->at_charge_mode) // test mode
    {
        printk(KERN_INFO "[BAT] %s() at_charge mode is %d\n", __func__, batt_test->at_charge_mode);
        return 18;  // update dumpy temp
    }

    if (get_hw_rev() == REV_E)  // rev.E doesn't have temp line
        return 22;

    temp = old_temp;

    if ((0 == last_gauging_time || last_gauging_time + MEASURE_INTERVAL * HZ < get_jiffies_64() )
        && !(batt_test->test_mode_temperature))
    {
        last_gauging_time = get_jiffies_64();
        temp = tps6586x_battery_temperature();

        if (BATTERY_DATA_UNKNOWN == temp)           // if fail to read battery temp (no battery? or dummy battery?)
        {
            D("[BAT] %s() Fail to read from adc\n", __func__);
            old_temp = BATTERY_DATA_UNKNOWN;
        }
        else if (BATTERY_DATA_UNKNOWN == old_temp)  // if it's first read of battery temperature
        {
            D("[BAT] %s() 1st time reading\n", __func__);
            old_temp = temp;
        }
        else
        {
            // check difference between old temp and new temp.
            // if diff is bigger than MEASURE_DIFFERENCE, change only MEASURE_DIFFERENCE
            if (temp - old_temp > MEASURE_DIFFERENCE)
                old_temp = old_temp + MEASURE_DIFFERENCE;
            else if (temp - old_temp < -MEASURE_DIFFERENCE)
                old_temp = old_temp - MEASURE_DIFFERENCE;
            else
                old_temp = temp;
        }
    }

    if (batt_test->test_mode_temperature)
    {
        printk(KERN_INFO "[BAT] %s() test temperature is %d'C(test mode:%d)\n",
            __func__, batt_test->test_batt_temperature, batt_test->test_mode_temperature);
        old_temp = batt_test->test_batt_temperature;
    }

    // set rcomp if temperature is changed over/under limit
    if ((!temp_limit_high && !temp_limit_low)    // if first setting of rcomp
        || (old_temp >= temp_limit_high)         // if temp exceed high limit
        || (old_temp <= temp_limit_low))         // if temp go down low limit
    {
        temp_limit_high = old_temp + 5;
        temp_limit_low = old_temp - 5;
        max17040_set_rcomp(batt_dev->client, old_temp);
        D("[BAT] %s() %d < %d'C < %d\n", __func__, temp_limit_low, old_temp, temp_limit_high);
    }

    D("[BAT] %s() temp %d'C --> %d'C\n", __func__, temp, old_temp);
    return old_temp;
}

static int star_battery_get_data(battery_data *battery)
{
    int ret = 0;

    D_FUNC("[BAT] %s()\n", __func__);
    battery->batteryLifePercent = max17040_get_soc(batt_dev->client);
    battery->batteryVoltage = max17040_get_vcell(batt_dev->client);

    //Delay 300mS needs to be stable of BAT_THM voltage.
    msleep(300);
    battery->batteryTemperature = star_get_battery_temperature();

    batt_dev->present = (battery->batteryLifePercent != BATTERY_DATA_UNKNOWN);

    printk(KERN_INFO "battery - %d%%, %dmV, %d'C\n", battery->batteryLifePercent,
        battery->batteryVoltage, battery->batteryTemperature);

    // check read data
    if (battery->batteryLifePercent == BATTERY_DATA_UNKNOWN)
        battery->batteryLifePercent = 0;

    if (battery->batteryVoltage == BATTERY_DATA_UNKNOWN)
        battery->batteryVoltage = 0;

    D("[BAT] %s() - 2 %d%%, %dmV, %d'C, ret:%d\n", __func__, battery->batteryLifePercent,
        battery->batteryVoltage, battery->batteryTemperature, ret);

    return ret;
}

#define MAX_DISCHARGE_COUNT 5
static int star_battery_data(void)
{
    battery_data battery = {0};
    static u64 last_gauging_time = 0;
    uint soc_data;
    static int full_discharge_count = 0;
    static int is_custom_model = 0;
#ifdef CHECK_CHARGE_STATUS
    max17040_charge_line charger;
#endif

    D_FUNC("[BAT] %s()\n", __func__);

    if (0 == last_gauging_time || last_gauging_time + 1 * HZ < get_jiffies_64())
    {
        last_gauging_time = get_jiffies_64();
        if (star_battery_get_data(&battery) < 0)
        {
            last_gauging_time = 0;
            return 0;
        }
    }
    else
    {
        return 0;
    }

#ifdef BATTERY_CUSTOM_MODEL
    if (!batt_dev->load_custom_model)
    {
        // RCOMP-0xBD00 is for 6400 mAh battery.
        //max17040_set_rcomp(batt_dev->client, 20); // set default value
        if (0 == max17040_set_custom_model(batt_dev->client))
        {
            //max17040_quickstart(batt_dev->client);
            is_custom_model = 1;
        }
    }
#endif

    batt_dev->batt_vol = battery.batteryVoltage;
    batt_dev->batt_vol *= 1000; // Convert uV to mV
    batt_dev->batt_temp = battery.batteryTemperature;
    batt_dev->batt_temp *= 10;  // Android use x10 scale
    batt_dev->batt_soc = battery.batteryLifePercent;

    soc_data = battery.batteryLifePercent; // Get Gauging Percent from PMU data.

#ifdef CHECK_CHARGE_STATUS
    charger_status(&charger, 1);

    //Add Charging Done & Fault Detection by CHG_STAT_N pin and CHG_FAULT_N pin
    if (charger == charger_offline)
    {
        //If PMU Gauging Data = 100, it should be 99 to fix the bug "100(bat) -> 99 (AC by CHG_STAT_N pin Low)"
        if (100 == battery.batteryLifePercent)
        {
            batt_dev->batt_soc = 99; //No Full battery(100%) state without External Power.
        }
    }
    else // charger_ac_online, charger_usb_online,
    {
        int pin_value_status, pin_value_fault;

        mdelay(100); //For accurate GPIO detection, the OsSleep is necessary.
        pin_value_status = gpio_get_value(GPIO_CHARGE_STATUS_N);
        pin_value_fault = gpio_get_value(GPIO_CHARGE_FAULT_N);

        if (pin_value_fault != CHARGING_FAULT_DETECT)
        {
            if (pin_value_status == CHARGING_DONE && batt_dev->batt_vol > 4100 * 1000)
            {
                batt_dev->batt_soc = 100; //Charging Done by Only CHG_STAT_N pin state.
            }
            else
            {
                //Even though the Gauging value is 100%, the shown percent will be 99% until the CHG_STAT_N pin change.
                if (battery.batteryLifePercent == 100)
                    batt_dev->batt_soc = 99;
            }
        }
        else //Charging Fault Detection -> Battery Icon will be shown "?"
        {
            printk(KERN_INFO "[BAT] CHARGING_FAULT_DETECT!!!\n");
            batt_dev->batt_vol = 0;
            batt_dev->batt_soc = 0;
        }
    }
#endif

    // Change battery polling interval by battery level
    if (!(batt_test->test_mode_polling))
    {
        if (batt_dev->batt_soc > 10)
            batt_dev->batt_status_poll_period = BATTERY_POLLING_INTERVAL;
        else
            batt_dev->batt_status_poll_period = BATTERY_POLLING_INTERVAL_LOW;
    }

    //Battery Gauging 0%-Judgement by Battery Voltage(3400mV)
    if (batt_dev->present)
    {
        if (batt_dev->batt_vol < 3300 * 1000)
        {
            full_discharge_count = 4;
            battery_at_fuelrst();
        }
        else if ((batt_dev->batt_vol < 3400 * 1000) && (full_discharge_count < MAX_DISCHARGE_COUNT))
        {
            full_discharge_count++;

            if (0 == batt_dev->batt_soc)
                batt_dev->batt_soc = 1;

            //printk(KERN_INFO "%s: FullDischargeCount  = %d  \n", __func__, FullDischargeCount);
            if (MAX_DISCHARGE_COUNT - 2 == full_discharge_count)
            {
                batt_dev->batt_soc = 1;
                battery_at_fuelrst();
            }
        }
        else if (batt_dev->batt_vol >= 3400 * 1000)
        {
            //printk(KERN_INFO "%s: voltage = %d mV,  soc = 0% ->1% \n", __func__, batt_dev->batt_vol / 1000);
            if (batt_dev->batt_soc == 0)
                batt_dev->batt_soc = 1;
            full_discharge_count = 0;
        }

        //else if (full_discharge_count >= MAX_DISCHARGE_COUNT)
        if (full_discharge_count >= MAX_DISCHARGE_COUNT)
        {
            //printk(KERN_INFO "%s: FullDischargeCount  = %d  System Shut Down now\n", __func__, FullDischargeCount);
            //full_discharge_count = 0;
            batt_dev->batt_soc = 0;
        }
    }

    //
    if (batt_test->test_mode_soc)
        batt_dev->batt_soc = batt_test->test_batt_soc;

    D("[BAT] %s() %d%%, %dmV, %d.%dC, present:%d\n", __func__, batt_dev->batt_soc,
        batt_dev->batt_vol/1000, batt_dev->batt_temp / 10, batt_dev->batt_temp % 10, batt_dev->present);

    return 0;
}

static int star_power_get_property(struct power_supply *psy,
    enum power_supply_property psp, union power_supply_propval *val)
{
    max17040_charge_line charger;

    D_FUNC("[BAT] %s() psp:%d, present:%d\n", __func__, psp, batt_dev->present);

    switch (psp) {
    case POWER_SUPPLY_PROP_ONLINE:
        charger_status(&charger, 1);

        if (psy->type == POWER_SUPPLY_TYPE_MAINS)
            val->intval = (charger == charger_ac_online);
        else if (psy->type == POWER_SUPPLY_TYPE_USB)
            val->intval = (charger == charger_usb_online);
        else
            val->intval = 0;
        break;

    default:
        return -EINVAL;
    }
    return 0;
}

static int star_battery_get_property(struct power_supply *psy,
    enum power_supply_property psp, union power_supply_propval *val)
{
    int batt_tech = 0;
    max17040_charge_line charger = charger_offline;

    D_FUNC("[BAT] %s() psp:%d, present:%d\n", __func__, psp, batt_dev->present);
    star_battery_data();

    switch (psp) {
    case POWER_SUPPLY_PROP_STATUS:
        charger_status(&charger, 1);
        if (batt_dev->present)
        {
            if (charger_ac_online == charger || charger_usb_online == charger)
            {
                if (100 == batt_dev->batt_soc)
                    val->intval = POWER_SUPPLY_STATUS_FULL;
                else
                    val->intval = POWER_SUPPLY_STATUS_CHARGING;
            }
            else
            {
                val->intval = POWER_SUPPLY_STATUS_NOT_CHARGING;
            }
        }
        else
        {
            val->intval = POWER_SUPPLY_STATUS_UNKNOWN;
        }
        D("[BAT] %s() psp:%d -> value:0x%x\n", __func__, psp, val->intval);
        break;

    case POWER_SUPPLY_PROP_PRESENT:
        val->intval = batt_dev->present;
        break;

    case POWER_SUPPLY_PROP_TECHNOLOGY:
        if (batt_dev->present)
            batt_tech = POWER_SUPPLY_TECHNOLOGY_LION;
        else
            batt_tech = POWER_SUPPLY_TECHNOLOGY_UNKNOWN;

        val->intval = batt_tech;
        break;

    case POWER_SUPPLY_PROP_HEALTH:
        if (batt_dev->present)
            val->intval = POWER_SUPPLY_HEALTH_GOOD;
        else
            val->intval = POWER_SUPPLY_HEALTH_UNKNOWN;
        break;

    case POWER_SUPPLY_PROP_VOLTAGE_NOW:
    case POWER_SUPPLY_PROP_CAPACITY:
    case POWER_SUPPLY_PROP_TEMP:
        if (!batt_dev->present)
        {
            val->intval = POWER_SUPPLY_STATUS_UNKNOWN;
            return 0;
        }

        if (psp == POWER_SUPPLY_PROP_CAPACITY)
            val->intval = batt_dev->batt_soc;
        else if (psp == POWER_SUPPLY_PROP_VOLTAGE_NOW)
            val->intval = batt_dev->batt_vol;

        if (psp == POWER_SUPPLY_PROP_TEMP)
            val->intval = batt_dev->batt_temp;
        break;

    default:
        return -EINVAL;
    }
    return 0;
}

static void star_battery_poll_timer_func(unsigned long unused)
{
    D_FUNC("[CHG] %s()\n", __func__);
    power_supply_changed(&star_supplies[Charger_Type_Battery]);
    //power_supply_changed(&star_supplies[Charger_Type_USB]);
    //power_supply_changed(&star_supplies[Charger_Type_AC]);

    mod_timer(&(batt_dev->battery_poll_timer),
        jiffies + msecs_to_jiffies(batt_dev->batt_status_poll_period));
}

// CHG_STAT_N, AC_DET_N, USB_DET_N are all Active Low
#define STATE_ACTIVE   0
#define STATE_INACTIVE 1
#define STATE_AC_ONLINE 1
#define STATE_USB_HOST 1

static void charger_control(bool main_chg, bool usb_chg)
{
	D_FUNC("[BAT] %s() Main_Chg:%d, Usb_Chg:%d\n", __func__, main_chg, usb_chg);

	if (main_chg)
		gpio_set_value(GPIO_CHARGE_ENABLE_N, 0);
	else
		gpio_set_value(GPIO_CHARGE_ENABLE_N, 1);

	if (usb_chg)
		gpio_set_value(GPIO_USB_SUSPEND, 0);
	else
		gpio_set_value(GPIO_USB_SUSPEND, 1);
}

static void charger_status(max17040_charge_line* pstatus, int set_charger)
{
    int pin_value_ac = STATE_INACTIVE;
    int pin_value_usb = STATE_INACTIVE;
    int pin_value_usb_host = STATE_INACTIVE;
    static max17040_charge_line charger_line = charger_offline;

    pin_value_ac = gpio_get_value(GPIO_AC_DETECT);
    pin_value_usb = gpio_get_value(GPIO_USB_DETECT_N);
    pin_value_usb_host = gpio_get_value(GPIO_USB1_ID_INT);

    if ((get_hw_rev() >= REV_G && STATE_AC_ONLINE == pin_value_ac)
        || (get_hw_rev() < REV_G && STATE_ACTIVE == pin_value_ac))
    {
        if (set_charger)
        {
            charger_control(batt_dev->present, TRUE);

            if((get_hw_rev() > REV_F) && (get_hw_rev() < REV_1_0) )
                gpio_set_value(GPIO_THM_VTG_SEL, 1);
        }
        D("[CHG] AC - Charger is enabled\n");
        *pstatus = charger_ac_online;
    }
    else if (STATE_ACTIVE == pin_value_usb)
    {
        if (STATE_USB_HOST == pin_value_usb_host)
        {
            if (set_charger)
            {
                charger_control(FALSE, FALSE);

                if((get_hw_rev() > REV_F) && (get_hw_rev() < REV_1_0) )
                    gpio_set_value(GPIO_THM_VTG_SEL, 0);
            }
            D("[CHG] USB Host - Charger is disabled\n");
            *pstatus = charger_offline;
        }
        else
        {
            if (set_charger)
            {
                charger_control(TRUE, TRUE);

                if((get_hw_rev() > REV_F) && (get_hw_rev() < REV_1_0) )
                    gpio_set_value(GPIO_THM_VTG_SEL, 1);
            }
            D("[CHG] USB - Charger is enabled\n");
#ifdef USE_USB_CHARGING
            *pstatus = charger_usb_online;
#else
            *pstatus = charger_offline;  // charge with usb. but seems not charging (no charge ui)
#endif
        }
    }
    else
    {
        if (set_charger)
        {
            charger_control(FALSE, FALSE);

            if((get_hw_rev() > REV_F) && (get_hw_rev() < REV_1_0) )
                gpio_set_value(GPIO_THM_VTG_SEL, 0);
        }
        D("[CHG] Charger is disabled\n");
        *pstatus = charger_offline;
    }
    if (charger_line != *pstatus)
    {
        charger_line = *pstatus;
        mdelay(300);     // wait until battery temp adc is stabilized
    }
}

static void charger_state_handle(struct work_struct *wq)
{
    D_FUNC("[CHG] %s()\n", __func__);
    //mod_timer(&(batt_dev->battery_poll_timer),
    //    jiffies + msecs_to_jiffies(300)); // 300ms

    //power_supply_changed(&star_supplies[Charger_Type_USB]);
    power_supply_changed(&star_supplies[Charger_Type_AC]);
}

static irqreturn_t charger_interrupt_handler(int irq, void *dev_id)
{
    D_FUNC("[BAT] %s()\n", __func__);
    //struct star_charger_dev* battery_dev = (struct star_charger_dev*)dev_id;
#ifdef CONFIG_HAS_WAKELOCK
#ifdef CONFIG_USB_TEGRA_OTG
	if(!muic_is_host_mode())
#endif
    wake_lock_timeout(&batt_dev->wlock, 350);
#endif

    schedule_work(&batt_dev->work);

    //power_supply_changed(&star_supplies[Charger_Type_USB]);
    //power_supply_changed(&star_supplies[Charger_Type_AC]);

    return IRQ_HANDLED;
}

void battery_set_charge_mode(int value)
{
    D_FUNC("[CHG] %s(%d)\n", __func__, value);
    if (NULL != batt_test)
        batt_test->at_charge_mode = value;
    mod_timer(&(batt_dev->battery_poll_timer), jiffies + msecs_to_jiffies(BATTERY_POLLING_IMMEDIATELY));
}
EXPORT_SYMBOL(battery_set_charge_mode);

int battery_get_charge_mode(void)
{
    if (NULL == batt_test)
		D_FUNC("[CHG] [%s]batt_test is NULL\n", __func__);
	else
	{
		D_FUNC("[CHG] %s(%d)\n", __func__, batt_test->at_charge_mode);
		return batt_test->at_charge_mode;
	}

	return 0;
}
EXPORT_SYMBOL(battery_get_charge_mode);

int battery_get_charge_status(void)
{
    max17040_charge_line charger;

    D_FUNC("[CHG] %s()\n", __func__);

    charger_status(&charger, 1);

    if (charger_ac_online == charger || charger_usb_online == charger)
        return 1;
    else
        return 0;
}
EXPORT_SYMBOL(battery_get_charge_status);

void battery_at_fuelrst(void)
{
    D_FUNC("[CHG] %s()\n", __func__);
    max17040_quickstart(batt_dev->client);

    mod_timer(&batt_dev->battery_poll_timer, jiffies + msecs_to_jiffies(BATTERY_POLLING_IMMEDIATELY));
}
EXPORT_SYMBOL(battery_at_fuelrst);

static int __devinit star_battery_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    int i, rc;
    int error;

    batt_dev = kzalloc(sizeof(*batt_dev), GFP_KERNEL);
    if (!batt_dev) {
        return -ENOMEM;
    }
    memset(batt_dev, 0, sizeof(*batt_dev));

    batt_test = kzalloc(sizeof(*batt_test), GFP_KERNEL);
    if (!batt_test) {
        return -ENOMEM;
    }
    memset(batt_test, 0, sizeof(*batt_test));

    batt_dev->client = client;
    batt_dev->batt_status_poll_period = BATTERY_POLLING_INTERVAL_LOW;

    batt_dev->batt_vol = 3700;
    batt_dev->batt_soc = 50;
    batt_dev->present = 1;      /* Assume battery is present at start */

    INIT_WORK(&batt_dev->work, charger_state_handle);

#ifdef BATTERY_CUSTOM_MODEL
    batt_dev->load_custom_model = false;
#endif

    i2c_set_clientdata(client, batt_dev);

    //max17040_reset(client);
    max17040_get_version(client);

#ifdef CONFIG_HAS_WAKELOCK
	wake_lock_init(&batt_dev->wlock, WAKE_LOCK_SUSPEND, "battery_check");
#endif

    setup_timer(&(batt_dev->battery_poll_timer), star_battery_poll_timer_func, 0);
    mod_timer(&(batt_dev->battery_poll_timer),
        jiffies + msecs_to_jiffies(batt_dev->batt_status_poll_period));

#ifdef BATTERY_CUSTOM_MODEL
    max17040_set_custom_model(batt_dev->client);
#endif
    init_level_check();

    for (i = 0; i < ARRAY_SIZE(star_supplies); i++) {
        rc = power_supply_register(&client->dev, &star_supplies[i]);
        if (rc) {
            printk(KERN_ERR "Failed to register power supply\n");
            while (i--)
                power_supply_unregister(&star_supplies[i]);
            kfree(batt_dev);
            return rc;
        }
    }

    printk(KERN_INFO "[BAT] %s: battery driver registered\n", client->name);

    for (i = 0; i < ARRAY_SIZE(star_battery_attr); i++)
    {
        rc |= device_create_file(&client->dev, &star_battery_attr[i]);
    }

    if (rc) {
        for (i = 0; i < ARRAY_SIZE(star_supplies); i++) {
            power_supply_unregister(&star_supplies[i]);
        }
        del_timer_sync(&(batt_dev->battery_poll_timer));
        pr_err("%s:device_create_file FAILED\n", __func__);
        return rc;
    }

    /*------ Set GPIO configration for charger ------*/
    tegra_gpio_enable(GPIO_CHARGE_ENABLE_N);
    gpio_request_one(GPIO_CHARGE_ENABLE_N, GPIOF_OUT_INIT_HIGH, "charge_enable_n");
    tegra_gpio_enable(GPIO_USB_SUSPEND);
    gpio_request_one(GPIO_USB_SUSPEND, GPIOF_OUT_INIT_LOW, "usb_suspend");
    tegra_gpio_enable(GPIO_CHARGE_STATUS_N);
    gpio_request_one(GPIO_CHARGE_STATUS_N, GPIOF_IN, "charge_status_n");
    tegra_gpio_enable(GPIO_CHARGE_FAULT_N);
    gpio_request_one(GPIO_CHARGE_FAULT_N, GPIOF_IN, "charge_fault_n");
    tegra_gpio_enable(GPIO_AC_DETECT);
    gpio_request_one(GPIO_AC_DETECT, GPIOF_IN, "charge_ac_detect");
    tegra_gpio_enable(GPIO_USB_DETECT_N);
    gpio_request_one(GPIO_USB_DETECT_N, GPIOF_IN, "charge_usb_detect_n");

    //THM_VTG_SEL Gpio control for Thermal ADC reference votlage selection.
    tegra_gpio_enable(GPIO_THM_VTG_SEL);
    gpio_request_one(GPIO_THM_VTG_SEL, GPIOF_OUT_INIT_LOW, "thm_vtg_sel");

    // GPIO interrupt
    error = request_irq(gpio_to_irq(GPIO_AC_DETECT), charger_interrupt_handler,
        (IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING), "ac_detect", (void*)&batt_dev);
    if (error) {
        printk(KERN_ERR "[BAT] interrupt registeration fail! (err:%d)\n", error);
        goto err_request_irq_fail;
    }
    error = request_irq(gpio_to_irq(GPIO_USB_DETECT_N), charger_interrupt_handler,
        (IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING), "usb_detect", (void*)&batt_dev);
    if (error) {
        printk(KERN_ERR "[BAT] interrupt registeration fail! (err:%d)\n", error);
        goto err_request_irq_fail;
    }

    return 0;

err_request_irq_fail:
    free_irq(gpio_to_irq(GPIO_AC_DETECT), batt_dev);
    free_irq(gpio_to_irq(GPIO_USB_DETECT_N), batt_dev);
//Cleanup:
    if (batt_dev) {
        kfree(batt_dev);
        batt_dev = NULL;
    }

    return -1;
}

static int star_battery_remove(struct i2c_client *client)
{
    int i;
    D_FUNC("[BAT] %s()\n", __func__);

    del_timer_sync(&(batt_dev->battery_poll_timer));

    for (i = 0; i < ARRAY_SIZE(star_supplies); i++) {
        power_supply_unregister(&star_supplies[i]);
    }

    if (batt_dev) {
        for (i = 0; i < ARRAY_SIZE(star_battery_attr); i++)
        {
            device_remove_file(&client->dev, &star_battery_attr[i]);
        }
        del_timer_sync(&(batt_dev->battery_poll_timer));
        kfree(batt_dev);
        batt_dev = NULL;
    }

    if (batt_test)
    {
        kfree(batt_test);
        batt_test = NULL;
    }
    free_irq(gpio_to_irq(GPIO_AC_DETECT), batt_dev);
    free_irq(gpio_to_irq(GPIO_USB_DETECT_N), batt_dev);

    return 0;
}

static int star_battery_shutdown(struct i2c_client *client)
{
    D_FUNC("[BAT] %s()\n", __func__);
    del_timer_sync(&(batt_dev->battery_poll_timer));

    disable_irq(gpio_to_irq(GPIO_AC_DETECT));
    disable_irq(gpio_to_irq(GPIO_USB_DETECT_N));

    return 0;
}


//T20 RTC register write function needs busy control.
static int star_rtc_wait_while_busy(void)
{

	int retries = 15;
	while (readl(rtc_base + RTC_BUSY_0) == 1) {
		if (!retries--) {
			D("[CHG] RTC_BUSY_0  is timeout\n");
			return 0;
		}
		msleep(1);
		D("[CHG] RTC_BUSY_0  wait one more 1 mS.\n");
	}
	return 1;
}


static int star_battery_suspend(struct device *dev)
{
	//Low Voltage Cut off function for Battery Only mode
	if ( battery_get_charge_status() == 0)//Battery mode only.
	{

	unsigned long cpu_countdown_alarm_reg = 0x80000000; //Default value: Countdown Enable, No Repeat.
	unsigned long SleepCheckDurationTime = 0;

	int batteryVoltage = batt_dev->batt_vol/1000;

	D("[BAT] Battery Voltage = %dmV\n", batteryVoltage);

	del_timer_sync(&(batt_dev->battery_poll_timer));
	//Battery Votlage Check

	if ((batteryVoltage < 4200) && (batteryVoltage > 3700))
	{
		SleepCheckDurationTime =14400; // Per 4 hours, check the battery.
	}
	else if  ((batteryVoltage <= 3700) && (batteryVoltage > 3500))
	{
		SleepCheckDurationTime =3600; // Per 1 hours, check the battery.
	}
	else if  ((batteryVoltage <= 3500) && (batteryVoltage > 3450))
	{
		SleepCheckDurationTime =900; // Per 15 min, check the battery.
	}
	else if  (batteryVoltage <= 3450)
	{
		SleepCheckDurationTime =300; // Per 5 min, check the battery.
	}

	D("====SleepCheckDurationSet Battery Voltage = %d	 SleepCheckDurationTime(Sec) = %ld ====\n", batteryVoltage, SleepCheckDurationTime);

	//SleepCheckDurationTime = 60; //Stabilty Test only.

	cpu_countdown_alarm_reg = cpu_countdown_alarm_reg | SleepCheckDurationTime;

	D("[CHG] cpu_countdown_alarm_reg = %lX \n" , cpu_countdown_alarm_reg);

	if (star_rtc_wait_while_busy())
	{
		writel(cpu_countdown_alarm_reg, rtc_base + RTC_SECONDS_COUNTDOWN_ALARM0); //Countdown Alarm set with Sleep Duration Time.
		D("[CHG] RTC_SECONDS_COUNTDOWN_ALARM0  writes OK\n");
	}

	if (star_rtc_wait_while_busy()) //Interrupt Status register reset.
	{
		writel ( -1 , rtc_base + RTC_INTR_STATUS_0);
		D("[CHG] RTC_INTR_STATUS_0  writes OK\n");
	}

	if (star_rtc_wait_while_busy()) //Interrupt Mask register set.
	{
		writel (SEC_CDN_ALARM_INTR_SET, rtc_base + RTC_INTR_MASK_0); //SEC_CDN_ALARM interrupt enable - bit 3
		D("[CHG] RTC_INTR_MASK_0  writes OK\n");
	}

	}

    return 0;
}

static int star_battery_resume(struct device *dev)
{
	int bat_voltage;

	//Check battery status right after resume
	star_battery_poll_timer_func(0);
#ifdef CONFIG_HAS_WAKELOCK
	wake_lock_timeout(&batt_dev->wlock, 350);
#endif

	if (star_rtc_wait_while_busy())
	{
		writel (0x0, rtc_base + RTC_INTR_MASK_0); //Clear RTC Countdown alarm interrupt.
		D("[CHG] RTC_INTR_MASK_0  write OK\n");
	}

	if (star_rtc_wait_while_busy())
	{
		writel ( -1 , rtc_base + RTC_INTR_STATUS_0);
		D("[CHG] 0x02c  write OK\n");
	}

	//If the Battery voltage is under 3.4V, System will be shutdown.
	bat_voltage = max17040_get_vcell(batt_dev->client);

	D("[BAT] bat_voltage = %dmV\n", bat_voltage);

	if ( battery_get_charge_status() == 0)
	{
		if ( bat_voltage < 3400)
		{
			printk(KERN_CRIT "[BAT] Under 3.4V, star_system_power_off \n");
			batt_test->test_batt_soc = 0;
			batt_test->test_mode_soc = TRUE;
			mod_timer(&(batt_dev->battery_poll_timer), jiffies + msecs_to_jiffies(BATTERY_POLLING_IMMEDIATELY));
		}
	}

	return 0;
}

static const struct i2c_device_id max17040_id[] = {
    { "star_battery", 0 },
    { }
};
MODULE_DEVICE_TABLE(i2c, max17040_id);

static const struct dev_pm_ops star_battery_pm_ops = {
	.suspend	= star_battery_suspend,
	.resume		= star_battery_resume,
};

static struct i2c_driver max17040_i2c_driver = {
    .driver = {
        .name   = "star_battery",
		.pm		= &star_battery_pm_ops,
    },
    .probe  = star_battery_probe,
    .shutdown   = star_battery_shutdown,
    .remove     = star_battery_remove,
    .id_table   = max17040_id,
};


static int __init star_battery_init(void)
{
    return i2c_add_driver(&max17040_i2c_driver);
}

static void __exit star_battery_exit(void)
{
    i2c_del_driver(&max17040_i2c_driver);
}

module_init(star_battery_init);
module_exit(star_battery_exit);
MODULE_DESCRIPTION("STAR Battery Driver");
