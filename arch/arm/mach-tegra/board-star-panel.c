/*
 * arch/arm/mach-tegra/board-ventana-panel.c
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

#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/regulator/consumer.h>
#include <linux/resource.h>
#include <asm/mach-types.h>
#include <linux/platform_device.h>
#include <linux/earlysuspend.h>
#include <linux/pwm_backlight.h>
#include <mach/nvhost.h>
#include <mach/nvmap.h>
#include <mach/irqs.h>
#include <mach/iomap.h>
#include <mach/dc.h>
#include <mach/fb.h>
#include <mach/tegra_lm1_bl.h>
#include <mach/hardware.h>

#include "devices.h"
#include "board.h"
#include "gpio-names.h"

#define star_bl_enb		TEGRA_GPIO_PS6
#define star_lvds_shutdown	TEGRA_GPIO_PB2
#define star_panel_enb		TEGRA_GPIO_PH2
#define star_hdmi_hpd		TEGRA_GPIO_PN7
#define star_hdmi_enb		TEGRA_GPIO_PX7

#if defined (CONFIG_MACH_STARTABLET)
extern void tegra_panel_hdmi_earlysuspend(void);
extern void tegra_panel_hdmi_detect(void);
extern void tegra_panel_cancel_early_work(void);
static int is_early_suspend = 0;
static int v3_3_count = 0;
DEFINE_MUTEX(v3_3_control);

#endif


static struct regulator *star_hdmi_reg = NULL;
static struct regulator *star_hdmi_pll = NULL;

static struct platform_tegra_lm1_backlight_data star_backlight_data = {
    .lm1_id = 0,
    .max_brightness = 255,
    .dft_brightness = 84,
    .period = 0xFF,
    .clk_div = 1,
    .clk_select = 2,
};

static struct platform_device star_backlight_device = {
    .name   = "tegralm1bl",
    .dev    = {
        .platform_data = &star_backlight_data,
    },
};

static int star_panel_enable(void)
{
	return 0;
}

static int star_panel_disable(void)
{
	return 0;
}

static int star_hdmi_enable(void)
{
	if (!star_hdmi_reg) {
		star_hdmi_reg = regulator_get(NULL, "avdd_hdmi"); /* LD07 */
		if (IS_ERR_OR_NULL(star_hdmi_reg)) {
			pr_err("hdmi: couldn't get regulator avdd_hdmi\n");
			star_hdmi_reg = NULL;
			return PTR_ERR(star_hdmi_reg);
		}
	}
	regulator_enable(star_hdmi_reg);

	if (!star_hdmi_pll) {
		star_hdmi_pll = regulator_get(NULL, "avdd_hdmi_pll"); /* LD08 */
		if (IS_ERR_OR_NULL(star_hdmi_pll)) {
			pr_err("hdmi: couldn't get regulator avdd_hdmi_pll\n");
			star_hdmi_pll = NULL;
			regulator_disable(star_hdmi_reg);
			//WBT 196334
			regulator_put(star_hdmi_reg);
			//
			star_hdmi_reg = NULL;
			return PTR_ERR(star_hdmi_pll);
		}
	}
	regulator_enable(star_hdmi_pll);
	return 0;

}

static int star_hdmi_disable(void)
{
	printk("[PM] %s() 0x%x\n", __func__, (unsigned int)star_hdmi_reg);
	if (star_hdmi_reg)
		regulator_disable(star_hdmi_reg);
	if (star_hdmi_pll)
		regulator_disable(star_hdmi_pll);
	return 0;
}

static struct resource star_disp1_resources[] = {
	{
		.name	= "irq",
		.start	= INT_DISPLAY_GENERAL,
		.end	= INT_DISPLAY_GENERAL,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.name	= "regs",
		.start	= TEGRA_DISPLAY_BASE,
		.end	= TEGRA_DISPLAY_BASE + TEGRA_DISPLAY_SIZE-1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name	= "fbmem",
		.flags	= IORESOURCE_MEM,
	},
};

static struct resource star_disp2_resources[] = {
	{
		.name	= "irq",
		.start	= INT_DISPLAY_B_GENERAL,
		.end	= INT_DISPLAY_B_GENERAL,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.name	= "regs",
		.start	= TEGRA_DISPLAY2_BASE,
		.end	= TEGRA_DISPLAY2_BASE + TEGRA_DISPLAY2_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name	= "fbmem",
		.flags	= IORESOURCE_MEM,

	},
	{
		.name	= "hdmi_regs",
		.start	= TEGRA_HDMI_BASE,
		.end	= TEGRA_HDMI_BASE + TEGRA_HDMI_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	},
};

