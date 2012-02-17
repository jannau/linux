/*
 * arch/arm/mach-tegra/include/mach/hardware.h
 *
 * Copyright (C) 2010 Google, Inc.
 *
 * Author:
 *	Colin Cross <ccross@google.com>
 *	Erik Gilling <konkers@google.com>
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

#ifndef __MACH_TEGRA_HARDWARE_H
#define __MACH_TEGRA_HARDWARE_H

typedef enum
{
    REV_A = 0,
    REV_C,
    REV_E,
    REV_F,
    REV_G,
    REV_H,
    REV_I,
    REV_1_0,
    REV_1_1,
    REV_1_2,
    REV_1_3,
    REV_UNKNOWN,  // If fail to get rev, the device might be handled as latest version
} hw_rev;

hw_rev get_hw_rev(void);

/*
 * Check if modem connected
 * return value
 *   1 : modem connected
 *   0 : modem is not connected
 */
int is_modem_connected(void);

#endif
