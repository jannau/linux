// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Bluetooth HCI driver for Broadcom 4377 family devices attached via PCIe
 *
 * Copyright (C) The Asahi Linux Contributors
 *
 * Broadcom BCM4377-family PCI devices are combined Wireless LAN/Bluetooth
 * device which expose WiFi and Bluetooth as separate PCI functions.
 *
 * The Bluetooth function uses a simple IPC protocol based on DMA addressable
 * shared memory called "Converged IPC" in order to tunnel HCI frames.
 * Communication between the host and the devices happens over "transfer rings"
 * and "completion rings" for the separate transport types: Control, HCI,
 * ACL and SCO.
 *
 * The (official) terms "completion ring" and "transfer ring" are a bit
 * misleading:
 * For transfers from the host to the device an entry is enqueued in the
 * transfer ring and the device will acknowledge it by enqueueing and entry
 * in the corresponding completion ring.
 * For transfer initiated from the device however an entry will be enqueued
 * inside the completion ring (which has not corresponding entry in the transfer
 * ring). The transfer ring for this direction has no memory associated but
 * just a head and tail pointer. The message from the device is acknowledged
 * by simple advancing the head of the transfer ring and ringing a doorbell.
 *
 */

//#define DEBUG

#include <linux/async.h>
#include <linux/bitfield.h>
#include <linux/completion.h>
#include <linux/dma-mapping.h>
#include <linux/dmi.h>
#include <linux/firmware.h>
#include <linux/module.h>
#include <linux/msi.h>
#include <linux/of.h>
#include <linux/pci.h>
#include <linux/printk.h>

#include <asm/unaligned.h>

#include <net/bluetooth/bluetooth.h>
#include <net/bluetooth/hci_core.h>

enum bcm4377_chip {
	BCM4377 = 0,
	BCM4378,
	BCM4387,
};

#define BCM4377_DEVICE_ID 0x5fa0
#define BCM4378_DEVICE_ID 0x5f69
#define BCM4387_DEVICE_ID 0x5f71

#define BCM4377_DEFAULT_TIMEOUT 1000

/*
 * These devices only support DMA transactions inside a 32bit window
 * (possibly to avoid 64 bit arithmetic). The window size cannot exceed
 * 0xffffffff but is always aligned down to the previous 0x200 byte boundary
 * which effectively limits the window to [start, start+0xfffffe00].
 * We just limit the DMA window to [0, 0xfffffe00] to make sure we don't
 * run into this limitation.
 */
#define BCM4377_DMA_MASK 0xfffffe00

/* vendor-specific config space registers */
#define BCM4377_PCIECFG_BAR0_WINDOW0 0x80
#define BCM4377_PCIECFG_BAR0_WINDOW1 0x70
#define BCM4377_PCIECFG_BAR0_WINDOW4 0x74
#define BCM4377_PCIECFG_BAR0_WINDOW5 0x78
#define BCM4377_PCIECFG_BAR2_WINDOW 0x84

#define BCM4377_PCIECFG_BAR0_WINDOW4_DEFAULT 0x18011000
#define BCM4377_PCIECFG_BAR2_WINDOW_DEFAULT 0x19000000

#define BCM4377_PCIECFG_UNK_CTRL 0x88

/* BAR0 */
#define BCM4377_OTP_SIZE 0xe0
#define BCM4377_OTP_SYS_VENDOR 0x15
#define BCM4377_OTP_CIS 0x80
#define BCM4377_OTP_VENDOR_HDR 0x00000008
#define BCM4377_OTP_MAX_PARAM_LEN 16

#define BCM4377_BAR0_FW_DOORBELL 0x140
#define BCM4377_BAR0_RTI_CONTROL 0x144

#define BCM4377_BAR0_DOORBELL 0x174
#define BCM4377_BAR0_DOORBELL_VALUE GENMASK(31, 16)
#define BCM4377_BAR0_DOORBELL_IDX GENMASK(15, 8)
#define BCM4377_BAR0_DOORBELL_RING BIT(5)

#define BCM4377_BAR0_MSI_ADDR_LO 0x580
#define BCM4377_BAR0_MSI_ADDR_HI 0x584

#define BCM4377_BAR0_HOST_WINDOW_LO 0x590
#define BCM4377_BAR0_HOST_WINDOW_HI 0x594
#define BCM4377_BAR0_HOST_WINDOW_SIZE 0x598

/* BAR2 */
#define BCM4377_BAR2_BOOTSTAGE 0x200454

#define BCM4377_BAR2_FW_LO 0x200478
#define BCM4377_BAR2_FW_HI 0x20047c
#define BCM4377_BAR2_FW_SIZE 0x200480

#define BCM4377_BAR2_RTI_MSI_ADDR_LO 0x2004f8
#define BCM4377_BAR2_RTI_MSI_ADDR_HI 0x2004fc
#define BCM4377_BAR2_RTI_MSI_DATA 0x200500

#define BCM4377_BAR2_CONTEXT_ADDR_LO 0x20048c
#define BCM4377_BAR2_CONTEXT_ADDR_HI 0x200450

#define BCM4377_BAR2_RTI_STATUS 0x20045c
#define BCM4377_BAR2_RTI_WINDOW_LO 0x200494
#define BCM4377_BAR2_RTI_WINDOW_HI 0x200498
#define BCM4377_BAR2_RTI_WINDOW_SIZE 0x20049c

#define BCM4377_N_TRANSFER_RINGS 9
#define BCM4377_N_COMPLETION_RINGS 6

#define BCM4377_CONTROL_MSG_SIZE 0x34

#define BCM4377_MAX_RING_SIZE 256

#define BCM4377_MSGID_GENERATION GENMASK(15, 8)
#define BCM4377_MSGID_ID GENMASK(7, 0)

enum bcm4377_transfer_ring_id {
	BCM4377_XFER_RING_CONTROL = 0,
	BCM4377_XFER_RING_HCI_H2D = 1,
	BCM4377_XFER_RING_HCI_D2H = 2,
	BCM4377_XFER_RING_SCO_H2D = 3,
	BCM4377_XFER_RING_SCO_D2H = 4,
	BCM4377_XFER_RING_ACL_H2D = 5,
	BCM4377_XFER_RING_ACL_D2H = 6,
};

enum bcm4377_completion_ring_id {
	BCM4377_ACK_RING_CONTROL = 0,
	BCM4377_ACK_RING_HCI_ACL = 1,
	BCM4377_EVENT_RING_HCI_ACL = 2,
	BCM4377_ACK_RING_SCO = 3,
	BCM4377_EVENT_RING_SCO = 4,
};

enum bcm4377_doorbell {
	BCM4377_DOORBELL_CONTROL = 0,
	BCM4377_DOORBELL_HCI_H2D = 1,
	BCM4377_DOORBELL_HCI_D2H = 2,
	BCM4377_DOORBELL_ACL_H2D = 3,
	BCM4377_DOORBELL_ACL_D2H = 4,
	BCM4377_DOORBELL_SCO = 6,
};

#define BCM4377_XFER_RING_MAX_INPLACE_PAYLOAD_SIZE (4 * 0xff)

struct bcm4377_xfer_ring_entry {
#define BCM4377_XFER_RING_FLAG_PAYLOAD_MAPPED BIT(0)
#define BCM4377_XFER_RING_FLAG_PAYLOAD_IN_FOOTER BIT(1)
	u8 flags;
	__le16 len;
	u8 _unk0;
	__le64 payload;
	__le16 id;
	u8 _unk1[2];
} __packed;
static_assert(sizeof(struct bcm4377_xfer_ring_entry) == 0x10);

struct bcm4377_completion_ring_entry {
	u8 flags;
	u8 _unk0;
	__le16 ring_id;
	__le16 msg_id;
	__le32 len;
	u8 _unk1[6];
} __packed;
static_assert(sizeof(struct bcm4377_completion_ring_entry) == 0x10);

enum bcm4377_control_message_type {
	BCM4377_CONTROL_MSG_CREATE_XFER_RING = 1,
	BCM4377_CONTROL_MSG_CREATE_COMPLETION_RING = 2,
	BCM4377_CONTROL_MSG_DESTROY_XFER_RING = 3,
	BCM4377_CONTROL_MSG_DESTROY_COMPLETION_RING = 4,
	// BCM4377_CONTROL_MSG_ABORT_CMDQ = 5,
};

struct bcm4377_create_completion_ring_msg {
	u8 msg_type;
	u8 header_size;
	u8 footer_size;
	u8 _unk0;
	__le16 id;
	__le16 id_again;
	__le64 ring_iova;
	__le16 n_elements;
	__le32 unk;
	u8 _unk1[6];
	__le16 msi;
	__le16 intmod_delay;
	__le32 intmod_bytes;
	__le16 accum_delay;
	__le32 accum_bytes;
	u8 _unk2[10];
} __packed;
static_assert(sizeof(struct bcm4377_create_completion_ring_msg) ==
	      BCM4377_CONTROL_MSG_SIZE);

struct bcm4377_destroy_completion_ring_msg {
	u8 msg_type;
	u8 _pad0;
	__le16 ring_id;
	u8 _pad1[48];
} __packed;
static_assert(sizeof(struct bcm4377_destroy_completion_ring_msg) ==
	      BCM4377_CONTROL_MSG_SIZE);

struct bcm4377_create_transfer_ring_msg {
	u8 msg_type;
	u8 header_size;
	u8 footer_size;
	u8 _unk0;
	__le16 ring_id;
	__le16 ring_id_again;
	__le64 ring_iova;
	u8 _unk1[8];
	__le16 n_elements;
	__le16 completion_ring_id;
	__le16 doorbell;
#define BCM4377_XFER_RING_FLAG_VIRTUAL BIT(7)
#define BCM4377_XFER_RING_FLAG_SYNC BIT(8)
	__le16 flags;
	u8 _unk2[20];
} __packed;
static_assert(sizeof(struct bcm4377_create_transfer_ring_msg) ==
	      BCM4377_CONTROL_MSG_SIZE);

struct bcm4377_destroy_transfer_ring_msg {
	u8 msg_type;
	u8 _pad0;
	__le16 ring_id;
	u8 _pad1[48];
} __packed;
static_assert(sizeof(struct bcm4377_destroy_transfer_ring_msg) ==
	      BCM4377_CONTROL_MSG_SIZE);

