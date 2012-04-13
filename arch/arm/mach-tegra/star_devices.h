
#include <linux/proc_fs.h>
#include <linux/switch.h>

#include <mach/hardware.h>

#include <linux/mfd/wm8994/pdata.h>
#include <linux/regulator/machine.h>
#include <linux/regulator/fixed.h>

#include <linux/mpu.h>
#include <sound/fm31392.h>

#include "gpio-names.h"
/*
***************************************************************************************************
*                                       Get HardWare Revision
***************************************************************************************************
*/
static hw_rev startablet_hw_rev;

typedef struct {
	hw_rev hw_rev_type;
	char* hw_rev_name;
} hw_rev_info;

#define NCT1008_THERM2_GPIO	TEGRA_GPIO_PK2 //THERM2 Interrupt


static hw_rev_info star_hw_rev_values[] = {
	{REV_A, "Rev_A"},
	{REV_C, "Rev_C"},
	{REV_E, "Rev_E"},
	{REV_F, "Rev_F"},
	{REV_G, "Rev_G"},
	{REV_H, "Rev_H"},
	{REV_I, "Rev_I"},
	{REV_1_0, "Rev_1_0"},
	{REV_1_1, "Rev_1_1"},
	{REV_1_2, "Rev_1_2"},
	{REV_1_3, "Rev_1_3"},
	{REV_UNKNOWN, "Unknown"},
};

hw_rev get_hw_rev(void)
{
	//printk(KERN_DEBUG "HW_REV of this board is %d\n\n\n", startablet_hw_rev);
	return startablet_hw_rev;
}

EXPORT_SYMBOL(get_hw_rev);

static int __init tegra_hw_rev_setup(char *line)
{
	int i;
	char board_rev[10];

	strlcpy(board_rev, line, 10);

	for (i = 0; i < ARRAY_SIZE(star_hw_rev_values); i++)
	{
		if (!strncmp(board_rev, star_hw_rev_values[i].hw_rev_name, strlen(board_rev)))
		{
			//printk("[REV] %s() %s = %s(%d)\n", __func__, board_rev, star_hw_rev_values[i].rev_name, i);
			startablet_hw_rev = star_hw_rev_values[i].hw_rev_type;
			return 1;
		}
	}

	// If it doesn't return during for loop, it is problme!!
	printk(KERN_ERR "FAILED!!! board_rev: %s\n", board_rev);
	startablet_hw_rev = REV_UNKNOWN;

	return 1;
}
__setup("hw_rev=", tegra_hw_rev_setup);

static void __init save_hw_rev(void)
{
	int i;
	struct proc_dir_entry *hwrevproc_root_fp = NULL;

	hwrevproc_root_fp = proc_mkdir("hw_rev", 0);

	for (i = 0; i < ARRAY_SIZE(star_hw_rev_values); i++)
	{
		if (get_hw_rev() == star_hw_rev_values[i].hw_rev_type)
		{
			create_proc_entry( star_hw_rev_values[i].hw_rev_name, S_IFREG | S_IRWXU, hwrevproc_root_fp );
			return;
		}
	}

	// If it doesn't return during for loop, it is problme!!
	create_proc_entry("unknown", S_IFREG | S_IRWXU, hwrevproc_root_fp );

}

/*
***************************************************************************************************
*                                       I2C board register
***************************************************************************************************
*/
static int __init star_register_numbered_i2c_devices(int bus_id, struct i2c_board_info const *info, unsigned len)
{
	int err = 0;
	if ((bus_id < 0) || (bus_id > 8)) {
		err = -1;
		goto fail;
	}

	if (info) {
		err = i2c_register_board_info(bus_id , info, len);
		if (err) {
			goto fail;
		}
	}

	printk("[%s] : all devices for I2c bus %d have been registered successfully\n",
		__FUNCTION__, bus_id);

	return err;

fail:
	printk("[%s] : err = %d\n", __FUNCTION__, err);	return err;
};

/*
***************************************************************************************************
*                                       Startablet specific devices
***************************************************************************************************
*/

