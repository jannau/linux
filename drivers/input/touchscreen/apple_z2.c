// SPDX-License-Identifier: GPL-2.0
/*
 * Apple Z2 touchscreen driver
 *
 * Copyright (C) The Asahi Linux Contributors
 */

#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/firmware.h>
#include <linux/input.h>
#include <linux/input/mt.h>

#define Z2_NUM_FINGERS_OFFSET 16
#define Z2_FINGERS_OFFSET 24
#define Z2_TOUCH_STARTED 3
#define Z2_TOUCH_MOVED 4
#define Z2_CMD_READ_INTERRUPT_DATA 0xEB
#define Z2_HBPP_CMD_BLOB 0x3001
#define Z2_FW_MAGIC 0x5746325A
#define LOAD_COMMAND_INIT_PAYLOAD 0
#define LOAD_COMMAND_SEND_BLOB 1
#define LOAD_COMMAND_SEND_CALIBRATION 2

struct apple_z2 {
	struct spi_device *spidev;
	struct gpio_desc *reset_gpio;
	struct input_dev *input_dev;
	struct completion boot_irq;
	int booted;
	int counter;
	int y_size;
	const char *fw_name;
	const char *cal_blob;
	int cal_size;
};

struct z2_finger {
	u8 finger;
	u8 state;
	__le16 unknown2;
	__le16 abs_x;
	__le16 abs_y;
	__le16 rel_x;
	__le16 rel_y;
	__le16 tool_major;
	__le16 tool_minor;
	__le16 orientation;
	__le16 touch_major;
	__le16 touch_minor;
	__le16 unused[2];
	__le16 pressure;
	__le16 multi;
} __packed;

struct z2_hbpp_blob_hdr {
	u16 cmd;
	u16 len;
	u32 addr;
	u16 checksum;
} __packed;

struct z2_fw_hdr {
	u32 magic;
	u32 version;
} __packed;

struct z2_read_interrupt_cmd {
	u8 cmd;
	u8 counter;
	u8 unused[12];
	__le16 checksum;
} __packed;

static void apple_z2_parse_touches(struct apple_z2 *z2, char *msg, size_t msg_len)
{
	int i;
	int nfingers;
	int slot;
	int slot_valid;
	struct z2_finger *fingers;

	if (msg_len <= Z2_NUM_FINGERS_OFFSET)
		return;
	nfingers = msg[Z2_NUM_FINGERS_OFFSET];
	fingers = (struct z2_finger *)(msg + Z2_FINGERS_OFFSET);
	for (i = 0; i < nfingers; i++) {
		slot = input_mt_get_slot_by_key(z2->input_dev, fingers[i].finger);
		if (slot < 0) {
			dev_warn(&z2->spidev->dev, "unable to get slot for finger");
			continue;
		}
		slot_valid = fingers[i].state == Z2_TOUCH_STARTED ||
			fingers[i].state == Z2_TOUCH_MOVED;
		input_mt_slot(z2->input_dev, slot);
		input_mt_report_slot_state(z2->input_dev, MT_TOOL_FINGER, slot_valid);
		if (!slot_valid)
			continue;
		input_report_abs(z2->input_dev, ABS_MT_POSITION_X,
				 le16_to_cpu(fingers[i].abs_x));
		input_report_abs(z2->input_dev, ABS_MT_POSITION_Y,
				 z2->y_size - le16_to_cpu(fingers[i].abs_y));
		input_report_abs(z2->input_dev, ABS_MT_WIDTH_MAJOR,
				 le16_to_cpu(fingers[i].tool_major));
		input_report_abs(z2->input_dev, ABS_MT_WIDTH_MINOR,
				 le16_to_cpu(fingers[i].tool_minor));
		input_report_abs(z2->input_dev, ABS_MT_ORIENTATION,
				 le16_to_cpu(fingers[i].orientation));
		input_report_abs(z2->input_dev, ABS_MT_TOUCH_MAJOR,
				 le16_to_cpu(fingers[i].touch_major));
		input_report_abs(z2->input_dev, ABS_MT_TOUCH_MINOR,
				 le16_to_cpu(fingers[i].touch_minor));
	}
	input_mt_sync_frame(z2->input_dev);
	input_sync(z2->input_dev);
}