struct bcm4377_context {
	__le16 version;
	__le16 size;
	__le32 enabled_caps;

	__le64 peripheral_info_addr;

	/* ring heads and tails */
	__le64 completion_ring_heads_addr;
	__le64 xfer_ring_tails_addr;
	__le64 completion_ring_tails_addr;
	__le64 xfer_ring_heads_addr;
	__le16 n_completion_rings;
	__le16 n_xfer_rings;

	/* control ring configuration */
	__le64 control_completion_ring_addr;
	__le64 control_xfer_ring_addr;
	__le16 control_xfer_ring_n_entries;
	__le16 control_completion_ring_n_entries;
	__le16 control_xfer_ring_doorbell;
	__le16 control_completion_ring_doorbell;
	__le16 control_xfer_ring_msi;
	__le16 control_completion_ring_msi;
	u8 control_xfer_ring_header_size;
	u8 control_xfer_ring_footer_size;
	u8 control_completion_ring_header_size;
	u8 control_completion_ring_footer_size;

	__le16 _unk0; // inPlaceComp and oOOComp
	__le16 _unk1; // piMsi -> interrupt for new perInfo data?

	__le64 scratch_pad;
	__le32 scratch_pad_size;

	__le32 res;
} __packed;
static_assert(sizeof(struct bcm4377_context) == 0x68);

struct bcm4377_hci_send_calibration_cmd {
	u8 unk;
	__le16 blocks_left;
	u8 data[0xe6];
} __packed;

struct bcm4377_hci_send_ptb_cmd {
	__le16 blocks_left;
	u8 data[0xcf];
} __packed;

struct bcm4377_ring_state {
	__le16 completion_ring_head[BCM4377_N_COMPLETION_RINGS];
	__le16 completion_ring_tail[BCM4377_N_COMPLETION_RINGS];
	__le16 xfer_ring_head[BCM4377_N_TRANSFER_RINGS];
	__le16 xfer_ring_tail[BCM4377_N_TRANSFER_RINGS];
};

struct bcm4377_transfer_ring {
	enum bcm4377_transfer_ring_id ring_id;
	enum bcm4377_doorbell doorbell;
	size_t payload_size;
	size_t mapped_payload_size;
	u8 completion_ring;
	u16 n_entries;
	u8 generation;

	bool sync;
	bool virtual;
	bool d2h_buffers_only;
	bool allow_wait;
	bool enabled;

	void *ring;
	dma_addr_t ring_dma;

	void *payloads;
	dma_addr_t payloads_dma;

	struct completion **events;
	DECLARE_BITMAP(msgids, BCM4377_MAX_RING_SIZE);
	spinlock_t lock;
};

struct bcm4377_completion_ring {
	enum bcm4377_completion_ring_id ring_id;
	u16 payload_size;
	u16 delay;
	u16 n_entries;
	bool enabled;

	u16 head;
	u16 tail;

	void *ring;
	dma_addr_t ring_dma;

	unsigned long transfer_rings;
};

struct bcm4377_data;

struct bcm4377_hw {
	const char *name;

	u32 otp_offset;

	u32 bar0_window0;
	u32 bar0_window1;
	u32 bar0_window5;

	bool has_bar0_window5;
	bool m2m_reset_on_ss_reset_disabled;

	const char *board_type;

	int (*send_calibration)(struct bcm4377_data *bcm4377);
	int (*send_ptb)(struct bcm4377_data *bcm4377);
};

struct bcm4377_data {
	struct pci_dev *pdev;
	struct hci_dev *hdev;

	void __iomem *bar0;
	void __iomem *bar2;

	u32 bootstage;
	u32 rti_status;

	const struct bcm4377_hw *hw;

	const void *taurus_cal_blob;
	int taurus_cal_size;
	const void *taurus_beamforming_cal_blob;
	int taurus_beamforming_cal_size;

	char stepping[BCM4377_OTP_MAX_PARAM_LEN];
	char vendor[BCM4377_OTP_MAX_PARAM_LEN];
	const char *board_type;

	struct completion event;

	int irq;

	struct bcm4377_context *ctx;
	dma_addr_t ctx_dma;

	struct bcm4377_ring_state *ring_state;
	dma_addr_t ring_state_dma;

	/*
	 * The HCI and ACL rings have to be merged because this structure is
	 * hardcoded in the firmware.
	 */
	struct bcm4377_completion_ring control_ack_ring;
	struct bcm4377_completion_ring hci_acl_ack_ring;
	struct bcm4377_completion_ring hci_acl_event_ring;
	struct bcm4377_completion_ring sco_ack_ring;
	struct bcm4377_completion_ring sco_event_ring;

	struct bcm4377_transfer_ring control_h2d_ring;
	struct bcm4377_transfer_ring hci_h2d_ring;
	struct bcm4377_transfer_ring hci_d2h_ring;
	struct bcm4377_transfer_ring sco_h2d_ring;
	struct bcm4377_transfer_ring sco_d2h_ring;
	struct bcm4377_transfer_ring acl_h2d_ring;
	struct bcm4377_transfer_ring acl_d2h_ring;
};

static void bcm4377_ring_doorbell(struct bcm4377_data *bcm4377, u8 doorbell,
				  u16 val)
{
	u32 db = 0;

	db |= FIELD_PREP(BCM4377_BAR0_DOORBELL_VALUE, val);
	db |= FIELD_PREP(BCM4377_BAR0_DOORBELL_IDX, doorbell);
	db |= BCM4377_BAR0_DOORBELL_RING;

	dev_dbg(&bcm4377->pdev->dev, "write %d to doorbell #%d (0x%x)\n", val,
		doorbell, db);
	iowrite32(db, bcm4377->bar0 + BCM4377_BAR0_DOORBELL);
}

static int bcm4377_extract_msgid(struct bcm4377_data *bcm4377,
				 struct bcm4377_transfer_ring *ring,
				 u16 raw_msgid, u8 *msgid)
{
	u8 generation = FIELD_GET(BCM4377_MSGID_GENERATION, raw_msgid);
	*msgid = FIELD_GET(BCM4377_MSGID_ID, raw_msgid);

	if (generation != ring->generation) {
		dev_warn(
			&bcm4377->pdev->dev,
			"invalid message generation %d should be %d in entry for ring %d\n",
			generation, ring->generation, ring->ring_id);
		return -EINVAL;
	}

	if (*msgid >= ring->n_entries) {
		dev_warn(&bcm4377->pdev->dev,
			 "invalid message id in entry for ring %d: %d > %d\n",
			 ring->ring_id, *msgid, ring->n_entries);
		return -EINVAL;
	}

	return 0;
}

static void bcm4377_handle_event(struct bcm4377_data *bcm4377,
				 struct bcm4377_transfer_ring *ring,
				 u16 raw_msgid, u8 entry_flags, u8 type,
				 void *payload, size_t len)
{
	struct sk_buff *skb;
	u16 head;
	u8 msgid;
	unsigned long flags;

	spin_lock_irqsave(&ring->lock, flags);
	if (!ring->enabled) {
		dev_warn(&bcm4377->pdev->dev,
			 "event for disabled transfer ring %d\n",
			 ring->ring_id);
		goto out;
	}

	if (ring->d2h_buffers_only &&
	    entry_flags & BCM4377_XFER_RING_FLAG_PAYLOAD_MAPPED) {
		if (bcm4377_extract_msgid(bcm4377, ring, raw_msgid, &msgid))
			goto out;

		if (len > ring->mapped_payload_size) {
			dev_warn(
				&bcm4377->pdev->dev,
				"invalid payload len in event for ring %d: %zu > %zu\n",
				ring->ring_id, len, ring->mapped_payload_size);
			goto out;
		}

		payload = ring->payloads + msgid * ring->mapped_payload_size;
	}

	skb = bt_skb_alloc(len, GFP_ATOMIC);
	if (!skb)
		goto out;

	memcpy(skb_put(skb, len), payload, len);
	hci_skb_pkt_type(skb) = type;
	hci_recv_frame(bcm4377->hdev, skb);

out:
	head = le16_to_cpu(bcm4377->ring_state->xfer_ring_head[ring->ring_id]);
	head = (head + 1) % ring->n_entries;
	bcm4377->ring_state->xfer_ring_head[ring->ring_id] = cpu_to_le16(head);

	bcm4377_ring_doorbell(bcm4377, ring->doorbell, head);

	spin_unlock_irqrestore(&ring->lock, flags);
}

static void bcm4377_handle_ack(struct bcm4377_data *bcm4377,
			       struct bcm4377_transfer_ring *ring,
			       u16 raw_msgid)
{
	unsigned long flags;
	u8 msgid;

	spin_lock_irqsave(&ring->lock, flags);

	if (bcm4377_extract_msgid(bcm4377, ring, raw_msgid, &msgid))
		goto unlock;

	if (!test_bit(msgid, ring->msgids)) {
		dev_warn(
			&bcm4377->pdev->dev,
			"invalid message id in ack for ring %d: %d is not used\n",
			ring->ring_id, msgid);
		goto unlock;
	}

	if (ring->allow_wait && ring->events[msgid]) {
		complete(ring->events[msgid]);
		ring->events[msgid] = NULL;
	}

	bitmap_release_region(ring->msgids, msgid, ring->n_entries);

unlock:
	spin_unlock_irqrestore(&ring->lock, flags);
}