extern void tegra_throttling_enable(bool enable);

static struct nct1008_platform_data star_nct1008_pdata = {
	.supported_hwrev = true,
	.ext_range = false,
	.conv_rate = 0x08,
	.offset = 0,
	.hysteresis = 0,
	.shutdown_ext_limit = 115,
	.shutdown_local_limit = 120,
	.throttling_ext_limit = 90,
	.alarm_fn = tegra_throttling_enable,
};

static struct platform_device star_powerkey = {
	.name   = "star_powerkey",
	.id     = -1,
	.dev    = {
	},
};

static struct platform_device tegra_gps_gpio = {
	.name   = "tegra_gps_gpio",
	.id     = -1,
	.dev    = {
	},
};

/* unused variable
// for Rev.C
static struct platform_device tegra_gyro_accel = {
	.name   = "tegra_gyro_accel",
	.id     = -1,
	.dev    = {
	},
};
*/

static struct platform_device tegra_misc = {
	.name   = "tegra_misc",
	.id     = -1,
	.dev    = {
	},
};

struct platform_device tegra_leds = {
	.name   = "tegra_leds",
	.id     = -1,
	.dev    = {
	},
};

static struct gpio_switch_platform_data star_headset_data = {
	.name = "h2w",
    .gpio = TEGRA_GPIO_PW6,
};

static struct platform_device star_headset_device = {
	.name		= "star_headset",
	.id		= -1,
	.dev.platform_data = &star_headset_data,
};

struct platform_device tegra_gyroscope_accelerometer = {
	.name   = "tegra_gyro_accel",
	.id     = -1,
	.dev    = {
	},
};
struct platform_device tegra_camera = {
	.name   = "tegra_camera",
	.id     = -1,
	.dev    = {
	},
};

struct platform_device tegra_camera_flash = {
	.name   = "tegra_camera_flash",
	.id     = -1,
	.dev    = {
	},
};

struct platform_device tegra_displaytest =
{
    .name = "tegra_displaytest",
    .id   = -1,
};

#define MPU_ACCEL_BUS_NUM	1
#define MPU_ACCEL_IRQ_GPIO	0
#define MPU_GYRO_BUS_NUM	1
#define MPU_GYRO_IRQ_GPIO	TEGRA_GPIO_PZ4
#define MPU_GYRO_NAME		"mpu3050"
#define MPU_COMPASS_BUS_NUM	1
#define MPU_COMPASS_IRQ_GPIO	TEGRA_GPIO_PN5

static struct mpu3050_platform_data mpu3050_data = {
	.int_config  = 0x10,
	.orientation = {
		1, 0, 0,
		0, 1, 0,
		0, 0, 1
	},
	.level_shifter = 1,
	.accel = {
		.get_slave_descr	= lis331dlh_get_slave_descr,
		.adapt_num		= 1,  //gen2
		.bus			= EXT_SLAVE_BUS_SECONDARY,
		.address		= 0x19,
		.orientation		= {
			 0, 1, 0,
			-1, 0, 0,
			 0, 0, 1
		},
	},
	.compass = {
		.get_slave_descr	= ami304_get_slave_descr,
		.adapt_num		= 1,                    //bus number 3 on ventana
		.bus			= EXT_SLAVE_BUS_PRIMARY,
		.address		= 0x0E,
		.orientation		= {
			 0, -1,  0,
			-1,  0,  0,
			 0,  0, -1
		},
	},
};

/*
***************************************************************************************************
*                                       Startablet Devices & Resources
***************************************************************************************************
*/
/* unused variable
static struct resource pwm_resource[] = {
	[0] = {
		.start	= TEGRA_PWFM_BASE,
		.end	= TEGRA_PWFM_BASE + TEGRA_PWFM_SIZE-1,
		.flags	= IORESOURCE_MEM,
	},
};
*/
/*
***************************************************************************************************
*                                       I2Cs Device Information
***************************************************************************************************
*/
//I2C1 device board information
static struct i2c_board_info __initdata star_i2c_bus1_devices_info[] ={
	{
		I2C_BOARD_INFO("star_muic", STAR_I2C_DEVICE_ADDR_MUIC),
	},
	{
		I2C_BOARD_INFO("mXT1386", STAR_I2C_DEVICE_ADDR_TOUCH),
	},
};

