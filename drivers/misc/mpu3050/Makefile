
# Kernel makefile for motions sensors
#
#

# MPU
obj-y	+= mpu3050.o
mpu3050-objs +=	mpu-dev.o \
	mpu-i2c.o \
	mlsl-kernel.o \
	mlos-kernel.o \
	mldl_cfg.o \
	lis331.o \
	ami304.o
ifdef CONFIG_MPU_SENSORS_AMI306
mpu3050-objs += $(MLLITE_DIR)compass/ami306.o
endif

ifdef CONFIG_MPU_SENSORS_YAS530
mpu3050-objs += $(MLLITE_DIR)compass/yas530.o
endif


EXTRA_CFLAGS += -Idrivers/misc/mpu3050 \
                -Iinclude/linux

ifdef CONFIG_SENSORS_MPU3050_DEBUG