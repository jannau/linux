/*
 * arch/arm/mach-tegra/board-star.h
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

#ifndef _MACH_TEGRA_BOARD_STAR_H
#define _MACH_TEGRA_BOARD_STAR_H

int star_regulator_init(void);
void star_pinmux_init(void);
void star_fb_alloc(void);
int star_panel_init(void);
int star_sdhci_init(void);
int star_emc_init(void);

#endif
