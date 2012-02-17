#include <linux/syscalls.h>
#include <asm/uaccess.h>

#include "nv_data.h"

#define FASTBOOT	100
#define OTP		107
#define FRST_FLAG	111
#define RECOVERY 0

#define NV_SECTOR_SIZE	2048
#define NV_BUFFER_SIZE	512

#define LGE_FILENAME	"/dev/block/mmcblk0p7"
#define MSC_FILENAME	"/dev/block/mmcblk0p3"

int set_frst_flag(void)
{
	int fd=0;
	const char *mode="4";
	char nv_buf[NV_BUFFER_SIZE];
	mm_segment_t old_fs;

	printk(KERN_INFO "%s: frst: %s: start\n", __func__, mode);

	old_fs = get_fs();
	set_fs(KERNEL_DS);

	fd = sys_open(LGE_FILENAME, O_RDWR | O_CREAT | O_APPEND, 0644);
	if(fd < 0)
	{
		printk(KERN_ERR "fail to open nv.\n");
		return -1;
	}

	sys_lseek(fd, FRST_FLAG * NV_SECTOR_SIZE, SEEK_SET);
	sprintf(nv_buf, "%s", mode);
	sys_write(fd, nv_buf, NV_BUFFER_SIZE);
	sys_close(fd);
	set_fs(old_fs);
	sys_sync();

	printk(KERN_INFO "%s: frst: %s: done\n", __func__, mode);

	return 0;
}

int write_rr_into_nv(const char *cmd)
{
	int fd;
	char nv_buf[NV_BUFFER_SIZE];

	if ( !strncmp(cmd, "bootloader", sizeof("bootloader")) )
	{
		const char *mode="FASTBOOT";
		mm_segment_t old_fs = get_fs();

		printk(KERN_INFO "%s: reason: %s: start\n", __func__, cmd);

		set_fs(KERNEL_DS);

		fd = sys_open(LGE_FILENAME, O_RDWR | O_CREAT | O_APPEND, 0644);
		if(fd < 0)
		{
			printk(KERN_ERR "fail to open nv.\n");
			return -1;
		}

		sys_lseek(fd, FASTBOOT * NV_SECTOR_SIZE, SEEK_SET);
		sprintf(nv_buf, "%s", mode);
		sys_write(fd, nv_buf, NV_BUFFER_SIZE);
		sys_close(fd);
		set_fs(old_fs);
		sys_sync();

		printk(KERN_INFO "%s: reason: %s: done\n", __func__, cmd);
	}
	else if ( !strncmp(cmd, "recovery", sizeof("recovery")) )
	{
		const char *mode="boot-recovery";
		mm_segment_t old_fs = get_fs();

		printk(KERN_INFO "%s: reason: %s: start\n", __func__, cmd);

		set_fs(KERNEL_DS);

		fd = sys_open(MSC_FILENAME, O_RDWR | O_CREAT | O_APPEND, 0644);
		if(fd < 0)
		{
			printk(KERN_ERR "fail to open nv.\n");
			return -1;
		}

		sys_lseek(fd, RECOVERY * NV_SECTOR_SIZE, SEEK_SET);
		sprintf(nv_buf, "%s", mode);
		sys_write(fd, nv_buf, NV_BUFFER_SIZE);
		sys_close(fd);
		set_fs(old_fs);
		sys_sync();

		printk(KERN_INFO "%s: reason: %s: done\n", __func__, cmd);
		set_frst_flag();
	}
	else if ( !strncmp(cmd, "otp", sizeof("otp")) )
	{
		const char *mode="OTP";
		mm_segment_t old_fs = get_fs();

		printk(KERN_INFO "%s: reason: %s: start\n", __func__, cmd);

		set_fs(KERNEL_DS);

		fd = sys_open(LGE_FILENAME, O_RDWR | O_CREAT | O_APPEND, 0644);
		if(fd < 0)
		{
			printk(KERN_ERR "fail to open nv.\n");
			return -1;
		}

		sys_lseek(fd, OTP * NV_SECTOR_SIZE, SEEK_SET);
		sprintf(nv_buf, "%s", mode);
		sys_write(fd, nv_buf, NV_BUFFER_SIZE);
		sys_close(fd);
		set_fs(old_fs);
		sys_sync();

		printk(KERN_INFO "%s: reason: %s: done\n", __func__, cmd);
	}
	else
	{
		printk(KERN_DEBUG "%s: %s: No Reason for Reboot!!!\n", __func__, cmd);
	}

	return 0;
}
