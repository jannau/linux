/* bh1721_als.h
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

int als_cmd(int cmd, int arg);

enum {
	/* cmd */
	ALS_OFF = 0,/* ALS_OFF and ALS_ON values are uesed in sensor HAL, so  */
	ALS_ON	= 1,/* they should not be changed */
	REPORT,
	RES_MODE,
	SET_MGT,
	PW,
	DVI,
	RESOL,
	SCAIL_M,
	SCAIL_N,
	POLLTIME,
	READ_LUX,
	ALS_TEST_READ,
#if defined (LED_LIGHT_REMOVAL)
	ALS_LED_BLINK,
	ALS_LED_VALUE,
#endif
		/* arg */
		/* arg for cmd RES_MODE */
		AUTO_RES_MODE=100,
		HIGH_RES_MODE,
		LOW_RES_MODE,
		/* arg for cmd REPORT */
		REPORT_OFF=200,
		REPORT_ON,
		REPORT_WORK,
};

