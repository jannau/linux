/*
 * Copyright (C) 2011 LGE Inc.
 *
 * StarTablet  Motion header file.
 *
 */

 #ifndef __STARTABLET_MOTION_H__
#define __STARTABLET_MOTION_H__

/*==================================================================================================
				LGE Definitions
==================================================================================================*/

void sensorWaitCheck(void);
int MLDLSetI2CBypass(struct mldl_cfg *mldl_cfg,
			void *mlsl_handle,
			unsigned char enable);

#define	STAR_SENSOR_NONE  	0x0000
#define	STAR_ACCELEROMETER  0x0001
#define	STAR_MAGNETIC  		0x0002
#define	STAR_ORIENTATION  	0x0004
#define	STAR_GYRO  			0x0008
#define	STAR_FACTORY_TEST	0x0010

#define	STAR_DELAY 			0x0400
#define	STAR_SUSPEND  		0x1000


#define	STAR_ACCELEROMETER_BIT  	0
#define	STAR_MAGNETIC_BIT			1
#define	STAR_ORIENTATION_BIT  		2
#define	STAR_GYRO_BIT				3
#define	STAR_FACTORY_TEST_BIT  		4

#define	STAR_DELAY_BIT  		10
#define	STAR_SUSPEND_BIT  		12

/*typedef enum
{
	STARTABLET_SENSOR_NONE 	    = 0x00,
	STARTABLET_ACCELEROMETER   	= 0x01,
	STARTABLET_ORIENTATION 	    = 0x02,
	STARTABLET_TILT  		= 0x04,
	STARTABLET_SHAKE 		= 0x08,
	STARTABLET_SNAP 		= 0x10,
	STARTABLET_FLIP 		= 0x20,
	STARTABLET_TAP 			= 0x40,
	STARTABLET_GYRO 		= 0x80
}startablet_sensor_enum_type;*/


/*-------------------------------------------------------------------*/
/*		   			                   IOCTL					                        */
/*-------------------------------------------------------------------*/
#define MOTION_MAGIC					     		0xA3

#define MOTION_IOCTL_ENABLE_DISABLE	    	_IOR(MOTION_MAGIC, 0x01, short)
#define MOTION_IOCTL_SENSOR_DELAY	    	_IOR(MOTION_MAGIC, 0x02, short)
#define MOTION_IOCTL_ACCEL_RAW      	    _IO(MOTION_MAGIC, 0x03)
#define MOTION_IOCTL_MAGNETIC_RAW  	    	_IOW(MOTION_MAGIC, 0x04, int[3])
#define MOTION_IOCTL_ORIENTATION   	    	_IOW(MOTION_MAGIC, 0x05, int[3])
#define MOTION_IOCTL_GYRO_RAW      	    	_IOW(MOTION_MAGIC, 0x06, int[3])
#define MOTION_IOCTL_COMPOSITE         	   	_IOW(MOTION_MAGIC, 0x07, int[16])
#define MOTION_IOCTL_MPU3050_SLEEP_WAKE_UP 	_IOW(MOTION_MAGIC, 0x08, int)
#define MOTION_IOCTL_ACCEL_SLEEP_MODE   	_IOW(MOTION_MAGIC, 0x09, int)
#define MOTION_IOCTL_TESTRESULT        	    _IOWR(MOTION_MAGIC, 0x0A, int[2])
#define MOTION_IOCTL_ACCEL_BIAS        	    _IOW(MOTION_MAGIC, 0x0B, int[3])

#endif

