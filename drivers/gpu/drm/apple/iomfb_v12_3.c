// SPDX-License-Identifier: GPL-2.0-only OR MIT
/* Copyright The Asahi Linux Contributors */

#include "iomfb_v12_3.h"
#include "iomfb_v13_2.h"
#include "version_utils.h"

static const struct dcp_method_entry dcp_methods[dcpep_num_methods] = {
	IOMFB_METHOD("A000", dcpep_late_init_signal),
	IOMFB_METHOD("A029", dcpep_setup_video_limits),
	IOMFB_METHOD("A131", iomfbep_a131_pmu_service_matched),
	IOMFB_METHOD("A132", iomfbep_a132_backlight_service_matched),
	IOMFB_METHOD("A357", dcpep_set_create_dfb),
	IOMFB_METHOD("A358", iomfbep_a358_vi_set_temperature_hint),
	IOMFB_METHOD("A401", dcpep_start_signal),
	IOMFB_METHOD("A407", dcpep_swap_start),
	IOMFB_METHOD("A408", dcpep_swap_submit),
	IOMFB_METHOD("A410", dcpep_set_display_device),
	IOMFB_METHOD("A411", dcpep_is_main_display),
	IOMFB_METHOD("A412", dcpep_set_digital_out_mode),
	IOMFB_METHOD("A426", iomfbep_get_color_remap_mode),
	IOMFB_METHOD("A439", dcpep_set_parameter_dcp),
	IOMFB_METHOD("A443", dcpep_create_default_fb),
	IOMFB_METHOD("A447", dcpep_enable_disable_video_power_savings),
	IOMFB_METHOD("A454", dcpep_first_client_open),
	IOMFB_METHOD("A455", iomfbep_last_client_close),
	IOMFB_METHOD("A460", dcpep_set_display_refresh_properties),
	IOMFB_METHOD("A463", dcpep_flush_supports_power),
	IOMFB_METHOD("A468", dcpep_set_power_state),
};

#define DCP_FW v12_3
#define DCP_FW_VER DCP_FW_VERSION(12, 3, 0)

#include "iomfb_template.c"

void DCP_FW_NAME(iomfb_start)(struct apple_dcp *dcp)
{
	dcp->cb_handlers = cb_handlers;

	dcp_start_signal(dcp, false, dcp_started, NULL);
}

#undef DCP_FW_VER
#undef DCP_FW
