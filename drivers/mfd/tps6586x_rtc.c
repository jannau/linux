#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mfd/core.h>
#include <linux/mfd/tps6586x.h>

#define uint32 unsigned int

// macro OFFSET_BASE_YEAR if 1, uses epoch as reference year instead of 1970
// This is because RTC in PMU TPS6586x can store duration of 34 years,
// else we cannot retain date beyond 2004
#define OFFSET_BASE_YEAR 1
#if OFFSET_BASE_YEAR
static unsigned long epoch = 2009;
#define epoch_end            2037L
static unsigned long epoch_sec = 0;
#endif

#define RTC_LIMITED                     1

/* RTC Registers */
#define TPS6586x_RB3_INT_MASK4              0xB3
#define TPS6586x_RB4_INT_MASK5              0xB4

#define TPS6586x_RC0_RTC_CTRL               0xC0
#define TPS6586x_RC1_RTC_ALARM1_HI          0xC1
#define TPS6586x_RC2_RTC_ALARM1_MID         0xC2
#define TPS6586x_RC3_RTC_ALARM1_LO          0xC3
#define TPS6586x_RC4_RTC_ALARM2_HI          0xC4
#define TPS6586x_RC5_RTC_ALARM2_LO          0xC5
#define TPS6586x_RC6_RTC_COUNT4             0xC6
#define TPS6586x_RC7_RTC_COUNT3             0xC7
#define TPS6586x_RC8_RTC_COUNT2             0xC8
#define TPS6586x_RC9_RTC_COUNT1             0xC9
#define TPS6586x_RCA_RTC_COUNT0             0xCA

static int tps6586x_rtc_is_nopower_start(void);

extern struct device* tps6586x_dev;

static int rtc_alarm_active = 0;      /* RTC_ALARM_ACTIVE */
static int alarm1_used = 0;
static int is_rtc_initialized = 0;

/* Read RTC count register */
int tps6586x_rtc_count_read(uint32* count)
{
    uint32 read_buffer[3];
    // 1) The I2C address pointer must not be left pointing in the range 0xC6 to 0xCA
    // 2) The maximum time for the address pointer to be in this range is 1ms
    // 3) Always read RTC_ALARM2 in the following order to prevent the address pointer
    // from stopping at 0xC6: RTC_ALARM2_LO, then RTC_ALARM2_HI

    if (!tps6586x_rtc_is_nopower_start() && !is_rtc_initialized)
    {
#if RTC_LIMITED
        tps6586x_rtc_count_write((uint32)mktime(epoch,1,1,0,0,0));
        *count = 0;
#else   /* RTC_LIMITED */
        tps6586x_rtc_count_write(0);
        *count = 0;
#endif  /* RTC_LIMITED */
    }
    else
    {
        // The unit of the RTC count is second!!! 1024 tick = 1s.
        // Read all 40 bit and right move 10 = Read the hightest 32bit and right move 2
        tps6586x_read32(tps6586x_dev, TPS6586x_RC6_RTC_COUNT4, (uint8_t*)&read_buffer[0]);

        tps6586x_read(tps6586x_dev, TPS6586x_RCA_RTC_COUNT0, (uint8_t*)&read_buffer[1]);

        tps6586x_read(tps6586x_dev, TPS6586x_RC0_RTC_CTRL, (uint8_t*)&read_buffer[1]);

        // return second
        *count = read_buffer[0]>>2;
    }
#if OFFSET_BASE_YEAR
    // calculate epoch_sec once
    if (!epoch_sec)
        epoch_sec = mktime(epoch,1,1,0,0,0);
    *count += (uint32)epoch_sec;
#endif

    return 0;
}
EXPORT_SYMBOL(tps6586x_rtc_count_read);

