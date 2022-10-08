// SPDX-License-Identifier: GPL-2.0-only OR MIT
/* Copyright 2021 Alyssa Rosenzweig <alyssa@rosenzweig.io> */

#undef TRACE_SYSTEM
#define TRACE_SYSTEM iomfb

#if !defined(_TRACE_IOMFB_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_IOMFB_H

#include "dcp-internal.h"

#include <linux/stringify.h>
#include <linux/types.h>
#include <linux/tracepoint.h>

TRACE_EVENT(iomfb_swap_submit,
	    TP_PROTO(struct apple_dcp *dcp, u32 swap_id),
	    TP_ARGS(dcp, swap_id),
	    TP_STRUCT__entry(
			     __field(u64, dcp)
			     __field(u32, swap_id)
	    ),
	    TP_fast_assign(
			   __entry->dcp = (u64)dcp;
			   __entry->swap_id = swap_id;
	    ),
	    TP_printk("dcp=%llx, swap_id=%d",
		      __entry->dcp,
		      __entry->swap_id)
);

TRACE_EVENT(iomfb_swap_complete,
	    TP_PROTO(struct apple_dcp *dcp, u32 swap_id),
	    TP_ARGS(dcp, swap_id),
	    TP_STRUCT__entry(
			     __field(u64, dcp)
			     __field(u32, swap_id)
	    ),
	    TP_fast_assign(
			   __entry->dcp = (u64)dcp;
			   __entry->swap_id = swap_id;
	    ),
	    TP_printk("dcp=%llx, swap_id=%d",
		      __entry->dcp,
		      __entry->swap_id
	    )
);

TRACE_EVENT(iomfb_swap_complete_intent_gated,
	    TP_PROTO(struct apple_dcp *dcp, u32 swap_id, u32 width, u32 height),
	    TP_ARGS(dcp, swap_id, width, height),
	    TP_STRUCT__entry(
			     __field(u64, dcp)
			     __field(u32, swap_id)
			     __field(u32, width)
			     __field(u32, height)
	    ),
	    TP_fast_assign(
			   __entry->dcp = (u64)dcp;
			   __entry->swap_id = swap_id;
			   __entry->height = height;
			   __entry->width = width;
	    ),
	    TP_printk("dcp=%llx, swap_id=%u %ux%u",
		      __entry->dcp,
		      __entry->swap_id,
		      __entry->width,
		      __entry->height
	    )
);

#endif /* _TRACE_IOMFB_H */

/* This part must be outside protection */

#undef TRACE_INCLUDE_FILE
#define TRACE_INCLUDE_FILE trace

#undef TRACE_INCLUDE_PATH
#define TRACE_INCLUDE_PATH .

#include <trace/define_trace.h>

