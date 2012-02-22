#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mfd/core.h>
#include <linux/mfd/tps6586x.h>
#include <linux/proc_fs.h>
#include <linux/delay.h>

#include <mach/hardware.h>

#define uint32 unsigned int
#define int16 signed short

/* RGB LED Data */
#define TPS6586x_R50_RGB1FLASH              0x50
#define TPS6586x_R51_RGB1RED                0x51
#define TPS6586x_R52_RGB1GREEN              0x52
#define TPS6586x_R53_RGB1BLUE               0x53

#define TPS6586x_RGB1_SINK_CURRENT_0MA          0x00
#define TPS6586x_RGB1_SINK_CURRENT_3_7MA        0x01
#define TPS6586x_RGB1_SINK_CURRENT_7_4MA        0x02
#define TPS6586x_RGB1_SINK_CURRENT_11_1MA       0x03

#define TPS6586x_RGB1_FLASH_ON_100MS            0x00
#define TPS6586x_RGB1_FLASH_ON_150MS            0x01
#define TPS6586x_RGB1_FLASH_ON_200MS            0x02
#define TPS6586x_RGB1_FLASH_ON_250MS            0x03
#define TPS6586x_RGB1_FLASH_ON_300MS            0x04
#define TPS6586x_RGB1_FLASH_ON_400MS            0x05
#define TPS6586x_RGB1_FLASH_ON_500MS            0x06
#define TPS6586x_RGB1_FLASH_ON_600MS            0x07

#define TPS6586x_RGB1_SINK_CURRENT_REGISTER     TPS6586x_R51_RGB1RED
#define TPS6586x_RGB1_SINK_CURRENT_SHIFT        0x5
#define TPS6586x_RGB1_SINK_CURRENT_MASK         0x3

#define TPS6586x_RGB1_INTENSITY_MASK            0x1F

/* ADC0 Engine Data */
#define TPS6586x_R61_ADC0_SET               0x61
#define TPS6586x_R62_ADC0_WAIT              0x62
#define TPS6586x_R94_ADC0_SUM2              0x94
#define TPS6586x_R95_ADC0_SUM1              0x95
#define TPS6586x_R9A_ADC0_INT               0x9A

#define ADC_CONVERSION_DELAY_USEC      70
#define ADC_CONVERSION_TIMEOUT_USEC    500
#define ADC_CONVERSION_VOLTAGE_RANGE   2000
#define ADC_CONVERSION_DIVIDOR         3
#define ADC_CONVERSION_PRECISION       10
#define ADC_CONVERSION_SUB_OFFSET      2250
#define ADC_FULL_SCALE_READING_MV_BAT  4622
#define ADC_FULL_SCALE_READING_MV_TS   2600
#define ADC_CONVERSION_PREWAIT_MS      26
#define ADC_FULL_SCALE_READING_MV_ANLG 2600

/* System Status */
#define TPS6586x_RBB_STAT3              0xBB

#define BATTERY_DATA_UNKNOWN            0x7FFFFFFF

#define ON  1
#define OFF 0

extern struct device* tps6586x_dev;

bool led_on_state = OFF;


//Battery Temp. ADC converting table
// This table is calculated by NTC R-T Characterisitcs. [yeonhwa.so]
// These values could be changed by Real measurement value.

//  { Temperature , Battery Temp. ADC Voltage from PMIC }

