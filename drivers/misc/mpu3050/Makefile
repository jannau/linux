
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

EXTRA_CFLAGS += -Idrivers/misc/mpu3050 \
                -Iinclude/linux

ifdef CONFIG_SENSORS_MPU3050_DEBUG
EXTRA_CFLAGS += -DDEBUG
endif