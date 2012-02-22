/*
 * arch/arm/mach-tegra/board-ventana.c
 *
 * Copyright (c) 2010, NVIDIA Corporation.
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

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/ctype.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/serial_8250.h>
#include <linux/i2c.h>
#include <linux/dma-mapping.h>
#include <linux/delay.h>
#include <linux/i2c-tegra.h>
#include <linux/gpio.h>
#include <linux/gpio_keys.h>
#include <linux/input.h>
#include <linux/tegra_usb.h>
#include <linux/usb/android_composite.h>
#ifdef CONFIG_USB_ANDROID_ACCESSORY
#include <linux/usb/f_accessory.h>
#endif
#include <linux/memblock.h>
#include <mach/clk.h>
#include <mach/iomap.h>
#include <mach/irqs.h>
#include <mach/pinmux.h>
#include <mach/iomap.h>
#include <mach/io.h>
#include <mach/i2s.h>
#include <mach/spdif.h>
#include <mach/audio.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <mach/usb_phy.h>

#include <linux/regulator/consumer.h>
#include <linux/pwm_backlight.h>
#include <linux/fsl_devices.h>
#include <linux/spi/spi.h>
#include <linux/nct1008.h>
#include <linux/i2c-gpio.h>
#include <linux/tegra_audio.h>
#include <mach/tegra_lm1_bl.h>
#include <mach/gpio-names.h>
#include <mach/suspend.h>
#include <mach/hardware.h>
#include <asm/setup.h>
#include <mach/tegra_das.h>

#include "board.h"
#include "clock.h"
#include "board-star.h"
#include "devices.h"
#include "gpio-names.h"
#include "fuse.h"
#include "power.h"
#include "wakeups-t2.h"

#if defined (CONFIG_MACH_STARTABLET)
#include "star_i2c_device_address.h"
#include "star_devices.h"
#endif

#define CARVEOUT_320MB	1	// 1: 320MB, 0: 256MB (default)

static struct plat_serial8250_port debug_uart_platform_data[] = {
	{
		.membase	= IO_ADDRESS(TEGRA_UARTB_BASE),
		.mapbase	= TEGRA_UARTB_BASE,
		.irq		= INT_UARTB,
		.flags		= UPF_BOOT_AUTOCONF,
		.iotype		= UPIO_MEM,
		.regshift	= 2,
		.uartclk	= 216000000,
	}, {
		.flags		= 0
	}
};

static struct platform_device debug_uart = {
	.name = "serial8250",
	.id = PLAT8250_DEV_PLATFORM,
	.dev = {
		.platform_data = debug_uart_platform_data,
	},
};

static struct tegra_audio_platform_data tegra_spdif_pdata = {
	//.master = true,
	.dma_on = true,  /* use dma by default */
	.i2s_clk_rate = 5644800,
	.mode = SPDIF_BIT_MODE_MODE16BIT,
	.fifo_fmt = 0,
};
static struct tegra_utmip_config utmi_phy_config[] = {
	[0] = {
			.hssync_start_delay = 0,
			.idle_wait_delay = 17,
			.elastic_limit = 16,
			.term_range_adj = 6,
			.xcvr_setup = 15,
			.xcvr_lsfslew = 2,
			.xcvr_lsrslew = 2,
	},
	[1] = {
			.hssync_start_delay = 0,
			.idle_wait_delay = 17,
			.elastic_limit = 16,
			.term_range_adj = 6,
			.xcvr_setup = 8,
			.xcvr_lsfslew = 2,
			.xcvr_lsrslew = 2,
	},
};

static struct tegra_ulpi_config ulpi_phy_config = {
	.reset_gpio = TEGRA_GPIO_PG2,
	.clk = "clk_dev2",
};
static __initdata struct tegra_clk_init_table star_clk_init_table[] = {
	/* name		parent		rate		enabled */
	{ "uartb",	"pll_p",	216000000,	true},
	{ "pll_m",	"clk_m",	600000000,	true},
	{ "uartc",      "pll_m",        600000000,      false},
	{ "sdmmc4",	"pll_p",	52000000,	false},
	//clock setting for spi1 and spi2 port
	{ "sbc1",	"pll_p",	96000000,	true},
	//{ "sbc2",	"pll_p",	96000000,	true}, //disables spi secondary port
	//clock setting for spi1 and spi2 port
	{ "blink",	"clk_32k",	32768,		true},
	{ "pll_a",	NULL,		11289600,	true},
	{ "pll_a_out0",	NULL,		11289600,	true},
	{ "i2s1",	"pll_a_out0",	11289600,	true},
	{ "i2s2",	"pll_a_out0",	11289600,	true},
	{ "audio",	"pll_a_out0",	11289600,	true},
	{ "audio_2x",	"audio",	22579200,	true},
	{ "spdif_out",	"pll_a_out0",	5644800,	false},
	{ "clk_dev1",	NULL,		26000000,	true},
	{ "pwm",	"clk_32k",	32768,		false},
	{ NULL,		NULL,		0,		0},
};