static int16 Temp_adc_ac_table[][2] =
{
			{-99, 2500}, /* In case of no battery */
			{-20, 2026}, {-19, 2015}, {-18, 2003}, {-17, 1991}, {-16, 1978},
			{-15, 1965}, {-14, 1951},{-13, 1937}, {-12, 1922},{-11, 1907},
			{-10, 1891}, {-9, 1875}, {-8, 1858}, {-7, 1840}, {-6, 1822},
			{-5, 1804}, {-4, 1785},	{-3, 1765}, {-2, 1745},	{-1, 1724},
			{0, 1703}, {1, 1679},	{2, 1654}, {3, 1629},	{4, 1604},
			{5, 1578}, {6, 1555},	{7, 1531}, {8, 1507},	{9, 1483},
			{10, 1458}, {11, 1435}, {12, 1412}, {13, 1388}, {14, 1364},
			{15, 1341}, {16, 1317}, {17, 1292}, {18, 1268}, {19, 1243},
			{20, 1217}, {21, 1191}, {22, 1166}, {23, 1141}, {24, 1115},
			{25, 1090}, {26, 1065}, {27, 1040}, {28, 1015}, {29, 989},
			{30, 964}, {31, 940}, {32, 915}, {33, 891}, {34, 867},
			{35, 844}, {36, 821}, {37, 799}, {38, 778}, {39, 756},
			{40, 735}, {41, 715}, {42, 695}, {43, 675}, {44, 656},
			{45, 637}, {46, 619}, {47, 601}, {48, 583}, {49, 566},
			{50, 549}, {51, 533}, {52, 517}, {53, 501}, {54, 486},
			{55, 471}, {56, 457}, {57, 443}, {58, 428}, {59, 414},
			{60, 401}, {61, 388}, {62, 375}, {63, 363}, {64, 351},
			{65, 339}, {66, 329}, {67, 319}, {68, 310}, {69, 301},
			{70, 292}, {71, 284}, {72, 275}, {73, 268}, {74, 260},
			{75, 253}, {76, 246}, {77, 239}, {78, 232}, {79, 226},
			{80, 220}
};


static int16 Temp_adc_bat_table[][2] =
{
			{-99, 2500}, /* In case of no battery */
			{-20, 2147}, {-19, 2136}, {-18, 2123}, {-17, 2110}, {-16, 2097},
			{-15, 2083}, {-14, 2069},{-13, 2054}, {-12, 2038},{-11, 2022},
			{-10, 2005}, {-9, 1987}, {-8, 1970}, {-7, 1951}, {-6, 1932},
			{-5, 1912}, {-4, 1892},	{-3, 1871}, {-2, 1850},	{-1, 1828},
			{0, 1806}, {1, 1781},	{2, 1755}, {3, 1728},	{4, 1701},
			{5, 1674}, {6, 1649},	{7, 1624}, {8, 1599},	{9, 1573},
			{10, 1547}, {11, 1522}, {12, 1498}, {13, 1473}, {14, 1447},
			{15, 1422}, {16, 1396}, {17, 1371}, {18, 1345}, {19, 1318},
			{20, 1291}, {21, 1264}, {22, 1237}, {23, 1210}, {24, 1183},
			{25, 1156}, {26, 1129}, {27, 1103}, {28, 1076}, {29, 1049},
			{30, 1023}, {31, 996}, {32, 971}, {33, 945}, {34, 920},
			{35, 895}, {36, 871}, {37, 848}, {38, 825}, {39, 802},
			{40, 780}, {41, 758}, {42, 737}, {43, 716}, {44, 696},
			{45, 676}, {46, 656}, {47, 637}, {48, 618}, {49, 600},
			{50, 582}, {51, 565}, {52, 548}, {53, 532}, {54, 516},
			{55, 500}, {56, 485}, {57, 469}, {58, 454}, {59, 440},
			{60, 425}, {61, 412}, {62, 398}, {63, 385}, {64, 372},
			{65, 360}, {66, 349}, {67, 339}, {68, 329}, {69, 319},
			{70, 310}, {71, 301}, {72, 292}, {73, 284}, {74, 276},
			{75, 268}, {76, 261}, {77, 253}, {78, 246}, {79, 240},
			{80, 233}
};

