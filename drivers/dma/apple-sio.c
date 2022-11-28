// SPDX-License-Identifier: GPL-2.0-only OR MIT
/*
 * Driver for SIO coprocessor on t8103 (M1) and other Apple SoCs
 *
 * Copyright (C) The Asahi Linux Contributors
 */

#define DEBUG

#include <linux/bitfield.h>
#include <linux/bits.h>
#include <linux/completion.h>
#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/of_dma.h>
#include <linux/soc/apple/rtkit.h>

#include "dmaengine.h"

#define NCHANNELS_MAX	0x80

#define REG_CPU_CONTROL	0x44
#define CPU_CONTROL_RUN BIT(4)

#define SIOMSG_DATA	GENMASK(63, 32)
#define SIOMSG_TYPE	GENMASK(23, 16)
#define SIOMSG_PARAM	GENMASK(31, 24)
#define SIOMSG_TAG	GENMASK(13, 8)
#define SIOMSG_EP	GENMASK(7, 0)

#define EP_SIO		0x20

#define MSG_START	0x2
#define MSG_SETUP	0x3
#define MSG_CONFIGURE	0x5
#define MSG_ISSUE	0x6
#define MSG_TERMINATE	0x8
#define MSG_ACK		0x65
#define MSG_NACK	0x66
#define MSG_STARTED	0x67
#define MSG_REPORT	0x68

#define SIO_CALL_TIMEOUT_MS	100
#define SIO_SHMEM_SIZE		0x1000 /* TODO */
#define SIO_NO_DESC_SLOTS	32 /* TODO */

/*
 * Maximum number of descriptors we can queue up
 * -- a made-up number
 */
#define SIO_MAX_NINFLIGHT	4

struct sio_desc {
	u32 pad1;
	u32 flag;
	u64 unk;
	u64 iova;
	u64 size;
	u64 pad2;
	u64 pad3;
	u8 pad4[48]; /* TODO */
} __attribute__((packed));
static_assert(sizeof(struct sio_desc) == 48 * 2);

struct sio_chan_config {
	u32 datashape;
	u32 timeout;
	u32 fifo;
	u32 threshold;
	u32 limit;
} __attribute__((packed));

struct sio_data;
struct sio_tx;

struct sio_chan {
	unsigned int no;
	struct sio_data *host;
	struct dma_chan chan;
	struct tasklet_struct tasklet;
	struct work_struct terminate_wq;

	spinlock_t lock;
	struct sio_tx *current_tx;
	/*
	 * 'tx_cookie' is used for distinguishing between transactions from
	 * within tag ack/nack callbacks. Without it, we would have no way
	 * of knowing if the current transaction is the one the callback handler
	 * was installed for.
	 */
	unsigned long tx_cookie;
	int nperiod_acks;

	/*
	 * We maintain a 'submitted' and 'issued' list mainly for interface
	 * correctness. Typical use of the driver (per channel) will be
	 * prepping, submitting and issuing a single cyclic transaction which
	 * will stay current until terminate_all is called.
	 */
	struct list_head submitted;
	struct list_head issued;

	struct list_head to_free;
};

#define USABLE_TAGS	15

typedef void (*sio_ack_callback)(struct sio_chan *, void *, bool);

struct sio_data {
	void __iomem *base;
	struct dma_device dma;
	struct device *dev;
	struct apple_rtkit *rtk;
	void *shmem;
	struct sio_desc *desc_base;
	volatile unsigned long *desc_allocated;

	struct sio_tagdata {
		volatile unsigned long allocated;
		int last_tag;

		struct completion completions[USABLE_TAGS + 1];
		bool atomic[USABLE_TAGS + 1];
		bool acked[USABLE_TAGS + 1];

		sio_ack_callback ack_callback[USABLE_TAGS + 1];
		void *cookie[USABLE_TAGS + 1];
	} tags;

	int nchannels;
	struct sio_chan channels[];
};

