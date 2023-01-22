// SPDX-License-Identifier: GPL-2.0
/*
 * DisplayPort alternate mode support for TI TPS6598x and Apple CD321x
 * Copyright (C) 2022 Sven Peter <sven@svenpeter.dev>
 *
 * Note that this driver currently assumes that the chip is running in
 * AMAutomaticMode and handles the DisplayPort alternate mode entry
 * itself. It just exposes the results to the typec subsystem to forward
 * the current state to the typec_mux and the HPD signal to DRM.
 */

#include "tps6598x.h"
#include "trace.h"

#include <linux/usb/pd_vdo.h>
#include <linux/usb/role.h>
#include <linux/usb/typec.h>
#include <linux/usb/typec_altmode.h>
#include <linux/usb/typec_dp.h>

static int tps6598x_displayport_register_partner(struct tps6598x *tps, u32 vdo)
{
	struct typec_altmode_desc desc = {};

	desc.svid = USB_TYPEC_DP_SID;
	desc.mode = USB_TYPEC_DP_MODE;
	desc.vdo = vdo;

	tps->dp_configured = false;
	tps->dp_partner = typec_partner_register_altmode(tps->partner, &desc);
	if (IS_ERR(tps->dp_partner)) {
		int ret = PTR_ERR(tps->dp_partner);
		tps->dp_partner = NULL;
		return ret;
	}

	return 0;
}

void tps6598x_displayport_update_dp_sid(struct tps6598x *tps)
{
	struct tps6598x_dp_sid_status status;
	u32 dp;
	int ret;

	dev_err(tps->dev, "%s\n", __func__);

	if (!tps->dp_port)
		return;

	ret = tps6598x_block_read(tps, TPS_REG_DP_SID_STATUS, &status,
				  sizeof(status));
	if (ret) {
		dev_err(tps->dev, "failed to read DP_SID_STATUS: %d\n", ret);
		return;
	}
	trace_tps6598x_dp_sid_status(&status);

	mutex_lock(&tps->dp_lock);
	tps->dp_status = status;
	mutex_unlock(&tps->dp_lock);

	/*
	 * Unregister partner altmode if it's not active since we don't support
	 * control anyway.
	 */
	if (!(status.status & TPS_DP_SID_ACTIVE)) {
		tps6598x_displayport_unregister_partner(tps);
		return;
	}

	if (!tps->dp_partner) {
		u32 vdo = le32_to_cpu(status.dp_mode);
		ret = tps6598x_displayport_register_partner(tps, vdo);
		if (ret) {
			dev_err(tps->dev, "failed to register DP partner: %d\n",
				ret);
			return;
		}
	}
	dp = le32_to_cpu(status.dp_status_rx);
	dev_err(tps->dev, "%s: dp_status_rx: %s %s %s (0x%02x)\n", __func__,
		dp & DP_STATUS_SWITCH_TO_USB ? "SW_USB" : "",
		dp & DP_STATUS_EXIT_DP_MODE ? "EXIT_DP" : "",
		dp & DP_STATUS_HPD_STATE ? "HPD" : "", dp);

	if (!tps->dp_configured)
		return;
	typec_altmode_attention(tps->dp_port, le32_to_cpu(status.dp_status_rx));
}

static int tps6598x_displayport_enter(struct typec_altmode *alt, u32 *vdo)
{
	struct tps6598x *tps = typec_altmode_get_drvdata(alt);
	int svdm_version, ret = 0;

	{
		const struct typec_altmode *partner =
			typec_altmode_get_partner(alt);
		dev_warn(&partner->dev, "%s:\n", __func__);
	}

	mutex_lock(&tps->dp_lock);
	if (tps->dp_configured) {
		const struct typec_altmode *p = typec_altmode_get_partner(alt);

		dev_warn(
			&p->dev,
			"Firmware doesn't support alternate mode overriding\n");
		ret = -EOPNOTSUPP;
		goto out_unlock;
	}

	svdm_version = typec_altmode_get_svdm_version(tps->dp_port);
	if (svdm_version < 0) {
		ret = svdm_version;
		goto out_unlock;
	}

	if (tps->dp_vdo_header) {
		ret = -EBUSY;
		goto out_unlock;
	}

	tps->dp_vdo_header =
		VDO(USB_TYPEC_DP_SID, 1, svdm_version, CMD_ENTER_MODE);
	tps->dp_vdo_header |= VDO_OPOS(USB_TYPEC_DP_MODE);
	tps->dp_vdo_header |= VDO_CMDT(CMDT_RSP_ACK);
	schedule_work(&tps->dp_work);

out_unlock:
	mutex_unlock(&tps->dp_lock);
	return ret;
}

