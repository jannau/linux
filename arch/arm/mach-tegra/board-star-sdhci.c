/*
 * arch/arm/mach-tegra/board-star-sdhci.c
 *
 * Copyright (C) 2010 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/resource.h>
#include <linux/platform_device.h>
#include <linux/wlan_plat.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/clk.h>
#include <linux/err.h>

#include <asm/mach-types.h>
#include <mach/irqs.h>
#include <mach/iomap.h>
#include <mach/sdhci.h>
#include <mach/pinmux.h>
#include <mach/hardware.h>

#include "gpio-names.h"
#include "board.h"

/* 	20100615 seokhee.han@lge.com
	SDIO4 for eMMC, instance: 1
	SDIO1 for WIFI, instance: 2
*/

#define STARTABLET_WLAN_PWR1	TEGRA_GPIO_PQ5
#define STARTABLET_WLAN_PWR2	TEGRA_GPIO_PU2

#define STARTABLET_WLAN_RST	TEGRA_GPIO_PN6

#define STARTABLET_WLAN_WOW	TEGRA_GPIO_PY6

static void (*wifi_status_cb)(int card_present, void *dev_id);
static void *wifi_status_cb_devid;
static int startablet_wifi_status_register(void (*callback)(int , void *), void *);
static struct clk *wifi_32k_clk;

static int startablet_wifi_reset(int on);
static int startablet_wifi_power(int on);
static int startablet_wifi_set_carddetect(int val);

static struct wifi_platform_data startablet_wifi_control = {
	.set_power	= startablet_wifi_power,
	.set_reset	= startablet_wifi_reset,
	.set_carddetect = startablet_wifi_set_carddetect,
};

static struct resource wifi_resource[] = {
	[0] = {
		.name  = "bcm4329_wlan_irq",
		.start = TEGRA_GPIO_TO_IRQ(STARTABLET_WLAN_WOW),
		.end   = TEGRA_GPIO_TO_IRQ(STARTABLET_WLAN_WOW),
		.flags = IORESOURCE_IRQ | IORESOURCE_IRQ_HIGHLEVEL | IORESOURCE_IRQ_SHAREABLE,
	},
};

static struct platform_device startablet_wifi_device = {
	.name		= "bcm4329_wlan",
	.id		= 1,
	.num_resources	= 1,
	.resource	= wifi_resource,
	.dev		= {
		.platform_data = &startablet_wifi_control,
	},
};

static struct resource sdhci_resource0[] = {
	[0] = {
		.start	= INT_SDMMC1,
		.end	= INT_SDMMC1,
		.flags	= IORESOURCE_IRQ,
	},
	[1] = {
		.start	= TEGRA_SDMMC1_BASE,
		.end	= TEGRA_SDMMC1_BASE + TEGRA_SDMMC1_SIZE-1,
		.flags	= IORESOURCE_MEM,
	},
};

static struct resource sdhci_resource3[] = {
	[0] = {
		.start	= INT_SDMMC4,
		.end	= INT_SDMMC4,
		.flags	= IORESOURCE_IRQ,
	},
	[1] = {
		.start	= TEGRA_SDMMC4_BASE,
		.end	= TEGRA_SDMMC4_BASE + TEGRA_SDMMC4_SIZE-1,
		.flags	= IORESOURCE_MEM,
	},
};

static struct embedded_sdio_data embedded_sdio_data0 = {
	.cccr   = {
		.sdio_vsn	= 2,
		.multi_block	= 1,
		.low_speed	= 0,
		.wide_bus	= 0,
		.high_power	= 1,
		.high_speed	= 1,
	},
	.cis  = {
		.vendor 	= 0x02d0,
		.device 	= 0x4329,
	},
};

static struct tegra_sdhci_platform_data tegra_sdhci_platform_data0 = {
	.mmc_data = {
		.register_status_notify	= startablet_wifi_status_register,
		.embedded_sdio = &embedded_sdio_data0,
		.built_in = 1,
	},
	.cd_gpio = -1 /* TEGRA_GPIO_PQ5 */,
	.wp_gpio = -1,
	.power_gpio = -1,
	.is_always_on = 1,
};