/* read voltage from ADC CH10(battery) */
int tps6586x_adc_vbat_sense_read(uint32 *volt)
{
    uint32 timeout = 0;
    uint8_t data_s1 = 0;
    uint8_t data_h = 0;
    uint8_t data_l = 0;

    *volt = 0;    // Default is 0mV.
    // Configuring the adc conversion cycle
    // ADC0_WAIT register(0x62)
    // Reset all ADC engines and return them to the idle state; ADC0_RESET: 1
    if (tps6586x_write(tps6586x_dev, TPS6586x_R62_ADC0_WAIT, 0x80))
        return (-EIO);

    // ADC0_SET register(0x61)
    // ADC0_EN: 0(Don't start conversion); Number of Readings: 16; CHANNEL: CH10(battery)
    if (tps6586x_write(tps6586x_dev, TPS6586x_R61_ADC0_SET, 0x19))
        return (-EIO);

    // ADC0_WAIT register(0x62)
    // REF_EN: 0; AUTO_REF: 1; Wait time: 0.062ms
    if (tps6586x_write(tps6586x_dev, TPS6586x_R62_ADC0_WAIT, 0x21))
        return (-EIO);

    // Start conversion!!
    if (tps6586x_write(tps6586x_dev, TPS6586x_R61_ADC0_SET, 0x99))
        return (-EIO);

    // Wait for conversion
    //udelay(ADC_CONVERSION_DELAY_USEC);
    mdelay(ADC_CONVERSION_PREWAIT_MS);

    // Make sure the conversion is completed - check for ADC error.
    while (1)
    {
        // Read ADC status register
        if (tps6586x_read(tps6586x_dev, TPS6586x_R9A_ADC0_INT, &data_s1))
            return (-EIO);

        // Conversion is done!
        if (data_s1 & 0x80)
            break;

        // ADC error!
        if (data_s1 & 0x40)
        {
            printk("ADC conversion error(#1)\n");
            return (-EIO);
        }

        udelay(ADC_CONVERSION_DELAY_USEC);
        timeout += ADC_CONVERSION_DELAY_USEC;
        if (timeout >= ADC_CONVERSION_TIMEOUT_USEC)
            return (-EIO);
    }

    // Read the ADC conversion Average (SUM).
    if (tps6586x_read(tps6586x_dev, TPS6586x_R94_ADC0_SUM2, &data_h))
        return (-EIO);

    if (tps6586x_read(tps6586x_dev, TPS6586x_R95_ADC0_SUM1, &data_l))
        return (-EIO);

    // Get a result value with mV.
    *volt = (((data_h << 8) | data_l) * ADC_FULL_SCALE_READING_MV_BAT) / 1023 / 16;

    return 0;
}

/* read voltage from ADC CH5(temperature) */
int tps6586x_adc_vbat_temp_read(uint32 *volt)
{
    uint32 timeout = 0;
    uint8_t data_s1 = 0;
    uint8_t data_h = 0;
    uint8_t data_l = 0;

    *volt = 0;    // Default is 0'C.

    // Configuring the adc conversion cycle
    // ADC0_WAIT register(0x62)
    // Reset all ADC engines and return them to the idle state; ADC0_RESET: 1
    if (tps6586x_write(tps6586x_dev, TPS6586x_R62_ADC0_WAIT, 0x80))
        return (-EIO);
    // ADC0_SET register(0x61)
    // ADC0_EN: 0(Don't start conversion); Number of Readings: 16; CHANNEL: CH5(temperature)
    //For Battery Temp. PMIC ADC CH2 read ;Number of Readings: 16;
   // if (tps6586x_write(hDevice, TPS6586x_R61_ADC0_SET, 0x14))
     if (tps6586x_write(tps6586x_dev, TPS6586x_R61_ADC0_SET, 0x11))
        return (-EIO);

    // ADC0_WAIT register(0x62)
    // REF_EN: 0; AUTO_REF: 1; Wait time: 0.062ms
    if (tps6586x_write(tps6586x_dev, TPS6586x_R62_ADC0_WAIT, 0x21))
        return (-EIO);

    // Start conversion!!
    //For Battery Temp. PMIC ADC CH2 read ;Number of Readings: 16;
    //if (tps6586x_write(tps6586x_dev, TPS6586x_R61_ADC0_SET, 0x94))
    if (tps6586x_write(tps6586x_dev, TPS6586x_R61_ADC0_SET, 0x91))
        return (-EIO);

    // Wait for conversion
    // udelay(ADC_CONVERSION_DELAY_USEC);
    mdelay(ADC_CONVERSION_PREWAIT_MS);

    // make sure the conversion is completed, or adc error.
    while (1)
    {
        // Read ADC status register
        if (tps6586x_read(tps6586x_dev, TPS6586x_R9A_ADC0_INT, &data_s1))
            return (-EIO);

        // Conversion is done!
        if (data_s1 & 0x80)
            break;

        // ADC error!
        if (data_s1 & 0x40)
        {
            printk("ADC conversion error(#2)\n");
            return (-EIO);
        }

        udelay(ADC_CONVERSION_DELAY_USEC);
        timeout += ADC_CONVERSION_DELAY_USEC;
        if (timeout >= ADC_CONVERSION_TIMEOUT_USEC)
            return (-EIO);
    }

    // Read the ADC conversion Average (SUM).
    if (tps6586x_read(tps6586x_dev, TPS6586x_R94_ADC0_SUM2, &data_h))
        return (-EIO);

    if (tps6586x_read(tps6586x_dev, TPS6586x_R95_ADC0_SUM1, &data_l))
        return (-EIO);

    // Get a result value with mV.
    *volt = (((data_h << 8) | data_l) *  ADC_FULL_SCALE_READING_MV_TS) / 1023 / 16;

    return 0;
}

