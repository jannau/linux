/*
 * tegra_soc.h  --  SoC audio for tegra
 *
 * (c) 2010 Nvidia Graphics Pvt. Ltd.
 *  http://www.nvidia.com
 *
 * Copyright 2007 Wolfson Microelectronics PLC.
 * Author: Graeme Gregory
 *         graeme.gregory@wolfsonmicro.com or linux@wolfsonmicro.com
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 */

#ifndef __TEGRA_SOC_H
#define __TEGRA_SOC_H

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/jiffies.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/dma-mapping.h>
#include <linux/kthread.h>
#include <linux/moduleparam.h>
#include <linux/timer.h>
#include <linux/interrupt.h>
#include <linux/i2c.h>
#include <linux/tegra_audio.h>
#include <mach/iomap.h>
#include <mach/tegra2_i2s.h>
#include <mach/irqs.h>
#include <mach/pinmux.h>
#include <mach/audio.h>
#include <mach/tegra_das.h>
#include <mach/dma.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/initval.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <sound/soc-dai.h>
#include <sound/tlv.h>
#include <asm/mach-types.h>
#include <asm/hardware/scoop.h>

#define STATE_INIT	0
#define STATE_ABORT	1
#define STATE_EXIT	2
#define STATE_EXITED	3
#define STATE_INVALID	4

#define I2S_I2S_FIFO_TX_BUSY	I2S_I2S_STATUS_FIFO1_BSY
#define I2S_I2S_FIFO_TX_QS	I2S_I2S_STATUS_QS_FIFO1
#define I2S_I2S_FIFO_RX_BUSY	I2S_I2S_STATUS_FIFO2_BSY
#define I2S_I2S_FIFO_RX_QS	I2S_I2S_STATUS_QS_FIFO2

//startablet: audio enable :  wm8994 codec i2s clock setting
#define I2S_CLK 		2822400
//#define I2S_CLK 		11289600
#define TEGRA_DEFAULT_SR	44100

#define DAP_MCLK_1		11289600
#define DAP_MCLK_1_PARENT 	56448000
#define DAP_MCLK_2 		12288000
#define DAP_MCLK_2_PARENT 	73728000


#define TEGRA_I2S_MCLK	0
#define TEGRA_I2S_BCLK	1

#define TEGRA_SAMPLE_RATES \
	(SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_32000 | SNDRV_PCM_RATE_44100 | \
	SNDRV_PCM_RATE_48000 | SNDRV_PCM_RATE_88200 | SNDRV_PCM_RATE_96000)
#define TEGRA_VOICE_SAMPLE_RATES SNDRV_PCM_RATE_8000

#define DMA_STEP_SIZE_MIN 8

struct tegra_dma_channel;

struct tegra_runtime_data {
	struct snd_pcm_substream *substream;
	int size;
	int dma_pos;
	struct tegra_dma_req dma_req1, dma_req2;
	volatile int state;
	int period_index;
	int i2s_shutdown;
	int dma_state;
	struct tegra_dma_channel *dma_chan;
};

struct tegra_audio_data {
	struct snd_soc_codec *codec;
	int play_device;
	int capture_device;
	bool is_call_mode;
};

/* i2s controller */
struct tegra_i2s_info {
	struct platform_device *pdev;
	struct tegra_audio_platform_data *pdata;
	struct clk *i2s_clk;
	struct clk *dap_mclk;
	struct clk *dap_clk;
	struct clk *audio_sync_clk;
	phys_addr_t i2s_phys;
	void __iomem *i2s_base;

	unsigned long dma_req_sel;

	int irq;
	unsigned int bit_fmt;
	unsigned int bit_size;
	unsigned int fifo_fmt;
	struct i2s_runtime_data i2s_regs;
	struct das_regs_cache das_regs;
};

int tegra_controls_init(struct snd_soc_codec *codec);
void tegra_controls_exit(void);

//int tegra_jack_init(struct snd_soc_codec *codec);
//void tegra_jack_exit(void);
void tegra_switch_set_state(int state);

void setup_dma_request(struct snd_pcm_substream *substream,
			struct tegra_dma_req *req,
			void (*dma_callback)(struct tegra_dma_req *req),
			void *dma_data);

#endif /* __TEGRA_SOC_H */