static struct tegra_sdhci_platform_data tegra_sdhci_platform_data3 = {
	.is_8bit = 1,
	.cd_gpio = -1,
	.wp_gpio = -1,
	/* .power_gpio = TEGRA_GPIO_PI6, */
	.mmc_data = {
		.built_in = 1,
	}
};

static struct platform_device tegra_sdhci_device0 = {
	.name		= "sdhci-tegra",
	.id		= 0,
	.resource	= sdhci_resource0,
	.num_resources	= ARRAY_SIZE(sdhci_resource0),
	.dev = {
		.platform_data = &tegra_sdhci_platform_data0,
	},
};

static struct platform_device tegra_sdhci_device3 = {
	.name		= "sdhci-tegra",
	.id		= 3,
	.resource	= sdhci_resource3,
	.num_resources	= ARRAY_SIZE(sdhci_resource3),
	.dev = {
		.platform_data = &tegra_sdhci_platform_data3,
	},
};

static int startablet_wifi_status_register(
		void (*callback)(int card_present, void *dev_id),
		void *dev_id)
{
	if (wifi_status_cb)
		return -EAGAIN;
	wifi_status_cb = callback;
	wifi_status_cb_devid = dev_id;
	return 0;
}

static int startablet_wifi_set_carddetect(int val)
{
	pr_debug("%s: %d\n", __func__, val);
	if (wifi_status_cb)
		wifi_status_cb(val, wifi_status_cb_devid);
	else
		pr_warning("%s: Nobody to notify\n", __func__);
	return 0;
}

static int startablet_wifi_power(int on)
{
	pr_debug("%s: %d\n", __func__, on);

	if (!on)
		return 0;

	if (get_hw_rev() <= REV_1_2)
		gpio_set_value(STARTABLET_WLAN_PWR1, on);
	else
		gpio_set_value(STARTABLET_WLAN_PWR2, on);
	mdelay(200);

	if (on)
		clk_enable(wifi_32k_clk);
	else
		clk_disable(wifi_32k_clk);

	return 0;
}

static int startablet_wifi_reset(int on)
{
	pr_debug("%s: do nothing\n", __func__);
	if (get_hw_rev() <= REV_1_2)
		gpio_set_value(STARTABLET_WLAN_PWR1, on);
	else
		gpio_set_value(STARTABLET_WLAN_PWR2, on);
	mdelay(200);
	return 0;
}

static int __init startablet_wifi_init(void)
{
	wifi_32k_clk = clk_get_sys(NULL, "blink");
	if (IS_ERR(wifi_32k_clk)) {
		pr_err("%s: unable to get blink clock\n", __func__);
		return PTR_ERR(wifi_32k_clk);
	}

	if (get_hw_rev() <= REV_1_2) {
		gpio_request(STARTABLET_WLAN_PWR1, "wlan_power");
		tegra_gpio_enable(STARTABLET_WLAN_PWR1);
		gpio_direction_output(STARTABLET_WLAN_PWR1, 0);
	} else {
		gpio_request(STARTABLET_WLAN_PWR2, "wlan_power");
		tegra_gpio_enable(STARTABLET_WLAN_PWR2);
		gpio_direction_output(STARTABLET_WLAN_PWR2, 0);
	}
	gpio_request(STARTABLET_WLAN_RST, "wlan_rst");
	tegra_gpio_enable(STARTABLET_WLAN_RST);
	gpio_direction_output(STARTABLET_WLAN_RST, 0);

	gpio_request(STARTABLET_WLAN_WOW, "bcmsdh_sdmmc");
	tegra_gpio_enable(STARTABLET_WLAN_WOW);
	gpio_direction_input(STARTABLET_WLAN_WOW);

	platform_device_register(&startablet_wifi_device);

	device_init_wakeup(&startablet_wifi_device.dev, 1);
	device_set_wakeup_enable(&startablet_wifi_device.dev, 0);

	return 0;
}
int __init star_sdhci_init(void)
{
	/* tegra_gpio_enable(tegra_sdhci_platform_data3.power_gpio); */

	platform_device_register(&tegra_sdhci_device3);
	platform_device_register(&tegra_sdhci_device0);

	startablet_wifi_init();

	return 0;
}
