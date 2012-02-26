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
#include <linux/delay.h>
#include <linux/gpio.h>

#include <asm/mach-types.h>
#include <mach/irqs.h>
#include <mach/iomap.h>
#include <mach/sdhci.h>
#include <mach/pinmux.h>
#include <mach/gpio-names.h>
#include <mach/hardware.h>

/* 	20100615 seokhee.han@lge.com
	SDIO4 for eMMC, instance: 1
	SDIO1 for WIFI, instance: 2
*/
static struct resource sdhci_resource1[] = {
	[0] = {
		.start  = INT_SDMMC1,
		.end    = INT_SDMMC1,
		.flags  = IORESOURCE_IRQ,
	},
	[1] = {
		.start	= TEGRA_SDMMC1_BASE,
		.end	= TEGRA_SDMMC1_BASE + TEGRA_SDMMC1_SIZE-1,
		.flags	= IORESOURCE_MEM,
	},
};

static struct resource sdhci_resource4[] = {
	[0] = {
		.start  = INT_SDMMC4,
		.end    = INT_SDMMC4,
		.flags  = IORESOURCE_IRQ,
	},
	[1] = {
		.start	= TEGRA_SDMMC4_BASE,
		.end	= TEGRA_SDMMC4_BASE + TEGRA_SDMMC4_SIZE-1,
		.flags	= IORESOURCE_MEM,
	},
};

static struct tegra_sdhci_platform_data tegra_sdhci_platform_data1 = {
	.cd_gpio = TEGRA_GPIO_PU2,
/* LGE_CHANGE_START, [jisung.yang@lge.com], 2010-11-22, <star smartphone patch> */
	.is_always_on = 1,
/* LGE_CHANGE_START, [jisung.yang@lge.com], 2010-11-22, <star smartphone patch> */
};

static struct tegra_sdhci_platform_data tegra_sdhci_platform_data4 = {
	.cd_gpio = -1,
};

/* WLAN */
static struct platform_device tegra_sdhci_device1 = {
	.name		= "sdhci-tegra",
	.id		= 0,
	.resource	= sdhci_resource1,
	.num_resources	= ARRAY_SIZE(sdhci_resource1),
	.dev = {
		.platform_data = &tegra_sdhci_platform_data1,
	},
};

/* eMMC */
static struct platform_device tegra_sdhci_device4 = {
	.name		= "sdhci-tegra",
	.id		= 3,
	.resource	= sdhci_resource4,
	.num_resources	= ARRAY_SIZE(sdhci_resource4),
	.dev = {
		.platform_data = &tegra_sdhci_platform_data4,
	},
};

int __init star_sdhci_init(void)
{
	if (get_hw_rev() <= REV_1_2)
		tegra_sdhci_platform_data1.cd_gpio = TEGRA_GPIO_PQ5;

	gpio_request(tegra_sdhci_platform_data1.cd_gpio, "sdhci1_cd");	//mingi
	tegra_gpio_enable(tegra_sdhci_platform_data1.cd_gpio);
	gpio_direction_output(tegra_sdhci_platform_data1.cd_gpio, 0);

	platform_device_register(&tegra_sdhci_device1);
	platform_device_register(&tegra_sdhci_device4);

	return 0;
}
