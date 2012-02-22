//**************************
//* 100901 gw.wang@lge.com
//* nv partition access by proc fs
//**************************
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/proc_fs.h>
#include<linux/file.h>
#include<linux/syscalls.h>
#include<asm/uaccess.h>
#include<linux/fs.h>
#include<linux/fcntl.h>
#include<linux/init.h>
#include <linux/seq_file.h>

#define PROCFS_MAX_SIZE 256
#define PROCFS_NAME "nvdata"

typedef struct FlagInfo {
    const char *name;
    const char *flag_num;
}FlagInfo;

static FlagInfo flag_table[] = {
/* renew information every flashing */
	{ "PRODUCT_NAME" , "0" },
	{ "STARTABLET_VER" , "1" },
	{ "OEM_LOCK" , "2" },
	{ "FBOOT" , "3" },
	{ "BOOTLOADER_VER" , "4" },
	{ "MODEM_UPDATE" , "5" },

/* Do not initialize the flags below on the Factory Reset */
	{ "BDADDR_FLAG" , "10" },
	{ "BDADDR" , "11" },
	{ "INFO" , "12" },
	{ "WIFI_MACADDR" , "13" },
	{ "WIFI_FLAG" , "14" },
	{ "IMEI" , "15" },
	{ "IMPL" , "16" },
	{ "IMEICK" , "17" },
	{ "IMPLCK" , "18" },
	{ "NV_INIT" , "19" },
	{ "SENSOR_CAL" , "20" },
	{ "SERIAL_NO" , "21" },
	{ "USB_SERIAL" , "22" },
	{ "QTEST" , "23" },
	{ "CSN", "24" },
	{ "FUSE", "25" },

/* initialize the flags below on the Factory Reset */
	{ "FASTBOOT" , "100" },
	{ "NV_BITMAP" , "101" },
	{ "BDADDR_DUT" , "102" },
	{ "MUIC_PATH" , "103" },
	{ "TEMP" , "104" },
	{ "BOOT_FLAG" , "105" },
	{ "BASEBAND_VER" , "106" },
	{ "OTP" , "107" },
	{ "UTP" , "108" },
	{ "HW_REV" , "109" },
	{ "REBOOT_REASON" , "110" },
	{ "FRST_FLAG" , "111" },
	{ "BOOTLOADER_UPDATE" , "112" },
	{ "MDP" , "113" },

/* Do not initialize the flags below on the Factory Reset */
};
#define PROC_LIST_SIZE (sizeof(flag_table) / sizeof(flag_table[0]))
static struct proc_dir_entry *nv_proc_list[PROC_LIST_SIZE];
static struct proc_dir_entry *recovery_proc;
static struct proc_dir_entry *nvdata_proc_dir;

static char procfs_buffer[PROCFS_MAX_SIZE];

static unsigned long procfs_buffer_size=0;

static int procfile_read(char *buffer,char **buffer_location,off_t offset,int buffer_length,int *eof,void *data)
{
	int len;
	mm_segment_t old_fs;
	char buf_to_read[PROCFS_MAX_SIZE]={0,};
	int fd;
	int add_offset=-1;

	old_fs=get_fs();
	set_fs(KERNEL_DS);
	if (!strncmp(data,"boot_cmd",strlen("boot_cmd"))){
		add_offset=0;
		fd=sys_open("/dev/block/mmcblk0p3",O_RDWR|O_CREAT|O_APPEND,0644);
	}
	else{
		sscanf(data,"%d",&add_offset);
		fd=sys_open("/dev/block/mmcblk0p7",O_RDWR|O_CREAT|O_APPEND,0644);
	}
	if(fd<0)
	{
		printk("[%s] >> Cannot Open file..address:%s\n",__func__, (char *)data);
		return -EFAULT;
	}

	sys_lseek(fd,2048*add_offset,SEEK_SET);
	sys_read(fd,buf_to_read,PROCFS_MAX_SIZE-1);
	len=sprintf(buffer, "%s\n", buf_to_read);
	sys_close(fd);
	set_fs(old_fs);
	printk("[%s] Read file...address:%s  data:%s\n",__func__, (char *)data, buf_to_read);
	return len;
}

static int procfile_write(struct file *file,const char *buffer,unsigned long count,void *data)
{
	int fd;
	loff_t pos=0;
	char buf_to_write[PROCFS_MAX_SIZE]={0,};
	struct file *m_file;
	mm_segment_t old_fs;
	int add_offset=-1;
	procfs_buffer_size=count;
	old_fs=get_fs();
	set_fs(KERNEL_DS);

	if (!strncmp(data,"boot_cmd",strlen("boot_cmd"))){
		add_offset=0;
		fd=sys_open("/dev/block/mmcblk0p3",O_RDWR|O_CREAT|O_APPEND,0666);
	}
	else{
        sscanf(data,"%d",&add_offset);
		fd=sys_open("/dev/block/mmcblk0p7",O_RDWR|O_CREAT|O_APPEND,0666);
	}
	if(fd<0)
	{
		printk("[%s] >> Cannot Open file..address:%s\n",__func__, (char *)data);
		return -EFAULT;
	}

	if(procfs_buffer_size>PROCFS_MAX_SIZE)
		procfs_buffer_size=PROCFS_MAX_SIZE;

	if(copy_from_user(procfs_buffer,buffer,procfs_buffer_size))
		return -EFAULT;
	m_file=fget(fd);

	if(m_file){
		memset(buf_to_write,0,PROCFS_MAX_SIZE);
		memcpy(buf_to_write,buffer,procfs_buffer_size-1);
		pos=vfs_llseek(m_file,2048*add_offset,SEEK_SET);
		if( add_offset != 19 )
			vfs_write(m_file,buf_to_write,PROCFS_MAX_SIZE,&pos);
		else if( add_offset == 19 && !strcmp(buf_to_write,"reset") )
			vfs_write(m_file,buf_to_write,PROCFS_MAX_SIZE,&pos);

		fput(m_file);
	}

	sys_close(fd);
	set_fs(old_fs);
	printk("[%s] Write File...address:%s  data:%s\n",__func__, (char *)data, buf_to_write);
	return procfs_buffer_size;
}

static int __init nv_init(void)
{
	int i=0;
	nvdata_proc_dir = proc_mkdir("nvdata", NULL);

	for(i=0;i<PROC_LIST_SIZE;i++)
	{
		FlagInfo *info = &flag_table[i];
		nv_proc_list[i]=create_proc_entry(info->name,0666,nvdata_proc_dir);
		nv_proc_list[i]->read_proc=procfile_read;
		nv_proc_list[i]->write_proc=procfile_write;
		nv_proc_list[i]->data=(void *)info->flag_num;
	}

	recovery_proc=create_proc_entry("boot_cmd",0666,NULL);
	recovery_proc->read_proc=procfile_read;
	recovery_proc->write_proc=procfile_write;
	recovery_proc->data="boot_cmd";

	return 0;

}

module_init(nv_init);
