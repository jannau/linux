/*
 * include/sound/fm31392.h -- Platform data for FM31-392
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __LINUX_SND_FM31392_H
#define __LINUX_SND_FM31392_H

struct fm31392_platform_data {
	int gpio_power;
	int gpio_reset;
	int gpio_bypass;
};

#endif /* __LINUX_SND_FM31392_H */