struct sio_tx {
	struct dma_async_tx_descriptor tx;
	struct list_head node;
	struct completion done;

	bool terminated;
	size_t period_len;
	int nperiods;
	int ninflight;
	int next;

	struct sio_desc *siodesc[];
};

static int sio_send_siomsg(struct sio_data *sio, u64 msg);
static int sio_send_siomsg_atomic(struct sio_data *sio, u64 msg,
				  sio_ack_callback ack_callback,
				  void *cookie);
static int sio_call(struct sio_data *sio, u64 msg);

static struct sio_chan *to_sio_chan(struct dma_chan *chan)
{
	return container_of(chan, struct sio_chan, chan);
}

static struct sio_tx *to_sio_tx(struct dma_async_tx_descriptor *tx)
{
	return container_of(tx, struct sio_tx, tx);
}

static int sio_alloc_tag(struct sio_data *sio)
{
	struct sio_tagdata *tags = &sio->tags;
	int tag, i;

	/*
	 * The usable tag range is 1...USABLE_TAGS, so to
	 * pick the next tag we do modulo *then* plus one.
	 */
	tag = (tags->last_tag % USABLE_TAGS) + 1;

	for (i = 0; i < USABLE_TAGS; i++) {
		if (!test_and_set_bit(tag, &tags->allocated))
			break;

		tag = (tag % USABLE_TAGS) + 1;
	}

	/* TODO: locking or something weaker */
	tags->last_tag = tag;

	if (i < USABLE_TAGS) {
		return tag;
	} else {
		return -EBUSY;
	}
}

static void sio_free_tag(struct sio_data *sio, int tag)
{
	struct sio_tagdata *tags = &sio->tags;

	if (WARN_ON(tag >= USABLE_TAGS + 1))
		return;

	tags->atomic[tag] = false;
	tags->ack_callback[tag] = NULL;

	WARN_ON(!test_and_clear_bit(tag, &tags->allocated));
}

static void sio_set_tag_atomic(struct sio_data *sio, int tag,
			       sio_ack_callback ack_callback,
			       void *cookie)
{
	struct sio_tagdata *tags = &sio->tags;

	tags->atomic[tag] = true;
	tags->ack_callback[tag] = ack_callback;
	tags->cookie[tag] = cookie;
}

struct sio_desc *sio_alloc_desc(struct sio_data *sio)
{
	int i;

	for (i = 0; i < SIO_NO_DESC_SLOTS; i++)
		if (!test_and_set_bit(i, sio->desc_allocated))
			return sio->desc_base + i;

	return NULL;
}

void sio_free_desc(struct sio_data *sio, struct sio_desc *desc)
{
	clear_bit(desc - sio->desc_base, sio->desc_allocated);
}

int sio_desc_slot(struct sio_data *sio, struct sio_desc *desc)
{
	return (desc - sio->desc_base) * 8;
}

static enum dma_transfer_direction sio_chan_direction(int channo)
{
	/* Channel directions are fixed based on channel number */
	return (channo & 1) ? DMA_DEV_TO_MEM : DMA_MEM_TO_DEV;
}

static dma_cookie_t sio_tx_submit(struct dma_async_tx_descriptor *tx)
{
	struct sio_tx *siotx = to_sio_tx(tx);
	struct sio_chan *siochan = to_sio_chan(tx->chan);
	unsigned long flags;
	dma_cookie_t cookie;

	spin_lock_irqsave(&siochan->lock, flags);
	cookie = dma_cookie_assign(tx);
	list_add_tail(&siotx->node, &siochan->submitted);
	spin_unlock_irqrestore(&siochan->lock, flags);

	return cookie;
}

static int sio_tx_desc_free(struct dma_async_tx_descriptor *tx)
{
	struct sio_data *sio = to_sio_chan(tx->chan)->host;
	struct sio_tx *siotx = to_sio_tx(tx);
	int i;

	for (i = 0; i < siotx->nperiods; i++)
		if (siotx->siodesc[i])
			sio_free_desc(sio, siotx->siodesc[i]);
	kfree(siotx);

	return 0;
}