static int apple_z2_read_packet(struct apple_z2 *z2)
{
	struct spi_message msg;
	struct spi_transfer xfer;
	struct z2_read_interrupt_cmd len_cmd;
	char len_rx[16];
	size_t pkt_len;
	char *pkt_rx;
	int err = 0;

	spi_message_init(&msg);
	memset(&xfer, 0, sizeof(xfer));
	memset(&len_cmd, 0, sizeof(len_cmd));
	len_cmd.cmd = Z2_CMD_READ_INTERRUPT_DATA;
	len_cmd.counter = z2->counter + 1;
	len_cmd.checksum = cpu_to_le16(Z2_CMD_READ_INTERRUPT_DATA + 1 + z2->counter);
	z2->counter = 1 - z2->counter;
	xfer.tx_buf = &len_cmd;
	xfer.rx_buf = len_rx;
	xfer.len = sizeof(len_cmd);
	spi_message_add_tail(&xfer, &msg);
	err = spi_sync(z2->spidev, &msg);
	if (err)
		return err;

	pkt_len = ((len_rx[1] | len_rx[2] << 8) + 8) & (-4);
	pkt_rx = kzalloc(pkt_len, GFP_KERNEL);
	if (!pkt_rx)
		return -ENOMEM;

	spi_message_init(&msg);
	xfer.rx_buf = pkt_rx;
	xfer.len = pkt_len;
	spi_message_add_tail(&xfer, &msg);
	err = spi_sync(z2->spidev, &msg);

	if (!err)
		apple_z2_parse_touches(z2, pkt_rx + 5, pkt_len - 5);

	kfree(pkt_rx);
	return err;
}

static irqreturn_t apple_z2_irq(int irq, void *data)
{
	struct spi_device *spi = data;
	struct apple_z2 *z2 = spi_get_drvdata(spi);

	if (!z2->booted)
		complete(&z2->boot_irq);
	else
		apple_z2_read_packet(z2);

	return IRQ_HANDLED;
}


// Return value must be freed with kfree
static char *apple_z2_build_cal_blob(struct apple_z2 *z2, u32 address, u32 *size)
{
	u16 len_words = (z2->cal_size + 3) / 4;
	u32 checksum = 0;
	u16 checksum_hdr = 0;
	char *data;
	int i;
	struct z2_hbpp_blob_hdr *hdr;

	*size = z2->cal_size + sizeof(struct z2_hbpp_blob_hdr) + 4;

	data = kzalloc(*size, GFP_KERNEL);
	hdr = (struct z2_hbpp_blob_hdr *)data;
	hdr->cmd = Z2_HBPP_CMD_BLOB;
	hdr->len = len_words;
	hdr->addr = address;

	for (i = 2; i < 8; i++)
		checksum_hdr += data[i];

	hdr->checksum = checksum_hdr;
	memcpy(data + 10, z2->cal_blob, z2->cal_size);

	for (i = 0; i < z2->cal_size; i++)
		checksum += z2->cal_blob[i];

	*(u32 *)(data + z2->cal_size + 10) = checksum;

	return data;
}

static int apple_z2_send_firmware_blob(struct apple_z2 *z2, const char *data, u32 size, u8 bpw)
{
	struct spi_message msg;
	struct spi_transfer blob_xfer, ack_xfer;
	char int_ack[] = {0x1a, 0xa1};
	char ack_rsp[] = {0, 0};
	int err;

	spi_message_init(&msg);
	memset(&blob_xfer, 0, sizeof(blob_xfer));
	memset(&ack_xfer, 0, sizeof(ack_xfer));
	blob_xfer.tx_buf = data;
	blob_xfer.len = size;
	blob_xfer.bits_per_word = bpw;
	spi_message_add_tail(&blob_xfer, &msg);
	ack_xfer.tx_buf = int_ack;
	ack_xfer.rx_buf = ack_rsp;
	ack_xfer.len = 2;
	spi_message_add_tail(&ack_xfer, &msg);
	reinit_completion(&z2->boot_irq);
	err = spi_sync(z2->spidev, &msg);
	if (err)
		return err;
	wait_for_completion_timeout(&z2->boot_irq, msecs_to_jiffies(20));
	return 0;
}