static struct tegra_dc_mode star_panel_modes[] = {
	{
		.pclk = 64700000,
		.h_ref_to_sync = 4,
		.v_ref_to_sync = 2,
		.h_sync_width = 32,
		.v_sync_width = 6,
		.h_back_porch = 44,
		.v_back_porch = 13,
		.h_active = 1280,
		.v_active = 768,
		.h_front_porch = 48,
		.v_front_porch = 3,
	},
};

static struct tegra_fb_data star_fb_data = {
	.win		= 0,
	.xres		= 1280,
	.yres		= 768,
	.bits_per_pixel = 32,
};

static struct tegra_fb_data star_hdmi_fb_data = {
	.win		= 0,
	.xres		= 1920,
	.yres		= 1080,
	.bits_per_pixel	= 16,
};

static struct tegra_dc_out star_disp1_out = {
	.type		= TEGRA_DC_OUT_RGB,

	.align		= TEGRA_DC_ALIGN_MSB,
	.order		= TEGRA_DC_ORDER_RED_BLUE,

	.modes	 	= star_panel_modes,
	.n_modes 	= ARRAY_SIZE(star_panel_modes),

	.enable		= star_panel_enable,
	.disable	= star_panel_disable,
};

static struct tegra_dc_out star_disp2_out = {
	.type		= TEGRA_DC_OUT_HDMI,
	.flags		= TEGRA_DC_OUT_HOTPLUG_HIGH,

	.dcc_bus	= 2,
	.hotplug_gpio	= star_hdmi_hpd,

	.align		= TEGRA_DC_ALIGN_MSB,
	.order		= TEGRA_DC_ORDER_RED_BLUE,

	.enable		= star_hdmi_enable,
	.disable	= star_hdmi_disable,
};

static struct tegra_dc_platform_data star_disp1_pdata = {
	.flags		= TEGRA_DC_FLAG_ENABLED,
	.default_out	= &star_disp1_out,
	.fb		= &star_fb_data,
};

static struct tegra_dc_platform_data star_disp2_pdata = {
	.flags		= 0,
	.default_out	= &star_disp2_out,
	.fb		= &star_hdmi_fb_data,
};

static struct nvhost_device star_disp1_device = {
	.name		= "tegradc",
	.id		= 0,
	.resource	= star_disp1_resources,
	.num_resources	= ARRAY_SIZE(star_disp1_resources),
	.dev = {
		.platform_data = &star_disp1_pdata,
	},
};

static struct nvhost_device star_disp2_device = {
	.name		= "tegradc",
	.id		= 1,
	.resource	= star_disp2_resources,
	.num_resources	= ARRAY_SIZE(star_disp2_resources),
	.dev = {
		.platform_data = &star_disp2_pdata,
	},
};

static struct nvmap_platform_carveout star_carveouts[] = {
	[0] = {
		.name		= "iram",
		.usage_mask	= NVMAP_HEAP_CARVEOUT_IRAM,
		.base		= TEGRA_IRAM_BASE,
		.size		= TEGRA_IRAM_SIZE,
		.buddy_size	= 0, /* no buddy allocation for IRAM */
	},
	[1] = {
		.name		= "generic-0",
		.usage_mask	= NVMAP_HEAP_CARVEOUT_GENERIC,
		.buddy_size	= SZ_32K,
	},
};

static struct nvmap_platform_data star_nvmap_data = {
	.carveouts	= star_carveouts,
	.nr_carveouts	= ARRAY_SIZE(star_carveouts),
};

static struct platform_device star_nvmap_device = {
	.name	= "tegra-nvmap",
	.id	= -1,
	.dev	= {
		.platform_data = &star_nvmap_data,
	},
};

static struct platform_device *star_gfx_devices[] __initdata = {
	&star_nvmap_device,
	&tegra_grhost_device,
};

#ifdef CONFIG_HAS_EARLYSUSPEND
/* put early_suspend/late_resume handlers here for the display in order
 * to keep the code out of the display driver, keeping it closer to upstream
 */
struct early_suspend star_panel_early_suspender;