static void bcm4377_handle_completion(struct bcm4377_data *bcm4377,
				      struct bcm4377_completion_ring *ring,
				      u16 pos)
{
	struct bcm4377_completion_ring_entry *entry;
	u16 msg_id, transfer_ring;
	size_t entry_size, data_len;
	void *data;

	if (pos >= ring->n_entries) {
		dev_warn(&bcm4377->pdev->dev, "invalid pos: %d\n", pos);
		return;
	}

	entry_size = sizeof(*entry) + ring->payload_size;
	entry = ring->ring + pos * entry_size;
	data = ring->ring + pos * entry_size + sizeof(*entry);
	data_len = le32_to_cpu(entry->len);
	msg_id = le16_to_cpu(entry->msg_id);
	transfer_ring = le16_to_cpu(entry->ring_id);

	if ((ring->transfer_rings & BIT(transfer_ring)) == 0) {
		dev_warn(
			&bcm4377->pdev->dev,
			"invalid entry at offset %d for transfer ring %d in completion ring %d\n",
			pos, transfer_ring, ring->ring_id);
		return;
	}

	dev_dbg(&bcm4377->pdev->dev,
		"entry in completion ring %d for transfer ring %d with msg_id %d\n",
		ring->ring_id, transfer_ring, msg_id);

	switch (transfer_ring) {
	case BCM4377_XFER_RING_CONTROL:
		bcm4377_handle_ack(bcm4377, &bcm4377->control_h2d_ring, msg_id);
		break;
	case BCM4377_XFER_RING_HCI_H2D:
		bcm4377_handle_ack(bcm4377, &bcm4377->hci_h2d_ring, msg_id);
		break;
	case BCM4377_XFER_RING_SCO_H2D:
		bcm4377_handle_ack(bcm4377, &bcm4377->sco_h2d_ring, msg_id);
		break;
	case BCM4377_XFER_RING_ACL_H2D:
		bcm4377_handle_ack(bcm4377, &bcm4377->acl_h2d_ring, msg_id);
		break;

	case BCM4377_XFER_RING_HCI_D2H:
		bcm4377_handle_event(bcm4377, &bcm4377->hci_d2h_ring, msg_id,
				     entry->flags, HCI_EVENT_PKT, data,
				     data_len);
		break;
	case BCM4377_XFER_RING_SCO_D2H:
		bcm4377_handle_event(bcm4377, &bcm4377->sco_d2h_ring, msg_id,
				     entry->flags, HCI_SCODATA_PKT, data,
				     data_len);
		break;
	case BCM4377_XFER_RING_ACL_D2H:
		bcm4377_handle_event(bcm4377, &bcm4377->acl_d2h_ring, msg_id,
				     entry->flags, HCI_ACLDATA_PKT, data,
				     data_len);
		break;

	default:
		dev_err(&bcm4377->pdev->dev,
			"entry in completion ring %d for unknown transfer ring %d with msg_id %d\n",
			ring->ring_id, transfer_ring, msg_id);
	}
}

static void bcm4377_poll_completion_ring(struct bcm4377_data *bcm4377,
					 struct bcm4377_completion_ring *ring)
{
	u16 tail;
	__le16 *heads = bcm4377->ring_state->completion_ring_head;
	__le16 *tails = bcm4377->ring_state->completion_ring_tail;

	if (!ring->enabled)
		return;

	tail = le16_to_cpu(tails[ring->ring_id]);
	dev_dbg(&bcm4377->pdev->dev,
		"completion ring #%d: head: %d, tail: %d\n", ring->ring_id,
		le16_to_cpu(heads[ring->ring_id]), tail);

	while (tail != le16_to_cpu(READ_ONCE(heads[ring->ring_id]))) {
		/*
		 * ensure the CPU doesn't speculate through the comparison.
		 * otherwise it might already read the (empty) queue entry
		 * before the updated head has been loaded and checked.
		 */
		dma_rmb();

		bcm4377_handle_completion(bcm4377, ring, tail);

		tail = (tail + 1) % ring->n_entries;
		tails[ring->ring_id] = cpu_to_le16(tail);
	}
}

static irqreturn_t bcm4377_irq(int irq, void *data)
{
	struct bcm4377_data *bcm4377 = data;
	u32 bootstage, rti_status;

	bootstage = ioread32(bcm4377->bar2 + BCM4377_BAR2_BOOTSTAGE);
	rti_status = ioread32(bcm4377->bar2 + BCM4377_BAR2_RTI_STATUS);

	if (bootstage != bcm4377->bootstage ||
	    rti_status != bcm4377->rti_status) {
		dev_dbg(&bcm4377->pdev->dev,
			"bootstage = %d -> %d, rti state = %d -> %d\n",
			bcm4377->bootstage, bootstage, bcm4377->rti_status,
			rti_status);
		complete(&bcm4377->event);
		bcm4377->bootstage = bootstage;
		bcm4377->rti_status = rti_status;
	}

	bcm4377_poll_completion_ring(bcm4377, &bcm4377->control_ack_ring);
	bcm4377_poll_completion_ring(bcm4377, &bcm4377->hci_acl_event_ring);
	bcm4377_poll_completion_ring(bcm4377, &bcm4377->hci_acl_ack_ring);
	bcm4377_poll_completion_ring(bcm4377, &bcm4377->sco_ack_ring);
	bcm4377_poll_completion_ring(bcm4377, &bcm4377->sco_event_ring);

	return IRQ_HANDLED;
}

static int bcm4377_enqueue(struct bcm4377_data *bcm4377,
			   struct bcm4377_transfer_ring *ring, void *data,
			   size_t len, bool wait)
{
	unsigned long flags;
	struct bcm4377_xfer_ring_entry *entry;
	void *payload;
	size_t offset;
	u16 head, tail, new_head;
	u16 raw_msgid;
	int ret, msgid;
	DECLARE_COMPLETION_ONSTACK(event);

	if (len > ring->payload_size && len > ring->mapped_payload_size) {
		dev_warn(
			&bcm4377->pdev->dev,
			"payload len %zu is too large for ring %d (max is %zu or %zu)\n",
			len, ring->ring_id, ring->payload_size,
			ring->mapped_payload_size);
		return -EINVAL;
	}
	if (wait && !ring->allow_wait)
		return -EINVAL;
	if (ring->virtual)
		return -EINVAL;

	spin_lock_irqsave(&ring->lock, flags);

	head = le16_to_cpu(bcm4377->ring_state->xfer_ring_head[ring->ring_id]);

	/* tail is changed using DMA; prevent stale reads */
	dma_rmb();
	tail = le16_to_cpu(bcm4377->ring_state->xfer_ring_tail[ring->ring_id]);

	new_head = (head + 1) % ring->n_entries;

	if (new_head == tail) {
		dev_warn(&bcm4377->pdev->dev,
			 "can't send message because ring %d is full\n",
			 ring->ring_id);
		ret = -EINVAL;
		goto out;
	}

	msgid = bitmap_find_free_region(ring->msgids, ring->n_entries, 0);
	if (msgid < 0) {
		dev_warn(&bcm4377->pdev->dev,
			 "can't find message id for ring %d\n", ring->ring_id);
		ret = -EINVAL;
		goto out;
	}

	raw_msgid = FIELD_PREP(BCM4377_MSGID_GENERATION, ring->generation);
	raw_msgid |= FIELD_PREP(BCM4377_MSGID_ID, msgid);

	offset = head * (sizeof(*entry) + ring->payload_size);
	entry = ring->ring + offset;

	memset(entry, 0, sizeof(*entry));
	entry->id = cpu_to_le16(raw_msgid);
	entry->len = cpu_to_le16(len);

	if (len <= ring->payload_size) {
		entry->flags = BCM4377_XFER_RING_FLAG_PAYLOAD_IN_FOOTER;
		payload = ring->ring + offset + sizeof(*entry);
	} else {
		entry->flags = BCM4377_XFER_RING_FLAG_PAYLOAD_MAPPED;
		entry->payload = cpu_to_le64(ring->payloads_dma +
					     msgid * ring->mapped_payload_size);
		payload = ring->payloads + msgid * ring->mapped_payload_size;
	}

	memcpy(payload, data, len);

	if (wait)
		ring->events[msgid] = &event;

	dev_dbg(&bcm4377->pdev->dev,
		"updating head for transfer queue #%d to %d\n", ring->ring_id,
		new_head);
	bcm4377->ring_state->xfer_ring_head[ring->ring_id] =
		cpu_to_le16(new_head);

	// TODO: check if this is actually correct for sync rings
	if (!ring->sync)
		bcm4377_ring_doorbell(bcm4377, ring->doorbell, new_head);
	ret = 0;

out:
	spin_unlock_irqrestore(&ring->lock, flags);

	if (ret == 0 && wait) {
		ret = wait_for_completion_interruptible_timeout(
			&event, BCM4377_DEFAULT_TIMEOUT);
		if (ret == 0)
			ret = -ETIMEDOUT;
		else if (ret > 0)
			ret = 0;

		spin_lock_irqsave(&ring->lock, flags);
		ring->events[msgid] = NULL;
		spin_unlock_irqrestore(&ring->lock, flags);
	}

	return ret;
}

static int bcm4377_create_completion_ring(struct bcm4377_data *bcm4377,
					  struct bcm4377_completion_ring *ring)
{
	struct bcm4377_create_completion_ring_msg msg;
	int ret;

	if (ring->enabled) {
		dev_warn(&bcm4377->pdev->dev, "ring already enabled\n");
		return 0;
	}

	memset(ring->ring, 0,
	       ring->n_entries * (sizeof(struct bcm4377_completion_ring_entry) +
				  ring->payload_size));
	memset(&msg, 0, sizeof(msg));
	msg.msg_type = BCM4377_CONTROL_MSG_CREATE_COMPLETION_RING;
	msg.id = cpu_to_le16(ring->ring_id);
	msg.id_again = cpu_to_le16(ring->ring_id);
	msg.ring_iova = cpu_to_le64(ring->ring_dma);
	msg.n_elements = cpu_to_le16(ring->n_entries);
	msg.intmod_bytes = cpu_to_le32(0xffffffff);
	msg.unk = cpu_to_le32(0xffffffff);
	msg.intmod_delay = cpu_to_le16(ring->delay);
	msg.footer_size = ring->payload_size / 4;

	ret = bcm4377_enqueue(bcm4377, &bcm4377->control_h2d_ring, &msg,
			      sizeof(msg), true);
	if (!ret)
		ring->enabled = true;

	return ret;
}

static int bcm4377_destroy_completion_ring(struct bcm4377_data *bcm4377,
					   struct bcm4377_completion_ring *ring)
{
	struct bcm4377_destroy_completion_ring_msg msg;
	int ret;

	memset(&msg, 0, sizeof(msg));
	msg.msg_type = BCM4377_CONTROL_MSG_DESTROY_COMPLETION_RING;
	msg.ring_id = cpu_to_le16(ring->ring_id);

