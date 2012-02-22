/*
 * arch/arm/mach-tegra/tegra_als.c
 *
 *ALS device using NVIDIA Tegra ODM kit
 * board_nvodm.c
 */

#include <linux/kernel.h>
#include <linux/device.h>

#include <linux/platform_device.h>
#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <mach/gpio.h>

#include <linux/delay.h>
#include <nvodm_services.h>
#include "nvodm_query_discovery.h" //NvOdmPeripheralConnectivity *pConnectivity = NULL;
#include "nvos.h"
#include <nvrm_gpio.h>
#define ALS_GUID 	NV_ODM_GUID('N','V','O','D','M','A','L','S')


// I2C device address from ambient light sensor.
enum { I2C_ALS_ADDRESS = 0x46};
// Timeout for I2C transaction.
enum { I2C_ALS_TRANSACTION_TIMEOUT = 1000 };
// Maximum number of packetsize supported by the I2C controller.
enum { I2C_ALS_READ_PACKET_SIZE = 2};

static NvU8 s_WriteBuffer;
static NvU8 s_ReadBuffer[I2C_ALS_READ_PACKET_SIZE];


typedef struct ALSDeviceRec *NvOdmALSHandle;


/* Defines the way to read ambient light sensor lux.
 */
typedef NvBool (*AmbientlightLuxRead)(NvOdmALSHandle hDevice,NvU16* value);

/*
 * Defines the way to write ambient light sensor commands.
 */
typedef NvBool (*AmbientlightCommandWrite)(NvOdmALSHandle hDevice, NvU8 cmd);


typedef struct ALSDeviceRec
{    
    NvOdmServicesGpioHandle hGpio;
    NvOdmServicesI2cHandle hI2c;
    NvU32 I2CChannelId;
    NvU8 nDevAddr;
    NvOdmGpioPinHandle s_hResetALSGpioPin;
    NvU32 resetpin, resetport;
    AmbientlightLuxRead LUXRead;
    AmbientlightCommandWrite CMDWrite;
//   	struct input_dev	*input_dev;
} ALS_Device;




static ALS_Device s_hALSHandle;

/*
 * Initialize I2C for ambient light sensor.
 */
 
NvBool NvAmbientsensorI2COpen(NvOdmServicesI2cHandle *hI2CDevice, NvU32 id)
{
    // Open I2C handle.
    *hI2CDevice = NvOdmI2cPinMuxOpen(NvOdmIoModule_I2c, id, NvOdmI2cPinMap_Config2);
    if (*hI2CDevice == NULL)
    {
        return NV_FALSE;
    }
    return NV_TRUE;
}


/*
 * De-initialize I2C for ambient light sensor.
 */
void NvAmbientsensorI2CClose(NvOdmServicesI2cHandle hI2CDevice)
{
    // Close I2C handle.
    if(NULL != hI2CDevice)
    {
        NvOdmI2cClose(hI2CDevice);
    }
}


/*
 * Write I2C register function.
 * Returns NV_TRUE if successful, or NV_FALSE otherwise.
 */
NvBool NvAmbientsensorI2CSetCmd(NvOdmALSHandle hDevice, NvU8 cmd)
{
    NvOdmI2cTransactionInfo TransactionInfo;

    if(NULL == hDevice)
    {
        return NV_FALSE;
    }
    
    s_WriteBuffer = cmd;

    TransactionInfo.Address = hDevice->nDevAddr;
    TransactionInfo.Buf = &s_WriteBuffer;
    TransactionInfo.Flags = NVODM_I2C_IS_WRITE;
    TransactionInfo.NumBytes = 1;

    if(NvOdmI2cStatus_Success != NvOdmI2cTransaction(hDevice->hI2c, &TransactionInfo, 1, 400, I2C_ALS_TRANSACTION_TIMEOUT))
    {
        return NV_FALSE;
    };    
        
    return NV_TRUE;
}

/*
 * Read I2C register function.
 * Returns NV_TRUE if successful, or NV_FALSE otherwise.
 */

NvBool NvAmbientsensorI2CGetLx(NvOdmALSHandle hDevice, NvU16* value)
{
    NvOdmI2cTransactionInfo TransactionInfo;
    NvU16 byte_swap;
    if( (NULL == hDevice) || (NULL == value))
    {
        return NV_FALSE;
    }
    
    NvOdmOsMemset(s_ReadBuffer, 0, sizeof(s_ReadBuffer));    

    TransactionInfo.Address = (hDevice->nDevAddr| 0x1);
    TransactionInfo.Buf = s_ReadBuffer;
    TransactionInfo.Flags = 0;
    TransactionInfo.NumBytes = I2C_ALS_READ_PACKET_SIZE;

    //Read the data from the eeprom at the specified offset
    if(NvOdmI2cStatus_Success != NvOdmI2cTransaction(hDevice->hI2c, &TransactionInfo, 1, 400, I2C_ALS_TRANSACTION_TIMEOUT))
    {
        return NV_FALSE;
    };    

    byte_swap = s_ReadBuffer[0];
    s_ReadBuffer[0] = s_ReadBuffer[1];
    s_ReadBuffer[1] = byte_swap;

    *value = *( NvU16 *)s_ReadBuffer;
    return NV_TRUE;
}


