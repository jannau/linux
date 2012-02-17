/*
* Copyright (C) 2010 LGE, Inc.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2 as
* published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
* 02111-1307, USA
*/

#ifndef __IMX072_EEPROM_H__
#define __IMX072_EEPROM_H__

#include <linux/ioctl.h> /* For IOCTL macros */

#define EEPROM_IOCTL_WRITE_REG				_IOW('o', 1, struct eeprom_reg)
#define EEPROM_IOCTL_READ_REG					_IOWR('o', 2, struct eeprom_reg)


struct eeprom_reg {
	u16 addr;
	u32 val;
};

#endif

