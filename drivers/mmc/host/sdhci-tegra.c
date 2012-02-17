/*
 * drivers/mmc/host/sdhci-tegra.c
 *
 * Copyright (C) 2009 Palm, Inc.
 * Author: Yvonne Yip <y@palm.com>
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

#include <linux/err.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <linux/mmc/card.h>
/* LGE_CHANGE_START, [jisung.yang@lge.com], 2010-11-22, <star smartphone patch> */
#include <linux/delay.h>
/* LGE_CHANGE_END, [jisung.yang@lge.com], 2010-11-22, <star smartphone patch> */

#include <mach/sdhci.h>

#include <mach/hardware.h>
#include <mach/gpio-names.h>

#include "sdhci.h"

#define DRIVER_NAME    "sdhci-tegra"

#define SDHCI_VENDOR_CLOCK_CNTRL       0x100

struct tegra_sdhci_host {
	struct sdhci_host *sdhci;
	struct clk *clk;
#ifdef CONFIG_EMBEDDED_MMC_START_OFFSET
	unsigned int           StartOffset;
#endif
	int clk_enabled;
/* LGE_CHANGE_START, [jisung.yang@lge.com], 2010-11-22, <star smartphone patch> */
	bool			card_always_on;
	int sdhci_ints;
/* LGE_CHANGE_END, [jisung.yang@lge.com], 2010-11-22, <star smartphone patch> */
};

static irqreturn_t carddetect_irq(int irq, void *data)
{
	struct sdhci_host *sdhost = (struct sdhci_host *)data;

    printk("[mingi.sung@lge.com] %s:%d\n",__func__,__LINE__);

	sdhci_card_detect_callback(sdhost);
	return IRQ_HANDLED;
};

static void tegra_sdhci_status_notify_cb(int card_present, void *dev_id)
{
	struct sdhci_host *sdhci = (struct sdhci_host *)dev_id;
	pr_debug("%s: card_present %d\n",
		mmc_hostname(sdhci->mmc), card_present);
	sdhci_card_detect_callback(sdhci);
}

static int tegra_sdhci_enable_dma(struct sdhci_host *host)
{
	return 0;
}


#ifdef CONFIG_EMBEDDED_MMC_START_OFFSET
static unsigned int tegra_sdhci_get_StartOffset(struct sdhci_host *host)
{
	struct tegra_sdhci_host *t_sdhci_host;

	t_sdhci_host = sdhci_priv(host);

	return t_sdhci_host->StartOffset;
}
#endif

static void tegra_sdhci_enable_clock(struct tegra_sdhci_host *host, int enable)
{
	if (enable && !host->clk_enabled) {
		clk_enable(host->clk);
		sdhci_writeb(host->sdhci, 1, SDHCI_VENDOR_CLOCK_CNTRL);
		host->clk_enabled = 1;
	} else if (!enable && host->clk_enabled) {
		sdhci_writeb(host->sdhci, 0, SDHCI_VENDOR_CLOCK_CNTRL);
		clk_disable(host->clk);
		host->clk_enabled = 0;
	}
}

static void tegra_sdhci_set_clock(struct sdhci_host *sdhci, unsigned int clock)
{
	struct tegra_sdhci_host *host = sdhci_priv(sdhci);
	pr_debug("tegra sdhci clock %s %u enabled=%d\n",
		mmc_hostname(sdhci->mmc), clock, host->clk_enabled);

	tegra_sdhci_enable_clock(host, clock);
}

static struct sdhci_ops tegra_sdhci_ops = {
	.enable_dma = tegra_sdhci_enable_dma,
	.set_clock = tegra_sdhci_set_clock,
#ifdef CONFIG_EMBEDDED_MMC_START_OFFSET
	.get_startoffset	= tegra_sdhci_get_StartOffset,
#endif
};