enum { 
    ALS_RESEPIN_ACTIVE_LOW =0,
    ALS_RESEPIN_DEACTIVE_HIGH,
    ALS_PWDOWN,
    ALS_PWON,
    ALS_AUTO_RES_MODE,
    ALS_HIGH_RES_MODE,
    ALS_LOW_RES_MODE,
    ALS_LUX_UPDATA
};

#define CMD_ALS_PWDOWN 0x0
#define CMD_ALS_PWON 0x01
#define CMD_ALS_AUTO_RES_MODE 0x10
#define CMD_ALS_HIGH_RES_MODE 0x12
#define CMD_ALS_LOW_RES_MODE 0x13


static ssize_t als_control_show(struct device *dev, 
		struct device_attribute *attr, char *buf)
{
	NvU32 pinValue;
	NvOdmGpioGetState(s_hALSHandle.hGpio, s_hALSHandle.s_hResetALSGpioPin, &pinValue); 

	return sprintf(buf, "%d\n", pinValue);
}

static ssize_t als_control_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{

    NvBool status = NV_FALSE;
    int cmd =0, arg=0;
	printk(KERN_ERR "als_control_store\n");
	sscanf(buf, "%d %d", &cmd, &arg);

    switch(cmd)
    {
        case ALS_PWDOWN:
            status = s_hALSHandle.CMDWrite(&s_hALSHandle, CMD_ALS_PWDOWN);
            break;
        case ALS_PWON:
            status = s_hALSHandle.CMDWrite(&s_hALSHandle, CMD_ALS_PWON);
            break;
        case ALS_RESEPIN_ACTIVE_LOW:
            NvOdmGpioSetState(s_hALSHandle.hGpio, s_hALSHandle.s_hResetALSGpioPin, NvRmGpioPinState_Low);
            status = NV_TRUE;
            break;
        case ALS_RESEPIN_DEACTIVE_HIGH:
            NvOdmGpioSetState(s_hALSHandle.hGpio, s_hALSHandle.s_hResetALSGpioPin, NvRmGpioPinState_High);
            status = NV_TRUE;
            break;
        case ALS_AUTO_RES_MODE:
            status = s_hALSHandle.CMDWrite(&s_hALSHandle, CMD_ALS_AUTO_RES_MODE);
            break;            
        case ALS_HIGH_RES_MODE:
            status = s_hALSHandle.CMDWrite(&s_hALSHandle, CMD_ALS_HIGH_RES_MODE);
            break;

        case ALS_LOW_RES_MODE:
            status = s_hALSHandle.CMDWrite(&s_hALSHandle, ALS_LOW_RES_MODE);
            break;
        case ALS_LUX_UPDATA:
//            s_hALSHandle.LUXRead(&s_hALSHandle, &s_lux);
            break;
        default:
            break;
    }
    printk(KERN_ERR "get cmd = %d\n",cmd);
    if (status == NV_FALSE)
        printk(KERN_ERR "als_control_store error\n");
    
	return size;
}


static ssize_t als_data_show(struct device *dev, 
		struct device_attribute *attr, char *buf)
{
    NvBool status=NV_FALSE;
    NvU16 lux = 255; //test



//    s_hALSHandle.CMDWrite(&s_hALSHandle, ALS_HIGH_RES_MODE);
//    mdelay(180);
    status = s_hALSHandle.LUXRead(&s_hALSHandle, &lux);

    if (status == NV_FALSE)
    {
      printk(KERN_ERR "als_data_show error");
      lux =0;
    }
    return sprintf(buf, "%d\n", lux);
}

static ssize_t als_data_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{

	printk(KERN_ERR "als_data_store\n");
	//sscanf(buf, "%d %d", &cmd, &arg);
	return size;
}

// 2 files : 1 for control, 1 for data

static DEVICE_ATTR(control, S_IRUGO | S_IWUSR, als_control_show, als_control_store);
static DEVICE_ATTR(data, S_IRUGO | S_IWUSR, als_data_show, als_data_store);