#ifdef CONFIG_USB_ANDROID_LGE_GADGET
static char *usb_functions_lge_mobile_modem_driver[] = { "lge_acm", "lge_serial" };

/* USB mode changed by enabling/disabling adb or rndis function */
static char *usb_functions_lge_mtp_driver[] = { "mtp" };
static char *usb_functions_lge_android_mtp_adb_driver[] = { "mtp", "adb" };

static char *usb_functions_lge_android_modem_mtp_adb_driver[] = { "lge_android_acm", "lge_android_serial", "mtp", "adb" };
static char *usb_functions_lge_android_modem_mtp_driver[] = { "lge_android_acm", "lge_android_serial", "mtp" };

#ifdef CONFIG_USB_ANDROID_RNDIS
static char *usb_functions_lge_androidnet_adb_driver[] = { "lge_android_acm", "lge_android_serial", "rndis", "adb" };
static char *usb_functions_lge_androidnet_driver[] = { "lge_android_acm", "lge_android_serial", "rndis" };
#endif

#ifdef CONFIG_USB_ANDROID_ACCESSORY
static char *usb_functions_accessory[] = { "accessory" };
static char *usb_functions_accessory_adb[] = { "accessory", "adb" };
#endif

static char *usb_functions_lge_android_driver_all[] = {
	"lge_android_acm",
	"lge_android_serial",
#ifdef CONFIG_USB_ANDROID_RNDIS
	"rndis",
#endif
	"mtp",
#ifdef CONFIG_USB_ANDROID_ACCESSORY
	"accessory",
#endif
	"adb"
};

static struct android_usb_product usb_products[] = {
	{
		.vendor_id      = 0x1004,
		.product_id     = 0x61D5,
		.num_functions  = ARRAY_SIZE(usb_functions_lge_mtp_driver),
		.functions      = usb_functions_lge_mtp_driver,
	},
	{
		.vendor_id      = 0x1004,
		.product_id     = 0x61F9,
		.num_functions  = ARRAY_SIZE(usb_functions_lge_android_mtp_adb_driver),
		.functions      = usb_functions_lge_android_mtp_adb_driver,
	},
#ifdef CONFIG_USB_ANDROID_RNDIS
	{
		.vendor_id      = 0x1004,
		.product_id     = 0x61FE,
		.num_functions  = ARRAY_SIZE(usb_functions_lge_androidnet_adb_driver),
		.functions      = usb_functions_lge_androidnet_adb_driver,
	},
	{
		.vendor_id      = 0x1004,
		.product_id     = 0x61FE,
		.num_functions  = ARRAY_SIZE(usb_functions_lge_androidnet_driver),
		.functions      = usb_functions_lge_androidnet_driver,
	},
#endif
#ifdef CONFIG_USB_ANDROID_ACCESSORY
	{
		.vendor_id	= USB_ACCESSORY_VENDOR_ID,
		.product_id	= USB_ACCESSORY_PRODUCT_ID,
		.num_functions	= ARRAY_SIZE(usb_functions_accessory),
		.functions	= usb_functions_accessory,
	},
	{
		.vendor_id	= USB_ACCESSORY_VENDOR_ID,
		.product_id	= USB_ACCESSORY_ADB_PRODUCT_ID,
		.num_functions	= ARRAY_SIZE(usb_functions_accessory_adb),
		.functions	= usb_functions_accessory_adb,
	},
#endif
};

/* standard android USB platform data */
static struct android_usb_platform_data andusb_plat = {
	.vendor_id                      = 0x1004,
	.product_id                     = 0x61D5,
	.manufacturer_name      = "LG Electronics, Inc.",
	.product_name               = "LG Android USB Device",
	.num_products = ARRAY_SIZE(usb_products),
	.products = usb_products,
	.num_functions = ARRAY_SIZE(usb_functions_lge_android_driver_all),
	.functions = usb_functions_lge_android_driver_all,
};
#else
static char *usb_functions[] = { "usb_mass_storage" };
static char *usb_functions_adb[] = { "usb_mass_storage", "adb" };