static int tps6598x_displayport_vdm(struct typec_altmode *alt, u32 header,
				    const u32 *data, int count)
{
	const struct typec_altmode *partner = typec_altmode_get_partner(alt);
	struct tps6598x *tps = typec_altmode_get_drvdata(alt);
	int cmd_type = PD_VDO_CMDT(header);
	int cmd = PD_VDO_CMD(header);
	int svdm_version;
	int ret = 0;

	dev_warn(&partner->dev, "%s: cmd: %d\n", __func__, cmd);

	mutex_lock(&tps->dp_lock);

	// if (tps->dp_configured) {
	// 	dev_warn(
	// 		&partner->dev,
	// 		"Firmware doesn't support alternate mode overriding\n");
	// 	ret = -EOPNOTSUPP;
	// 	goto out_unlock;
	// }

	svdm_version = typec_altmode_get_svdm_version(alt);
	if (svdm_version < 0) {
		ret = svdm_version;
		goto out_unlock;
	}

	if (cmd_type != CMDT_INIT) {
		dev_warn(&partner->dev, "Unexpected VDM type: 0x%08x\n",
			 cmd_type);
		goto out_unlock;
	}

	if (PD_VDO_SVDM_VER(header) < svdm_version) {
		typec_partner_set_svdm_version(tps->partner,
					       PD_VDO_SVDM_VER(header));
		svdm_version = PD_VDO_SVDM_VER(header);
	}

	tps->dp_vdo_header = VDO(USB_TYPEC_DP_SID, 1, svdm_version, cmd);
	tps->dp_vdo_header |= VDO_OPOS(USB_TYPEC_DP_MODE);

	switch (cmd) {
	case DP_CMD_STATUS_UPDATE:
		tps->dp_vdo_header |= VDO_CMDT(CMDT_RSP_ACK);
		tps->dp_vdo_send_status = true;
		break;
	case DP_CMD_CONFIGURE:
		tps->dp_vdo_header |= VDO_CMDT(CMDT_RSP_ACK);
		typec_altmode_update_active(tps->dp_partner, true);
		tps->dp_configured = true;
		/*
		 * On Apple Silicon platforms we switched to USB_ROLE_NONE
		 * before setting up the alternate mode and have to bring dwc3
		 * back up here.
		 */
		// if (tps->cd321x)
		// 	usb_role_switch_set_role(tps->role_sw, USB_ROLE_HOST);
		break;
	default:
		dev_warn(&partner->dev, "Unexpected VDM cmd: 0x%08x\n", cmd);
		tps->dp_vdo_header |= VDO_CMDT(CMDT_RSP_NAK);
		break;
	}
	dev_warn(&partner->dev, "%s: done\n", __func__);

	schedule_work(&tps->dp_work);

out_unlock:
	mutex_unlock(&tps->dp_lock);
	return ret;
}

int tps6598x_displayport_notify(struct typec_altmode *alt, unsigned long conf,
				void *data)
{
	const struct typec_altmode *partner = typec_altmode_get_partner(alt);
	struct tps6598x *tps = typec_altmode_get_drvdata(alt);

	if (!tps->cd321x)
		return 0;

	/*
	 * On Apple Silicon platforms we have to switch to USB_ROLE_NONE before
	 * setting up the alternate mode to shut down dwc3 and prevent it from
	 * locking up.
	 */
	usb_role_switch_set_role(tps->role_sw, USB_ROLE_NONE);
	usb_role_switch_set_role(tps->role_sw, USB_ROLE_HOST);

	return 0;
}

static const struct typec_altmode_ops tps6598x_displayport_ops = {
	.enter = tps6598x_displayport_enter,
	.vdm = tps6598x_displayport_vdm,
	.notify = tps6598x_displayport_notify,
};

