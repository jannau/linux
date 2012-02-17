/* Tegra LM1 based backlight data *
 *
 * Copyright (C) 2010 NVIDIA Corporation
 * Author: Renuka Apte <rapte@nvidia.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#ifndef TEGRA_LM1_H
#define TEGRA_LM1_H

struct platform_tegra_lm1_backlight_data {
    int lm1_id;
    unsigned int dft_brightness;
    unsigned int max_brightness;
    unsigned int period;
    unsigned int clk_div;
    unsigned int clk_select;
};

#endif // TERGA_LM1_H