static struct android_usb_product usb_products[] = {
	{
		.product_id     = 0xDEAD,
		.num_functions  = ARRAY_SIZE(usb_functions),
		.functions      = usb_functions,
	},
	{
		.product_id     = 0x7100,
		.num_functions  = ARRAY_SIZE(usb_functions_adb),
		.functions      = usb_functions_adb,
	},
};

/* standard android USB platform data */
static struct android_usb_platform_data andusb_plat = {
	.vendor_id                      = 0x0955,
	.product_id                     = 0x7000,
	.manufacturer_name      = "NVIDIA Corporation",
	.product_name               = "ADB Composite Device",
	.num_products = ARRAY_SIZE(usb_products),
	.products = usb_products,
	.num_functions = ARRAY_SIZE(usb_functions_adb),
	.functions = usb_functions_adb,
};
#endif

int muic_path = 0;
int muic_status = 0x0B;

#define USB_SERIAL_LENGTH 20
static int __init tegra_usb_serial_setup(char *line)
{
       static char usb_serial_buf[USB_SERIAL_LENGTH];

       andusb_plat.serial_number = usb_serial_buf;

	strlcpy(andusb_plat.serial_number, line, USB_SERIAL_LENGTH);
	return 1;
}

__setup("usb_serial=", tegra_usb_serial_setup);

static struct platform_device androidusb_device = {
	.name   = "android_usb",
	.id     = -1,
	.dev    = {
		.platform_data  = &andusb_plat,
	},
};
/*
***************************************************************************************************
*                                       I2C platform_data
***************************************************************************************************
*/
static struct tegra_i2c_platform_data star_i2c1_platform_data = {
	.adapter_nr	= 0,
	.bus_count	= 1,
	.bus_clk_rate	= { 400000, 0 },
};

static const struct tegra_pingroup_config i2c2_ddc = {
	.pingroup	= TEGRA_PINGROUP_DDC,
	.func		= TEGRA_MUX_I2C2,
};

static const struct tegra_pingroup_config i2c2_gen2 = {
	.pingroup	= TEGRA_PINGROUP_PTA,
	.func		= TEGRA_MUX_I2C2,
};

static struct tegra_i2c_platform_data star_i2c2_platform_data = {
	.adapter_nr	= 1,
	.bus_count	= 2,
	.bus_clk_rate	= { 400000, 100000 },
	.bus_mux	= { &i2c2_gen2, &i2c2_ddc },
	.bus_mux_len	= { 1, 1 },
};

static struct tegra_i2c_platform_data star_i2c3_platform_data = {
	.adapter_nr	= 3,
	.bus_count	= 1,
	.bus_clk_rate	= { 400000, 0 },
};

static struct tegra_i2c_platform_data star_dvc_platform_data = {
	.adapter_nr	= 4,
	.bus_count	= 1,
	.bus_clk_rate	= { 100000, 0 },
	.is_dvc		= true,
};

static struct tegra_audio_platform_data tegra_audio_pdata[] = {
	/* For I2S1 */
	[0] = {
		.i2s_master	= true,
		.dsp_master	= false,
		.dma_on		= true,  /* use dma by default */
		.i2s_master_clk = 44100,
		.dsp_master_clk = 8000,
		.i2s_clk_rate	= 24000000,
		.dap_mclk	= "pll_a_out0",
		.dap_clk	= "clk_dev1",
		.audio_sync_clk = "audio_2x",
		.mode		= I2S_BIT_FORMAT_I2S,
		.fifo_fmt	= I2S_FIFO_PACKED,
		.bit_size	= I2S_BIT_SIZE_16,
		.i2s_bus_width = 32,
		.dsp_bus_width = 16,
	},
	/* For I2S2 */
	[1] = {
		.i2s_master	= false,
		.dsp_master	= true,
		.dma_on		= true,  /* use dma by default */
		.i2s_master_clk = 44100,
		.dsp_master_clk = 8000,
		.i2s_clk_rate	= 24000000,
		.dap_mclk	= "pll_a_out0",
		.dap_clk	= "clk_dev1",
		.audio_sync_clk = "audio_2x",
		.mode		= I2S_BIT_FORMAT_DSP,
		.fifo_fmt	= I2S_FIFO_16_LSB,
		.bit_size	= I2S_BIT_SIZE_16,
		.i2s_bus_width = 32,
		.dsp_bus_width = 16,
	}
};