/* Calculate the battery temperature */
//20100712, yongtae.jeon, Battery Temp. ADC will be converted by the Temp. ADC Table
int tps6586x_battery_temperature(void)
{
    int i = 0;
    int battery_temp = BATTERY_DATA_UNKNOWN;
    int comp_temp = 0;
    //int temp_offset = 7; //Defined by the Real Measurement
    uint32 bat_temp;
    uint8_t external_power = 0;

    uint8_t data = 0;

    tps6586x_adc_read(ANLG_2, &bat_temp);

    //printk("tps6586x_adc_read  ADC = %d\n", bat_temp);

    if (bat_temp < 200)
        return 0; //Exceptional State :No External Power,  VBatTemp is under 200 mV.

    //External Power check to select ADC table. (AC table or Battery table)
    tps6586x_read(tps6586x_dev, TPS6586x_RBB_STAT3, &data);

    external_power = ((data & 0x8) >> 3);

    //printk("TPS6586x_RBB_STAT3  = %X  external_power = %d \n", data, external_power);

    if (external_power == 1)
    {
        //printk("AcLine_Online Temp_adc_ac_table use\n");

        for (i = 0 ; i<101 ; i++)
        {
            comp_temp = Temp_adc_ac_table[i][1] - bat_temp;
            if ( comp_temp < 0)
            {
                if( comp_temp < ( bat_temp - Temp_adc_ac_table[i-1][1] ) )
                {
                    battery_temp = Temp_adc_ac_table[i-1][0];
                    break;
                }
                else
                {
                    battery_temp = Temp_adc_ac_table[i][0];
                    break;
                }
            }
        }
    }
    else
    {
        //printk("AcLine_Offline Temp_adc_bat_table use\n");
        for (i = 0 ; i<101 ; i++)
        {
            comp_temp = Temp_adc_bat_table[i][1] - bat_temp;
            if (comp_temp < 0)
            {
                if( comp_temp < ( bat_temp - Temp_adc_bat_table[i-1][1] ) )
                {
                    battery_temp = Temp_adc_bat_table[i-1][0];
                    break;
                }
                else
                {
                    battery_temp = Temp_adc_bat_table[i][0];
                    break;
                }
            }
        }
    }

    //printk("battery_temp return = %d C\n", battery_temp);

    return battery_temp;
}