static struct dma_async_tx_descriptor *sio_prep_dma_cyclic(
		struct dma_chan *chan, dma_addr_t buf_addr, size_t buf_len,
		size_t period_len, enum dma_transfer_direction direction,
		unsigned long flags)
{
	struct sio_chan *siochan = to_sio_chan(chan);
	struct sio_tx *siotx = NULL;
	int i, nperiods = buf_len / period_len;

	if (direction != sio_chan_direction(siochan->no))
		return NULL;

	siotx = kzalloc(struct_size(siotx, siodesc, nperiods), GFP_NOWAIT);
	if (!siotx)
		return NULL;

	init_completion(&siotx->done);
	dma_async_tx_descriptor_init(&siotx->tx, chan);
	siotx->period_len = period_len;
	siotx->nperiods = nperiods;

	for (i = 0; i < nperiods; i++) {
		struct sio_desc *d;
		siotx->siodesc[i] = d = sio_alloc_desc(siochan->host);
		if (!d) {
			sio_tx_desc_free(&siotx->tx);
			return NULL;
		}

		d->flag = 1; // not sure what's up with this
		d->iova = buf_addr + period_len * i;
		d->size = period_len;
	}
	dma_wmb();

	siotx->tx.tx_submit = sio_tx_submit;
	siotx->tx.desc_free = sio_tx_desc_free;

	return &siotx->tx;
}

static enum dma_status sio_tx_status(struct dma_chan *chan, dma_cookie_t cookie,
				     struct dma_tx_state *txstate)
{
	struct sio_chan *siochan = to_sio_chan(chan);
	struct sio_tx *siotx;
	enum dma_status ret;
	unsigned long flags;
	int periods_residue;
	size_t residue;

	ret = dma_cookie_status(chan, cookie, txstate);
	if (ret == DMA_COMPLETE || !txstate)
		return ret;

	spin_lock_irqsave(&siochan->lock, flags);
	siotx = siochan->current_tx;

	if (siotx && siotx->tx.cookie == cookie) {
		ret = DMA_IN_PROGRESS;
		periods_residue = siotx->next - siotx->ninflight;
		while (periods_residue < 0)
			periods_residue += siotx->nperiods;
		residue = (siotx->nperiods - periods_residue) * siotx->period_len;
	} else {
		ret = DMA_IN_PROGRESS;
		residue = 0;
		list_for_each_entry(siotx, &siochan->issued, node) {
			if (siotx->tx.cookie == cookie) {
				residue = siotx->period_len * siotx->nperiods;
				break;
			}
		}
	}
	spin_unlock_irqrestore(&siochan->lock, flags);
	dma_set_residue(txstate, residue);

	return ret;
}

static bool sio_fill_in_locked(struct sio_chan *siochan);

static void sio_handle_issue_ack(struct sio_chan *siochan, void *cookie, bool ok)
{
	unsigned long flags;
	unsigned long tx_cookie = (unsigned long) cookie;
	struct sio_tx *tx;

	if (!ok) {
		dev_err(siochan->host->dev, "nacked issue on chan %d\n", siochan->no);
		return;
	}

	spin_lock_irqsave(&siochan->lock, flags);
	if (!siochan->current_tx || tx_cookie != siochan->tx_cookie ||
	    siochan->current_tx->terminated)
		goto out;

	tx = siochan->current_tx;
	tx->next = (tx->next + 1) % tx->nperiods;
	tx->ninflight++;
	sio_fill_in_locked(siochan);

out:
	spin_unlock_irqrestore(&siochan->lock, flags);
}

