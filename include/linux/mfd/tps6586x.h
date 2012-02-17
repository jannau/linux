#ifndef __LINUX_MFD_TPS6586X_H
#define __LINUX_MFD_TPS6586X_H

#define SM0_PWM_BIT 0
#define SM1_PWM_BIT 1
#define SM2_PWM_BIT 2

#ifdef CONFIG_MACH_STARTABLET
//#define LGE_LOAD_SWITCH
#endif

enum {
	TPS6586X_ID_SM_0,
	TPS6586X_ID_SM_1,
	TPS6586X_ID_SM_2,
	TPS6586X_ID_LDO_0,
	TPS6586X_ID_LDO_1,
	TPS6586X_ID_LDO_2,
	TPS6586X_ID_LDO_3,
	TPS6586X_ID_LDO_4,
	TPS6586X_ID_LDO_5,
	TPS6586X_ID_LDO_6,
	TPS6586X_ID_LDO_7,
	TPS6586X_ID_LDO_8,
	TPS6586X_ID_LDO_9,
	TPS6586X_ID_LDO_RTC,

	TPS6586X_ID_SOC_OFF,
#ifdef  LGE_LOAD_SWITCH
	TPS6586X_ID_LDS_USB_HOST,
	TPS6586X_ID_LDS_USB3,
	TPS6586X_ID_LDS_3V3,
	TPS6586X_ID_LDS_TOUCH,
	TPS6586X_ID_LDS_GYRO,
	TPS6586X_ID_LDS_5V0,
	TPS6586X_ID_LDS_3V3_ALWAYS,
#endif
	TPS6586X_ID_DUMMY,
};

enum {
	TPS6586X_INT_PLDO_0,
	TPS6586X_INT_PLDO_1,
	TPS6586X_INT_PLDO_2,
	TPS6586X_INT_PLDO_3,
	TPS6586X_INT_PLDO_4,
	TPS6586X_INT_PLDO_5,
	TPS6586X_INT_PLDO_6,
	TPS6586X_INT_PLDO_7,
	TPS6586X_INT_COMP_DET,
	TPS6586X_INT_ADC,
	TPS6586X_INT_PLDO_8,
	TPS6586X_INT_PLDO_9,
	TPS6586X_INT_PSM_0,
	TPS6586X_INT_PSM_1,
	TPS6586X_INT_PSM_2,
	TPS6586X_INT_PSM_3,
	TPS6586X_INT_RTC_ALM1,
	TPS6586X_INT_ACUSB_OVP,
	TPS6586X_INT_USB_DET,
	TPS6586X_INT_AC_DET,
	TPS6586X_INT_BAT_DET,
	TPS6586X_INT_CHG_STAT,
	TPS6586X_INT_CHG_TEMP,
	TPS6586X_INT_PP,
	TPS6586X_INT_RESUME,
	TPS6586X_INT_LOW_SYS,
	TPS6586X_INT_RTC_ALM2,
};

enum pwm_pfm_mode {
	PWM_ONLY,
	AUTO_PWM_PFM,
	NOT_CONFIGURABLE
};

struct tps6586x_settings {
	/* SM0, SM1 and SM2 have PWM-only and auto PWM/PFM mode */
	enum pwm_pfm_mode sm_pwm_mode;
};

enum {
	TPS6586X_RTC_CL_SEL_1_5PF  = 0x0,
	TPS6586X_RTC_CL_SEL_6_5PF  = 0x1,
	TPS6586X_RTC_CL_SEL_7_5PF  = 0x2,
	TPS6586X_RTC_CL_SEL_12_5PF = 0x3,
};

struct tps6586x_subdev_info {
	int		id;
	const char	*name;
	void		*platform_data;
};

struct tps6586x_epoch_start {
	int year;
	int month;
	int day;
	int hour;
	int min;
	int sec;
};

struct tps6586x_rtc_platform_data {
	int irq;
	struct tps6586x_epoch_start start;
	int cl_sel; /* internal XTAL capacitance, see TPS6586X_RTC_CL_SEL* */
};

struct tps6586x_platform_data {
	int num_subdevs;
	struct tps6586x_subdev_info *subdevs;

	int gpio_base;
	int irq_base;
};

typedef enum {
    ANLG_1 = 0x0,
    ANLG_2 = 0x1,
    ANLG_3 = 0x2,
    ANLG_MAX
} adc_channel;

/*
 * NOTE: the functions below are not intended for use outside
 * of the TPS6586X sub-device drivers
 */
extern int tps6586x_write(struct device *dev, int reg, uint8_t val);
extern int tps6586x_writes(struct device *dev, int reg, int len, uint8_t *val);
extern int tps6586x_read(struct device *dev, int reg, uint8_t *val);
extern int tps6586x_reads(struct device *dev, int reg, int len, uint8_t *val);
extern int tps6586x_write32(struct device *dev, int reg, uint32_t val32);
extern int tps6586x_read32(struct device *dev, int reg, uint32_t *val32);
extern int tps6586x_set_bits(struct device *dev, int reg, uint8_t bit_mask);
extern int tps6586x_clr_bits(struct device *dev, int reg, uint8_t bit_mask);
extern int tps6586x_update(struct device *dev, int reg, uint8_t val,
			   uint8_t mask);
extern int tps6586x_power_off(void);

/* ADC */
extern int tps6586x_battery_temperature(void);
extern int tps6586x_adc_read_battery_voltage(int *volt);
extern int tps6586x_adc_read(adc_channel adc_ch, int *volt);
#ifdef CONFIG_MACH_STARTABLET
//extern int tps6586x_save_hwrev(void);  // check hw rev at bootloader only
#endif

/* LED */
extern int tps6586x_set_led(unsigned int color); // color=0x00RRGGBB
extern int tps6586x_set_led_onoff_time(unsigned int on_time_ms, unsigned int off_time_ms);
extern bool tps6586x_led_check(void);

/* RTC */
extern int tps6586x_rtc_count_read(uint32_t* count);
extern int tps6586x_rtc_count_write(uint32_t count);
extern int tps6586x_rtc_alarm_count_read(uint32_t* count);
extern int tps6586x_rtc_alarm_count_write(uint32_t count);

#endif /*__LINUX_MFD_TPS6586X_H */