// ANLG1~3(ADC CH1~3) Read function
/* read voltage from ADC CH1~3(ANLG1~3) */
int tps6586x_adc_read(adc_channel adc_ch, int *volt)
{
    uint32 timeout = 0;
    uint8_t data_s1 = 0;
    uint8_t data_h = 0;
    uint8_t data_l = 0;

    *volt = 0;    // Default is 0'C.

    // Configuring the adc conversion cycle
    // ADC0_WAIT register(0x62)
    // Reset all ADC engines and return them to the idle state; ADC0_RESET: 1
    if (tps6586x_write(tps6586x_dev, TPS6586x_R62_ADC0_WAIT, 0x80))
        return (-EIO);
    // ADC0_SET register(0x61)
    // ADC0_EN: 0(Don't start conversion); Number of Readings: 16; CHANNEL: CH1~3 (ANLG1~3)
    if (tps6586x_write(tps6586x_dev, TPS6586x_R61_ADC0_SET, 0x10 | adc_ch))
        return (-EIO);

    // ADC0_WAIT register(0x62)
    // REF_EN: 0; AUTO_REF: 1; Wait time: 0.062ms
    if (tps6586x_write(tps6586x_dev, TPS6586x_R62_ADC0_WAIT, 0x21))
        return (-EIO);

    // Start conversion!!
    if (tps6586x_write(tps6586x_dev, TPS6586x_R61_ADC0_SET, 0x90 | adc_ch))
        return (-EIO);

    // Wait for conversion
    // udelay(ADC_CONVERSION_DELAY_USEC);
    mdelay(ADC_CONVERSION_PREWAIT_MS);

    // make sure the conversion is completed, or adc error.
    while (1)
    {
        // Read ADC status register
        if (tps6586x_read(tps6586x_dev, TPS6586x_R9A_ADC0_INT, &data_s1))
            return (-EIO);

        // Conversion is done!
        if (data_s1 & 0x80)
            break;

        // ADC error!
        if (data_s1 & 0x40)
        {
            printk("ADC conversion error(#3)\n");
            return (-EIO);
        }

        udelay(ADC_CONVERSION_DELAY_USEC);
        timeout += ADC_CONVERSION_DELAY_USEC;
        if (timeout >= ADC_CONVERSION_TIMEOUT_USEC)
            return (-EIO);
    }

    // Read the ADC conversion Average (SUM).
    if (tps6586x_read(tps6586x_dev, TPS6586x_R94_ADC0_SUM2, &data_h))
        return (-EIO);

    if (tps6586x_read(tps6586x_dev, TPS6586x_R95_ADC0_SUM1, &data_l))
        return (-EIO);

    // Get a result value with mV.
    *volt = (((data_h << 8) | data_l) *  ADC_FULL_SCALE_READING_MV_ANLG) / 1023 / 16;

    return 0;
}
EXPORT_SYMBOL(tps6586x_adc_read);

// Read battery voltage from ADC
int tps6586x_adc_read_battery_voltage(int *volt)
{
    int voltage;

    if (tps6586x_adc_read(ANLG_2, &voltage) < 0)
    {
        printk("ADC conversion error(#4)\n");
    }

    *volt = voltage * (5100 + 7500) / 7500;
    //printk("[ADC] %s() ADC :%dmV, Batt:%dmV\n", __func__, voltage, *volt);

    return 0;
}
EXPORT_SYMBOL(tps6586x_adc_read_battery_voltage);

#ifdef CONFIG_MACH_STARTABLET
#if 0 // check hw rev at bootloader only
static struct proc_dir_entry *hwrevproc_root_fp = NULL;

//Write HW rev. to the "/proc" file system_busy
// In the directory, "/proc/hw_rev", this function makes a file that has HW revison name.

struct hw_rev_adc {
    hw_rev hw_rev_type;
    char* hw_rev_name;
    int adc_mv;
    int offset_mv;
};

struct hw_rev_adc hw_rev_values[] =
{
    {REV_C, "Rev.C", 450, 50},
    //{REV_D, "Rev.D", 580, 50},
    {REV_E, "Rev.E", 720, 70},
    {REV_F, "Rev.F", 1050, 70},
    {REV_G, "Rev.G", 910, 70},
    {REV_H, "Rev.H", 1200, 70},
    {REV_I, "Rev.I", 1360, 70},
    {REV_1_0, "Rev_1_0", 1650, 50},
    {REV_1_1, "Rev_1_1", 1790, 70},
    {REV_1_2, "Rev_1_2", 580, 70},
    {REV_1_3, "Rev_1_3", 300, 70},
};