static bool sio_fill_in_locked(struct sio_chan *siochan)
{
	struct sio_data *sio = siochan->host;
	struct sio_tx *tx = siochan->current_tx;
	struct sio_desc *d = tx->siodesc[tx->next];
	int ret;

	if (tx->ninflight >= SIO_MAX_NINFLIGHT || tx->terminated)
		return false;

	ret = sio_send_siomsg_atomic(sio, FIELD_PREP(SIOMSG_EP, siochan->no) |
				     FIELD_PREP(SIOMSG_TYPE, MSG_ISSUE) |
				     FIELD_PREP(SIOMSG_DATA, sio_desc_slot(sio, d)),
				     sio_handle_issue_ack, (void *) siochan->tx_cookie);
	if (ret < 0)
		dev_err_ratelimited(sio->dev, "can't issue on chan %d ninflight %d: %d\n",
				    siochan->no, tx->ninflight, ret);
	return true;
}

static void sio_issue_pending(struct dma_chan *chan)
{
	struct sio_chan *siochan = to_sio_chan(chan);
	unsigned long flags;
	struct sio_tx *tx;

	spin_lock_irqsave(&siochan->lock, flags);
	list_splice_tail_init(&siochan->submitted, &siochan->issued);
	if (!list_empty(&siochan->issued) && !siochan->current_tx) {
		tx = list_first_entry(&siochan->issued, struct sio_tx, node);
		list_del(&tx->node);

		siochan->current_tx = tx;
		sio_fill_in_locked(siochan);
	}
	spin_unlock_irqrestore(&siochan->lock, flags);
}

static int sio_terminate_all(struct dma_chan *chan)
{
	struct sio_chan *siochan = to_sio_chan(chan);
	unsigned long flags;

	spin_lock_irqsave(&siochan->lock, flags);
	if (siochan->current_tx) {
		siochan->current_tx->terminated = true;
		schedule_work(&siochan->terminate_wq);
	}
	list_splice_tail_init(&siochan->submitted, &siochan->to_free);
	list_splice_tail_init(&siochan->issued, &siochan->to_free);
	spin_unlock_irqrestore(&siochan->lock, flags);

	return 0;
}

static void sio_terminate_work(struct work_struct *wq)
{
	struct sio_chan *siochan = container_of(wq, struct sio_chan, terminate_wq);
	struct sio_tx *tx, *_tx;
	unsigned long flags;
	LIST_HEAD(head);
	int ret;

	dev_dbg(siochan->host->dev, "entering terminate_work\n");

	spin_lock_irqsave(&siochan->lock, flags);
	tx = siochan->current_tx;
	list_splice_tail_init(&siochan->to_free, &head);
	spin_unlock_irqrestore(&siochan->lock, flags);

	if (WARN_ON(!tx))
		return;

	ret = sio_call(siochan->host, FIELD_PREP(SIOMSG_EP, siochan->no) |
				      FIELD_PREP(SIOMSG_TYPE, MSG_TERMINATE));
	if (ret < 0)
		dev_err(siochan->host->dev, "terminate call on chan %d failed: %d\n",
			siochan->no, ret);

	ret = wait_for_completion_timeout(&tx->done, msecs_to_jiffies(500));
	if (!ret)
		dev_err(siochan->host->dev, "terminate descriptor wait timed out\n");

	tasklet_kill(&siochan->tasklet); // TODO: where should this be placed?

	spin_lock_irqsave(&siochan->lock, flags);
	if (siochan->current_tx == tx)
		siochan->current_tx = NULL;
	spin_unlock_irqrestore(&siochan->lock, flags);

	sio_tx_desc_free(&tx->tx);

	list_for_each_entry_safe(tx, _tx, &head, node) {
		list_del(&tx->node);
		sio_tx_desc_free(&tx->tx);
	}

	/*
	 * If we were to uphold the dmaengine interface perfectly, we should
	 * at this point check whether any new descriptors were issued, but
	 * we count on the client calling sio_synchronize before issuing
	 * a new descriptor, so that there's no need.
	 */

	dev_dbg(siochan->host->dev, "leaving terminate_work\n");
}