	ret = bcm4377_enqueue(bcm4377, &bcm4377->control_h2d_ring, &msg,
			      sizeof(msg), true);
	if (ret)
		dev_warn(&bcm4377->pdev->dev,
			 "failed to destroy completion ring %d\n",
			 ring->ring_id);

	ring->enabled = false;
	return ret;
}

static int bcm4377_create_transfer_ring(struct bcm4377_data *bcm4377,
					struct bcm4377_transfer_ring *ring)
{
	struct bcm4377_create_transfer_ring_msg msg;
	u16 flags = 0;
	int ret, i;
	unsigned long spinlock_flags;

	if (ring->virtual)
		flags |= BCM4377_XFER_RING_FLAG_VIRTUAL;
	if (ring->sync)
		flags |= BCM4377_XFER_RING_FLAG_SYNC;

	spin_lock_irqsave(&ring->lock, spinlock_flags);
	memset(&msg, 0, sizeof(msg));
	msg.msg_type = BCM4377_CONTROL_MSG_CREATE_XFER_RING;
	msg.ring_id = cpu_to_le16(ring->ring_id);
	msg.ring_id_again = cpu_to_le16(ring->ring_id);
	msg.ring_iova = cpu_to_le64(ring->ring_dma);
	msg.n_elements = cpu_to_le16(ring->n_entries);
	msg.completion_ring_id = cpu_to_le16(ring->completion_ring);
	msg.doorbell = cpu_to_le16(ring->doorbell);
	msg.flags = cpu_to_le16(flags);
	msg.footer_size = ring->payload_size / 4;

	bcm4377->ring_state->xfer_ring_head[ring->ring_id] = 0;
	bcm4377->ring_state->xfer_ring_tail[ring->ring_id] = 0;
	ring->generation++;
	spin_unlock_irqrestore(&ring->lock, spinlock_flags);

	ret = bcm4377_enqueue(bcm4377, &bcm4377->control_h2d_ring, &msg,
			      sizeof(msg), true);

	spin_lock_irqsave(&ring->lock, spinlock_flags);

	if (ring->d2h_buffers_only) {
		for (i = 0; i < ring->n_entries; ++i) {
			struct bcm4377_xfer_ring_entry *entry =
				ring->ring + i * sizeof(*entry);
			u16 raw_msgid = FIELD_PREP(BCM4377_MSGID_GENERATION,
						   ring->generation);
			raw_msgid |= FIELD_PREP(BCM4377_MSGID_ID, i);

			memset(entry, 0, sizeof(*entry));
			entry->id = cpu_to_le16(raw_msgid);
			entry->len = cpu_to_le16(ring->mapped_payload_size);
			entry->flags = BCM4377_XFER_RING_FLAG_PAYLOAD_MAPPED;
			entry->payload =
				cpu_to_le64(ring->payloads_dma +
					    i * ring->mapped_payload_size);
		}
	}

	/* this primes the device->host side */
	if (ring->virtual || ring->d2h_buffers_only) {
		bcm4377->ring_state->xfer_ring_head[ring->ring_id] = 0xf;
		bcm4377_ring_doorbell(bcm4377, ring->doorbell, 0xf);
	}

	ring->enabled = true;
	spin_unlock_irqrestore(&ring->lock, spinlock_flags);

	return ret;
}

static int bcm4377_destroy_transfer_ring(struct bcm4377_data *bcm4377,
					 struct bcm4377_transfer_ring *ring)
{
	struct bcm4377_destroy_transfer_ring_msg msg;
	int ret;

	memset(&msg, 0, sizeof(msg));
	msg.msg_type = BCM4377_CONTROL_MSG_DESTROY_XFER_RING;
	msg.ring_id = cpu_to_le16(ring->ring_id);

	ret = bcm4377_enqueue(bcm4377, &bcm4377->control_h2d_ring, &msg,
			      sizeof(msg), true);
	if (ret)
		dev_warn(&bcm4377->pdev->dev,
			 "failed to destroy transfer ring %d\n", ring->ring_id);

	ring->enabled = false;
	return ret;
}

static int bcm4377_send_calibration(struct bcm4377_data *bcm4377,
				    const void *cal_blob, size_t cal_blob_size)
{
	struct bcm4377_hci_send_calibration_cmd cmd;
	struct sk_buff *skb;
	off_t done = 0;
	size_t left = cal_blob_size;
	u16 blocks_left;
	int ret;

	if (!cal_blob) {
		dev_err(&bcm4377->pdev->dev,
			"no calibration data available.\n");
		return -ENOENT;
	}

	blocks_left = DIV_ROUND_UP(left, sizeof(cmd.data)) - 1;

	while (left) {
		size_t transfer_len = min(left, sizeof(cmd.data));

		memset(&cmd, 0, sizeof(cmd));
		cmd.unk = 0x03;
		cmd.blocks_left = cpu_to_le16(blocks_left);
		memcpy(cmd.data, cal_blob + done, transfer_len);

		dev_dbg(&bcm4377->pdev->dev,
			"btbcmpci: sending calibration chunk; left (chunks): %d, left(bytes): %zu\n",
			cmd.blocks_left, left);

		skb = __hci_cmd_sync(bcm4377->hdev, 0xfd97, sizeof(cmd), &cmd,
				     HCI_INIT_TIMEOUT);
		if (IS_ERR(skb)) {
			ret = PTR_ERR(skb);
			dev_err(&bcm4377->pdev->dev,
				"btbcmpci: send calibration failed (%d)", ret);
			return ret;
		}
		kfree_skb(skb);

		blocks_left--;
		left -= transfer_len;
		done += transfer_len;
	}

	return 0;
}

static int bcm4378_send_calibration(struct bcm4377_data *bcm4377)
{
	if ((strcmp(bcm4377->stepping, "b1") == 0) ||
	    strcmp(bcm4377->stepping, "b3") == 0)
		return bcm4377_send_calibration(
			bcm4377, bcm4377->taurus_beamforming_cal_blob,
			bcm4377->taurus_beamforming_cal_size);
	else
		return bcm4377_send_calibration(bcm4377,
						bcm4377->taurus_cal_blob,
						bcm4377->taurus_cal_size);
}

static int bcm4387_send_calibration(struct bcm4377_data *bcm4377)
{
	if (strcmp(bcm4377->stepping, "c2") == 0)
		return bcm4377_send_calibration(
			bcm4377, bcm4377->taurus_beamforming_cal_blob,
			bcm4377->taurus_beamforming_cal_size);
	else
		return bcm4377_send_calibration(bcm4377,
						bcm4377->taurus_cal_blob,
						bcm4377->taurus_cal_size);
}

static const struct firmware *bcm4377_request_blob(struct bcm4377_data *bcm4377,
						   const char *suffix)
{
	const struct firmware *fw;
	char name[256];
	int ret;

	snprintf(name, sizeof(name), "brcm/brcmbt%s%s-%s-%s.%s",
		 bcm4377->hw->name, bcm4377->stepping, bcm4377->board_type,
		 bcm4377->vendor, suffix);
	dev_info(&bcm4377->pdev->dev, "Trying to load '%s'", name);

	ret = request_firmware(&fw, name, &bcm4377->pdev->dev);
	if (!ret)
		return fw;

	snprintf(name, sizeof(name), "brcm/brcmbt%s%s-%s.%s", bcm4377->hw->name,
		 bcm4377->stepping, bcm4377->board_type, suffix);
	dev_info(&bcm4377->pdev->dev, "Trying to load '%s'", name);

	ret = request_firmware(&fw, name, &bcm4377->pdev->dev);
	if (!ret)
		return fw;

	dev_err(&bcm4377->pdev->dev,
		"Unable to load firmware (type: %s, chip: %s, board: %s, stepping: %s, vendor: %s)",
		suffix, bcm4377->hw->name, bcm4377->board_type,
		bcm4377->stepping, bcm4377->vendor);
	return NULL;
}

static int bcm4377_send_ptb(struct bcm4377_data *bcm4377)
{
	const struct firmware *fw;
	struct sk_buff *skb;
	int ret = 0;

	fw = bcm4377_request_blob(bcm4377, "ptb");
	if (!fw) {
		dev_err(&bcm4377->pdev->dev, "failed to load PTB data");
		return -ENOENT;
	}

	skb = __hci_cmd_sync(bcm4377->hdev, 0xfd98, fw->size, fw->data,
			     HCI_INIT_TIMEOUT);
	if (IS_ERR(skb)) {
		ret = PTR_ERR(skb);
		dev_err(&bcm4377->pdev->dev, "sending ptb failed (%d)", ret);
		goto out;
	}
	kfree_skb(skb);

out:
	release_firmware(fw);
	return ret;
}

static int bcm4378_send_ptb(struct bcm4377_data *bcm4377)
{
	const struct firmware *fw;
	struct bcm4377_hci_send_ptb_cmd cmd;
	struct sk_buff *skb;
	off_t done = 0;
	size_t left;
	u16 blocks_left;
	int ret = 0;

	fw = bcm4377_request_blob(bcm4377, "ptb");
	if (!fw) {
		dev_err(&bcm4377->pdev->dev, "failed to load PTB data");
		return -ENOENT;
	}

	left = fw->size;
	blocks_left = DIV_ROUND_UP(left, sizeof(cmd.data)) - 1;

	while (left) {
		size_t transfer_len = min(left, sizeof(cmd.data));

		memset(&cmd, 0, sizeof(cmd));
		cmd.blocks_left = cpu_to_le16(blocks_left);
		memcpy(cmd.data, fw->data + done, transfer_len);

		dev_dbg(&bcm4377->pdev->dev,
			"btbcmpci: sending ptb chunk; left: %zu\n", left);

		skb = __hci_cmd_sync(bcm4377->hdev, 0xfe0d, sizeof(cmd), &cmd,
				     HCI_INIT_TIMEOUT);
		if (IS_ERR(skb)) {
			ret = PTR_ERR(skb);
			dev_err(&bcm4377->pdev->dev,
				"btbcmpci: sending ptb failed (%d)", ret);
			goto out;
		}
		kfree_skb(skb);

		blocks_left--;
		left -= transfer_len;
		done += transfer_len;
	}

out:
	release_firmware(fw);
	return ret;
}