static struct regulator_consumer_supply wm8994_fixed_voltage0_supplies[] = {
        REGULATOR_SUPPLY("DBVDD", "1-001a"),
        REGULATOR_SUPPLY("DCVDD", "1-001a"),
        REGULATOR_SUPPLY("AVDD2", "1-001a"),
        REGULATOR_SUPPLY("CPVDD", "1-001a"),
};

static struct regulator_consumer_supply wm8994_fixed_voltage1_supplies[] = {
        REGULATOR_SUPPLY("AVDD1", "1-001a"),
        REGULATOR_SUPPLY("SPKVDD1", "1-001a"),
        REGULATOR_SUPPLY("SPKVDD2", "1-001a"),
};

static struct regulator_init_data wm8994_fixed_voltage0_init_data = {
        .constraints = {
                .always_on = 1,
        },
        .num_consumer_supplies  = ARRAY_SIZE(wm8994_fixed_voltage0_supplies),
        .consumer_supplies      = wm8994_fixed_voltage0_supplies,
};

static struct regulator_init_data wm8994_fixed_voltage1_init_data = {
        .constraints = {
                .always_on = 1,
        },
        .num_consumer_supplies  = ARRAY_SIZE(wm8994_fixed_voltage1_supplies),
        .consumer_supplies      = wm8994_fixed_voltage1_supplies,
};

static struct fixed_voltage_config wm8994_fixed_voltage0_config = {
        .supply_name    = "VCC_1.8V_PDA",
        .microvolts     = 1100000,
        .gpio           = -EINVAL,
        .init_data      = &wm8994_fixed_voltage0_init_data,
};

static struct fixed_voltage_config wm8994_fixed_voltage1_config = {
        .supply_name    = "V_BAT",
        .microvolts     = 3600000,
        .gpio           = -EINVAL,
        .init_data      = &wm8994_fixed_voltage1_init_data,
};

static struct platform_device wm8994_fixed_voltage0 = {
        .name           = "reg-fixed-voltage",
        .id             = 0,
        .dev            = {
                .platform_data  = &wm8994_fixed_voltage0_config,
        },
};

static struct platform_device wm8994_fixed_voltage1 = {
        .name           = "reg-fixed-voltage",
        .id             = 1,
        .dev            = {
                .platform_data  = &wm8994_fixed_voltage1_config,
        },
};

static struct regulator_consumer_supply wm8994_avdd1_supply =
	REGULATOR_SUPPLY("AVDD1", "1-001a");

static struct regulator_consumer_supply wm8994_dcvdd_supply =
	REGULATOR_SUPPLY("DCVDD", "1-001a");

static struct regulator_init_data wm8994_ldo1_data = {
        .constraints    = {
                .name           = "AVDD1_3.0V",
		.always_on	= 1,
		.max_uV		= 3000000,
		.min_uV		= 3000000,
                .valid_ops_mask = REGULATOR_CHANGE_STATUS,
        },
        .num_consumer_supplies  = 1,
        .consumer_supplies      = &wm8994_avdd1_supply,
};

static struct regulator_init_data wm8994_ldo2_data = {
	.constraints    = {
		.name           = "DCVDD_1.0V",
		.always_on	= 1,
		.max_uV		= 1000000,
		.min_uV		= 1000000,
	},
	.num_consumer_supplies  = 1,
	.consumer_supplies      = &wm8994_dcvdd_supply,
};

static struct wm8994_pdata wm8994_mfd_pdata = {
	.ldo[0] = { 0, NULL, &wm8994_ldo1_data},
	.ldo[1] = { 0, NULL, &wm8994_ldo2_data},
};