static void sio_synchronize(struct dma_chan *chan)
{
	struct sio_chan *siochan = to_sio_chan(chan);

	flush_work(&siochan->terminate_wq);
}

static int sio_alloc_chan_resources(struct dma_chan *chan)
{
	struct sio_chan *siochan = to_sio_chan(chan);
	//struct sio_data *sio = siochan->host;

	dma_cookie_init(&siochan->chan);

	return 0;
}

static void sio_free_chan_resources(struct dma_chan *chan)
{
	sio_terminate_all(chan);
	sio_synchronize(chan);
}

static struct dma_chan *sio_dma_of_xlate(struct of_phandle_args *dma_spec,
					 struct of_dma *ofdma)
{
	struct sio_data *sio = (struct sio_data *) ofdma->of_dma_data;
	unsigned int index = dma_spec->args[0];

	if (dma_spec->args_count != 1 || index >= sio->nchannels)
		return ERR_PTR(-EINVAL);

	return dma_get_slave_channel(&sio->channels[index].chan);
}


static void sio_rtk_crashed(void *cookie)
{
	struct sio_data *sio = cookie;

	dev_err(sio->dev, "SIO down (crashed)");
}

static void sio_process_report(struct sio_chan *siochan)
{
	unsigned long flags;

	spin_lock_irqsave(&siochan->lock, flags);
	if (siochan->current_tx) {
		struct sio_tx *tx = siochan->current_tx;

		siochan->nperiod_acks++;
		if (tx->ninflight)
			tx->ninflight--;
		if (!tx->terminated)
			tasklet_schedule(&siochan->tasklet);
		if (!sio_fill_in_locked(siochan) && !tx->ninflight)
			complete(&tx->done);
	}
	spin_unlock_irqrestore(&siochan->lock, flags);
}

static void sio_recv_msg(void *cookie, u8 ep, u64 msg)
{
	struct sio_data *sio = cookie;
	struct sio_tagdata *tags = &sio->tags;
	u32 data;
	u8 param, type, tag, sioep;

	if (ep != EP_SIO)
		goto unknown;

	data  = FIELD_GET(SIOMSG_DATA, msg);
	param = FIELD_GET(SIOMSG_PARAM, msg);
	type  = FIELD_GET(SIOMSG_TYPE, msg);
	tag   = FIELD_GET(SIOMSG_TAG, msg);
	sioep = FIELD_GET(SIOMSG_EP, msg);

	switch (type) {
	case MSG_STARTED:
		dev_info(sio->dev, "SIO protocol v%u\n", data);
		type = MSG_ACK; /* Pretend this is an ACK */
		fallthrough;
	case MSG_ACK:
	case MSG_NACK:
		if (WARN_ON(tag >= USABLE_TAGS + 1))
			break;

		if (tags->atomic[tag]) {
			if (tags->ack_callback[tag] \
			    && !WARN_ON(sioep >= sio->nchannels))
				tags->ack_callback[tag](&sio->channels[sioep],
							tags->cookie[tag],
							type == MSG_ACK);
			if (type == MSG_NACK)
				dev_err(sio->dev, "got a NACK on channel %d\n", sioep);
			sio_free_tag(sio, tag);
		} else {
			tags->acked[tag] = (type == MSG_ACK);
			complete(&tags->completions[tag]);
		}
		break;

	case MSG_REPORT:
		if (WARN_ON(sioep >= sio->nchannels))
			break;

		sio_process_report(&sio->channels[sioep]);
		break;

	default:
		goto unknown;
	}
	return;

unknown:
	dev_warn(sio->dev, "received unknown message: ep %x data %016llx\n",
		 ep, msg);
}