static struct tegra_das_platform_data tegra_das_pdata = {
	.tegra_dap_port_info_table = {
		[0] = {
			.dac_port = tegra_das_port_none,
			.codec_type = tegra_audio_codec_type_none,
			.device_property = {
				.num_channels = 0,
				.bits_per_sample = 0,
				.rate = 0,
				.dac_dap_data_comm_format = 0,
			},
		},
		/* I2S1 <--> DAC1 <--> DAP1 <--> Hifi Codec */
		[1] = {
			.dac_port = tegra_das_port_i2s1,
			.codec_type = tegra_audio_codec_type_hifi,
			.device_property = {
				.num_channels = 2,
				.bits_per_sample = 16,
				.rate = 44100,
				.dac_dap_data_comm_format = dac_dap_data_format_i2s,
			},
		},
		[2] = {
			.dac_port = tegra_das_port_none,
			.codec_type = tegra_audio_codec_type_none,
			.device_property = {
				.num_channels = 0,
				.bits_per_sample = 0,
				.rate = 0,
				.dac_dap_data_comm_format = 0,
			},
		},
		[3] = {
			.dac_port = tegra_das_port_none,
			.codec_type = tegra_audio_codec_type_none,
			.device_property = {
				.num_channels = 0,
				.bits_per_sample = 0,
				.rate = 0,
				.dac_dap_data_comm_format = 0,
			},
		},
		[4] = {
			/* I2S2 <--> DAC2 <--> DAP4 <--> Bluetooth */
			.dac_port = tegra_das_port_i2s2,
			.codec_type = tegra_audio_codec_type_bluetooth,
			.device_property = {
				.num_channels = 1,
				.bits_per_sample = 16,
				.rate = 8000,
				.dac_dap_data_comm_format = dac_dap_data_format_pcm,
			},
		},
	},

	.tegra_das_con_table = {
		[0] = {
			.con_id = tegra_das_port_con_id_hifi,
			.num_entries = 2,
			.con_line = {
				[0] = {tegra_das_port_i2s1, tegra_das_port_dap1, true},
				[1] = {tegra_das_port_dap1, tegra_das_port_i2s1, false},
			},
		},
		[1] = {
			.con_id = tegra_das_port_con_id_bt_codec,
			.num_entries = 2,
			.con_line = {
				[0] = {tegra_das_port_i2s2, tegra_das_port_dap4, true},
				[1] = {tegra_das_port_dap4, tegra_das_port_i2s2, false},
			},
		},
	}
};

static struct i2c_gpio_platform_data i2c_gpio_data = {
	.sda_pin		= TEGRA_GPIO_PH1,
	.scl_pin		= TEGRA_GPIO_PI5,
	.sda_is_open_drain	= 0,
	.scl_is_open_drain	= 0,
	.udelay			= 5, 	/* cloase to 100kHz */
	.timeout		= HZ/10,
};

static struct platform_device i2c_gpio_controller = {
	.name	= "i2c-gpio",
	.id	= 7,
	.dev	= {
	  .platform_data = &i2c_gpio_data,
	},
};
static struct i2c_gpio_platform_data stereocam_i2c_gpio_data = {
	.sda_pin		= TEGRA_GPIO_PG1,
	.scl_pin		= TEGRA_GPIO_PH0,
	.sda_is_open_drain	= 0,
	.scl_is_open_drain	= 0,
	.udelay			= 2, 	/* cloase to 100kHz */
	.timeout		= 2000,
};

