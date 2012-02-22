/*
 * drivers/startablet/star_rfkill.h
 *
 * Star Primary rfkill client driver
 *
 * Copyright (c) 2010, LG Electronics Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

//===========================================================================
//  RFKILL REGISTERS
//===========================================================================

#ifndef _STAR_RFKILL_H_
#define _STAR_RFKILL_H_

#include <linux/spinlock.h>
#include <linux/rfkill.h>
#include <linux/list.h>
#include <linux/workqueue.h>

struct rfkill {
	spinlock_t		lock;

	const char		*name;
	enum rfkill_type	type;

	unsigned long		state;

	u32			idx;

	bool			registered;
	bool			persistent;

	const struct rfkill_ops	*ops;
	void			*data;

#ifdef CONFIG_RFKILL_LEDS
	struct led_trigger	led_trigger;
	const char		*ledtrigname;
#endif

	struct device		dev;
	struct list_head	node;

	struct delayed_work	poll_work;
	struct work_struct	uevent_work;
	struct work_struct	sync_work;
};

#endif // _STAR_RFKILL_H_