static int _sio_send_siomsg(struct sio_data *sio, u64 msg, bool atomic,
			    sio_ack_callback ack_callback, void *cookie)
{
	int tag, ret;

	tag = sio_alloc_tag(sio);
	if (tag < 0)
		return tag;

	if (atomic) {
		sio_set_tag_atomic(sio, tag, ack_callback, cookie);
	} else {
		reinit_completion(&sio->tags.completions[tag]);
	}

	msg &= ~SIOMSG_TAG;
	msg |= FIELD_PREP(SIOMSG_TAG, tag);
	ret = apple_rtkit_send_message(sio->rtk, EP_SIO, msg, NULL,
				       atomic);
	if (ret < 0) {
		sio_free_tag(sio, tag);
		return ret;
	}

	return tag;
}

static int sio_send_siomsg(struct sio_data *sio, u64 msg)
{
	return _sio_send_siomsg(sio, msg, false, NULL, NULL);
}

static int sio_send_siomsg_atomic(struct sio_data *sio, u64 msg,
				  sio_ack_callback ack_callback,
				  void *cookie)
{
	return _sio_send_siomsg(sio, msg, true, ack_callback, cookie);
}

static int sio_call(struct sio_data *sio, u64 msg)
{
	int tag, ret;

	tag = sio_send_siomsg(sio, msg);
	if (tag < 0)
		return tag;

	ret = wait_for_completion_timeout(&sio->tags.completions[tag],
					  msecs_to_jiffies(SIO_CALL_TIMEOUT_MS));
	if (!ret) {
		dev_warn(sio->dev, "call %8x timed out\n", msg);
		sio_free_tag(sio, tag);
		return -ETIME;
	}

	ret = sio->tags.acked[tag];
	sio_free_tag(sio, tag);

	return ret;
}

static const struct apple_rtkit_ops sio_rtkit_ops = {
	.crashed = sio_rtk_crashed,
	.recv_message = sio_recv_msg,
};

static void sio_chan_tasklet(struct tasklet_struct *t)
{
	struct sio_chan *siochan = container_of(t, struct sio_chan, tasklet);
	struct sio_tx *siotx;
	struct dmaengine_desc_callback cb;
	struct dmaengine_result tx_result;
	unsigned long flags;
	int nacks;

	spin_lock_irqsave(&siochan->lock, flags);
	siotx = siochan->current_tx;
	nacks = siochan->nperiod_acks;
	siochan->nperiod_acks = 0;
	spin_unlock_irqrestore(&siochan->lock, flags);

	if (!siotx || !nacks)
		return;

	tx_result.result = DMA_TRANS_NOERROR;
	tx_result.residue = 0;

	dmaengine_desc_get_callback(&siotx->tx, &cb);
	while (nacks--)
		dmaengine_desc_callback_invoke(&cb, &tx_result);
}

static int sio_device_config(struct dma_chan *chan,
			     struct dma_slave_config *config)
{
	struct sio_chan *siochan = to_sio_chan(chan);
	struct sio_data *sio = siochan->host;
	bool is_tx = sio_chan_direction(siochan->no) == DMA_MEM_TO_DEV;
	struct sio_chan_config *cfg = sio->shmem;
	int ret;

	switch (is_tx ? config->dst_addr_width : config->src_addr_width) {
	case DMA_SLAVE_BUSWIDTH_1_BYTE:
		cfg->datashape = 0;
		break;
	case DMA_SLAVE_BUSWIDTH_2_BYTES:
		cfg->datashape = 1;
		break;
	case DMA_SLAVE_BUSWIDTH_4_BYTES:
		cfg->datashape = 2;
		break;
	default:
		return -EINVAL;
	}

	cfg->fifo = 0x800;
	cfg->limit = 0x800;
	cfg->threshold = 0x800;
	dma_wmb();

	ret = sio_call(sio, FIELD_PREP(SIOMSG_TYPE, MSG_CONFIGURE) |
			    FIELD_PREP(SIOMSG_EP, siochan->no));

	if (ret == 1)
		ret = 0;
	else if (ret == 0)
		ret = -EINVAL;
	return ret;
}