static struct platform_device stereocam_i2c_gpio_controller = {
	.name	= "i2c-gpio",
	.id	= 6,
	.dev	= {
	  .platform_data = &stereocam_i2c_gpio_data,
	},
};
// #4
static void star_i2c_init(void)
{
	hw_rev board_rev=0;
	board_rev = get_hw_rev();
	tegra_i2c_device1.dev.platform_data = &star_i2c1_platform_data;
	tegra_i2c_device2.dev.platform_data = &star_i2c2_platform_data;
	tegra_i2c_device3.dev.platform_data = &star_i2c3_platform_data;
	tegra_i2c_device4.dev.platform_data = &star_dvc_platform_data;

	platform_device_register(&tegra_i2c_device4);
	platform_device_register(&tegra_i2c_device3);
	platform_device_register(&tegra_i2c_device2);
	platform_device_register(&tegra_i2c_device1);

	platform_device_register(&i2c_gpio_controller);
	if( board_rev>=REV_1_2)
		platform_device_register(&stereocam_i2c_gpio_controller);

	//register i2c devices attached to I2C1
	star_register_numbered_i2c_devices(0 , star_i2c_bus1_devices_info, ARRAY_SIZE(star_i2c_bus1_devices_info));
	//register i2c devices attached to I2C2
	star_register_numbered_i2c_devices(1 , star_i2c_bus2_devices_info, ARRAY_SIZE(star_i2c_bus2_devices_info));
	//register i2c devices attached to I2C3
	star_register_numbered_i2c_devices(3 , star_i2c_bus3_devices_info, ARRAY_SIZE(star_i2c_bus3_devices_info));
	//register i2c devices attached to I2C4(PWR_I2C)
	star_register_numbered_i2c_devices(4 , star_i2c_bus4_devices_info, ARRAY_SIZE(star_i2c_bus4_devices_info));

	//register i2c devices attached to STEREOCAM-I2C-GPIO
	if  (board_rev >= REV_1_2)
		star_register_numbered_i2c_devices(6 , star_i2c_stereo_camera_info, ARRAY_SIZE(star_i2c_stereo_camera_info));
	else
		star_register_numbered_i2c_devices(4 , star_i2c_stereo_camera_info, ARRAY_SIZE(star_i2c_stereo_camera_info));  // PWR_I2C

	//register i2c devices attached to I2C-GPIO (Echo canceller)
	if  (board_rev >= REV_G)
		star_register_numbered_i2c_devices(7 , star_i2c_bus7_echo_info, ARRAY_SIZE(star_i2c_bus7_echo_info));
	else
		star_register_numbered_i2c_devices(1 , star_i2c_bus7_echo_info, ARRAY_SIZE(star_i2c_bus7_echo_info));	// GEN2_I2C
}

#define GPIO_KEY(_id, _gpio, _iswake)		\
	{					\
		.code = _id,			\
		.gpio = TEGRA_GPIO_##_gpio,	\
		.active_low = 1,		\
		.desc = #_id,			\
		.type = EV_KEY,			\
		.wakeup = _iswake,		\
		.debounce_interval = 10,	\
	}

static struct gpio_keys_button star_keys[] = {
	[0] = GPIO_KEY(KEY_VOLUMEDOWN, PG3, 0),
	[1] = GPIO_KEY(KEY_VOLUMEUP, PG2, 0),
//	[2] = GPIO_KEY(KEY_POWER, PV2, 1),
};

#define PMC_WAKE_STATUS 0x14

#if 0 // sync with ventana, but donot use it. star has powerkey driver
static int star_wakeup_key(void)
{
	unsigned long status =
		readl(IO_ADDRESS(TEGRA_PMC_BASE) + PMC_WAKE_STATUS);

	return status & TEGRA_WAKE_GPIO_PV2 ? KEY_POWER : KEY_RESERVED;
}
#endif

static struct gpio_keys_platform_data star_keys_platform_data = {
	.buttons	= star_keys,
	.nbuttons	= ARRAY_SIZE(star_keys),
//	.wakeup_key	= star_wakeup_key,
};

struct platform_device star_keys_device = {
	.name	= "gpio-keys",
	.id	= -1,
	.dev	= {
		.platform_data	= &star_keys_platform_data,
	},
};




struct platform_device tegra_echo = {
	.name   = "tegra_echo",
	.id     = -1,
	.dev    = {
	},
};

#ifdef CONFIG_BCM4329_RFKILL
static struct resource star_bcm4329_rfkill_resources[] = {
	{
		.name   = "bcm4329_nreset_gpio",
		.start  = TEGRA_GPIO_PP0,
		.end    = TEGRA_GPIO_PP0,
		.flags  = IORESOURCE_IO,
	},
/*	{
		.name   = "bcm4329_nshutdown_gpio",
		.start  = TEGRA_GPIO_PQ2,
		.end    = TEGRA_GPIO_PQ2,
		.flags  = IORESOURCE_IO,
	},
*/
};