//I2C2 device board information
static struct i2c_board_info __initdata star_i2c_bus2_devices_info[] = {
	{
		I2C_BOARD_INFO("wm8994", STAR_I2C_DEVICE_ADDR_WM8994),
		//.platform_data = &wm8994_mfd_pdata,
	},
	{
		I2C_BOARD_INFO("mpu3050", STAR_I2C_DEVICE_ADDR_GYRO),
		.irq = TEGRA_GPIO_TO_IRQ(MPU_GYRO_IRQ_GPIO),
		.platform_data = &mpu3050_data,
	},
	{
		I2C_BOARD_INFO("bh1721_als", STAR_I2C_DEVICE_ADDR_ALS),
	},
};

//I2C3 device board information
static struct i2c_board_info __initdata star_i2c_bus3_devices_info[] = {
	{
		I2C_BOARD_INFO("star_cam_pmic", STAR_I2C_DEVICE_ADDR_CAM_PMIC),
	},
	{
		I2C_BOARD_INFO("imx072", STAR_I2C_DEVICE_ADDR_CAM_IMX072),
	},
	{
		I2C_BOARD_INFO("dw9716", STAR_I2C_DEVICE_ADDR_FOCUSER_DW9716),
	},
	{
		I2C_BOARD_INFO("s5k5bafx", STAR_I2C_DEVICE_ADDR_CAM_S5K5BAFX),
	},
	{
		I2C_BOARD_INFO("imx072_eeprom", STAR_I2C_DEVICE_ADDR_CAM_EEPROM),
	},
};

//I2C4(PWR_I2C) device board information
static struct i2c_board_info __initdata star_i2c_bus4_devices_info[] = {
	{
		I2C_BOARD_INFO("star_battery", STAR_I2C_DEVICE_ADDR_FUEL_GAUGING),
	},
	{
		I2C_BOARD_INFO("nct1008", 0x4C),
		.irq = TEGRA_GPIO_TO_IRQ(NCT1008_THERM2_GPIO),
		.platform_data = &star_nct1008_pdata,
	},
};

static struct i2c_board_info __initdata star_i2c_stereo_camera_info[] = {
	{
		I2C_BOARD_INFO("imx072R", STAR_I2C_DEVICE_ADDR_CAM_IMX072),
	},
	{
		I2C_BOARD_INFO("dw9716R", STAR_I2C_DEVICE_ADDR_FOCUSER_DW9716),
	},
};


#define GPIO_ECHO_BP_N                  TEGRA_GPIO_PJ6
#define GPIO_ECHO_PWDN_N                TEGRA_GPIO_PJ5
#define GPIO_ECHO_RST_N                 TEGRA_GPIO_PU4

static struct fm31392_platform_data star_fm31392_pdata = {
	.gpio_power	= GPIO_ECHO_PWDN_N,
	.gpio_reset	= GPIO_ECHO_RST_N,
	.gpio_bypass	= GPIO_ECHO_BP_N,
};

//I2C-GPIO device board information
static struct i2c_board_info __initdata star_i2c_bus7_echo_info[] = {
	{
		I2C_BOARD_INFO("fm31392", STAR_I2C_GPIO_DEVICE_ADDR_ECHO),
		.platform_data = &star_fm31392_pdata,
	},
};

/*
***************************************************************************************************
*                                       Startablet Poweroff
***************************************************************************************************
*/

int is_modem_connected(void)
{
#define GPIO_SUB_DET_N  TEGRA_GPIO_PX5
    int modem_exist;
    static int init = 1;

    if (init)
    {
        tegra_gpio_enable(GPIO_SUB_DET_N);
        gpio_request_one(GPIO_SUB_DET_N, GPIOF_IN, "sub_modem_detect");
        init = 0;
    }

    if (0 == gpio_get_value(GPIO_SUB_DET_N))
    {
        // Modem exist
        modem_exist = 1;
    }
    else
    {
        // Modem is not exist
        modem_exist = 0;
    }
    //gpio_free(GPIO_SUB_DET_N);

    //printk(KERN_INFO "%s : Detecting modem : %d\n", __func__, modem_exist);
    return modem_exist;
}