static int sio_alloc_shmem(struct sio_data *sio)
{
	dma_addr_t iova;
	int err;

	sio->shmem = dma_alloc_coherent(sio->dev, SIO_SHMEM_SIZE,
					&iova, GFP_KERNEL);
	if (!sio->shmem)
		return -ENOMEM;

	sio->desc_base = (struct sio_desc *) (sio->shmem + 56);
	sio->desc_allocated = devm_kzalloc(sio->dev, SIO_NO_DESC_SLOTS / 32,
					   GFP_KERNEL);
	if (!sio->desc_allocated)
		return -ENOMEM;

	err = sio_call(sio, FIELD_PREP(SIOMSG_TYPE, MSG_SETUP) |
			    FIELD_PREP(SIOMSG_PARAM, 1) |
			    FIELD_PREP(SIOMSG_DATA, iova >> 12));
	if (err != 1) {
		if (err == 0)
			err = -EINVAL;
		return err;
	}

	err = sio_call(sio, FIELD_PREP(SIOMSG_TYPE, MSG_SETUP) |
			    FIELD_PREP(SIOMSG_PARAM, 2) |
			    FIELD_PREP(SIOMSG_DATA, SIO_SHMEM_SIZE));
	if (err != 1) {
		if (err == 0)
			err = -EINVAL;
		return err;
	}

	return 0;
}

static int sio_send_dt_params(struct sio_data *sio)
{
	struct device_node *np = sio->dev->of_node;
	const char *propname = "apple,sio-firmware-params";
	int nparams, err, i;

	nparams = of_property_count_u32_elems(np, propname);
	if (nparams < 0) {
		err = nparams;
		goto badprop;
	}

	for (i = 0; i < nparams / 2; i++) {
		u32 key, val;

		err = of_property_read_u32_index(np, propname, 2 * i, &key);
		if (err) goto badprop;
		err = of_property_read_u32_index(np, propname, 2 * i + 1, &val);
		if (err) goto badprop;

		err = sio_call(sio, FIELD_PREP(SIOMSG_TYPE, MSG_SETUP) |
				    FIELD_PREP(SIOMSG_PARAM, key & 0xff) |
				    FIELD_PREP(SIOMSG_EP, key >> 8) |
				    FIELD_PREP(SIOMSG_DATA, val));
		if (err < 1) {
			if (err == 0)
				err = -ENXIO;
			return dev_err_probe(sio->dev, err, "sending SIO parameter %#x value %#x\n",
					     key, val);
		}
	}

	return 0;

badprop:
	return dev_err_probe(sio->dev, err, "failed to read '%s'\n",
			     propname);
}