int tps6586x_save_hwrev(void)
{
    uint32 volt;
    int i;
    hw_rev rev;

    if (tps6586x_adc_read(ANLG_3, &volt) < 0)
    {
        printk("ADC conversion error(#5)\n");
    }

    hwrevproc_root_fp = proc_mkdir( "hw_rev", 0 );

    for (i = 0; i < ARRAY_SIZE(hw_rev_values); i++)
    {
        if (volt > hw_rev_values[i].adc_mv - hw_rev_values[i].offset_mv
            && volt < hw_rev_values[i].adc_mv + hw_rev_values[i].offset_mv)
        {
            create_proc_entry( hw_rev_values[i].hw_rev_name, S_IFREG | S_IRWXU, hwrevproc_root_fp );
            rev = hw_rev_values[i].hw_rev_type;
            printk(KERN_INFO "[ADC] %s() HW revision is %s(%d)\n", __func__,   \
                hw_rev_values[i].hw_rev_name, hw_rev_values[i].hw_rev_type);
            return 0;
        }
    }

    create_proc_entry( "Revision_read_error", S_IFREG | S_IRWXU, hwrevproc_root_fp );
    printk(KERN_ERR"[ADC] %s() HW revision read_error (%dmV) \n", __func__, volt);
    return (-EIO);
}
EXPORT_SYMBOL(tps6586x_save_hwrev);
#endif
#endif  // CONFIG_MACH_STARTABLET

#define DEFAULT_RGB1_SYNC_CURRENT       TPS6586x_RGB1_SINK_CURRENT_3_7MA

static int get_rgb(unsigned int color, int* red, int* green, int* blue)
{
    if(red == NULL || green == NULL || blue == NULL)
        return 0;

    *red = ((color >> 16) & 0x00ff) >> 3;
    *green = ((color >> 8) & 0x00ff) >> 3;
    *blue = (color & 0x00ff) >> 3;

    return 0;
}

bool tps6586x_led_check(void)
{
	if (led_on_state == ON)
		return 1;
	else
		return 0;
}


static int tps6586x_led_enable(int led_on)
{
    uint8_t data = 0;

    //printk("[LED] %s() led_type : %d\n", __func__, led_type);

    // Set LED_EN as 1
    if (tps6586x_read(tps6586x_dev, TPS6586x_R52_RGB1GREEN, &data))
        return (-EIO);

    //data_temp = data & (1 << 7);

    //if (!data_temp) // If LED_EN is not set
	if ( led_on == ON)
	{
		if (tps6586x_write(tps6586x_dev, TPS6586x_R52_RGB1GREEN, (led_on << 7) | data))
        	{
            		return (-EIO);
        	}

		led_on_state = ON;

	}
	else if (led_on == OFF)
	{
		if (tps6586x_write(tps6586x_dev, TPS6586x_R52_RGB1GREEN,(~(0x1 << 7)) & data))
		{
			return (-EIO);
		}
		led_on_state = OFF;
	}

    // Set Current SINK
    data = 0;
    if (tps6586x_read(tps6586x_dev, TPS6586x_RGB1_SINK_CURRENT_REGISTER, &data))
        return (-EIO);

    //if (DEFAULT_RGB1_SYNC_CURRENT !=
    //    (data >> TPS6586x_RGB1_SINK_CURRENT_SHIFT) & TPS6586x_RGB1_SINK_CURRENT_MASK)
    {
        if (tps6586x_write(tps6586x_dev, TPS6586x_RGB1_SINK_CURRENT_REGISTER,
            ((DEFAULT_RGB1_SYNC_CURRENT << TPS6586x_RGB1_SINK_CURRENT_SHIFT) |
              ~(TPS6586x_RGB1_SINK_CURRENT_MASK << TPS6586x_RGB1_SINK_CURRENT_SHIFT)) & data))
        {
            return (-EIO);
        }
    }

    return 0;
}

