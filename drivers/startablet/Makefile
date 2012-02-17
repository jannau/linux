# startablet peripheral devices

ccflags-$(CONFIG_DEBUG_GPIO)	+= -DDEBUG

obj-$(CONFIG_STARTABLET_MUIC)   += star_muic.o
obj-y				+= star_powerkey.o
obj-y				+= star_cam_pmic.o
obj-y				+= headset_det.o
obj-$(CONFIG_STARTABLET_GPS)	+= star_gps.o
obj-$(CONFIG_STARTABLET_RFKILL)	+= star_rfkill.o
obj-$(CONFIG_STARTABLET_BATTERY)+= star_battery.o
#LGE_KERNEL_MUX START
EXTRA_CFLAGS += -DLGE_KERNEL_MUX
## LGE_UPDATE_S CP Reset & Response halt Recovery
EXTRA_CFLAGS += -DLGE_FEATURE_CP_RESET_RECOVERY
EXTRA_CFLAGS += -DLGE_FEATURE_CP_HALT_DETECTION
## LGE_UPDATE_E CP Reset & Response halt Recovery
obj-$(CONFIG_STAR_RIL) += ts0710_mux.o
obj-$(CONFIG_STAR_RIL) += cpwatcher.o
obj-$(CONFIG_SPI_TEGRA) += ifx_n721_spi.o
obj-$(CONFIG_SPI_MDM6600) += spi_mdm6600.o
obj-$(CONFIG_RINETWORK_DRIVER) += rindrv.o

#LGE_KERNEL_MUX END
obj-$(CONFIG_STARTABLET_MISC)	+= star_misc.o
obj-$(CONFIG_STARTABLET_MISC)	+= star_leds.o
#obj-$(CONFIG_STARTABLET_GYRO_ACCEL) += star_accelerometer.o
#obj-$(CONFIG_STARTABLET_GYRO_ACCEL) += star_gyro.o
#obj-$(CONFIG_STARTABLET_COMPASS) += star_compass.o
obj-$(CONFIG_STARTABLET_ALS)	+= bh1721_als.o
obj-y					+= tegra_displaytest.o
obj-$(CONFIG_STARTABLET_ECHO) += star_echo.o
obj-y     += camera_flash.o
obj-$(CONFIG_TOUCHSCREEN_ATMEL_MAXTOUCH)+= star_touch.o

obj-y += star_ats.o
obj-y += lge_ats_eta.o
obj-y += lge_ats_event_log.o

