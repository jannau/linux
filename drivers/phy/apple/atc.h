// SPDX-License-Identifier: GPL-2.0
/*
 * Apple Type-C PHY driver
 *
 * Copyright (C) 2021 The Asahi Linux Contributors
 * Author: Sven Peter <sven@svenpeter.dev>
 */

#ifndef APPLE_PHY_ATC_H
#define APPLE_PHY_ATC_H 1

#include <linux/mutex.h>
#include <linux/phy/phy.h>
#include <linux/usb/typec_mux.h>
#include <linux/reset-controller.h>
#include <linux/types.h>
#include <linux/usb/typec.h>
#include <linux/usb/typec_altmode.h>
#include <linux/usb/typec_dp.h>
#include <linux/usb/typec_tbt.h>
#include <linux/workqueue.h>

enum atcphy_mode {
	APPLE_ATCPHY_MODE_OFF,
	APPLE_ATCPHY_MODE_USB2,
	APPLE_ATCPHY_MODE_USB3,
	APPLE_ATCPHY_MODE_USB3_DP,
	APPLE_ATCPHY_MODE_USB4,
	APPLE_ATCPHY_MODE_DP,
	APPLE_ATCPHY_MODE_MAX,
};

struct atcphy_mode_configuration {
	u32 crossbar;
	u32 lane_mode[2];
	bool set_swap;
};

struct atcphy_tunable {
	size_t sz;
	struct {
		u32 offset;
		u32 mask;
		u32 value;
	} * values;
};

struct apple_atcphy {
	struct device_node *np;
	struct device *dev;

	struct {
		unsigned int t8103_cio3pll_workaround : 1;
	} quirks;

	/* calibration fuse values; names adapted from macOS serial output and seem to be related to clocks/PLLs */
	struct {
		u32 aus_cmn_shm_vreg_trim;
		u32 auspll_rodco_encap;
		u32 auspll_rodco_bias_adjust;
		u32 auspll_fracn_dll_start_capcode;
		u32 auspll_dtc_vreg_adjust;
		u32 cio3pll_dco_coarsebin[2];
		u32 cio3pll_dll_start_capcode[2];
		u32 cio3pll_dtc_vreg_adjust;
	} fuses;

	/* tunables provided by firmware through the device tree */
	struct {
		struct atcphy_tunable axi2af;
		struct atcphy_tunable common;
		struct atcphy_tunable lane_usb3[2];
		struct atcphy_tunable lane_displayport[2];
		struct atcphy_tunable lane_usb4[2];
	} tunables;

	bool usb3_support;
	bool usb3_power_on;
	bool swap_lanes;

	bool usb3_configured;
	bool usb3_configure_setup_cio;
	enum atcphy_mode mode;
	enum atcphy_mode target_mode;

	struct {
		void __iomem *core;
		void __iomem *axi2af;
		void __iomem *usb2phy;
		void __iomem *pipehandler;
	} regs;

	struct phy *phy_usb2;
	struct phy *phy_usb3;
	struct phy_provider *phy_provider;
	struct reset_controller_dev rcdev;
	struct typec_switch *sw;
	struct typec_mux *mux;

	struct mutex lock;
};

#endif