// color=0x00RRGGBB
int tps6586x_led_turn_on(unsigned int color)
{
    int red = 0, green = 0, blue = 0;
	uint8_t reg_red, reg_green, reg_blue;

    //printk("[LED] %s() led_type : %d, On value : %d\n", __func__, led_type, on_value);

    if (WARN_ON(!tps6586x_dev))
        return -EIO;

    if (tps6586x_led_enable(ON))
    {
        printk("[LED] tps6586x_led_enable error\n");
        return (-EIO);
    }
    tps6586x_read(tps6586x_dev, TPS6586x_R51_RGB1RED, &reg_red);
    tps6586x_read(tps6586x_dev, TPS6586x_R52_RGB1GREEN, &reg_green);
    tps6586x_read(tps6586x_dev, TPS6586x_R53_RGB1BLUE, &reg_blue);

    if (color)
    {
        get_rgb(color, &red, &green, &blue);

        // Set Intensity
        tps6586x_write(tps6586x_dev, TPS6586x_R51_RGB1RED, (red | (reg_red & ~TPS6586x_RGB1_INTENSITY_MASK)));
        tps6586x_write(tps6586x_dev, TPS6586x_R52_RGB1GREEN, (green| (reg_green & ~TPS6586x_RGB1_INTENSITY_MASK)));
        tps6586x_write(tps6586x_dev, TPS6586x_R53_RGB1BLUE, (blue | (reg_blue & ~TPS6586x_RGB1_INTENSITY_MASK)));

/*	printk("[LED] red_reg = %x green_reg = %x blue_reg = %x\n",
		(red | (reg_red & ~TPS6586x_RGB1_INTENSITY_MASK)),
		(green| (reg_green & ~TPS6586x_RGB1_INTENSITY_MASK)),
		(blue | (reg_blue & ~TPS6586x_RGB1_INTENSITY_MASK))
		);
*/
    }
    else
    {
        // Turn off LED
        tps6586x_write(tps6586x_dev, TPS6586x_R51_RGB1RED, (reg_red & ~TPS6586x_RGB1_INTENSITY_MASK));
        tps6586x_write(tps6586x_dev, TPS6586x_R52_RGB1GREEN, (reg_green & ~TPS6586x_RGB1_INTENSITY_MASK));
        tps6586x_write(tps6586x_dev, TPS6586x_R53_RGB1BLUE, (reg_blue & ~TPS6586x_RGB1_INTENSITY_MASK));

        if (tps6586x_led_enable(OFF))
            return (-EIO);
    }

    return 0;
}

// color=0x00RRGGBB
int tps6586x_set_led(unsigned int color)
{
    if (tps6586x_led_turn_on(color)) {
        printk("[LED] %s() Failed !!!\n", __func__);
        return -EIO;
    }

    return 0;
}
EXPORT_SYMBOL(tps6586x_set_led);

int tps6586x_set_led_onoff_time(unsigned int on_time_ms, unsigned int off_time_ms)
{
    unsigned int period;
    uint8_t flash_period, flash_on;

    if (WARN_ON(!tps6586x_dev))
        return -EIO;

    period = on_time_ms + off_time_ms;
    //printk("[LED] LED flahsing period=[%d]\n",period);

    if (0 == off_time_ms) // always on
    {
        if (tps6586x_write(tps6586x_dev, TPS6586x_R50_RGB1FLASH, 0xFF))
            return (-EIO);
        return 0;
    }

    // FLASH1_PER Settings - FLASH1_PER[3:0]
    //  0000 1.0s       0001 1.5s       0010 2.0s  ..... 1110   8.0s    1111  always on
    period = (period > 8000) ? 8000 : period;  // if period is over 8.0sec, treat it as 8.0s
    period = (period < 1000) ? 1000 : period;  // if period is under 1.0sec, treat it as 1.0s
    flash_period = period / 500 - 2;
    //printk("[LED] LED flahsing period reg=[%d]\n",flash_period);

    // FLASH1_ON Settings - FLASH1_PER[6:4]
    // 000  0.10s       001 0.15s       010 0.20s   011 0.25s
    // 100  0.30s       101 0.40s       110 0.50s   111 0.60s
    on_time_ms = (on_time_ms > 600) ? 600 : on_time_ms;  // if on_time is over 0.6sec, treat it as 0.6s
    on_time_ms = (on_time_ms < 100) ? 100 : on_time_ms;  // if on_time is over 0.1sec, treat it as 0.1s
    if (on_time_ms < 300)
        flash_on = on_time_ms / 50 - 2;
    else
        flash_on = on_time_ms / 100 + 1;

    //printk("[LED] LED flashing = [%d]\n", (flash_on << 3 | flash_period));
    if (tps6586x_write(tps6586x_dev, TPS6586x_R50_RGB1FLASH, (flash_on << 3 | flash_period)))
        return (-EIO);

    return 0;
}
EXPORT_SYMBOL(tps6586x_set_led_onoff_time);