// resouce for previous hw rev ( <= 1.2)
static struct resource star_1_2_bcm4329_rfkill_resources[] = {
	{
		.name   = "bcm4329_nreset_gpio",
		.start  = TEGRA_GPIO_PQ2,
		.end    = TEGRA_GPIO_PQ2,
		.flags  = IORESOURCE_IO,
	},
/*	{
		.name   = "bcm4329_nshutdown_gpio",
		.start  = TEGRA_GPIO_PQ2,
		.end    = TEGRA_GPIO_PQ2,
		.flags  = IORESOURCE_IO,
	},
*/
};

static struct platform_device star_bcm4329_rfkill_device = {
	.name = "bcm4329_rfkill",
	.id             = -1,
	.num_resources  = ARRAY_SIZE(star_bcm4329_rfkill_resources),
	.resource       = star_bcm4329_rfkill_resources,
};

static noinline void __init star_bt_rfkill(void)
{
        if (get_hw_rev() <= REV_1_2)
        {
            star_bcm4329_rfkill_device.num_resources = ARRAY_SIZE(star_1_2_bcm4329_rfkill_resources);
            star_bcm4329_rfkill_device.resource = star_1_2_bcm4329_rfkill_resources;
        }

        /*Add Clock Resource*/
        /* Always turn on 32k clock */
        //clk_add_alias("bcm4329_32k_clk", star_bcm4329_rfkill_device.name,
        //                        "blink", NULL);

        platform_device_register(&star_bcm4329_rfkill_device);

        return;
}
#else
static inline void star_bt_rfkill(void) { }
#endif

#ifdef CONFIG_STARTABLET_GPS_BCM4751
//LGE_UPDATE_S jayeong.im@lge.com 2010-11-30 star_gps_init
static int __init star_gps_init(void)
{
/* Always turn on 32k clock
             struct clk *clk32 = clk_get_sys(NULL, "blink");
             if (!IS_ERR(clk32)) {
                           clk_set_rate(clk32,clk32->parent->rate);
                           clk_enable(clk32);
             }
*/
             return 0;
}
//LGE_UPDATE_E jayeong.im@lge.com 2010-11-30 star_gps_init
#endif

#if defined(CONFIG_ANDROID_RAM_CONSOLE)
#if CARVEOUT_384MB
#define STARTABLET_RAM_CONSOLE_BASE	((640-2)*SZ_1M)	// 384MB Carveout
#elif CARVEOUT_352MB
#define STARTABLET_RAM_CONSOLE_BASE	((672-2)*SZ_1M)	// 352MB Carveout
#elif CARVEOUT_320MB
#define STARTABLET_RAM_CONSOLE_BASE	((704-2)*SZ_1M)	// 320MB Carveout
#else
#define STARTABLET_RAM_CONSOLE_BASE	((768-2)*SZ_1M)	// 256MB Carveout
#endif
#define STARTABLET_RAM_CONSOLE_SIZE (SZ_1M)

static struct resource star_ram_console_resource[] = {
	{
		.name	= "ram_console",
		.start	= STARTABLET_RAM_CONSOLE_BASE,
		.end	= STARTABLET_RAM_CONSOLE_BASE + STARTABLET_RAM_CONSOLE_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	}
};

static struct platform_device star_ram_console_device = {
	.name = "ram_console",
	.id = -1,
	.num_resources	= ARRAY_SIZE(star_ram_console_resource),
	.resource		= star_ram_console_resource,
};
#endif

static struct platform_device *star_devices[] __initdata = {
	&androidusb_device,
	&debug_uart,
	&tegra_uartc_device,
//	&tegra_hsuart2,
	&tegra_udc_device,
	&star_powerkey,
	&tegra_gps_gpio,
	&tegra_misc,
	&tegra_spi_device1,
#if 0 //disables spi2, spi3, spi4 device drivers
	&tegra_spi_device2,
	&tegra_spi_device3,
	&tegra_spi_device4,
#endif

	&androidums_device,
	&tegra_gart_device,
	&tegra_aes_device,
	&star_keys_device,
	&tegra_leds, //2010.08.11 ch.han@lge.com for leds drives
	&star_headset_device,
	//&tegra_rtc_device,
	&bcm_bt_lpm,
	&tegra_camera,
	&tegra_i2s_device1,
	&tegra_i2s_device2,
        &tegra_spdif_device,
	&tegra_avp_device,
	&tegra_echo,
	&tegra_displaytest,
	&tegra_camera_flash, //2010.12.08 hyungmoo.huh@lge.com for camera flash LED
#ifdef CONFIG_STARTABLET_GPS_BCM4751
	&tegra_uartd_device, //jayeong.im@lge.com 2010-11-30 star_gps_init
#endif
	&tegra_das_device,
#if defined(CONFIG_ANDROID_RAM_CONSOLE)
	&star_ram_console_device,
#endif
};