static int bcm4377_hci_open(struct hci_dev *hdev)
{
	struct bcm4377_data *bcm4377 = hci_get_drvdata(hdev);
	int ret;

	ret = bcm4377_create_completion_ring(bcm4377,
					     &bcm4377->hci_acl_ack_ring);
	if (ret)
		return ret;
	ret = bcm4377_create_completion_ring(bcm4377,
					     &bcm4377->hci_acl_event_ring);
	if (ret)
		goto destroy_hci_acl_ack;
	ret = bcm4377_create_completion_ring(bcm4377, &bcm4377->sco_ack_ring);
	if (ret)
		goto destroy_hci_acl_event;
	ret = bcm4377_create_completion_ring(bcm4377, &bcm4377->sco_event_ring);
	if (ret)
		goto destroy_sco_ack;
	dev_dbg(&bcm4377->pdev->dev,
		"all completion rings successfully created!\n");

	ret = bcm4377_create_transfer_ring(bcm4377, &bcm4377->hci_h2d_ring);
	if (ret)
		goto destroy_sco_event;
	ret = bcm4377_create_transfer_ring(bcm4377, &bcm4377->hci_d2h_ring);
	if (ret)
		goto destroy_hci_h2d;
	ret = bcm4377_create_transfer_ring(bcm4377, &bcm4377->sco_h2d_ring);
	if (ret)
		goto destroy_hci_d2h;
	ret = bcm4377_create_transfer_ring(bcm4377, &bcm4377->sco_d2h_ring);
	if (ret)
		goto destroy_sco_h2d;
	ret = bcm4377_create_transfer_ring(bcm4377, &bcm4377->acl_h2d_ring);
	if (ret)
		goto destroy_sco_d2h;
	ret = bcm4377_create_transfer_ring(bcm4377, &bcm4377->acl_d2h_ring);
	if (ret)
		goto destroy_acl_h2d;
	dev_dbg(&bcm4377->pdev->dev,
		"all transfer rings successfully created!\n");

	return 0;

destroy_acl_h2d:
	bcm4377_destroy_transfer_ring(bcm4377, &bcm4377->acl_h2d_ring);
destroy_sco_d2h:
	bcm4377_destroy_transfer_ring(bcm4377, &bcm4377->sco_d2h_ring);
destroy_sco_h2d:
	bcm4377_destroy_transfer_ring(bcm4377, &bcm4377->sco_h2d_ring);
destroy_hci_d2h:
	bcm4377_destroy_transfer_ring(bcm4377, &bcm4377->hci_h2d_ring);
destroy_hci_h2d:
	bcm4377_destroy_transfer_ring(bcm4377, &bcm4377->hci_d2h_ring);
destroy_sco_event:
	bcm4377_destroy_completion_ring(bcm4377, &bcm4377->sco_event_ring);
destroy_sco_ack:
	bcm4377_destroy_completion_ring(bcm4377, &bcm4377->sco_ack_ring);
destroy_hci_acl_event:
	bcm4377_destroy_completion_ring(bcm4377, &bcm4377->hci_acl_event_ring);
destroy_hci_acl_ack:
	bcm4377_destroy_completion_ring(bcm4377, &bcm4377->hci_acl_ack_ring);

	dev_warn(&bcm4377->pdev->dev, "Creating rings failed with %d\n", ret);
	return ret;
}

static int bcm4377_hci_close(struct hci_dev *hdev)
{
	struct bcm4377_data *bcm4377 = hci_get_drvdata(hdev);

	bcm4377_destroy_transfer_ring(bcm4377, &bcm4377->acl_d2h_ring);
	bcm4377_destroy_transfer_ring(bcm4377, &bcm4377->acl_h2d_ring);
	bcm4377_destroy_transfer_ring(bcm4377, &bcm4377->sco_d2h_ring);
	bcm4377_destroy_transfer_ring(bcm4377, &bcm4377->sco_h2d_ring);
	bcm4377_destroy_transfer_ring(bcm4377, &bcm4377->hci_d2h_ring);
	bcm4377_destroy_transfer_ring(bcm4377, &bcm4377->hci_h2d_ring);

	bcm4377_destroy_completion_ring(bcm4377, &bcm4377->sco_event_ring);
	bcm4377_destroy_completion_ring(bcm4377, &bcm4377->sco_ack_ring);
	bcm4377_destroy_completion_ring(bcm4377, &bcm4377->hci_acl_event_ring);
	bcm4377_destroy_completion_ring(bcm4377, &bcm4377->hci_acl_ack_ring);

	return 0;
}

static int bcm4377_hci_setup(struct hci_dev *hdev)
{
	struct bcm4377_data *bcm4377 = hci_get_drvdata(hdev);
	int ret;

	if (bcm4377->hw->send_calibration) {
		ret = bcm4377->hw->send_calibration(bcm4377);
		if (ret)
			return ret;
	}

	ret = bcm4377->hw->send_ptb(bcm4377);
	if (ret)
		return ret;

	return 0;
}

static int bcm4377_hci_send_frame(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct bcm4377_data *bcm4377 = hci_get_drvdata(hdev);
	struct bcm4377_transfer_ring *ring;
	int ret;

	switch (hci_skb_pkt_type(skb)) {
	case HCI_COMMAND_PKT:
		hdev->stat.cmd_tx++;
		ring = &bcm4377->hci_h2d_ring;
		break;

	case HCI_ACLDATA_PKT:
		hdev->stat.acl_tx++;
		ring = &bcm4377->acl_h2d_ring;
		break;

	case HCI_SCODATA_PKT:
		hdev->stat.sco_tx++;
		ring = &bcm4377->sco_h2d_ring;
		break;

	default:
		return -EILSEQ;
	}

	ret = bcm4377_enqueue(bcm4377, ring, skb->data, skb->len, false);
	if (ret < 0) {
		hdev->stat.err_tx++;
		return ret;
	}

	hdev->stat.byte_tx += skb->len;
	kfree_skb(skb);
	return ret;
}

static int bcm4377_hci_set_bdaddr(struct hci_dev *hdev, const bdaddr_t *bdaddr)
{
	struct sk_buff *skb;
	int err;

	skb = __hci_cmd_sync(hdev, 0xfc01, 6, bdaddr, HCI_INIT_TIMEOUT);
	if (IS_ERR(skb)) {
		err = PTR_ERR(skb);
		bt_dev_err(hdev,
			   "hci_bcm4377: Change address command failed (%d)",
			   err);
		return err;
	}
	kfree_skb(skb);

	return 0;
}

static int bcm4377_alloc_transfer_ring(struct bcm4377_data *bcm4377,
				       struct bcm4377_transfer_ring *ring)
{
	size_t entry_size;

	spin_lock_init(&ring->lock);
	ring->payload_size = ALIGN(ring->payload_size, 4);
	ring->mapped_payload_size = ALIGN(ring->mapped_payload_size, 4);

	if (ring->payload_size > BCM4377_XFER_RING_MAX_INPLACE_PAYLOAD_SIZE)
		return -EINVAL;
	if (ring->n_entries > BCM4377_MAX_RING_SIZE)
		return -EINVAL;
	if (ring->virtual && ring->allow_wait)
		return -EINVAL;

	if (ring->d2h_buffers_only) {
		if (ring->virtual)
			return -EINVAL;
		if (ring->payload_size)
			return -EINVAL;
		if (!ring->mapped_payload_size)
			return -EINVAL;
	}
	if (ring->virtual)
		return 0;

	entry_size =
		ring->payload_size + sizeof(struct bcm4377_xfer_ring_entry);
	ring->ring = dmam_alloc_coherent(&bcm4377->pdev->dev,
					 ring->n_entries * entry_size,
					 &ring->ring_dma, GFP_KERNEL);
	if (!ring->ring)
		return -ENOMEM;

	if (ring->allow_wait) {
		ring->events = devm_kcalloc(&bcm4377->pdev->dev,
					    ring->n_entries,
					    sizeof(*ring->events), GFP_KERNEL);
		if (!ring->events)
			return -ENOMEM;
	}

	if (ring->mapped_payload_size) {
		ring->payloads = dmam_alloc_coherent(
			&bcm4377->pdev->dev,
			ring->n_entries * ring->mapped_payload_size,
			&ring->payloads_dma, GFP_KERNEL);
		if (!ring->payloads)
			return -ENOMEM;
	}

	return 0;
}

static int bcm4377_alloc_completion_ring(struct bcm4377_data *bcm4377,
					 struct bcm4377_completion_ring *ring)
{
	size_t entry_size;

	ring->payload_size = ALIGN(ring->payload_size, 4);
	if (ring->payload_size > BCM4377_XFER_RING_MAX_INPLACE_PAYLOAD_SIZE)
		return -EINVAL;
	if (ring->n_entries > BCM4377_MAX_RING_SIZE)
		return -EINVAL;

	entry_size = ring->payload_size +
		     sizeof(struct bcm4377_completion_ring_entry);

	ring->ring = dmam_alloc_coherent(&bcm4377->pdev->dev,
					 ring->n_entries * entry_size,
					 &ring->ring_dma, GFP_KERNEL);
	if (!ring->ring)
		return -ENOMEM;
	return 0;
}