static int __devinit tegra_sdhci_probe(struct platform_device *pdev)
{
	int rc;
	struct tegra_sdhci_platform_data *plat;
	struct sdhci_host *sdhci;
	struct tegra_sdhci_host *host;
	struct resource *res;
	int irq;
	void __iomem *ioaddr;

	plat = pdev->dev.platform_data;

	if (plat == NULL)
		return -ENXIO;

	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (res == NULL)
		return -ENODEV;

	irq = res->start;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (res == NULL)
		return -ENODEV;

	ioaddr = ioremap(res->start, res->end - res->start);

	sdhci = sdhci_alloc_host(&pdev->dev, sizeof(struct tegra_sdhci_host));
	if (IS_ERR(sdhci)) {
		rc = PTR_ERR(sdhci);
		goto err_unmap;
	}

	host = sdhci_priv(sdhci);
	host->sdhci = sdhci;
#ifdef CONFIG_EMBEDDED_MMC_START_OFFSET
	host->StartOffset = plat->startoffset;
	printk(KERN_INFO "tegra_sdhci_probe: host->StartOffset: %d\n", host->StartOffset);
#endif

	host->clk = clk_get(&pdev->dev, plat->clk_id);
	if (IS_ERR(host->clk)) {
		rc = PTR_ERR(host->clk);
		goto err_free_host;
	}

	rc = clk_enable(host->clk);
	if (rc != 0)
		goto err_clkput;

	host->clk_enabled = 1;
/* LGE_CHANGE_START, [jisung.yang@lge.com], 2010-11-22, <star smartphone patch> */
	host->card_always_on = plat->is_always_on;
	printk("[MINGI] card_always_on value is %d, platform device is is %d", host->card_always_on, pdev->id);
/* LGE_CHANGE_END, [jisung.yang@lge.com], 2010-11-22, <star smartphone patch> */

	sdhci->hw_name = "tegra";
	sdhci->ops = &tegra_sdhci_ops;
	sdhci->irq = irq;
	sdhci->ioaddr = ioaddr;
	sdhci->version = SDHCI_SPEC_200;
	sdhci->quirks = SDHCI_QUIRK_BROKEN_TIMEOUT_VAL |
			SDHCI_QUIRK_SINGLE_POWER_WRITE |
			SDHCI_QUIRK_ENABLE_INTERRUPT_AT_BLOCK_GAP |
			SDHCI_QUIRK_BROKEN_WRITE_PROTECT |
			SDHCI_QUIRK_BROKEN_CTRL_HISPD |
			SDHCI_QUIRK_NO_HISPD_BIT |
			SDHCI_QUIRK_8_BIT_DATA |
			SDHCI_QUIRK_NO_VERSION_REG |
			SDHCI_QUIRK_BROKEN_ADMA_ZEROLEN_DESC |
			SDHCI_QUIRK_RUNTIME_DISABLE;

	if (plat->force_hs != 0)
		sdhci->quirks |= SDHCI_QUIRK_FORCE_HIGH_SPEED_MODE;
#ifdef CONFIG_MMC_EMBEDDED_SDIO
	mmc_set_embedded_sdio_data(sdhci->mmc,
			&plat->cis,
			&plat->cccr,
			plat->funcs,
			plat->num_funcs);
#endif
	if (host->card_always_on)
		sdhci->mmc->pm_flags |= MMC_PM_IGNORE_PM_NOTIFY;

	rc = sdhci_add_host(sdhci);
	if (rc)
		goto err_clk_disable;

	platform_set_drvdata(pdev, host);

	if (plat->cd_gpio != -1) {
		rc = request_irq(gpio_to_irq(plat->cd_gpio), carddetect_irq,
			IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING,
			mmc_hostname(sdhci->mmc), sdhci);

		if (rc)
			goto err_remove_host;
	} else if (plat->register_status_notify) {
		plat->register_status_notify(
			tegra_sdhci_status_notify_cb, sdhci);
	}

	if (plat->board_probe)
		plat->board_probe(pdev->id, sdhci->mmc);

	printk(KERN_INFO "sdhci%d: initialized irq %d ioaddr %p\n", pdev->id,
			sdhci->irq, sdhci->ioaddr);

	return 0;

err_remove_host:
	sdhci_remove_host(sdhci, 1);
err_clk_disable:
	clk_disable(host->clk);
err_clkput:
	clk_put(host->clk);
err_free_host:
	if (sdhci)
		sdhci_free_host(sdhci);
err_unmap:
	iounmap(sdhci->ioaddr);

	return rc;
}