static void star_keys_init(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(star_keys); i++)
		tegra_gpio_enable(star_keys[i].gpio);
}


static struct tegra_ehci_platform_data tegra_ehci_pdata[] = {
	[0] = {
			.phy_config = &utmi_phy_config[0],
#ifdef CONFIG_USB_TEGRA_OTG
			.operating_mode = TEGRA_USB_OTG,
#else
			.operating_mode = TEGRA_USB_HOST,
#endif
			.power_down_on_bus_suspend = 0,
	},
	[1] = {
			.phy_config = &ulpi_phy_config,
			.operating_mode = TEGRA_USB_HOST,
			.power_down_on_bus_suspend = 1,
	},
	[2] = {
			.phy_config = &utmi_phy_config[1],
			.operating_mode = TEGRA_USB_HOST,
			.power_down_on_bus_suspend = 0,
	},
};

#ifdef CONFIG_USB_TEGRA_OTG
static struct platform_device *tegra_usb_otg_host_register(void)
{
	struct platform_device *pdev;
	void *platform_data;
	int val;

	pdev = platform_device_alloc(tegra_ehci1_device.name, tegra_ehci1_device.id);
	if (!pdev)
		return NULL;

	val = platform_device_add_resources(pdev, tegra_ehci1_device.resource,
		tegra_ehci1_device.num_resources);
	if (val)
		goto error;

	pdev->dev.dma_mask =  tegra_ehci1_device.dev.dma_mask;
	pdev->dev.coherent_dma_mask = tegra_ehci1_device.dev.coherent_dma_mask;

	platform_data = kmalloc(sizeof(struct tegra_ehci_platform_data), GFP_KERNEL);
	if (!platform_data)
		goto error;

	memcpy(platform_data, &tegra_ehci_pdata[0],
				sizeof(struct tegra_ehci_platform_data));
	pdev->dev.platform_data = platform_data;

	val = platform_device_add(pdev);
	if (val)
		goto error_add;

	return pdev;

error_add:
	kfree(platform_data);
error:
	pr_err("%s: failed to add the host contoller device\n", __func__);
	platform_device_put(pdev);
	return NULL;
}

static void tegra_usb_otg_host_unregister(struct platform_device *pdev)
{
	platform_device_unregister(pdev);
}

static struct tegra_otg_platform_data tegra_otg_pdata = {
	.host_register = &tegra_usb_otg_host_register,
	.host_unregister = &tegra_usb_otg_host_unregister,
};
#endif

static void star_usb_init(void)
{
#ifdef CONFIG_USB_TEGRA_OTG
	tegra_otg_device.dev.platform_data = &tegra_otg_pdata;
	platform_device_register(&tegra_otg_device);
#endif

#if !defined (CONFIG_MACH_STARTABLET)
	tegra_ehci3_device.dev.platform_data=&tegra_ehci_pdata[2];
	platform_device_register(&tegra_ehci3_device);
#endif
}

#ifdef CONFIG_USB_ANDROID_LGE_GADGET
int usb_modem_mode = 0;

static int __init usb_mode_setup(char *line)
{
	if (sscanf(line, "%1d", &usb_modem_mode) != 1) {
		usb_modem_mode = 0;
	}
	return 1;
}

__setup("usb_mode=", usb_mode_setup);
#endif

static int __init star_muic_path_setup(char *line)
{
	if (sscanf(line, "%1d:%02x", &muic_path, &muic_status) != 2) {
		muic_path = 0;
		muic_status = 0x0B;
	}
	return 1;
}

__setup("muic_path=", star_muic_path_setup);

#ifdef CONFIG_USB_ANDROID_LGE_GADGET
int factory_download_cable_detected(void)
{
	if ((muic_status & 0x0f) == 0x0A) {
		return 1;
	}
	return 0;
}
#endif

static void __init star_power_off_init(void)
{
      pm_power_off = star_power_off;
}

extern char boot_command_line[COMMAND_LINE_SIZE];

static void star_nct1008_init(void)
{
	tegra_gpio_enable(TEGRA_GPIO_PK2);
	gpio_request(TEGRA_GPIO_PK2, "temp_alert");
	gpio_direction_input(TEGRA_GPIO_PK2);
}