static int bcm4377_init_context(struct bcm4377_data *bcm4377)
{
	struct device *dev = &bcm4377->pdev->dev;
	dma_addr_t peripheral_info_dma;

	bcm4377->ctx = dmam_alloc_coherent(dev, sizeof(*bcm4377->ctx),
					   &bcm4377->ctx_dma, GFP_KERNEL);
	if (!bcm4377->ctx)
		return -ENOMEM;
	memset(bcm4377->ctx, 0, sizeof(*bcm4377->ctx));

	bcm4377->ring_state =
		dmam_alloc_coherent(dev, sizeof(*bcm4377->ring_state),
				    &bcm4377->ring_state_dma, GFP_KERNEL);
	if (!bcm4377->ring_state)
		return -ENOMEM;
	memset(bcm4377->ring_state, 0, sizeof(*bcm4377->ring_state));

	bcm4377->ctx->version = cpu_to_le16(1);
	bcm4377->ctx->size = cpu_to_le16(sizeof(*bcm4377->ctx));
	bcm4377->ctx->enabled_caps = cpu_to_le16(2);

	/*
	 * The BT device will write 0x20 bytes of data to this buffer but
	 * the exact contents are unknown. It only needs to exist for BT
	 * to work such that we can just allocate and then ignore it.
	 */
	if (!dmam_alloc_coherent(&bcm4377->pdev->dev, 0x20,
				 &peripheral_info_dma, GFP_KERNEL))
		return -ENOMEM;
	bcm4377->ctx->peripheral_info_addr = cpu_to_le64(peripheral_info_dma);

	bcm4377->ctx->xfer_ring_heads_addr = cpu_to_le64(
		bcm4377->ring_state_dma +
		offsetof(struct bcm4377_ring_state, xfer_ring_head));
	bcm4377->ctx->xfer_ring_tails_addr = cpu_to_le64(
		bcm4377->ring_state_dma +
		offsetof(struct bcm4377_ring_state, xfer_ring_tail));
	bcm4377->ctx->completion_ring_heads_addr = cpu_to_le64(
		bcm4377->ring_state_dma +
		offsetof(struct bcm4377_ring_state, completion_ring_head));
	bcm4377->ctx->completion_ring_tails_addr = cpu_to_le64(
		bcm4377->ring_state_dma +
		offsetof(struct bcm4377_ring_state, completion_ring_tail));

	bcm4377->ctx->n_completion_rings =
		cpu_to_le16(BCM4377_N_COMPLETION_RINGS);
	bcm4377->ctx->n_xfer_rings = cpu_to_le16(BCM4377_N_TRANSFER_RINGS);

	bcm4377->ctx->control_completion_ring_addr =
		cpu_to_le64(bcm4377->control_ack_ring.ring_dma);
	bcm4377->ctx->control_completion_ring_n_entries =
		cpu_to_le16(bcm4377->control_ack_ring.n_entries);
	bcm4377->ctx->control_completion_ring_doorbell = cpu_to_le16(0xffff);
	bcm4377->ctx->control_completion_ring_msi = 0;
	bcm4377->ctx->control_completion_ring_header_size = 0;
	bcm4377->ctx->control_completion_ring_footer_size = 0;

	bcm4377->ctx->control_xfer_ring_addr =
		cpu_to_le64(bcm4377->control_h2d_ring.ring_dma);
	bcm4377->ctx->control_xfer_ring_n_entries =
		cpu_to_le16(bcm4377->control_h2d_ring.n_entries);
	bcm4377->ctx->control_xfer_ring_doorbell =
		cpu_to_le16(bcm4377->control_h2d_ring.doorbell);
	bcm4377->ctx->control_xfer_ring_msi = 0;
	bcm4377->ctx->control_xfer_ring_header_size = 0;
	bcm4377->ctx->control_xfer_ring_footer_size =
		bcm4377->control_h2d_ring.payload_size / 4;

	return 0;
}

static int bcm4377_prepare_rings(struct bcm4377_data *bcm4377)
{
	int ret;

	/*
	 * Even though many of these settings appear to be configurable
	 * when sending the "create ring" messages most of these are
	 * actually hardcoded in some (and quite possibly all) firmware versions
	 * and changing them on the host has no effect.
	 * Specifically, this applies to at least the doorbells, the transfer
	 * and completion ring ids and their mapping (e.g. both HCI and ACL
	 * entries will always be queued in completion rings 1 and 2 no matter
	 * what we configure here).
	 */
	bcm4377->control_ack_ring.ring_id = BCM4377_ACK_RING_CONTROL;
	bcm4377->control_ack_ring.n_entries = 32;
	bcm4377->control_ack_ring.transfer_rings =
		BIT(BCM4377_XFER_RING_CONTROL);

	bcm4377->hci_acl_ack_ring.ring_id = BCM4377_ACK_RING_HCI_ACL;
	bcm4377->hci_acl_ack_ring.n_entries = 256;
	bcm4377->hci_acl_ack_ring.transfer_rings =
		BIT(BCM4377_XFER_RING_HCI_H2D) | BIT(BCM4377_XFER_RING_ACL_H2D);
	bcm4377->hci_acl_ack_ring.delay = 1000;

	/*
	 * A payload size of HCI_MAX_EVENT_SIZE is enough here since large ACL
	 * packets will be transmitted inside buffers mapped via acl_d2h_ring
	 * anyway.
	 */
	bcm4377->hci_acl_event_ring.ring_id = BCM4377_EVENT_RING_HCI_ACL;
	bcm4377->hci_acl_event_ring.payload_size = HCI_MAX_EVENT_SIZE;
	bcm4377->hci_acl_event_ring.n_entries = 256;
	bcm4377->hci_acl_event_ring.transfer_rings =
		BIT(BCM4377_XFER_RING_HCI_D2H) | BIT(BCM4377_XFER_RING_ACL_D2H);
	bcm4377->hci_acl_event_ring.delay = 1000;

	bcm4377->sco_ack_ring.ring_id = BCM4377_ACK_RING_SCO;
	bcm4377->sco_ack_ring.n_entries = 128;
	bcm4377->sco_ack_ring.transfer_rings = BIT(BCM4377_XFER_RING_SCO_H2D);

	bcm4377->sco_event_ring.ring_id = BCM4377_EVENT_RING_SCO;
	bcm4377->sco_event_ring.payload_size = HCI_MAX_SCO_SIZE;
	bcm4377->sco_event_ring.n_entries = 128;
	bcm4377->sco_event_ring.transfer_rings = BIT(BCM4377_XFER_RING_SCO_D2H);

	bcm4377->control_h2d_ring.ring_id = BCM4377_XFER_RING_CONTROL;
	bcm4377->control_h2d_ring.doorbell = BCM4377_DOORBELL_CONTROL;
	bcm4377->control_h2d_ring.payload_size = BCM4377_CONTROL_MSG_SIZE;
	bcm4377->control_h2d_ring.completion_ring = BCM4377_ACK_RING_CONTROL;
	bcm4377->control_h2d_ring.allow_wait = true;
	bcm4377->control_h2d_ring.n_entries = 128;

	bcm4377->hci_h2d_ring.ring_id = BCM4377_XFER_RING_HCI_H2D;
	bcm4377->hci_h2d_ring.doorbell = BCM4377_DOORBELL_HCI_H2D;
	bcm4377->hci_h2d_ring.payload_size = HCI_MAX_EVENT_SIZE;
	bcm4377->hci_h2d_ring.completion_ring = BCM4377_ACK_RING_HCI_ACL;
	bcm4377->hci_h2d_ring.n_entries = 128;

	bcm4377->hci_d2h_ring.ring_id = BCM4377_XFER_RING_HCI_D2H;
	bcm4377->hci_d2h_ring.doorbell = BCM4377_DOORBELL_HCI_D2H;
	bcm4377->hci_d2h_ring.completion_ring = BCM4377_EVENT_RING_HCI_ACL;
	bcm4377->hci_d2h_ring.virtual = true;
	bcm4377->hci_d2h_ring.n_entries = 128;

	bcm4377->sco_h2d_ring.ring_id = BCM4377_XFER_RING_SCO_H2D;
	bcm4377->sco_h2d_ring.doorbell = BCM4377_DOORBELL_SCO;
	bcm4377->sco_h2d_ring.payload_size = HCI_MAX_SCO_SIZE;
	bcm4377->sco_h2d_ring.completion_ring = BCM4377_ACK_RING_SCO;
	bcm4377->sco_h2d_ring.sync = true;
	bcm4377->sco_h2d_ring.n_entries = 128;

	bcm4377->sco_d2h_ring.ring_id = BCM4377_XFER_RING_SCO_D2H;
	bcm4377->sco_d2h_ring.doorbell = BCM4377_DOORBELL_SCO;
	bcm4377->sco_d2h_ring.completion_ring = BCM4377_EVENT_RING_SCO;
	bcm4377->sco_d2h_ring.virtual = true;
	bcm4377->sco_d2h_ring.sync = true;
	bcm4377->sco_d2h_ring.n_entries = 128;

	/*
	 * This ring has to use mapped_payload_size because the largest ACL
	 * packet doesn't fit inside the largest possible footer
	 */
	bcm4377->acl_h2d_ring.ring_id = BCM4377_XFER_RING_ACL_H2D;
	bcm4377->acl_h2d_ring.doorbell = BCM4377_DOORBELL_ACL_H2D;
	bcm4377->acl_h2d_ring.mapped_payload_size = HCI_MAX_FRAME_SIZE + 4;
	bcm4377->acl_h2d_ring.completion_ring = BCM4377_ACK_RING_HCI_ACL;
	bcm4377->acl_h2d_ring.n_entries = 128;

	/*
	 * This ring only contains empty buffers to be used by incoming
	 * ACL packets that do not fit inside the footer of hci_acl_event_ring
	 */
	bcm4377->acl_d2h_ring.ring_id = BCM4377_XFER_RING_ACL_D2H;
	bcm4377->acl_d2h_ring.doorbell = BCM4377_DOORBELL_ACL_D2H;
	bcm4377->acl_d2h_ring.completion_ring = BCM4377_EVENT_RING_HCI_ACL;
	bcm4377->acl_d2h_ring.d2h_buffers_only = true;
	bcm4377->acl_d2h_ring.mapped_payload_size = HCI_MAX_FRAME_SIZE + 4;
	bcm4377->acl_d2h_ring.n_entries = 128;

	/*
	 * no need for any cleanup since this is only called from _probe
	 * and only devres-managed allocations are used
	 */
	ret = bcm4377_alloc_transfer_ring(bcm4377, &bcm4377->control_h2d_ring);
	if (ret)
		return ret;
	ret = bcm4377_alloc_transfer_ring(bcm4377, &bcm4377->hci_h2d_ring);
	if (ret)
		return ret;
	ret = bcm4377_alloc_transfer_ring(bcm4377, &bcm4377->hci_d2h_ring);
	if (ret)
		return ret;
	ret = bcm4377_alloc_transfer_ring(bcm4377, &bcm4377->sco_h2d_ring);
	if (ret)
		return ret;
	ret = bcm4377_alloc_transfer_ring(bcm4377, &bcm4377->sco_d2h_ring);
	if (ret)
		return ret;
	ret = bcm4377_alloc_transfer_ring(bcm4377, &bcm4377->acl_h2d_ring);
	if (ret)
		return ret;
	ret = bcm4377_alloc_transfer_ring(bcm4377, &bcm4377->acl_d2h_ring);
	if (ret)
		return ret;

	ret = bcm4377_alloc_completion_ring(bcm4377,
					    &bcm4377->control_ack_ring);
	if (ret)
		return ret;
	ret = bcm4377_alloc_completion_ring(bcm4377,
					    &bcm4377->hci_acl_ack_ring);
	if (ret)
		return ret;
	ret = bcm4377_alloc_completion_ring(bcm4377,
					    &bcm4377->hci_acl_event_ring);
	if (ret)
		return ret;
	ret = bcm4377_alloc_completion_ring(bcm4377, &bcm4377->sco_ack_ring);
	if (ret)
		return ret;
	ret = bcm4377_alloc_completion_ring(bcm4377, &bcm4377->sco_event_ring);
	if (ret)
		return ret;

	return 0;
}

