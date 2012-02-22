#include <linux/delay.h>
#include <linux/platform_device.h>

#include <linux/gpio.h>
#include <typedefs.h>
#include <bcmdefs.h>
#include <osl.h>
#include <bcmutils.h>
#include <siutils.h>
#include <hndsoc.h>
#include <sbchipc.h>
#include <pcicfg.h>

#ifdef CONFIG_MACH_SAMSUNG_VARIATION_TEGRA
extern void tegra_sdhci_force_presence_change();
extern void p3_wlan_gpio_enable();
extern void p3_wlan_gpio_disable();
#endif

#ifdef CONFIG_MACH_STARTABLET
extern void tegra_sdhci_force_presence_change();
extern void startablet_wlan_gpio_enable();
extern void startablet_wlan_gpio_disable();

#include "../mach-tegra/gpio-names.h"

static int interrupt_en_flag = 0;               //by sjpark 11-03-10
#endif


/* this is called by exit() */
void nvidia_wlan_poweron(int on, int flag)
{
        if (flag == 1) {
#ifdef CONFIG_MACH_SAMSUNG_VARIATION_TEGRA
                p3_wlan_gpio_enable();
                tegra_sdhci_force_presence_change();
#endif
#ifdef CONFIG_MACH_STARTABLET
		/* printk(KERN_CRIT "%s: on: %d, flag: %d\n", __func__, on, flag); */
		startablet_wlan_gpio_enable();
        /* } else if (flag == 2) { */
	/* 	printk(KERN_CRIT "%s: on: %d, flag: %d\n", __func__, on, flag); */
	/* 	if (get_hw_rev() <= REV_1_2) */
	/* 		gpio_set_value(TEGRA_GPIO_PQ5, 1); */
	/* 	else */
	/* 		gpio_set_value(TEGRA_GPIO_PU2, 1); */
	/* 	mdelay(150); */
#endif
	} else
		OSL_DELAY(150);
}
EXPORT_SYMBOL(nvidia_wlan_poweron);
void nvidia_wlan_poweroff(int off, int flag)
{
        if (flag == 1) {
#ifdef CONFIG_MACH_SAMSUNG_VARIATION_TEGRA
                p3_wlan_gpio_disable();
                tegra_sdhci_force_presence_change();
#endif
#ifdef CONFIG_MACH_STARTABLET
		/* printk(KERN_CRIT "%s: off: %d, flag: %d\n", __func__, off, flag); */
                startablet_wlan_gpio_disable();
		if (interrupt_en_flag == 1) {
			printk("[sj-debug] POWER OFF : enable irq.\n");
			if (get_hw_rev() <= REV_1_2)
				enable_irq(gpio_to_irq(TEGRA_GPIO_PQ5));        //by sjpark 11-03-10
			else
				enable_irq(gpio_to_irq(TEGRA_GPIO_PU2));       //by sjpark 11-03-10
			interrupt_en_flag = 0;          //by sjpark 11-03-11
		}
		mdelay(150);
        /* } else if (flag == 2) { */
	/* 	printk(KERN_CRIT "%s: off: %d, flag: %d\n", __func__, off, flag); */
	/* 	if (get_hw_rev() <= REV_1_2) { */
	/* 		disable_irq(gpio_to_irq(TEGRA_GPIO_PQ5));       //by sjpark 11-03-10 */
	/* 		gpio_set_value(TEGRA_GPIO_PQ5, 0); */
	/* 		interrupt_en_flag = 1;          //by sjpark 11-03-11 */
	/* 	} else { */
	/* 		disable_irq(gpio_to_irq(TEGRA_GPIO_PU2));       //by sjpark 11-03-10 */
	/* 		gpio_set_value(TEGRA_GPIO_PU2, 0); */
	/* 		interrupt_en_flag = 1;          //by sjpark 11-03-11 */
	/* 	} */
	/* 	mdelay(150); */
#endif
        } else
		pr_info("nvidia_wlan_poweroff ==== skip\n");
}
EXPORT_SYMBOL(nvidia_wlan_poweroff);