static int tegra_sdhci_remove(struct platform_device *pdev)
{
	struct tegra_sdhci_host *host = platform_get_drvdata(pdev);
	if (host) {
		struct tegra_sdhci_platform_data *plat;
		plat = pdev->dev.platform_data;
		if (plat && plat->board_probe)
			plat->board_probe(pdev->id, host->sdhci->mmc);

		sdhci_remove_host(host->sdhci, 0);
		sdhci_free_host(host->sdhci);
	}
	return 0;
}

/* LGE_CHANGE_START, [jisung.yang@lge.com], 2010-11-22, <star smartphone patch> */
#if defined(CONFIG_BRCM_LGE_WL_HOSTWAKEUP)
#include <linux/earlysuspend.h>
struct early_suspend dhdpm;
EXPORT_SYMBOL(dhdpm);

void register_mmc_card_pm(struct early_suspend *cardpm)
{
	if((cardpm != NULL) && (cardpm->suspend !=NULL) && (cardpm->resume != NULL))
	{
		dhdpm.suspend = cardpm->suspend;
		dhdpm.resume = cardpm->resume;
		printk("[sdhci-tegra]%s:%d - dhdpm callback func register OK\n",__func__,__LINE__);
	}
	else
		printk("[sdhci-tegra]%s:%d - dhdpm callback func register Fail\n",__func__,__LINE__);
}
EXPORT_SYMBOL(register_mmc_card_pm);

void unregister_mmc_card_pm(void)
{
	printk("[sdhci-tegra]%s:%d - dhdpm callback func set NULL\n",__func__,__LINE__);
	dhdpm.suspend = NULL;
	dhdpm.resume  = NULL;
}
EXPORT_SYMBOL(unregister_mmc_card_pm);
#endif /* CONFIG_BRCM_LGE_WL_HOSTWAKEUP */

#define is_card_sdio(_card) \
((_card) && ((_card)->type == MMC_TYPE_SDIO))

#ifdef CONFIG_PM
#define dev_to_host(_dev) platform_get_drvdata(to_platform_device(_dev))

static void tegra_sdhci_restore_interrupts(struct sdhci_host *sdhost)
{
	u32 ierr;
	u32 clear = SDHCI_INT_ALL_MASK;
	struct tegra_sdhci_host *host = sdhci_priv(sdhost);

	/* enable required interrupts */
	ierr = sdhci_readl(sdhost, SDHCI_INT_ENABLE);
	ierr &= ~clear;
	ierr |= host->sdhci_ints;
	sdhci_writel(sdhost, ierr, SDHCI_INT_ENABLE);
	sdhci_writel(sdhost, ierr, SDHCI_SIGNAL_ENABLE);

	if ((host->sdhci_ints & SDHCI_INT_CARD_INT) &&
		(sdhost->quirks & SDHCI_QUIRK_ENABLE_INTERRUPT_AT_BLOCK_GAP)) {
		u8 gap_ctrl = sdhci_readb(sdhost, SDHCI_BLOCK_GAP_CONTROL);
		gap_ctrl |= 0x8;
		sdhci_writeb(sdhost, gap_ctrl, SDHCI_BLOCK_GAP_CONTROL);
	}
}

