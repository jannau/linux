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

const char *tegra_partition_list = NULL;
static int __init tegrapart_setup(char *options)
{
    if (options && *options && !tegra_partition_list)
        tegra_partition_list = options;
    return 0;
}

__setup("tegrapart=", tegrapart_setup);

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
	.clk_id = NULL,
	.force_hs = 0,
	.cd_gpio = TEGRA_GPIO_PU2,
/* LGE_CHANGE_START, [jisung.yang@lge.com], 2010-11-22, <star smartphone patch> */
	.is_always_on = 1,
/* LGE_CHANGE_START, [jisung.yang@lge.com], 2010-11-22, <star smartphone patch> */
};

static struct tegra_sdhci_platform_data tegra_sdhci_platform_data4 = {
	.clk_id = NULL,
	.force_hs = 0,
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

static int Isdigit(int c)
{
    return (c>='0' && c<='9');
}

static int Isxdigit(int c)
{
    return (c>='0' && c<='9') || (c>='A' && c<='F') || (c>='a' && c<='f');
}

static int CharToXDigit(int c)
{
    return (c>='0' && c<='9') ? c - '0' :
           (c>='a' && c<='f') ? c - 'a' + 10 :
           (c>='A' && c<='F') ? c - 'A' + 10 : -1;
}

static unsigned long long int Strtoull(const char *s, char **endptr, int base)
{
    int neg = 0;
    unsigned long long int val = 0;

    if (*s == '-') {
        s++;
        neg = 1;
    }
    if (s[0]=='0' && (s[1]=='x' || s[1]=='X')) {
        if (base == 10) {
            if (endptr) {
                *endptr = (char*)s+1;
                return val;
            }
        }
        s += 2;
        base = 16;
    }

    if (base == 16) {
        while (Isxdigit(*s)) {
            val <<= 4;
            val +=  CharToXDigit(*s);
            s++;
#ifdef DEBUG
            printk("0x%x\n", (int)val);
#endif
        }
    } else {
        while (Isdigit(*s)) {
            val *= 10;
            val += CharToXDigit(*s);
            s++;
        }
    }

    if (endptr) {
        *endptr = (char*)s;
    }
    return neg ? ((~val)+1) : val;
}

static int tegra_get_partition_info_by_name(
    const char *PartName,
    unsigned long long      *pSectorStart,
    unsigned long long      *pSectorLength,
    unsigned int	        *pSectorSize)
{
    int Len = strlen(PartName);
    const char *Ptr = tegra_partition_list;
    char *End;

    if (!Ptr)
        return -1;

    while (*Ptr && *Ptr!=' ')
    {
        if (!strncmp(Ptr, PartName, Len) && Ptr[Len]==':')
        {
            Ptr += Len + 1;
            *pSectorStart = Strtoull(Ptr, &End, 16);
            if (*End!=':')
                return -1;
            Ptr = End+1;
            *pSectorLength = Strtoull(Ptr, &End, 16);
            if (*End!=':')
                return -1;
            Ptr = End+1;
            *pSectorSize = Strtoull(Ptr, &End, 16);
			printk(KERN_DEBUG "tegra_get_partition_info_by_name: %s: pSectorStart: %d, pSectorLength: %d, pSectorSize: %d\n", PartName, (int) *pSectorStart, (int) *pSectorLength, (int) *pSectorSize);
            if (*End!=',' && *End!=' ' && *End)
                return -1;
            return 0;
        }
        else
        {
            while (*Ptr != ',' && *Ptr)
                Ptr++;
            if (!*Ptr)
                return -1;
            Ptr++;
        }
    }
    return -1;
}

int __init star_sdhci_init(void)
{
	//unsigned long long start, length;
	//unsigned int sector_size;
	//int err;

	if (get_hw_rev() <= REV_1_2)
		tegra_sdhci_platform_data1.cd_gpio = TEGRA_GPIO_PQ5;

	gpio_request(tegra_sdhci_platform_data1.cd_gpio, "sdhci1_cd");	//mingi
	tegra_gpio_enable(tegra_sdhci_platform_data1.cd_gpio);
	gpio_direction_output(tegra_sdhci_platform_data1.cd_gpio, 0);

	/*look for mbr partition*/
//	err = tegra_get_partition_info_by_name("mbr", &start, &length, &sector_size);
//	tegra_sdhci_platform_data4.startoffset = start * (unsigned long long)sector_size;
//	printk(KERN_INFO "star_init_sdhci: MBR: err: %d, sector_start: %d, sector_length: %d, sector_size: %d, startoffset: %d\n", err, (int) start, (int) length, (int) sector_size, tegra_sdhci_platform_data4.startoffset);
	platform_device_register(&tegra_sdhci_device1);
	platform_device_register(&tegra_sdhci_device4);

	return 0;
}