/* Write RTC count register */
int tps6586x_rtc_count_write(uint32 count)
{
    uint32 read_buffer = 0;

#if OFFSET_BASE_YEAR
    // calculate epoch_sec once
    if (!epoch_sec)
        epoch_sec = mktime(epoch,1,1,0,0,0);
    if (count < (uint32)epoch_sec)
    {
        // prevent setting date earlier than 'epoch'
        pr_warning("\n Date being set cannot be earlier than least "
            "year=%d. Setting as least year. ", (int)epoch);
        // base year seconds count is 0
#if RTC_LIMITED
        count = (uint32)epoch_sec; //reset RTC count to 2010/01/01 00:00:00
#else   /* RTC_LIMITED */
        count = 0;
#endif  /* RTC_LIMITED */
    }
    else
        count -= (uint32)epoch_sec;

#if RTC_LIMITED
    if (count > (uint32)mktime(epoch_end,12,31,23,59,59))
        count = (uint32)epoch_sec; //reset RTC count to 2010/01/01 00:00:00
#endif  /* RTC_LIMITED */

#endif  /*OFFSET_BASE_YEAR */

    // Switch to 32KHz crystal oscillator
    // POR_SRC_SEL=1 and OSC_SRC_SEL=1
    tps6586x_read(tps6586x_dev, TPS6586x_RC0_RTC_CTRL, (uint8_t*)&read_buffer);
    read_buffer = read_buffer | 0xC0;
    tps6586x_write(tps6586x_dev, TPS6586x_RC0_RTC_CTRL, (uint8_t)read_buffer);

    // To enable incrementing of the RTC_COUNT[39:0] from an initial value set by the host,
    // the RTC_ENABLE bit should be written to 1 only after the RTC_OUT voltage reaches
    // the operating range

    // Clear RTC_ENABLE before writing RTC_COUNT
    tps6586x_read(tps6586x_dev, TPS6586x_RC0_RTC_CTRL, &read_buffer);
    read_buffer = read_buffer & 0xDF;
    tps6586x_write(tps6586x_dev, TPS6586x_RC0_RTC_CTRL, read_buffer);

    tps6586x_write32(tps6586x_dev, TPS6586x_RC6_RTC_COUNT4, &read_buffer);
    tps6586x_write(tps6586x_dev,  TPS6586x_RCA_RTC_COUNT0, 0);

    // Set RTC_ENABLE after writing RTC_COUNT
    tps6586x_read(tps6586x_dev, TPS6586x_RC0_RTC_CTRL, (uint8_t*)&read_buffer);
    read_buffer = read_buffer | 0x20;
    tps6586x_write(tps6586x_dev, TPS6586x_RC0_RTC_CTRL, (uint8_t)read_buffer);

    if (!is_rtc_initialized)
        is_rtc_initialized = 1;

    return 0;
}
EXPORT_SYMBOL(tps6586x_rtc_count_write);

/* Read RTC alarm count register */
int tps6586x_rtc_alarm_count_read(uint32* count)
{
    uint32 counter;
    uint32 read_buffer[3];
    if (rtc_alarm_active) {
        if (alarm1_used) {
            tps6586x_read(tps6586x_dev, TPS6586x_RC3_RTC_ALARM1_LO,       &read_buffer[2]);
            tps6586x_read(tps6586x_dev, TPS6586x_RC2_RTC_ALARM1_MID,      &read_buffer[1]);
            tps6586x_read(tps6586x_dev, TPS6586x_RC1_RTC_ALARM1_HI,       &read_buffer[0]);
            counter = (read_buffer[0] << 16) + (read_buffer[1] << 8) + read_buffer[2];
            *count = counter >> 10;
        } else {        //( alarm1_used )
            tps6586x_read(tps6586x_dev, TPS6586x_RC5_RTC_ALARM2_LO,       &read_buffer[1]);
            tps6586x_read(tps6586x_dev, TPS6586x_RC4_RTC_ALARM2_HI,       &read_buffer[0]);
            counter = (read_buffer[0]<<8) + read_buffer[1];
            *count = counter << 2;
        }               //( alarm1_used )

        return 0;
    } else {    // ( rtc_alarm_active )
        return -EIO;
    }           // ( rtc_alarm_active )

}
EXPORT_SYMBOL(tps6586x_rtc_alarm_count_read);

