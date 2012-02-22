/*
 * Copyright (C) 2010 NVIDIA, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307, USA
 */
#include <linux/i2c.h>
#include <linux/regulator/machine.h>
#include <linux/mfd/tps6586x.h>
#include <linux/gpio.h>
#include <linux/err.h>
#include <mach/suspend.h>
#include <linux/io.h>

#include <mach/iomap.h>
#include <mach/irqs.h>
#include <mach/hardware.h>

#include "gpio-names.h"
#include "fuse.h"
#include "power.h"
#include "wakeups-t2.h"
#include "board.h"

#define PMC_CTRL		0x0
#define PMC_CTRL_INTR_LOW	(1 << 17)

static struct regulator_consumer_supply tps658621_sm0_supply[] = {
	REGULATOR_SUPPLY("vdd_core", NULL),
};
static struct regulator_consumer_supply tps658621_sm1_supply[] = {
	REGULATOR_SUPPLY("vdd_cpu", NULL),
};
static struct regulator_consumer_supply tps658621_sm2_supply[] = {
	REGULATOR_SUPPLY("vdd_sm2", NULL),
};
static struct regulator_consumer_supply tps658621_ldo0_supply[] = {
	REGULATOR_SUPPLY("bcm", NULL),          // EVB only
	REGULATOR_SUPPLY("touch_key", NULL),    // Rev.A/A1(B)
	REGULATOR_SUPPLY("touch_panel", NULL),  // Rev.C
	REGULATOR_SUPPLY("sensors", NULL),      // Rev.E/F (touch, ambient, accel, compass, gyro)
};
static struct regulator_consumer_supply tps658621_ldo1_supply[] = {
	REGULATOR_SUPPLY("avdd_pll", NULL),
};
static struct regulator_consumer_supply tps658621_ldo2_supply[] = {
	REGULATOR_SUPPLY("vdd_rtc", NULL),
        REGULATOR_SUPPLY("vdd_aon", NULL),
};
static struct regulator_consumer_supply tps658621_ldo3_supply[] = {
	REGULATOR_SUPPLY("avdd_usb", NULL),
	REGULATOR_SUPPLY("avdd_usb_pll", NULL),
};
static struct regulator_consumer_supply tps658621_ldo4_supply[] = {
	REGULATOR_SUPPLY("avdd_osc", NULL),
};
static struct regulator_consumer_supply tps658621_ldo5_supply[] = {
//	REGULATOR_SUPPLY("vmmc", "sdhci-tegra.0"),	// emmc
};
static struct regulator_consumer_supply tps658621_ldo6_supply[] = {
	REGULATOR_SUPPLY("vdd_fuse", NULL),
};
static struct regulator_consumer_supply tps658621_ldo7_supply[] = {
	REGULATOR_SUPPLY("avdd_hdmi", NULL),
};
static struct regulator_consumer_supply tps658621_ldo8_supply[] = {
	REGULATOR_SUPPLY("avdd_hdmi_pll", NULL),
};
static struct regulator_consumer_supply tps658621_ldo9_supply[] = {
	REGULATOR_SUPPLY("vdd_ddr_rx", NULL),
};
static struct regulator_consumer_supply tps658621_soc_off_supply[] = {
	REGULATOR_SUPPLY("pmic_soc_off", NULL),
};
static struct regulator_consumer_supply tps658621_dummy_supply[] = {
		REGULATOR_SUPPLY("DBVDD", NULL),
		REGULATOR_SUPPLY("DCVDD", NULL),
		REGULATOR_SUPPLY("AVDD1", NULL),
		REGULATOR_SUPPLY("AVDD2", NULL),
		REGULATOR_SUPPLY("CPVDD", NULL),
		REGULATOR_SUPPLY("SPKVDD1", NULL),
		REGULATOR_SUPPLY("SPKVDD2", NULL),
};

#ifdef  LGE_LOAD_SWITCH
static struct regulator_consumer_supply tps658621_lds_usb_host_supply[] = {
	REGULATOR_SUPPLY("usb_host", NULL),
};
static struct regulator_consumer_supply tps658621_lds_usb3_supply[] = {
	REGULATOR_SUPPLY("usb3_kbd_5v0", NULL),
};
static struct regulator_consumer_supply tps658621_lds_3v3_supply[] = {
	REGULATOR_SUPPLY("vdd_3v3", NULL),
};
static struct regulator_consumer_supply tps658621_lds_touch_supply[] = {
	REGULATOR_SUPPLY("touch", NULL),
};
static struct regulator_consumer_supply tps658621_lds_gyro_supply[] = {
	REGULATOR_SUPPLY("gyro_1v8", NULL),
};
static struct regulator_consumer_supply tps658621_lds_5v0_supply[] = {
	REGULATOR_SUPPLY("vdd_5v0", NULL),
};
static struct regulator_consumer_supply tps658621_lds_3v3_always_supply[] = {
	REGULATOR_SUPPLY("vdd_3v3_always", NULL),
};
#endif