static void __init tegra_star_init(void)
{
	tegra_common_init();
	tegra_clk_init_from_table(star_clk_init_table);
	star_pinmux_init();

	star_i2c_init();
	star_regulator_init();

#ifdef CONFIG_DUAL_SPI //disables spi secondary port
#ifdef CONFIG_SPI_MDM6600
/* spi secondary port use spi3 when hw revision less than f */
	if (get_hw_rev() < REV_F) {
		tegra_spi_devices[1].bus_num = 2;
		tegra_spi_devices[1].chip_select = 2;
	}
#endif /* CONFIG_SPI_MDM6600 */
#endif

	spi_register_board_info(tegra_spi_devices, ARRAY_SIZE(tegra_spi_devices));
	tegra_i2s_device1.dev.platform_data = &tegra_audio_pdata[0];
	tegra_i2s_device2.dev.platform_data = &tegra_audio_pdata[1];
	tegra_spdif_device.dev.platform_data = &tegra_spdif_pdata;
	tegra_das_device.dev.platform_data = &tegra_das_pdata;
#ifdef CONFIG_USB_ANDROID_LGE_GADGET
	if (factory_download_cable_detected()) {
		andusb_plat.product_id = 0x6000;
		andusb_plat.functions = usb_functions_lge_mobile_modem_driver;
		andusb_plat.num_functions = ARRAY_SIZE(usb_functions_lge_mobile_modem_driver);
	} else {
		if(usb_modem_mode == 1) {
			andusb_plat.product_id = 0x61F2;
			usb_products[0].product_id = 0x61F2;
			usb_products[0].num_functions = ARRAY_SIZE(usb_functions_lge_android_modem_mtp_driver);
			usb_products[0].functions = usb_functions_lge_android_modem_mtp_driver;
			usb_products[1].product_id = 0x61F1;
			usb_products[1].num_functions = ARRAY_SIZE(usb_functions_lge_android_modem_mtp_adb_driver);
			usb_products[1].functions = usb_functions_lge_android_modem_mtp_adb_driver;
		}
	}
#endif

	if (strstr(boot_command_line, "ttyS0")!=NULL) {
		star_devices[1] = &debug_uart;
	}else{
		star_devices[1] = &tegra_uartb_device;
	}
	platform_add_devices(star_devices, ARRAY_SIZE(star_devices));

	// star_i2c_init();
	star_sdhci_init();
	// star_regulator_init();
	star_keys_init();

	star_usb_init();
	star_panel_init();
	star_bt_rfkill();
	save_hw_rev();
#ifdef CONFIG_STARTABLET_GPS_BCM4751
	printk("star_gps_init BCM4751 \n") ;
	star_gps_init(); // jayeong.im@lge.com 2010-11-30 star_gps_init
#endif
	star_power_off_init();
	star_emc_init();
	star_nct1008_init();//Thermal IC enable
}

void __init star_map_io(void)
{
	tegra_map_common_io();
}

int __init tegra_star_protected_aperture_init(void)
{
    tegra_protected_aperture_init(tegra_grhost_aperture);
    return 0;
}
late_initcall(tegra_star_protected_aperture_init);

void __init tegra_star_reserve(void)
{
    //long ret;
    if (memblock_reserve(0x0, 4096) < 0)
            pr_warn("Cannot reserve first 4K of memory for safety\n");

#if CARVEOUT_384MB
    tegra_reserve(SZ_256M|SZ_128M, SZ_8M, SZ_16M);
#elif CARVEOUT_352MB
	tegra_reserve(SZ_256M|SZ_128M - SZ_32M, SZ_8M, SZ_16M);
#elif CARVEOUT_320MB
	tegra_reserve(SZ_256M|SZ_64M, SZ_8M, SZ_16M);
#else // 256MB
    tegra_reserve(SZ_256M, SZ_8M, SZ_16M);
#endif
}

MACHINE_START(STARTABLET, "startablet")
      .boot_params  = 0x00000100,
      .phys_io        = IO_APB_PHYS,
      .io_pg_offst    = ((IO_APB_VIRT) >> 18) & 0xfffc,
      .init_irq       = tegra_init_irq,
      .init_machine   = tegra_star_init,
      .map_io         = star_map_io,
      .reserve        = tegra_star_reserve,
      .timer          = &tegra_timer,
MACHINE_END
