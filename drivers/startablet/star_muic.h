/*
 * drivers/startablet/star_muic.c
 *
 * Muic class driver for platforms
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
//  MAX14526 REGISTERS
//===========================================================================

#ifndef _STAR_MUIC_H_
#define _STAR_MUIC_H_
#define SLAVE_ADDR    0x88

//0x00
#define DEVICEID_REG  0x00
#define VENDOR_ID_M   0xF0
#define CHIP_REV_M    0x0F

//0x01
#define CTRL1_REG     0x01
#define ID_2P2_M      0x40
#define ID_620_M      0x20
#define ID_200_M      0x10
#define VLDO_M        0x08
#define SEMREN_M      0x04
#define ADC_EN_M      0x02
#define CP_EN_M       0x01

//0x02
#define CTRL2_REG     0x02
#define INTPOL_M      0x80
#define INT_EN_M      0x40
#define MIC_LP_M      0x20
#define CP_AUD_M      0x10
#define CHG_TYPE_M    0x02
#define USB_DET_DIS_M 0x01

//0x03
#define SW_CTRL_REG   0x03
#define VID_ON_M      0x80
#define MIC_ON_M      0x40
#define DP2_M         0x38
#define DM1_M         0x07
// DP2_M
#define COMP2_TO_DP2  0x00
#define COMP2_TO_U2   0x08
#define COMP2_TO_AUD2 0x10
#define COMP2_TO_HZ   0x20
// DM1_M
#define COMN1_TO_DN1  0x00
#define COMN1_TO_U1   0x01
#define COMN1_TO_AUD1 0x02
#define COMN1_TO_C1COMP 0x03
#define COMN1_TO_HZ   0x04

//0x04
#define INT_STAT_REG  0x04
#define CHGDET_M      0x80
#define MR_COMP_M     0x40
#define SEND_END_M    0x20
#define VBUS_M        0x10
#define IDNO_M        0x0F
// IDNO_M
#define IDNO_0000     0x00
#define IDNO_0001     0x01
#define IDNO_0010     0x02
#define IDNO_0011     0x03
#define IDNO_0100     0x04
#define IDNO_0101     0x05
#define IDNO_0110     0x06
#define IDNO_0111     0x07
#define IDNO_1000     0x08
#define IDNO_1001     0x09
#define IDNO_1010     0x0A
#define IDNO_1011     0x0B

#define IDNO_GND      0x00
#define IDNO_OPEN     0x0B

//0x05
#define STATUS_REG    0x05
#define DCPORT_M      0x80
#define CHPORT_M      0x40
#define C1COMP_M      0x01

//===========================================================================
//
//===========================================================================

enum muic_port_setting_type
{
	PORT_SETTING_AUTO = 0,
	PORT_SETTING_AP_USB,
	PORT_SETTING_AP_UART,
	PORT_SETTING_CP_UART,
	PORT_SETTING_CP_USB
};

#endif // _STAR_MUIC_H_