#define REGULATOR_INIT(_id, _minmv, _maxmv)				\
	{								\
		.constraints = {					\
			.min_uV = (_minmv)*1000,			\
			.max_uV = (_maxmv)*1000,			\
			.valid_modes_mask = (REGULATOR_MODE_NORMAL |	\
					     REGULATOR_MODE_STANDBY),	\
			.valid_ops_mask = (REGULATOR_CHANGE_MODE |	\
					   REGULATOR_CHANGE_STATUS |	\
					   REGULATOR_CHANGE_VOLTAGE),	\
		},							\
		.num_consumer_supplies = ARRAY_SIZE(tps658621_##_id##_supply),\
		.consumer_supplies = tps658621_##_id##_supply,		\
	}

static struct regulator_init_data sm0_data = REGULATOR_INIT(sm0, 725, 1500);
static struct regulator_init_data sm1_data = REGULATOR_INIT(sm1, 725, 1500);
static struct regulator_init_data sm2_data = REGULATOR_INIT(sm2, 3000, 4550);
static struct regulator_init_data ldo0_data = REGULATOR_INIT(ldo0, 1250, 3300);
static struct regulator_init_data ldo1_data = REGULATOR_INIT(ldo1, 725, 1500);
static struct regulator_init_data ldo2_data = REGULATOR_INIT(ldo2, 725, 1500);
static struct regulator_init_data ldo3_data = REGULATOR_INIT(ldo3, 1250, 3300);
static struct regulator_init_data ldo4_data = REGULATOR_INIT(ldo4, 1700, 2475);
static struct regulator_init_data ldo5_data = REGULATOR_INIT(ldo5, 1250, 3300);
static struct regulator_init_data ldo6_data = REGULATOR_INIT(ldo6, 1250, 3300);
static struct regulator_init_data ldo7_data = REGULATOR_INIT(ldo7, 1250, 3300);
static struct regulator_init_data ldo8_data = REGULATOR_INIT(ldo8, 1250, 3300);
static struct regulator_init_data ldo9_data = REGULATOR_INIT(ldo9, 1250, 3300);
static struct regulator_init_data soc_off_data = REGULATOR_INIT(soc_off, 1250, 3300);
static struct regulator_init_data dummy_data = REGULATOR_INIT(dummy, 1250, 5000);
#ifdef  LGE_LOAD_SWITCH
static struct regulator_init_data lds_usb_host_data = REGULATOR_INIT(lds_usb_host, 5000, 5000);
static struct regulator_init_data lds_usb3_data = REGULATOR_INIT(lds_usb3, 5000, 5000);
static struct regulator_init_data lds_3v3_data = REGULATOR_INIT(lds_3v3, 3300, 3300);
static struct regulator_init_data lds_touch_data = REGULATOR_INIT(lds_touch, 3300, 3300);
static struct regulator_init_data lds_gyro_data = REGULATOR_INIT(lds_gyro, 1800, 5000);
static struct regulator_init_data lds_5v0_data = REGULATOR_INIT(lds_5v0, 5000, 5000);
static struct regulator_init_data lds_3v3_always_data = REGULATOR_INIT(lds_3v3_always, 3300, 3300);
#endif

static struct tps6586x_rtc_platform_data rtc_data = {
	.irq = TEGRA_NR_IRQS + TPS6586X_INT_RTC_ALM1,
	.start = {
		.year = 2009,
		.month = 1,
		.day = 1,
	},
	.cl_sel = TPS6586X_RTC_CL_SEL_1_5PF /* use lowest (external 20pF cap) */
};

#define TPS_REG(_id, _data)			\
	{					\
		.id = TPS6586X_ID_##_id,	\
		.name = "tps6586x-regulator",	\
		.platform_data = _data,		\
	}

static struct tps6586x_subdev_info tps_devs[] = {
	TPS_REG(SM_0, &sm0_data),
	TPS_REG(SM_1, &sm1_data),
	TPS_REG(SM_2, &sm2_data),
	TPS_REG(LDO_0, &ldo0_data),
	TPS_REG(LDO_1, &ldo1_data),
	TPS_REG(LDO_2, &ldo2_data),
	TPS_REG(LDO_3, &ldo3_data),
	TPS_REG(LDO_4, &ldo4_data),
	TPS_REG(LDO_5, &ldo5_data),
	TPS_REG(LDO_6, &ldo6_data),
	TPS_REG(LDO_7, &ldo7_data),
	TPS_REG(LDO_8, &ldo8_data),
	TPS_REG(LDO_9, &ldo9_data),
	TPS_REG(SOC_OFF, &soc_off_data),
	TPS_REG(DUMMY, &dummy_data),
#ifdef  LGE_LOAD_SWITCH
	TPS_REG(LDS_USB_HOST, &lds_usb_host_data),
	TPS_REG(LDS_USB3, &lds_usb3_data),
	TPS_REG(LDS_3V3, &lds_3v3_data),
	TPS_REG(LDS_TOUCH, &lds_touch_data),
	TPS_REG(LDS_GYRO, &lds_gyro_data),
	TPS_REG(LDS_5V0, &lds_5v0_data),
	TPS_REG(LDS_3V3_ALWAYS, &lds_3v3_always_data),
#endif
	{
		.id	= 0,
		.name	= "tps6586x-rtc",
		.platform_data = &rtc_data,
	},
};

static struct tps6586x_platform_data tps_platform = {
	.irq_base = TEGRA_NR_IRQS,
	.num_subdevs = ARRAY_SIZE(tps_devs),
	.subdevs = tps_devs,
	.gpio_base = TEGRA_NR_GPIOS,
};

static struct i2c_board_info __initdata star_regulators[] = {
	{
		I2C_BOARD_INFO("tps6586x", 0x34),
		.irq		= INT_EXTERNAL_PMU,
		.platform_data = &tps_platform,
	},
};

#define STAR_WAKE_POWER_KEY_HIGH    TEGRA_WAKE_GPIO_PV2
#define STAR_WAKE_AC_DET_ANY        TEGRA_WAKE_GPIO_PU5
#define STAR_WAKE_USB_DET_ANY       TEGRA_WAKE_GPIO_PQ6
#define STAR_WAKE_WLAN_HOST_WAKEUP  TEGRA_WAKE_GPIO_PY6
#define STAR_WAKE_BT_HOST_WAKEUP    TEGRA_WAKE_GPIO_PC7
#define STAR_WAKE_CPU_RTC_ALARM TEGRA_WAKE_RTC_ALARM

static struct tegra_suspend_platform_data star_suspend = {
	/*
	 * Check power on time and crystal oscillator start time
	 * for appropriate settings.
	 */
       .cpu_timer = 5000,
       .cpu_off_timer = 5000,
       .suspend_mode = TEGRA_SUSPEND_LP0,
       .core_timer = 0x7e7e,
       .core_off_timer = 0x7f,
       .separate_req = true,
       .corereq_high = false,
       .sysclkreq_high = true,
       .wake_enb = STAR_WAKE_POWER_KEY_HIGH
               | STAR_WAKE_WLAN_HOST_WAKEUP
               | STAR_WAKE_AC_DET_ANY | STAR_WAKE_USB_DET_ANY | STAR_WAKE_CPU_RTC_ALARM,
       .wake_high = STAR_WAKE_POWER_KEY_HIGH | STAR_WAKE_CPU_RTC_ALARM,
       .wake_low = STAR_WAKE_WLAN_HOST_WAKEUP,
       .wake_any = STAR_WAKE_AC_DET_ANY | STAR_WAKE_USB_DET_ANY,
};

int __init star_regulator_init(void)
{
	void __iomem *pmc = IO_ADDRESS(TEGRA_PMC_BASE);
	void __iomem *chip_id = IO_ADDRESS(TEGRA_APB_MISC_BASE) + 0x804;
	u32 pmc_ctrl;
	u32 minor;

	minor = (readl(chip_id) >> 16) & 0xf;
	/* A03 (but not A03p) chips do not support LP0 (<=Rev.C) */
	if (minor == 3 && !(tegra_spare_fuse(18) || tegra_spare_fuse(19)))
		star_suspend.suspend_mode = TEGRA_SUSPEND_LP1;

	/* configure the power management controller to trigger PMU
	 * interrupts when low */
	pmc_ctrl = readl(pmc + PMC_CTRL);
	writel(pmc_ctrl | PMC_CTRL_INTR_LOW, pmc + PMC_CTRL);
	i2c_register_board_info(4, star_regulators, 1);
	tegra_init_suspend(&star_suspend);
	return 0;
}