static int tegra_sdhci_restore(struct sdhci_host *sdhost)
{
	unsigned long timeout;
	u8 mask = SDHCI_RESET_ALL;

	sdhci_writeb(sdhost, mask, SDHCI_SOFTWARE_RESET);

	sdhost->clock = 0;

	/* Wait max 100 ms */
	timeout = 100;

	/* hw clears the bit when it's done */
	while (sdhci_readb(sdhost, SDHCI_SOFTWARE_RESET) & mask) {
		if (timeout == 0) {
			printk(KERN_ERR "%s: Reset 0x%x never completed.\n",
				mmc_hostname(sdhost->mmc), (int)mask);
			return -EIO;
		}
		timeout--;
		mdelay(1);
	}

	tegra_sdhci_restore_interrupts(sdhost);
	sdhost->last_clk = 0;
	return 0;
}
/* LGE_CHANGE_END, [jisung.yang@lge.com], 2010-11-22, <star smartphone patch> */

/* LGE_CHANGE_START, [jisung.yang@lge.com], 2010-11-22, <star smartphone patch> */
#if 1
static int tegra_sdhci_suspend(struct platform_device *pdev, pm_message_t state)
{
	struct tegra_sdhci_host *host = platform_get_drvdata(pdev);
	struct sdhci_host *sdhost = host->sdhci;
	//struct pm_message event = { PM_EVENT_SUSPEND };

	int ret;

	if (get_hw_rev() <= REV_1_2) {
		printk("[sdhci-tegra]%s:%d - enter, WLAN_EN [%d:%d]\n", __func__, __LINE__, TEGRA_GPIO_PQ5, gpio_get_value(TEGRA_GPIO_PQ5));
	} else {
		printk("[sdhci-tegra]%s:%d - enter, WLAN_EN [%d:%d]\n", __func__, __LINE__, TEGRA_GPIO_PU2, gpio_get_value(TEGRA_GPIO_PU2));
	}

	if (host->card_always_on && is_card_sdio(sdhost->mmc->card)) {
		struct mmc_ios ios;
		ios.clock = 0;
		ios.vdd = 0;
		ios.power_mode = MMC_POWER_OFF;
		ios.bus_width = MMC_BUS_WIDTH_1;
		ios.timing = MMC_TIMING_LEGACY;

#if defined(CONFIG_BRCM_LGE_WL_HOSTWAKEUP)
			if(dhdpm.suspend != NULL) {
				printk("[sdhci-tegra]%s:%d - call dhdpm.suspend\n",__func__,__LINE__);
				dhdpm.suspend(NULL);
			}
			else
				printk("[sdhci-tegra]%s:%d - dhdpm.suspend is NULL\n",__func__,__LINE__);
#endif /* CONFIG_BRCM_LGE_WL_HOSTWAKEUP */

		/* save interrupt status before suspending */
		host->sdhci_ints = sdhci_readl(sdhost, SDHCI_INT_ENABLE);
		sdhost->mmc->ops->set_ios(sdhost->mmc, &ios);
		/* keep CARD_INT enabled - if used as wakeup source */
		if (host->sdhci_ints & SDHCI_INT_CARD_INT) {
			u32 ier = sdhci_readl(host->sdhci, SDHCI_INT_ENABLE);
			ier |= SDHCI_INT_CARD_INT;
			sdhci_writel(host->sdhci, ier, SDHCI_INT_ENABLE);
			sdhci_writel(host->sdhci, ier, SDHCI_SIGNAL_ENABLE);

			if (sdhost->quirks & SDHCI_QUIRK_ENABLE_INTERRUPT_AT_BLOCK_GAP) {
				u8 gap_ctrl = sdhci_readb(sdhost, SDHCI_BLOCK_GAP_CONTROL);
				gap_ctrl |= 0x8;
				sdhci_writeb(sdhost, gap_ctrl, SDHCI_BLOCK_GAP_CONTROL);
			}
		}

		return ret;
	}

	ret = sdhci_suspend_host(host->sdhci, state);
	if (ret)
		pr_err("%s: failed, error = %d\n", __func__, ret);

	return ret;
}