static int apple_z2_upload_firmware(struct apple_z2 *z2)
{
	const struct firmware *fw;
	struct z2_fw_hdr *fw_hdr;
	size_t fw_idx = sizeof(struct z2_fw_hdr);
	int err;
	u32 load_cmd;
	u32 size;
	u32 address;
	char *data;

	err = request_firmware(&fw, z2->fw_name, &z2->spidev->dev);
	if (err)
		return dev_err_probe(&z2->spidev->dev, err, "unable to load firmware");

	fw_hdr = (struct z2_fw_hdr *)fw->data;
	if (fw_hdr->magic != Z2_FW_MAGIC || fw_hdr->version != 1)
		return dev_err_probe(&z2->spidev->dev, -EINVAL, "invalid firmware header");

	while (fw_idx < fw->size) {
		if (fw->size - fw_idx < 8) {
			err = dev_err_probe(&z2->spidev->dev, -EINVAL, "firmware malformed");
			goto error;
		}

		load_cmd = *(u32 *)(fw->data + fw_idx);
		fw_idx += 4;
		if (load_cmd == LOAD_COMMAND_INIT_PAYLOAD || load_cmd == LOAD_COMMAND_SEND_BLOB) {
			size = *(u32 *)(fw->data + fw_idx);
			fw_idx += 4;
			if (fw->size - fw_idx < size) {
				err = dev_err_probe(&z2->spidev->dev,
						     -EINVAL, "firmware malformed");
				goto error;
			}
			err = apple_z2_send_firmware_blob(z2, fw->data + fw_idx,
							  size, load_cmd == LOAD_COMMAND_SEND_BLOB ? 16 : 8);
			if (err)
				goto error;
			fw_idx += size;
		} else if (load_cmd == 2) {
			address = *(u32 *)(fw->data + fw_idx);
			fw_idx += 4;
			data = apple_z2_build_cal_blob(z2, address, &size);
			err = apple_z2_send_firmware_blob(z2, data, size, 16);
			kfree(data);
			if (err)
				goto error;
		} else {
			err = dev_err_probe(&z2->spidev->dev, -EINVAL, "firmware malformed");
			goto error;
		}
		if (fw_idx % 4 != 0)
			fw_idx += 4 - (fw_idx % 4);
	}


	z2->booted = 1;
	apple_z2_read_packet(z2);
 error:
	release_firmware(fw);
	return err;
}

static int apple_z2_boot(struct apple_z2 *z2)
{
	int err;
	enable_irq(z2->spidev->irq);
	gpiod_direction_output(z2->reset_gpio, 1);
	wait_for_completion_timeout(&z2->boot_irq, msecs_to_jiffies(20));
	err = apple_z2_upload_firmware(z2);
	if (err) { // Boot failed, let's put the device into reset just in case.
		gpiod_direction_output(z2->reset_gpio, 0);
		disable_irq(z2->spidev->irq);
	}
	return err;
}

