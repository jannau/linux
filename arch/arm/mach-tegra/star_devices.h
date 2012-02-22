
#include <linux/proc_fs.h>

#include <mach/hardware.h>

#include <linux/mpu.h>

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
static struct usb_mass_storage_platform_data andums_plat = {
    .vendor = "LGE",
    .product = "Star Tablet",
    .release = 0x1,
    .nluns = 1,
};

static struct platform_device androidums_device = {
	.name   = "usb_mass_storage",
	.id     = -1,
	.dev    = {
		.platform_data  = &andums_plat,
	},
};

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

static struct mpu3050_platform_data mpu3050_data = {
#if 0
    .int_config  = 0x10,
    .orientation={0,1,0,
		  -1,0,0,
		  0,0,1},
    .level_shifter = 0,
    .accel = {
        .get_slave_descr = lis331dlh_get_slave_descr,
        .adapt_num   = 1,  //gen2
        .bus         = EXT_SLAVE_BUS_SECONDARY,
        .address     = 0x19,
	.orientation = {
			-1, 0, 0,
			0, -1, 0,
			0, 0, 1 },
			},
	.compass = {
		.get_slave_descr	= ami304_get_slave_descr,
		.adapt_num		= 1,	//bus number 3 on ventana
		.bus			= EXT_SLAVE_BUS_PRIMARY,
		.address		= 0x0E,
	.orientation = {
			-1, 0, 0,
			0, 1, 0,
			0, 0, -1 },
    },
};
#else
	.int_config  = 0x10,
	.orientation = {
		1,0,0,
		0,1,0,
		0,0,1
	},
	.level_shifter = 1,
	.accel = {
		.get_slave_descr	= lis331dlh_get_slave_descr,
		.adapt_num		= 1,  //gen2
		.bus			= EXT_SLAVE_BUS_SECONDARY,
		.address		= 0x19,
		.orientation = {
			 0, 1, 0,
			-1, 0, 0,
			 0, 0, 1
		},
	},
	 .compass = {
		.get_slave_descr = ami304_get_slave_descr,
		.adapt_num	 = 1,			 //bus number 3 on ventana
		.bus		 = EXT_SLAVE_BUS_PRIMARY,
		.address	 = 0x0E,
		.orientation = {
			 0, -1,  0,
			-1,  0,  0,
			 0,  0, -1
		},
	},
};

#endif

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
*                                       Startablet SPI devices
***************************************************************************************************
*/
//setting  ttyspi driver
#ifdef CONFIG_SPI_MDM6600
static struct spi_board_info tegra_spi_devices[] __initdata = {
	{
		.modalias = "mdm6600",
		.bus_num = 0,
		.chip_select = 0,
		.mode = SPI_MODE_1,
		.max_speed_hz = 24000000,
		.controller_data = &tegra_spi_device1,
		.irq = 0,
		//	.platform_data = &mdm6600
	},
#ifdef CONFIG_DUAL_SPI //disables spi secondary port
	{
		.modalias = "mdm6600",
		.bus_num = 1,
		.chip_select = 0,
		.mode = SPI_MODE_1,
		.max_speed_hz = 24000000,
		.controller_data = &tegra_spi_device2,
		.irq = 0,
		//	.platform_data = &mdm6600
	},
#endif
};
#else /* CONFIG_SPI_MDM6600 */
static struct spi_board_info tegra_spi_devices[] __initdata = {
{
		.modalias = "ifxn721",
		.bus_num = 0,
		.chip_select = 0,
		.mode = SPI_MODE_1,
		.max_speed_hz = 24000000,
		//.controller_data	= &tegra_spi_device1,
		.irq = 277,//0,//GPIO_IRQ(TEGRA_GPIO_PO5),
		//	.platform_data = &ifxn721
	},
};
#endif /* CONFIG_SPI_MDM6600 */

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

//I2C2 device board information
static struct i2c_board_info __initdata star_i2c_bus2_devices_info[] = {
	{
		I2C_BOARD_INFO("wm8994", STAR_I2C_DEVICE_ADDR_WM8994),
	},
	{
		I2C_BOARD_INFO("mpu3050", STAR_I2C_DEVICE_ADDR_GYRO),		  /*.irq = 299,*/
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

//I2C-GPIO device board information
static struct i2c_board_info __initdata star_i2c_bus7_echo_info[] = {
	{
		I2C_BOARD_INFO("tegra_echo", STAR_I2C_GPIO_DEVICE_ADDR_ECHO),
	},
};

/*
***************************************************************************************************
*                                       Startablet Poweroff
***************************************************************************************************
*/
#if 0	// do not need turn off ldo
#define MAX_REFCOUNT_LOOP   10
static char* poweroff_regs [] = {
    "touch_key",    // LDO0, for Rev.A Rev.A1
    "touch_panel",  // LDO0, for Rev.C~
    "avdd_osc",     // LDO4,
    "avdd_usb_pll",
    "vdd_fuse",     // LDO6,
    "avdd_hdmi",    // LDO7, // ??
    "avdd_hdmi_pll",// LDO8,
    "vdd_ddr_rx",   // LDO9,

    "vdd_5v0",      // VDD_5V0,

#ifdef LGE_LOAD_SWITCH
    "usb_host",
    "usb3_kbd_5v0",
    "vdd_3v3",
    "touch",
    "gyro_1v8",
#endif

    "vdd_sm2",      // DCD2,
};
#endif

static void star_power_off(void)
{
    struct regulator* regul;

#if 0	// do not need turn off ldo
    int i;

    for (i = 0; i < ARRAY_SIZE(poweroff_regs); i++)
    {
        regul = regulator_get(NULL,  poweroff_regs[i]);  /* digital core */
        if (IS_ERR(regul)) {
            printk(KERN_ERR "[PWR OFF] Failed to get regulator(%s)\n", poweroff_regs[i]);
            continue;
        }
        if (regulator_force_disable(regul))
        {
            printk(KERN_ERR "[PWR OFF] Failed to regulator_force_disable regulator(%s)\n", poweroff_regs[i]);
            continue;
        }
    }
#endif
    regul = regulator_get(NULL, "pmic_soc_off");  /* digital core */
    if (IS_ERR(regul)) {
        printk(KERN_ERR "[PWR OFF] Failed to get regulator(PMIC_SOC)\n");
        return;
    }
    regulator_enable(regul);
}

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