static int tegra_sdhci_resume(struct platform_device *pdev)
{

	struct tegra_sdhci_host *host = platform_get_drvdata(pdev);
	struct sdhci_host *sdhost = host->sdhci;

	int ret;
	//int ret_tegra_sdhci_resume;

	if (get_hw_rev() <= REV_1_2) {
		printk("[sdhci-tegra]%s:%d - enter, WLAN_EN [%d:%d]\n", __func__, __LINE__, TEGRA_GPIO_PQ5, gpio_get_value(TEGRA_GPIO_PQ5));
	} else {
		printk("[sdhci-tegra]%s:%d - enter, WLAN_EN [%d:%d]\n", __func__, __LINE__, TEGRA_GPIO_PU2, gpio_get_value(TEGRA_GPIO_PU2));
	}

	tegra_sdhci_enable_clock(host, 1);

	if(host->card_always_on && is_card_sdio(sdhost->mmc->card)) {
		int ret = 0;


		/* soft reset SD host controller and enable interrupts */
		ret = tegra_sdhci_restore(sdhost);
		if (ret) {
			dev_err(&pdev->dev, "failed to resume host\n");
			return ret;
		}

		mmiowb();
		sdhost->mmc->ops->set_ios(sdhost->mmc, &sdhost->mmc->ios);
#if defined(CONFIG_BRCM_LGE_WL_HOSTWAKEUP)
        if(dhdpm.resume != NULL) {
            printk("[sdhci-tegra]%s:%d - call dhdpm.resume\n",__func__,__LINE__);
            dhdpm.resume(NULL);
        }
        else
            printk("[sdhci-tegra]%s:%d - dhdpm.resume is NULL\n",__func__,__LINE__);
#endif /* CONFIG_BRCM_LGE_WL_HOSTWAKEUP */
		return 0;
	}

	ret = sdhci_resume_host(host->sdhci);
	if (ret)
		pr_err("%s: failed, error = %d\n", __func__, ret);

	return ret;
}

#else
static int tegra_sdhci_suspend(struct platform_device *pdev, pm_message_t state)
{
	struct tegra_sdhci_host *host = platform_get_drvdata(pdev);
	int ret;

	ret = sdhci_suspend_host(host->sdhci, state);
	if (ret)
		pr_err("%s: failed, error = %d\n", __func__, ret);

	tegra_sdhci_enable_clock(host, 0);
	return ret;
}

static int tegra_sdhci_resume(struct platform_device *pdev)
{
	struct tegra_sdhci_host *host = platform_get_drvdata(pdev);
	int ret;

	tegra_sdhci_enable_clock(host, 1);
	ret = sdhci_resume_host(host->sdhci);
	if (ret)
		pr_err("%s: failed, error = %d\n", __func__, ret);

	return ret;
}
#endif
/* LGE_CHANGE_END, [jisung.yang@lge.com], 2010-11-22, <star smartphone patch> */
#else
#define tegra_sdhci_suspend    NULL
#define tegra_sdhci_resume     NULL
#endif

static struct platform_driver tegra_sdhci_driver = {
	.probe = tegra_sdhci_probe,
	.remove = tegra_sdhci_remove,
	.suspend = tegra_sdhci_suspend,
	.resume = tegra_sdhci_resume,
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
	},
};

static int __init tegra_sdhci_init(void)
{
	return platform_driver_register(&tegra_sdhci_driver);
}

static void __exit tegra_sdhci_exit(void)
{
	platform_driver_unregister(&tegra_sdhci_driver);
}

module_init(tegra_sdhci_init);
module_exit(tegra_sdhci_exit);

MODULE_DESCRIPTION("Tegra SDHCI controller driver");
MODULE_LICENSE("GPL");