static int bcm4377_boot(struct bcm4377_data *bcm4377)
{
	u32 bootstage;
	const struct firmware *fw;
	void *bfr;
	dma_addr_t fw_dma;
	int ret = 0;

	fw = bcm4377_request_blob(bcm4377, "bin");
	if (!fw) {
		dev_err(&bcm4377->pdev->dev, "Failed to load firmware\n");
		return -ENOENT;
	}

	bfr = dma_alloc_coherent(&bcm4377->pdev->dev, fw->size, &fw_dma,
				 GFP_KERNEL);
	if (!bfr) {
		ret = -ENOMEM;
		goto out_release_fw;
	}

	memcpy(bfr, fw->data, fw->size);

	iowrite32(0, bcm4377->bar0 + BCM4377_BAR0_HOST_WINDOW_LO);
	iowrite32(0, bcm4377->bar0 + BCM4377_BAR0_HOST_WINDOW_HI);
	iowrite32(BCM4377_DMA_MASK,
		  bcm4377->bar0 + BCM4377_BAR0_HOST_WINDOW_SIZE);

	iowrite32(lower_32_bits(fw_dma), bcm4377->bar2 + BCM4377_BAR2_FW_LO);
	iowrite32(upper_32_bits(fw_dma), bcm4377->bar2 + BCM4377_BAR2_FW_HI);
	iowrite32(fw->size, bcm4377->bar2 + BCM4377_BAR2_FW_SIZE);
	iowrite32(0, bcm4377->bar0 + BCM4377_BAR0_FW_DOORBELL);

	ret = wait_for_completion_interruptible_timeout(
		&bcm4377->event, BCM4377_DEFAULT_TIMEOUT);
	if (ret == 0) {
		ret = -ETIMEDOUT;
		goto out_dma_free;
	} else if (ret < 0) {
		goto out_dma_free;
	}

	bootstage = ioread32(bcm4377->bar2 + BCM4377_BAR2_BOOTSTAGE);
	if (bootstage != 2) {
		dev_err(&bcm4377->pdev->dev, "boostage %d != 2\n", bootstage);
		ret = -ENXIO;
		goto out_dma_free;
	}

	dev_dbg(&bcm4377->pdev->dev, "firmware has booted (stage = %x)\n",
		bootstage);
	ret = 0;

out_dma_free:
	dma_free_coherent(&bcm4377->pdev->dev, fw->size, bfr, fw_dma);
out_release_fw:
	release_firmware(fw);
	return ret;
}

static int bcm4377_setup_rti(struct bcm4377_data *bcm4377)
{
	u32 rti_status;
	int ret;

	/* start RTI */
	iowrite32(1, bcm4377->bar0 + BCM4377_BAR0_RTI_CONTROL);

	ret = wait_for_completion_interruptible_timeout(
		&bcm4377->event, BCM4377_DEFAULT_TIMEOUT);
	if (ret == 0) {
		dev_err(&bcm4377->pdev->dev,
			"timed out while waiting for RTI to transition to state 1");
		return -ETIMEDOUT;
	} else if (ret < 0) {
		return ret;
	}

	rti_status = ioread32(bcm4377->bar2 + BCM4377_BAR2_RTI_STATUS);
	if (rti_status != 1) {
		dev_err(&bcm4377->pdev->dev, "RTI did not ack state 1 (%d)\n",
			rti_status);
		return -ENODEV;
	}
	dev_dbg(&bcm4377->pdev->dev, "RTI is in state 1\n");

	/* allow access to the entire IOVA space again */
	iowrite32(0, bcm4377->bar2 + BCM4377_BAR2_RTI_WINDOW_LO);
	iowrite32(0, bcm4377->bar2 + BCM4377_BAR2_RTI_WINDOW_HI);
	iowrite32(BCM4377_DMA_MASK,
		  bcm4377->bar2 + BCM4377_BAR2_RTI_WINDOW_SIZE);

	/* setup "Converged IPC" context */
	iowrite32(lower_32_bits(bcm4377->ctx_dma),
		  bcm4377->bar2 + BCM4377_BAR2_CONTEXT_ADDR_LO);
	iowrite32(upper_32_bits(bcm4377->ctx_dma),
		  bcm4377->bar2 + BCM4377_BAR2_CONTEXT_ADDR_HI);
	iowrite32(2, bcm4377->bar0 + BCM4377_BAR0_RTI_CONTROL);

	ret = wait_for_completion_interruptible_timeout(
		&bcm4377->event, BCM4377_DEFAULT_TIMEOUT);
	if (ret == 0) {
		dev_err(&bcm4377->pdev->dev,
			"timed out while waiting for RTI to transition to state 2");
		return -ETIMEDOUT;
	} else if (ret < 0) {
		return ret;
	}

	rti_status = ioread32(bcm4377->bar2 + BCM4377_BAR2_RTI_STATUS);
	if (rti_status != 2) {
		dev_err(&bcm4377->pdev->dev, "RTI did not ack state 2 (%d)\n",
			rti_status);
		return -ENODEV;
	}

	dev_dbg(&bcm4377->pdev->dev,
		"RTI is in state 2; control ring is ready\n");
	bcm4377->control_ack_ring.enabled = true;

	return 0;
}

static int bcm4377_parse_otp_board_params(struct bcm4377_data *bcm4377,
					  char tag, const char *val, size_t len)
{
	if (tag != 'V')
		return 0;

	strscpy(bcm4377->vendor, val, len + 1);
	return 0;
}

static int bcm4377_parse_otp_chip_params(struct bcm4377_data *bcm4377, char tag,
					 const char *val, size_t len)
{
	size_t idx = 0;

	if (tag != 's')
		return 0;

	/* 
	 * this won't write out of bounds since len < BCM4377_OTP_MAX_PARAM_LEN
	 * and sizeof(bcm4377->stepping) = BCM4377_OTP_MAX_PARAM_LEN
	 */
	while (len != 0) {
		bcm4377->stepping[idx] = tolower(val[idx]);
		if (val[idx] == '\0')
			return 0;

		idx++;
		len--;
	}

	bcm4377->stepping[idx] = '\0';
	return 0;
}

static int bcm4377_parse_opt_str(struct bcm4377_data *bcm4377, const u8 *str,
				 int (*parse_arg)(struct bcm4377_data *, char,
						  const char *, size_t))
{
	const char *p;
	int ret;

	p = skip_spaces(str);
	while (*p) {
		char tag = *p++;
		const char *end;
		size_t len;

		if (*p++ != '=') /* implicit NUL check */
			return -EINVAL;

		/* *p might be NUL here, if so end == p and len == 0 */
		end = strchrnul(p, ' ');
		len = end - p;

		/* leave 1 byte for NUL in destination string */
		if (len > (BCM4377_OTP_MAX_PARAM_LEN - 1))
			return -EINVAL;

		/* Copy len characters plus a NUL terminator */
		ret = parse_arg(bcm4377, tag, p, len);
		if (ret)
			return ret;

		/* Skip to next arg, if any */
		p = skip_spaces(end);
	}

	return 0;
}

static int bcm4377_parse_otp_sys_vendor(struct bcm4377_data *bcm4377, u8 *otp,
					size_t size)
{
	int idx = 4;
	const char *chip_params;
	const char *board_params;
	int ret;

	/* 4-byte header and two empty strings */
	if (size < 6)
		return -EINVAL;

	if (get_unaligned_le32(otp) != BCM4377_OTP_VENDOR_HDR)
		return -EINVAL;

	chip_params = &otp[idx];

	/* Skip first string, including terminator */
	idx += strnlen(chip_params, size - idx) + 1;
	if (idx >= size)
		return -EINVAL;

	board_params = &otp[idx];

	/* Skip to terminator of second string */
	idx += strnlen(board_params, size - idx);
	if (idx >= size)
		return -EINVAL;

	/* At this point both strings are guaranteed NUL-terminated */
	dev_dbg(&bcm4377->pdev->dev,
		"OTP: chip_params='%s' board_params='%s'\n", chip_params,
		board_params);

	ret = bcm4377_parse_opt_str(bcm4377, chip_params,
				    bcm4377_parse_otp_chip_params);
	if (ret)
		return ret;

	ret = bcm4377_parse_opt_str(bcm4377, board_params,
				    bcm4377_parse_otp_board_params);
	if (ret)
		return ret;

	dev_dbg(&bcm4377->pdev->dev, "OTP: stepping=%s, vendor=%s\n",
		bcm4377->stepping, bcm4377->vendor);

	if (!bcm4377->stepping[0] || !bcm4377->vendor[0])
		return -EINVAL;

	return 0;
}

static int bcm4377_read_otp(struct bcm4377_data *bcm4377)
{
	u8 otp[BCM4377_OTP_SIZE];
	int i;
	int ret = -ENOENT;

	for (i = 0; i < BCM4377_OTP_SIZE; ++i)
		otp[i] = ioread8(bcm4377->bar0 + bcm4377->hw->otp_offset + i);

	i = 0;
	while (i < (BCM4377_OTP_SIZE - 1)) {
		u8 type = otp[i];
		u8 length = otp[i + 1];

		if (type == 0)
			break;

		if ((i + 2 + length) > BCM4377_OTP_SIZE)
			break;

		switch (type) {
		case BCM4377_OTP_SYS_VENDOR:
			dev_dbg(&bcm4377->pdev->dev,
				"OTP @ 0x%x (%d): SYS_VENDOR", i, length);
			ret = bcm4377_parse_otp_sys_vendor(bcm4377, &otp[i + 2],
							   length);
			break;
		case BCM4377_OTP_CIS:
			dev_dbg(&bcm4377->pdev->dev,
				"OTP @ 0x%x (%d): BCM4377_CIS", i, length);
			break;
		default:
			dev_dbg(&bcm4377->pdev->dev, "OTP @ 0x%x (%d): unknown",
				i, length);
			break;
		}

		i += 2 + length;
	}

	return ret;
}

