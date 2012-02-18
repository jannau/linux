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

extern void tegra_sdhci_force_presence_change();
extern void p3_wlan_gpio_enable();
extern void p3_wlan_gpio_disable();

/* this is called by exit() */
void nvidia_wlan_poweron(int on, int flag)
{
        if (flag == 1) {
                p3_wlan_gpio_enable();
                tegra_sdhci_force_presence_change();
        } else
                OSL_DELAY(150);
}
EXPORT_SYMBOL(nvidia_wlan_poweron);
void nvidia_wlan_poweroff(int off, int flag)
{
        if (flag == 1) {
                p3_wlan_gpio_disable();
                tegra_sdhci_force_presence_change();
        } else
                pr_info("nvidia_wlan_poweroff ==== skip\n");
}
EXPORT_SYMBOL(nvidia_wlan_poweroff);
