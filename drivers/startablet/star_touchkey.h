/*
 *  Atmel maXTouch header file
 *  
 *  Copyright (c) 2010 Iiro Valkonen <iiro.valkonen@atmel.com>
 *  Copyright (c) 2010 Ulf Samuelsson <ulf.samuelsson@atmel.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 or 3 as
 *  published by the Free Software Foundation.
 *  See the file "COPYING" in the main directory of this archive
 *  for more details.
 *
 */

#define	SYNAPTICS_I2C_ADDR				0x2C
#define SYNAPTICS_CONFIG_REG_START_ADDR        0x0000
#define SYNAPTICS_CONFIG_REG_END_ADDR          0x0018
#define SYNAPTICS_NUM_CONFIG_REG_BYTES         ((SYNAPTICS_CONFIG_REG_END_ADDR - SYNAPTICS_CONFIG_REG_START_ADDR + 1)*2)
/* Total number of Configuration Bytes */
#define SYNAPTICS_NUM_CONFIG_BYTES             (SYNAPTICS_NUM_CONFIG_REG_BYTES + 2)

static u8    Synaptics_Config[50]= {
	/* High byte, followed by low Byte */
	//0x00, 0x00,     /* base address of config registers */
	0x00, 0x07,     /* values of 0x0000 */
	0x00, 0x00,     /* values of 0x0001 */
	0x00, 0x32,     /* values of 0x0002 */
	0x00, 0x00,     /* values of 0x0003 */
	//   0xB0, 0xD0,     /* values of 0x0004 */
	0x08, 0x33,     /* values of 0x0004 */
	0x00, 0x00,     /* values of 0x0005 */
	0x00, 0x00,     /* values of 0x0006 */
	0x00, 0x00,     /* values of 0x0007 */
	0x02, 0x0D,     /* values of 0x0008 */
	0x00, 0x00,     /* values of 0x0009 */
	0x00, 0x00,     /* values of 0x000A */
	0x00, 0x00,     /* values of 0x000B */
	0x00, 0x00,     /* values of 0x000C */
	0x00, 0x00,     /* values of 0x000D */
	0x1E, 0x01,     /* values of 0x000E */
	0x02, 0x00,     /* values of 0x000F */
	//Taeho, S0/1/4/5/11, 0x10HL, 0x12HL, 0x15H
	0x8C, 0x8C, //0xA7, 0xA7, //    0x00, 0xA1, 	/* values of 0x0010  S1, S0*/
	0x00, 0x00, //0x9D, 0xA7,     /* values of 0x0011 S3, S2*/
	0x8C, 0x8C, //0xA7, 0xB7, //    0x00, 0x9A,	/* values of 0x0012   S5 , S4*/
	0x00, 0x00, //0x9F, 0x91, /* values of 0x0013   S6  , S7*/
	0x00, 0x00, //0x8D, 0x00,     /* values of 0x0014 S9 S8 */
	0x8C, 0x00, //0xA7, 0x00, //0x00, 0x00    /* values of 0x0015   S11, S10*/
	0x00, 0x00, //0x92, 0x92,     /* values of 0x0016 S13 S12 */
	0x00, 0x00, //0x9D, 0x00, /* values of 0x0017   S14, S15 */
	0x00, 0x00      /* values of 0x0018 */
};