static int tegra_als_probe(struct platform_device *pdev)
{

    // gpio control
    // i2c handle create
    // attr file create

//	struct input_dev *input_dev = NULL;

    
	int retval;
	const NvOdmPeripheralConnectivity *pConnectivity = NULL;
  	int i;
	
	printk(KERN_ERR "tegra_als_reset_probe\n");


	pConnectivity = NvOdmPeripheralGetGuid(ALS_GUID);

	if (!pConnectivity)
	{
		printk("pConnectivity fail.\n");
		return 0;
	}
	
	if (NULL == (s_hALSHandle.hGpio = NvOdmGpioOpen()))
	{
		printk("gpio open fail.\n");
		return 0;

	}

	for (i = 0; i < pConnectivity->NumAddress; i++)
	{

		switch(pConnectivity->AddressList[i].Interface)
        {
            case NvOdmIoModule_Gpio:
	    		s_hALSHandle.resetport = pConnectivity->AddressList[i].Instance;
		    	s_hALSHandle.resetpin = pConnectivity->AddressList[i].Address;
			    printk("resetport = %d  resetpin = %d\n",s_hALSHandle.resetport,s_hALSHandle.resetpin);
                //TODO : check error
                break;

            case NvOdmIoModule_I2c:
                s_hALSHandle.I2CChannelId = pConnectivity->AddressList[i].Instance;
                s_hALSHandle.nDevAddr = (NvU8)pConnectivity->AddressList[i].Address;
  			    printk("I2CChannelId = %d  nDevAddr = %d\n",s_hALSHandle.I2CChannelId,s_hALSHandle.nDevAddr);
                //TODO : check error
                break;

            default:
                break;
		}
	}

	s_hALSHandle.s_hResetALSGpioPin = NvOdmGpioAcquirePinHandle(s_hALSHandle.hGpio, s_hALSHandle.resetport, s_hALSHandle.resetpin);
	NvOdmGpioConfig(s_hALSHandle.hGpio, s_hALSHandle.s_hResetALSGpioPin, NvOdmGpioPinMode_Output);
	NvOdmGpioSetState(s_hALSHandle.hGpio, s_hALSHandle.s_hResetALSGpioPin, NvRmGpioPinState_High);

    if(NV_FALSE == NvAmbientsensorI2COpen(&s_hALSHandle.hI2c, s_hALSHandle.I2CChannelId))
    {
        printk("i2c open error\n");
        goto error;
    };
    s_hALSHandle.LUXRead = NvAmbientsensorI2CGetLx;
    s_hALSHandle.CMDWrite = NvAmbientsensorI2CSetCmd;

	retval = device_create_file(&pdev->dev, &dev_attr_control);
	if (retval)
	{
        printk("control file create -error \n");
        goto error;
	}
	retval = device_create_file(&pdev->dev, &dev_attr_data);
	if (retval)
	{
        printk("data file create -error \n");
		goto error;
	}



    //jhchun test : enable als
    NvOdmGpioSetState(s_hALSHandle.hGpio, s_hALSHandle.s_hResetALSGpioPin, NvRmGpioPinState_Low);
    NvOdmGpioSetState(s_hALSHandle.hGpio, s_hALSHandle.s_hResetALSGpioPin, NvRmGpioPinState_High);
    s_hALSHandle.CMDWrite(&s_hALSHandle, CMD_ALS_PWON);
    s_hALSHandle.CMDWrite(&s_hALSHandle, CMD_ALS_HIGH_RES_MODE);
    return retval;

/*
    input_dev = input_allocate_device();

    if (input_dev == NULL) {
        err = -ENOMEM;
        pr_err("tegra_als_probe: Failed to allocate input device\n");
        goto allocate_dev_fail;
    }
    s_hALSHandle->input_dev = input_dev;
    set_bit(EV_ABS, s_hALSHandle->input_dev->evbit);
    input_set_abs_params(compass_dev->input_dev, ABS_PRESSURE, 0, 65535, 0, 0);
	s_hALSHandle->input_dev->name = "ALS";
	err = input_register_device(input_dev);
	if (err) {
		pr_err("tegra_als_probe: Unable to register %s\
				input device\n", input_dev->name);
		goto exit_input_register_device_failed;
	}

exit_input_register_device_failed:
	input_free_device(compass_dev->input_dev);
allocate_dev_fail:
	input_free_device(input_dev);
*/
error:
	printk(KERN_ERR "tegra_als_probe -Error\n");
    NvOdmGpioClose(s_hALSHandle.hGpio);
	device_remove_file(&pdev->dev, &dev_attr_control);
	device_remove_file(&pdev->dev, &dev_attr_data);
	
	return 0;
}



static int tegra_als_remove(struct platform_device *pdev)
{
	printk(KERN_ERR "tegra_als_remove\n");
	device_remove_file(&pdev->dev, &dev_attr_control);
	device_remove_file(&pdev->dev, &dev_attr_data);
	return 0;
}



// platform_driver
static struct platform_driver tegra_als_driver = {
    .probe      = tegra_als_probe,
    .remove     = tegra_als_remove,
    .driver     = {
        .name   = "tegra_als"
    },
};



static int __devinit tegra_als_init(void)
{
	printk(KERN_ERR "tegra_als_init\n");
    return platform_driver_register(&tegra_als_driver);
}

static void __exit tegra_als_exit(void)
{
	printk(KERN_ERR "tegra_als_exit\n");
    platform_driver_unregister(&tegra_als_driver);
}

module_init(tegra_als_init);
module_exit(tegra_als_exit);

MODULE_DESCRIPTION("Tegra ALS Driver");