/* Write RTC alarm count register */
int tps6586x_rtc_alarm_count_write(uint32 count)
{
    uint32 read_buffer32;
    uint32 counter;
    uint32 temp;

    uint32 alarm1_start_sec;
    uint32 alarm1_end_sec;
    uint32 alarm2_start_sec;
    uint32 alarm2_end_sec;

    alarm1_start_sec = 0;       //2^(10-10)-1 = 2^0-1 = 0
    alarm1_end_sec = 16384;     //2^(24-10) = 2^14 =16384 sec = 273 min 4 sec = 4 hr 33min 4 sec

    alarm2_start_sec = 3;       //2^(12-10)-1 = 2^2-1 = 3
    alarm2_end_sec = 262144;    //2^(28-10)= 2^18 = 262144sec = 4369min 4sec = 72hr 49min 4sec = 3day 0hr 49min 4sec

    if ( count < alarm2_end_sec && count > alarm1_start_sec ) {
        rtc_alarm_active = 1;
        if ( count < alarm1_end_sec )
            alarm1_used = 1;
        else
            alarm1_used = 0;
    }

    if (rtc_alarm_active) {
        if (alarm1_used) {
            if (count >= alarm1_end_sec || count <= alarm1_start_sec)
                return -EIO;

            tps6586x_read32(tps6586x_dev, TPS6586x_RC6_RTC_COUNT4, &read_buffer32);
            tps6586x_read(tps6586x_dev,  TPS6586x_RCA_RTC_COUNT0, &temp);
            read_buffer32 &= 0x0000ffff;     //bit[23:08]
            counter = (read_buffer32<<8)+ temp;
            counter += count << 10;

            tps6586x_write(tps6586x_dev, TPS6586x_RC3_RTC_ALARM1_LO,  ((counter >> 0) & 0xFF));
            tps6586x_write(tps6586x_dev, TPS6586x_RC2_RTC_ALARM1_MID, ((counter >> 8) & 0xFF));
            tps6586x_write(tps6586x_dev, TPS6586x_RC1_RTC_ALARM1_HI,  ((counter >> 16) & 0xFF));
            //FIXME:
            tps6586x_read(tps6586x_dev, TPS6586x_RB4_INT_MASK5, &temp);
            temp = temp & 0xEF;
            tps6586x_write(tps6586x_dev, TPS6586x_RB4_INT_MASK5, temp);
        } else {        //( alarm1_used )
            if (count >= alarm2_end_sec || count <= alarm2_start_sec)
                return -EIO;

            tps6586x_read32(tps6586x_dev, TPS6586x_RC6_RTC_COUNT4, &read_buffer32);
            tps6586x_read(tps6586x_dev,  TPS6586x_RCA_RTC_COUNT0, &temp);
            read_buffer32 &= 0x000ffff0;     //bit[27:12]
            counter = read_buffer32 >> 4;
            counter += count >>2; //(count*4sec)

            tps6586x_write(tps6586x_dev, TPS6586x_RC5_RTC_ALARM2_LO,  ((counter >> 0) & 0xFF));
            tps6586x_write(tps6586x_dev, TPS6586x_RC4_RTC_ALARM2_HI,  ((counter >> 8) & 0xFF));
        //FIXME:
            tps6586x_read(tps6586x_dev, TPS6586x_RB3_INT_MASK4, &temp);
            temp = temp & 0xFD;
            tps6586x_write(tps6586x_dev, TPS6586x_RB3_INT_MASK4, temp);

        }               //( alarm1_used )
        return 0;
    } else {    // ( rtc_alarm_active )
        return -EIO;
    }           // ( rtc_alarm_active )

}
EXPORT_SYMBOL(tps6586x_rtc_alarm_count_write);

/* Checks if boot was from nopower / powered state */
static int tps6586x_rtc_is_nopower_start(void)
{
    uint32 Data = 0;

    WARN_ON(!tps6586x_dev);
    if (!tps6586x_dev)
        return -EIO;

    if (!tps6586x_read(tps6586x_dev, TPS6586x_RC0_RTC_CTRL, &Data))
    {
        return ((Data & 0x20)? -EIO : 0);
    }
    return -EIO;
}