static void star_panel_early_suspend(struct early_suspend *h)
{
	if (num_registered_fb > 0) {
		printk("calling %s !\n", __func__);
		gpio_set_value(star_bl_enb, 0);
		msleep(500);

		gpio_set_value(star_lvds_shutdown, 0);
		msleep(1);
		mutex_lock(&v3_3_control);
		is_early_suspend = 1;
		mutex_unlock(&v3_3_control);
		if (get_hw_rev() >= REV_F) {
			disable_irq(gpio_to_irq(star_hdmi_hpd));
			gpio_set_value(star_panel_enb, 0);
			printk("v3.3 off in %s !\n", __func__);
			tegra_panel_hdmi_earlysuspend();
		}

		fb_blank(registered_fb[0], FB_BLANK_POWERDOWN);
	}
	printk("leaving  %s !\n", __func__);
}

static void star_panel_late_resume(struct early_suspend *h)
{
	if (num_registered_fb > 0) {
		printk("calling %s !\n", __func__);
		tegra_panel_cancel_early_work();
		if (get_hw_rev() >= REV_F) {
			gpio_set_value(star_panel_enb, 1);
			printk("v3.3 on in %s !\n", __func__);
		}
		mutex_lock(&v3_3_control);
		is_early_suspend = 0;
		mutex_unlock(&v3_3_control);
		fb_blank(registered_fb[0], FB_BLANK_UNBLANK);
		msleep(1);
		gpio_set_value(star_lvds_shutdown, 1);

		msleep(200);
		gpio_set_value(star_bl_enb, 1);

		tegra_panel_hdmi_detect();
	}
	printk("leaving %s !\n", __func__);
}

#endif

int __init star_panel_init(void)
{
	int err;
	struct resource *res;

	gpio_request(star_bl_enb, "wled_en");
	tegra_gpio_enable(star_bl_enb);
	gpio_direction_output(star_bl_enb, 1);

	gpio_request(star_lvds_shutdown, "lvds_en");
	tegra_gpio_enable(star_lvds_shutdown);
	gpio_direction_output(star_lvds_shutdown, 1);

	gpio_request(star_panel_enb, "vdd_3v3_en");
	tegra_gpio_enable(star_panel_enb);
	gpio_direction_output(star_panel_enb, 1);

	tegra_gpio_enable(star_hdmi_hpd);
	gpio_request(star_hdmi_hpd, "hdmi_hpd");
	gpio_direction_input(star_hdmi_hpd);

#ifdef CONFIG_HAS_EARLYSUSPEND
	star_panel_early_suspender.suspend = star_panel_early_suspend;
	star_panel_early_suspender.resume = star_panel_late_resume;
	star_panel_early_suspender.level = EARLY_SUSPEND_LEVEL_DISABLE_FB;
	register_early_suspend(&star_panel_early_suspender);
#endif

	star_carveouts[1].base = tegra_carveout_start;
	star_carveouts[1].size = tegra_carveout_size;

	err = platform_add_devices(star_gfx_devices,
				   ARRAY_SIZE(star_gfx_devices));

	res = nvhost_get_resource_byname(&star_disp1_device,
            IORESOURCE_MEM, "fbmem");
	res->start = tegra_fb_start;
	res->end = tegra_fb_start + tegra_fb_size - 1;

	res = nvhost_get_resource_byname(&star_disp2_device,
            IORESOURCE_MEM, "fbmem");
	res->start = tegra_fb2_start;
	res->end = tegra_fb2_start + tegra_fb2_size - 1;

	if (!err)
		err = nvhost_device_register(&star_disp1_device);

	if (!err)
		err = nvhost_device_register(&star_disp2_device);

	if (!err)
		platform_device_register(&star_backlight_device);

	return err;
}

#if defined (CONFIG_MACH_STARTABLET)
void set_v3_3(int i)
{

	mutex_lock(&v3_3_control);
	if (!is_early_suspend) {
		v3_3_count = 0;
		mutex_unlock(&v3_3_control);
		return;
	}
	if (!i)
		v3_3_count--;
	if (v3_3_count < 0)
		printk("%s: warning!! v3_3_count is negative !\n", __func__);
	if (v3_3_count == 0) {
		gpio_set_value(star_panel_enb, i);
		if (i)
			msleep(10);
	}
	if (i)
		v3_3_count++;

	mutex_unlock(&v3_3_control);
}
#endif