static int bcm4377_init_cfg(struct bcm4377_data *bcm4377)
{
	int ret;
	u32 ctrl;

	ret = pci_write_config_dword(bcm4377->pdev,
				     BCM4377_PCIECFG_BAR0_WINDOW0,
				     bcm4377->hw->bar0_window0);
	if (ret)
		return ret;

	ret = pci_write_config_dword(bcm4377->pdev,
				     BCM4377_PCIECFG_BAR0_WINDOW1,
				     bcm4377->hw->bar0_window1);
	if (ret)
		return ret;

	ret = pci_write_config_dword(bcm4377->pdev,
				     BCM4377_PCIECFG_BAR0_WINDOW4,
				     BCM4377_PCIECFG_BAR0_WINDOW4_DEFAULT);
	if (ret)
		return ret;

	if (bcm4377->hw->has_bar0_window5) {
		ret = pci_write_config_dword(bcm4377->pdev,
					     BCM4377_PCIECFG_BAR0_WINDOW5,
					     bcm4377->hw->bar0_window5);
		if (ret)
			return ret;
	}

	ret = pci_write_config_dword(bcm4377->pdev, BCM4377_PCIECFG_BAR2_WINDOW,
				     BCM4377_PCIECFG_BAR2_WINDOW_DEFAULT);
	if (ret)
		return ret;

	ret = pci_read_config_dword(bcm4377->pdev, BCM4377_PCIECFG_UNK_CTRL,
				    &ctrl);
	if (ret)
		return ret;

	// TODO: 19 and 16 are probably M2M and SS reset
	if (bcm4377->hw->m2m_reset_on_ss_reset_disabled)
		ctrl &= ~BIT(19); // BIT(19) = M2M reset?
	ctrl |= BIT(16);

	return pci_write_config_dword(bcm4377->pdev, BCM4377_PCIECFG_UNK_CTRL,
				      ctrl);
}

static int bcm4377_probe_of(struct bcm4377_data *bcm4377)
{
	struct device_node *np = bcm4377->pdev->dev.of_node;
	int ret;

	if (!np)
		return 0;

	ret = of_property_read_string(np, "brcm,board-type",
				      &bcm4377->board_type);
	if (ret) {
		dev_err(&bcm4377->pdev->dev, "no brcm,board-type property\n");
		return ret;
	}

	bcm4377->taurus_beamforming_cal_blob =
		of_get_property(np, "brcm,taurus-bf-cal-blob",
				&bcm4377->taurus_beamforming_cal_size);
	if (!bcm4377->taurus_beamforming_cal_blob) {
		dev_err(&bcm4377->pdev->dev,
			"no brcm,taurus-bf-cal-blob property\n");
		return -ENOENT;
	}
	bcm4377->taurus_cal_blob = of_get_property(np, "brcm,taurus-cal-blob",
						   &bcm4377->taurus_cal_size);
	if (!bcm4377->taurus_cal_blob) {
		dev_err(&bcm4377->pdev->dev,
			"no brcm,taurus-cal-blob property\n");
		return -ENOENT;
	}

	return 0;
}

static const struct bcm4377_hw bcm4377_hw_variants[];

static int bcm4377_probe(struct pci_dev *pdev, const struct pci_device_id *id)
{
	struct bcm4377_data *bcm4377;
	struct hci_dev *hdev;
	int ret;

	ret = dma_set_mask_and_coherent(&pdev->dev, BCM4377_DMA_MASK);
	if (ret)
		return ret;

	bcm4377 = devm_kzalloc(&pdev->dev, sizeof(*bcm4377), GFP_KERNEL);
	if (!bcm4377)
		return -ENOMEM;

	bcm4377->pdev = pdev;
	bcm4377->hw = &bcm4377_hw_variants[id->driver_data];
	init_completion(&bcm4377->event);

	ret = bcm4377_prepare_rings(bcm4377);
	if (ret)
		return ret;

	ret = bcm4377_init_context(bcm4377);
	if (ret)
		return ret;

	bcm4377->board_type = bcm4377->hw->board_type;
	ret = bcm4377_probe_of(bcm4377);
	if (ret)
		return ret;
	if (!bcm4377->board_type) {
		dev_err(&pdev->dev, "unable to determine board type\n");
		return ret;
	}

	ret = pci_enable_device(pdev);
	if (ret)
		return ret;
	pci_set_master(pdev);

	ret = bcm4377_init_cfg(bcm4377);
	if (ret)
		return ret;

	bcm4377->bar0 = pcim_iomap(pdev, 0, 0);
	if (!bcm4377->bar0)
		return -EBUSY;
	bcm4377->bar2 = pcim_iomap(pdev, 2, 0);
	if (!bcm4377->bar2)
		return -EBUSY;

	ret = bcm4377_read_otp(bcm4377);
	if (ret) {
		dev_err(&pdev->dev, "Reading OTP failed with %d\n", ret);
		return ret;
	}

	/*
	* Legacy interrupts result in an IRQ storm at least on Apple Silicon
	* platforms.
	*/
	ret = pci_alloc_irq_vectors(pdev, 1, 1, PCI_IRQ_MSI | PCI_IRQ_MSIX);
	if (ret < 0)
		return -ENODEV;
	ret = devm_add_action_or_reset(
		&pdev->dev, (void (*)(void *))pci_free_irq_vectors, pdev);
	if (ret)
		return ret;

	bcm4377->irq = pci_irq_vector(pdev, 0);
	if (bcm4377->irq <= 0)
		return -ENODEV;

	ret = devm_request_irq(&pdev->dev, bcm4377->irq, bcm4377_irq, 0,
			       "bcm4377", bcm4377);
	if (ret)
		return ret;

	hdev = hci_alloc_dev();
	if (!hdev)
		return -ENOMEM;
	ret = devm_add_action_or_reset(&pdev->dev,
				       (void (*)(void *))hci_free_dev, hdev);
	if (ret)
		return ret;

	bcm4377->hdev = hdev;

	hdev->bus = HCI_PCI;
	hdev->dev_type = HCI_PRIMARY;
	hdev->open = bcm4377_hci_open;
	hdev->close = bcm4377_hci_close;
	hdev->send = bcm4377_hci_send_frame;
	hdev->set_bdaddr = bcm4377_hci_set_bdaddr;
	hdev->setup = bcm4377_hci_setup;

	/* non-DT devices have the address stored inside a ROM */
	if (pdev->dev.of_node)
		set_bit(HCI_QUIRK_USE_BDADDR_PROPERTY, &hdev->quirks);
	set_bit(HCI_QUIRK_FIXUP_LE_EXT_ADV_REPORT_EVT_TYPE, &hdev->quirks);

	pci_set_drvdata(pdev, bcm4377);
	hci_set_drvdata(hdev, bcm4377);
	SET_HCIDEV_DEV(hdev, &pdev->dev);

	ret = bcm4377_boot(bcm4377);
	if (ret)
		return ret;

	ret = bcm4377_setup_rti(bcm4377);
	if (ret)
		return ret;

	ret = hci_register_dev(hdev);
	if (ret)
		return ret;
	return devm_add_action_or_reset(
		&pdev->dev, (void (*)(void *))hci_unregister_dev, hdev);
}

static const struct bcm4377_hw bcm4377_hw_variants[] = {
	[BCM4377] = {
		.name = "4377",
		.otp_offset = 0x4120,
		.bar0_window0 = 0x1800b000,
		.bar0_window1 = 0x1810c000,
		.board_type = "apple,formosa",
		.send_ptb = bcm4377_send_ptb,
	},

	[BCM4378] = {
		.name = "4378",
		.otp_offset = 0x4120,
		.bar0_window0 = 0x18002000,
		.bar0_window1 = 0x1810a000,
		.bar0_window5 = 0x18107000,
		.has_bar0_window5 = true,
		.send_calibration = bcm4378_send_calibration,
		.send_ptb = bcm4378_send_ptb,
	},

	[BCM4387]= {
		.name = "4387",
		.otp_offset = 0x413c,
		.bar0_window0 = 0x18002000,
		.bar0_window1 = 0x18109000,
		.bar0_window5 = 0x18106000,
		.has_bar0_window5 = true,
		.m2m_reset_on_ss_reset_disabled = true,
		.send_calibration = bcm4387_send_calibration,
		.send_ptb = bcm4378_send_ptb,
	},
};

#define BCM4377_DEVID_ENTRY(id)                                             \
	{                                                                   \
		PCI_VENDOR_ID_BROADCOM, BCM##id##_DEVICE_ID, PCI_ANY_ID,    \
			PCI_ANY_ID, PCI_CLASS_NETWORK_OTHER << 8, 0xffff00, \
			BCM##id                                             \
	}

static const struct pci_device_id bcm4377_devid_table[] = {
	BCM4377_DEVID_ENTRY(4377),
	BCM4377_DEVID_ENTRY(4378),
	BCM4377_DEVID_ENTRY(4387),
	{},
};
MODULE_DEVICE_TABLE(pci, bcm4377_devid_table);

static struct pci_driver bcm4377_pci_driver = {
	.name = "hci_bcm4377",
	.id_table = bcm4377_devid_table,
	.probe = bcm4377_probe,
};
module_pci_driver(bcm4377_pci_driver);

MODULE_AUTHOR("Sven Peter <sven@svenpeter.dev>");
MODULE_DESCRIPTION("Bluetooth support for Broadcom 4377-family PCIe devices");
MODULE_LICENSE("GPL");
MODULE_FIRMWARE("brcm/brcmbt43*.bin");
MODULE_FIRMWARE("brcm/brcmbt43*.ptb");