static int apple_z2_probe(struct spi_device *spi)
{
	struct device *dev = &spi->dev;
	struct apple_z2 *z2;
	int err;
	int x_size;
	const char *device_name;

	z2 = devm_kzalloc(dev, sizeof(*z2), GFP_KERNEL);
	if (!z2)
		return -ENOMEM;

	z2->spidev = spi;
	init_completion(&z2->boot_irq);
	spi_set_drvdata(spi, z2);

	z2->reset_gpio = devm_gpiod_get_index(dev, "reset", 0, 0);
	if (IS_ERR(z2->reset_gpio))
		return dev_err_probe(dev, PTR_ERR(z2->reset_gpio), "unable to get reset");

	err = devm_request_threaded_irq(dev, z2->spidev->irq, NULL,
					apple_z2_irq, IRQF_ONESHOT | IRQF_NO_AUTOEN,
					"apple-z2-irq", spi);
	if (err < 0)
		return dev_err_probe(dev, z2->spidev->irq, "unable to request irq");

	err = device_property_read_u32(dev, "touchscreen-size-x", &x_size);
	if (err)
		return dev_err_probe(dev, err, "unable to get touchscreen size");

	err = device_property_read_u32(dev, "touchscreen-size-y", &z2->y_size);
	if (err)
		return dev_err_probe(dev, err, "unable to get touchscreen size");

	err = device_property_read_string(dev, "apple,z2-device-name", &device_name);
	if (err)
		return dev_err_probe(dev, err, "unable to get device name");

	err = device_property_read_string(dev, "firmware-name", &z2->fw_name);
	if (err)
		return dev_err_probe(dev, err, "unable to get firmware name");

	z2->cal_blob = of_get_property(dev->of_node, "apple,z2-cal-blob", &z2->cal_size);
	if (!z2->cal_blob)
		return dev_err_probe(dev, -EINVAL, "unable to get calibration");

	z2->input_dev = devm_input_allocate_device(dev);
	if (!z2->input_dev)
		return -ENOMEM;
	z2->input_dev->name = device_name;
	z2->input_dev->phys = "apple_z2";
	z2->input_dev->dev.parent = dev;
	z2->input_dev->id.bustype = BUS_SPI;
	input_set_abs_params(z2->input_dev, ABS_MT_POSITION_X, 0, x_size, 0, 0);
	input_abs_set_res(z2->input_dev, ABS_MT_POSITION_X, 1);
	input_set_abs_params(z2->input_dev, ABS_MT_POSITION_Y, 0, z2->y_size, 0, 0);
	input_abs_set_res(z2->input_dev, ABS_MT_POSITION_Y, 1);
	input_set_abs_params(z2->input_dev, ABS_MT_WIDTH_MAJOR, 0, 65535, 0, 0);
	input_set_abs_params(z2->input_dev, ABS_MT_WIDTH_MINOR, 0, 65535, 0, 0);
	input_set_abs_params(z2->input_dev, ABS_MT_TOUCH_MAJOR, 0, 65535, 0, 0);
	input_set_abs_params(z2->input_dev, ABS_MT_TOUCH_MINOR, 0, 65535, 0, 0);
	input_set_abs_params(z2->input_dev, ABS_MT_ORIENTATION, -32768, 32767, 0, 0);
	input_mt_init_slots(z2->input_dev, 256, INPUT_MT_DIRECT);

	err = input_register_device(z2->input_dev);
	if (err < 0)
		return dev_err_probe(dev, err, "unable to register input device");


	// Reset the device on probe
	gpiod_direction_output(z2->reset_gpio, 0);
	usleep_range(5000, 10000);
	return apple_z2_boot(z2);
}

static void apple_z2_remove(struct spi_device *spi)
{
	struct apple_z2 *z2 = spi_get_drvdata(spi);

	disable_irq(z2->spidev->irq);
	gpiod_direction_output(z2->reset_gpio, 0);
}

static void apple_z2_shutdown(struct spi_device *spi)
{
	struct apple_z2 *z2 = spi_get_drvdata(spi);

	disable_irq(z2->spidev->irq);
	gpiod_direction_output(z2->reset_gpio, 0);
}

static int apple_z2_suspend(struct device *dev)
{
	apple_z2_shutdown(to_spi_device(dev));
	return 0;
}

static int apple_z2_resume(struct device *dev)
{
	struct apple_z2 *z2 = spi_get_drvdata(to_spi_device(dev));

	return apple_z2_boot(z2);
}

static DEFINE_SIMPLE_DEV_PM_OPS(apple_z2_pm, apple_z2_suspend, apple_z2_resume);

static const struct of_device_id apple_z2_of_match[] = {
	{ .compatible = "apple,z2-touchscreen" },
	{},
};
MODULE_DEVICE_TABLE(of, apple_z2_of_match);

static struct spi_device_id apple_z2_of_id[] = {
	{ .name = "z2-touchscreen" },
	{}
};
MODULE_DEVICE_TABLE(spi, apple_z2_of_id);

static struct spi_driver apple_z2_driver = {
	.driver = {
		.name	= "apple-z2",
		.pm	= pm_sleep_ptr(&apple_z2_pm),
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(apple_z2_of_match),
	},

	.id_table       = apple_z2_of_id,
	.probe		= apple_z2_probe,
	.remove		= apple_z2_remove,
	.shutdown	= apple_z2_shutdown,
};

module_spi_driver(apple_z2_driver);

MODULE_LICENSE("GPL");
MODULE_FIRMWARE("apple/mtfw-*.bin");
