/**
	@brief		 S5K5BAFX -samsung s5k5bafx 2M CMOS sensor driver
	@author		 hyungmoo.huh@lge.com
	@date		 2010.11.19
*/


#ifndef S5K5BAFX_H_
#define S5K5BAFX_H_
#include <linux/ioctl.h> /* For IOCTL macros */
#include <media/videobuf-dma-sg.h>
#include <media/v4l2-int-device.h>

#if defined(__cplusplus)
extern "C"
{
#endif

#define SEQUENCE_WAIT_MS   (0xFFFE)
#define SEQUENCE_END        (0xFFFF)

#define S5K5BAFX_IOCTL_SET_MODE				_IOW('o', 8, struct cam_yuv_mode)
#define S5K5BAFX_IOCTL_SET_COLOR_EFFECT				_IOW('o', 9, unsigned int)
#define S5K5BAFX_IOCTL_SET_WHITE_BALANCE			_IOW('o', 12, unsigned int)
#define S5K5BAFX_IOCTL_SET_EXPOSURE			_IOW('o', 13, int)

typedef struct i2c_cam_reg16_Rec {
	u16 addr;
	u16 val;
}i2c_cam_reg16;

struct cam_yuv_mode {
	int xres;
	int yres;
	int index;
};

struct cam_yuv_info {
	int mode;
	struct i2c_client *i2c_client;
	struct cam_yuv_platform_data *pdata;
	struct device dev;
};

typedef enum {
	YUVCamEffectMode_Color = 0,
	YUVCamEffectMode_WB,
	YUVCamEffectMode_Force32 = 0x7FFFFFFF
} YUVCamEffectMode;

typedef enum {
      YUVCamColorEffect_None= 0,
      YUVCamColorEffect_Negative =3,
      YUVCamColorEffect_Sketch =4,
      YUVCamColorEffect_Solarize = 10,
      YUVCamColorEffect_Posterize = 0x30000000,
      YUVCamColorEffect_Sepia,
      YUVCamColorEffect_Mono,
      YUVCamColorEffect_VendorStartUnused = 0x7F000000, /**< Reserved region for introducing Vendor Extensions */
      YUVCamColorEffect_Aqua,
      YUVCamColorEffect_Force32 = 0x7FFFFFFF
} YUVCamColorEffect;


typedef enum {
      YUVCamWhitebalance_Off = 0,
      YUVCamWhitebalance_Auto = 1,
      YUVCamWhitebalance_SunLight,
      YUVCamWhitebalance_Cloudy,
      YUVCamWhitebalance_Shade,
      YUVCamWhitebalance_Tungsten,
      YUVCamWhitebalance_Fluorescent,
      YUVCamWhitebalance_Incandescent,
      YUVCamWhitebalance_Flash,
      YUVCamWhitebalance_Horizon,
      YUVCamWhitebalance_Force32 = 0x7FFFFFFF
} YUVCamUserWhitebalance;


#ifdef __KERNEL__
struct cam_yuv_platform_data {
	int (*power_on)(void);
	int (*power_off)(void);

};
#endif /* __KERNEL__ */

#endif