static int sio_probe(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	struct sio_data *sio;
	struct dma_device *dma;
	int nchannels;
	int err, i;

	err = of_property_read_u32(np, "dma-channels", &nchannels);
	if (err || nchannels > NCHANNELS_MAX)
		return dev_err_probe(&pdev->dev, -EINVAL,
				     "missing or invalid dma-channels property\n");

	sio = devm_kzalloc(&pdev->dev, struct_size(sio, channels, nchannels), GFP_KERNEL);
	if (!sio)
		return -ENOMEM;

	platform_set_drvdata(pdev, sio);
	sio->dev = &pdev->dev;
	sio->nchannels = nchannels;

	sio->base = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(sio->base))
		return PTR_ERR(sio->base);

	sio->rtk = devm_apple_rtkit_init(&pdev->dev, sio, NULL, 0, &sio_rtkit_ops);
	if (IS_ERR(sio->rtk))
		return dev_err_probe(&pdev->dev, PTR_ERR(sio->rtk),
				     "couldn't initialize rtkit\n");
	for (i = 1; i < USABLE_TAGS + 1; i++)
		init_completion(&sio->tags.completions[i]);

	dma = &sio->dma;
	dma_cap_set(DMA_PRIVATE, dma->cap_mask);
	dma_cap_set(DMA_CYCLIC, dma->cap_mask);

	dma->dev = &pdev->dev;
	dma->device_alloc_chan_resources = sio_alloc_chan_resources;
	dma->device_free_chan_resources = sio_free_chan_resources;
	dma->device_tx_status = sio_tx_status;
	dma->device_issue_pending = sio_issue_pending;
	dma->device_terminate_all = sio_terminate_all;
	dma->device_synchronize = sio_synchronize;
	dma->device_prep_dma_cyclic = sio_prep_dma_cyclic;
	dma->device_config = sio_device_config;

	dma->directions = BIT(DMA_MEM_TO_DEV);
	dma->residue_granularity = DMA_RESIDUE_GRANULARITY_SEGMENT;
	dma->dst_addr_widths = BIT(DMA_SLAVE_BUSWIDTH_1_BYTE) |
			BIT(DMA_SLAVE_BUSWIDTH_2_BYTES) |
			BIT(DMA_SLAVE_BUSWIDTH_4_BYTES);

	INIT_LIST_HEAD(&dma->channels);
	for (i = 0; i < nchannels; i++) {
		struct sio_chan *siochan = &sio->channels[i];

		siochan->host = sio;
		siochan->no = i;
		siochan->chan.device = &sio->dma;
		spin_lock_init(&siochan->lock);
		INIT_LIST_HEAD(&siochan->submitted);
		INIT_LIST_HEAD(&siochan->issued);
		INIT_LIST_HEAD(&siochan->to_free);
		list_add_tail(&siochan->chan.device_node, &dma->channels);
		tasklet_setup(&siochan->tasklet, sio_chan_tasklet);
		INIT_WORK(&siochan->terminate_wq, sio_terminate_work);
	}

	writel(CPU_CONTROL_RUN, sio->base + REG_CPU_CONTROL);

	err = apple_rtkit_boot(sio->rtk);
	if (err)
		return dev_err_probe(&pdev->dev, err, "SIO did not boot\n");

	err = apple_rtkit_start_ep(sio->rtk, EP_SIO);
	if (err)
		return dev_err_probe(&pdev->dev, err, "starting SIO endpoint\n");

	err = sio_call(sio, FIELD_PREP(SIOMSG_TYPE, MSG_START));
	if (err < 1) {
		if (err == 0)
			err = -ENXIO;
		return dev_err_probe(&pdev->dev, err, "starting SIO service\n");
	}

	err = sio_send_dt_params(sio);
	if (err < 0)
		return dev_err_probe(&pdev->dev, err, "failed to send boot-up parameters\n");

	err = sio_alloc_shmem(sio);
	if (err < 0)
		return err;

	err = dma_async_device_register(&sio->dma);
	if (err)
		return dev_err_probe(&pdev->dev, err, "failed to register DMA device\n");

	err = of_dma_controller_register(pdev->dev.of_node, sio_dma_of_xlate, sio);
	if (err) {
		dma_async_device_unregister(&sio->dma);
		return dev_err_probe(&pdev->dev, err, "failed to register with OF\n");
	}

	return 0;
}

static int sio_remove(struct platform_device *pdev)
{
	struct sio_data *sio = platform_get_drvdata(pdev);

	of_dma_controller_free(pdev->dev.of_node);
	dma_async_device_unregister(&sio->dma);

	return 0;
}

static const struct of_device_id sio_of_match[] = {
	{ .compatible = "apple,sio", },
	{ }
};
MODULE_DEVICE_TABLE(of, sio_of_match);

static struct platform_driver apple_sio_driver = {
	.driver = {
		.name = "apple-sio",
		.of_match_table = sio_of_match,
	},
	.probe = sio_probe,
	.remove = sio_remove,
};
module_platform_driver(apple_sio_driver);

MODULE_AUTHOR("Martin Povišer <povik+lin@cutebit.org>");
MODULE_DESCRIPTION("Driver for SIO coprocessor on Apple SoCs");
MODULE_LICENSE("Dual MIT/GPL");