static void tps6598x_displayport_work(struct work_struct *work)
{
	struct tps6598x *tps = container_of(work, struct tps6598x, dp_work);
	int ret;
	u8 dp_vdo_header;
	u32 dp_vdo_data;
	u8 dp_vdo_size;

	mutex_lock(&tps->dp_lock);

	if (tps->dp_vdo_send_status) {
		dp_vdo_data = le32_to_cpu(tps->dp_status.dp_status_rx);
		dp_vdo_size = 1;
	} else {
		dp_vdo_size = 0;
	}

	dp_vdo_header = tps->dp_vdo_header;
	tps->dp_vdo_header = 0;
	tps->dp_vdo_send_status = false;

	/*
	 * Note that we have to unlock before calling typec_altmode_vdm here
	 * because otherwise the following lock inversion between the
	 * altmode displayport drivers's dp->lock (A) and tps->dp_lock (B) is
	 * at least theoretically possible:
	 *
	 *   dp_altmode_work (A) -> typec_altmode_enter -> tps6598x_displayport_enter (B)
	 *   tps6598x_altmode_work (B) -> typec_altmode_vdm -> dp_altmode_vdm (A)
	 *
	 * tps->dp_port is guaranteed to exist even outside of dp_lock since
	 * this work is cancelled before dp_port is unregistered and we've made
	 * local copies of everything else such that it's safe to unlock before
	 * calling typec_altmode_vdm here.
	 */
	mutex_unlock(&tps->dp_lock);

	ret = typec_altmode_vdm(tps->dp_port, dp_vdo_header,
				dp_vdo_size ? &dp_vdo_data : NULL, dp_vdo_size);
	if (ret)
		dev_err(&tps->dp_port->dev, "VDM 0x%x failed\n", dp_vdo_header);
}

int tps6598x_displayport_register_port(struct tps6598x *tps)
{
	struct typec_altmode_desc desc = {};
	struct tps6598x_dp_sid_config dp_sid_config;
	int ret;

	ret = tps6598x_block_read(tps, TPS_REG_DP_SID_CONFIG, &dp_sid_config,
				  sizeof(dp_sid_config));
	if (ret)
		return ret;

	trace_tps6598x_dp_sid_config(&dp_sid_config);
	if (!(dp_sid_config.config & TPS_DP_SID_ENABLE_DP_SID))
		return 0;
	if (!(dp_sid_config.config & TPS_DP_SID_ENABLE_DP_MODE))
		return 0;

	if (tps->cd321x) {
		/*
		 * Apple Silicon machines have reserved bits set inside the
		 * supported assignments and don't set the correct bits of
		 * actually supported assignments. Fix that up to known good
		 * values.
		 */
		const u8 dp_pin_assignments = BIT(DP_PIN_ASSIGN_C) |
					      BIT(DP_PIN_ASSIGN_D) |
					      BIT(DP_PIN_ASSIGN_E);
		dp_sid_config.dfp_d_assignments = dp_pin_assignments;
		dp_sid_config.ufp_d_assignments = dp_pin_assignments;
	}

	desc.svid = USB_TYPEC_DP_SID;
	desc.mode = USB_TYPEC_DP_MODE;
	desc.vdo = dp_sid_config.capabilities;
	desc.vdo |= dp_sid_config.dfp_d_assignments << 8;
	desc.vdo |= dp_sid_config.ufp_d_assignments << 16;

	tps->dp_port = typec_port_register_altmode(tps->port, &desc);
	if (IS_ERR(tps->dp_port)) {
		ret = PTR_ERR(tps->dp_port);
		tps->dp_port = NULL;
		return ret;
	}

	INIT_WORK(&tps->dp_work, tps6598x_displayport_work);
	typec_altmode_set_drvdata(tps->dp_port, tps);
	tps->dp_port->ops = &tps6598x_displayport_ops;

	return 0;
}

void tps6598x_displayport_unregister_port(struct tps6598x *tps)
{
	if (!tps->dp_port)
		return;

	cancel_work_sync(&tps->dp_work);
	typec_unregister_altmode(tps->dp_port);
	tps->dp_port = NULL;
}

void tps6598x_displayport_unregister_partner(struct tps6598x *tps)
{
	if (!tps->dp_partner)
		return;

	/*
	 * On Apple Silicon platforms we have to switch to USB_ROLE_NONE when
	 * disabling the alternate mode to shut down dwc3 and prevent it from
	 * locking up.
	 */
	if (tps->dp_configured && tps->cd321x)
		usb_role_switch_set_role(tps->role_sw, USB_ROLE_NONE);
	typec_altmode_update_active(tps->dp_partner, false);
	typec_unregister_altmode(tps->dp_partner);
	tps->dp_partner = NULL;
	tps->dp_configured = false;
}
