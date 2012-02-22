/*
 * tegra_soc_star.c  --  Tegra SoC audio for LGE Star Tablet
 *
 * (c) 2011 Nvidia Graphics Pvt. Ltd.
 *
 * Author: Jinyoung Park
 *	jinyoungp@nvidia.com
 * http://www.nvidia.com
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA	02110-1301, USA.
 */

#include "../codecs/wm8994.h"
#include "tegra_soc.h"
#include <mach/audio.h>
#include <linux/mfd/wm8994/registers.h>
#include "../../../drivers/startablet/headset_det.h"

extern struct snd_soc_dai tegra_i2s_dai[];
extern struct snd_soc_platform tegra_soc_platform;

static struct platform_device *tegra_snd_device;
static struct snd_soc_jack hs_jack;
static struct snd_soc_card tegra_snd_soc;

static struct tegra_audio_data *audio_data;
static int tegra_jack_func;
static int tegra_spk_func;
static int tegra_das_func;

#define TEGRA_HP		0
#define TEGRA_MIC		1
#define TEGRA_LINE		2
#define TEGRA_HEADSET		3
#define TEGRA_HP_OFF		4
#define TEGRA_ALL_ON		5

#define TEGRA_SPK_ON		0
#define TEGRA_SPK_OFF		1

#define TEGRA_DAS_HIFI		0
#define TEGRA_DAS_BT_SCO	1

/* Headset jack detection DAPM pins */
static struct snd_soc_jack_pin hs_jack_pins[] = {
	{
		.pin	= "Mic Jack",
		.mask	= SND_JACK_MICROPHONE,
	},
	{
		.pin	= "Headphone Jack",
		.mask	= SND_JACK_HEADPHONE,
	},
};

static void tegra_ext_control(struct snd_soc_codec *codec)
{
	/* set up jack connection */
	switch (tegra_jack_func) {
	case TEGRA_HP:
		/* set = unmute headphone */
		snd_soc_dapm_disable_pin(codec, "Mic Jack");
		snd_soc_dapm_disable_pin(codec, "Line Jack");
		snd_soc_dapm_enable_pin(codec, "Headphone Jack");
		snd_soc_dapm_disable_pin(codec, "Headset Jack");
		break;

	case TEGRA_MIC:
		/* reset = mute headphone */
		snd_soc_dapm_enable_pin(codec, "Mic Jack");
		snd_soc_dapm_disable_pin(codec, "Line Jack");
		snd_soc_dapm_disable_pin(codec, "Headphone Jack");
		snd_soc_dapm_disable_pin(codec, "Headset Jack");
		break;

	case TEGRA_LINE:
		snd_soc_dapm_disable_pin(codec, "Mic Jack");
		snd_soc_dapm_enable_pin(codec, "Line Jack");
		snd_soc_dapm_disable_pin(codec, "Headphone Jack");
		snd_soc_dapm_disable_pin(codec, "Headset Jack");
		break;

	case TEGRA_HEADSET:
		snd_soc_dapm_disable_pin(codec, "Mic Jack");
		snd_soc_dapm_enable_pin(codec, "Line Jack");
		snd_soc_dapm_enable_pin(codec, "Headphone Jack");
		snd_soc_dapm_disable_pin(codec, "Headset Jack");
		break;

	case TEGRA_ALL_ON:
		snd_soc_dapm_enable_pin(codec, "Mic Jack");
		snd_soc_dapm_enable_pin(codec, "Line Jack");
		snd_soc_dapm_enable_pin(codec, "Headphone Jack");
		snd_soc_dapm_enable_pin(codec, "Headset Jack");
		break;
	}

	if (tegra_spk_func == TEGRA_SPK_ON)
		snd_soc_dapm_enable_pin(codec, "Ext Spk");
	else
		snd_soc_dapm_disable_pin(codec, "Ext Spk");
	/* signal a DAPM event */
	snd_soc_dapm_sync(codec);
}

static int tegra_get_jack(struct snd_kcontrol *kcontrol,
				struct snd_ctl_elem_value *ucontrol)
{
	ucontrol->value.integer.value[0] = tegra_jack_func;
	return 0;
}

static int tegra_set_jack(struct snd_kcontrol *kcontrol,
				struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);

	if (tegra_jack_func == ucontrol->value.integer.value[0])
		return 0;

	tegra_jack_func = ucontrol->value.integer.value[0];
	tegra_ext_control(codec);
	return 1;
}

static int tegra_get_spk(struct snd_kcontrol *kcontrol,
				struct snd_ctl_elem_value *ucontrol)
{
	ucontrol->value.integer.value[0] = tegra_spk_func;
	return 0;
}

static int tegra_set_spk(struct snd_kcontrol *kcontrol,
				struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec =  snd_kcontrol_chip(kcontrol);

	if (tegra_spk_func == ucontrol->value.integer.value[0])
		return 0;

	tegra_spk_func = ucontrol->value.integer.value[0];
	tegra_ext_control(codec);
	return 1;
}

static int tegra_get_das(struct snd_kcontrol *kcontrol,
				struct snd_ctl_elem_value *ucontrol)
{
	ucontrol->value.integer.value[0] = tegra_das_func;
	return 0;
}

static int tegra_set_das(struct snd_kcontrol *kcontrol,
				struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec =  snd_kcontrol_chip(kcontrol);

	if (tegra_das_func == ucontrol->value.integer.value[0])
		return 0;

	tegra_das_func = ucontrol->value.integer.value[0];
	tegra_ext_control(codec);
	return 1;
}

/*tegra machine dapm widgets */
static const struct snd_soc_dapm_widget tegra_dapm_widgets[] = {
	SND_SOC_DAPM_HP("Headphone Jack", NULL),
	SND_SOC_DAPM_SPK("Ext Spk", NULL),
	SND_SOC_DAPM_MIC("Mic Jack", NULL),
	SND_SOC_DAPM_LINE("Line Jack", NULL),
	SND_SOC_DAPM_HP("Headset Jack", NULL),
};

/* Tegra machine audio map (connections to the codec pins) */
static const struct snd_soc_dapm_route audio_map[] = {
	/* headphone connected to HPOUT1L and HPOUT1R */
	{"Headphone Jack", NULL, "HPOUT1L"},
	{"Headphone Jack", NULL, "HPOUT1R"},

	/* speaker conntected to SPKOUTLN, SPKOUTLP, SPKOUTRN and SPKOUTRP */
	{"Ext Spk", NULL, "SPKOUTLN"},
	{"Ext Spk", NULL, "SPKOUTLP"},
	{"Ext Spk", NULL, "SPKOUTRN"},
	{"Ext Spk", NULL, "SPKOUTRP"},

	/* main mic is connected to INRN */
	{"MICBIAS1", NULL, "Mic Jack"},
	{"IN1RN", NULL, "MICBIAS1"},

	/* headset mic is connected to IN1LN */
	{"IN1LN", NULL, "Line Jack"},
};

static const char *jack_function[] = {"Headphone", "Mic", "Line", "Headset",
					"Off", "On"};
static const char *spk_function[] = {"On", "Off"};
static const char *das_function[] = {"HiFi", "Bluetooth"};
static const struct soc_enum tegra_enum[] = {
	SOC_ENUM_SINGLE_EXT(6, jack_function),
	SOC_ENUM_SINGLE_EXT(2, spk_function),
	SOC_ENUM_SINGLE_EXT(2, das_function),
};

static const struct snd_kcontrol_new tegra_controls[] = {
	SOC_ENUM_EXT("Jack Function", tegra_enum[0], tegra_get_jack,
			tegra_set_jack),
	SOC_ENUM_EXT("Speaker Function", tegra_enum[1], tegra_get_spk,
			tegra_set_spk),
	SOC_ENUM_EXT("Digital Audio Switch", tegra_enum[2], tegra_get_das,
			tegra_set_das),
};

static unsigned int tegra_i2s_get_mclk(struct snd_pcm_hw_params *params)
{
	int mclk = 0;

	switch (params_rate(params)) {
	case 96000:
	case 64000:
	case 48000:
	case 32000:
	case 16000:
	case 8000:
		mclk = 12288000;
		break;
	case 88200:
	case 44100:
	case 22050:
	case 11025:
		mclk = 11289600;
		break;
	default:
		pr_err("%s: unsupported sample rate: %d\n",
			__func__, params_rate(params));
		mclk = 0;
	}

	return mclk;
}

static unsigned int tegra_i2s_get_bclk(struct snd_pcm_hw_params *params)
{
	unsigned int bclk = 0, bit_size = 0;

	switch (params_format(params)) {
	case SNDRV_PCM_FORMAT_S16_LE:
		bit_size = 16;
		break;
	case SNDRV_PCM_FORMAT_S20_3LE:
		bit_size = 20;
		break;
	case SNDRV_PCM_FORMAT_S24_LE:
		bit_size = 24;
		break;
	case SNDRV_PCM_FORMAT_S32_LE:
		bit_size = 32;
		break;
	default:
		pr_err("%s: unsupported pcm format: %d\n",
			__func__, params_format(params));
		bclk = 0;
		goto out;
	}

	bclk = params_rate(params) * params_channels(params) * bit_size * 2;

out:
	return bclk;
}

static int tegra_codec_init(struct snd_soc_codec *codec)
{
	int ret = 0;

	if (!audio_data) {
		audio_data = kzalloc(sizeof(*audio_data), GFP_KERNEL);
		if (!audio_data) {
			pr_err("%s: failed to allocate tegra_audio_data\n",
				__func__);
			ret = -ENOMEM;
			goto out;
		}

		/* Add tegra specific controls */
		ret = snd_soc_add_controls(codec, tegra_controls,
					ARRAY_SIZE(tegra_controls));
		if (ret < 0) {
			pr_err("%s: failed to add snd controls\n", __func__);
			goto out_kfree;
		}

		/* Add tegra specific widgets */
		ret = snd_soc_dapm_new_controls(codec, tegra_dapm_widgets,
					ARRAY_SIZE(tegra_dapm_widgets));
		if (ret < 0) {
			pr_err("%s: failed to add new dapm controls\n",
				__func__);
			goto out_kfree;
		}

		/* Set up tegra specific audio path audio_map */
		ret = snd_soc_dapm_add_routes(codec, audio_map,
					ARRAY_SIZE(audio_map));
		if (ret < 0) {
			pr_err("%s: failed to add new dapm routes\n", __func__);
			goto out_kfree;
		}

		audio_data->codec = codec;

		/* Jack detection API stuff */
		ret = snd_soc_jack_new(&tegra_snd_soc, "Headset Jack",
					SND_JACK_HEADSET, &hs_jack);
		if (ret < 0) {
			pr_err("%s: failed to add new jack\n", __func__);
			goto out_kfree;
		}

		ret = snd_soc_jack_add_pins(&hs_jack, ARRAY_SIZE(hs_jack_pins),
					hs_jack_pins);
		if (ret < 0) {
			pr_err("%s: failed to add jack pins\n", __func__);
			goto out_kfree;
		}

		/* Default to HP output */
		tegra_jack_func = TEGRA_HP;
		tegra_spk_func = TEGRA_SPK_ON;
		tegra_das_func = TEGRA_DAS_HIFI;
		tegra_ext_control(codec);
		snd_soc_dapm_sync(codec);
	}

	goto out;

out_kfree:
	if (audio_data) {
		kfree(audio_data);
		audio_data = NULL;
	}
out:
	return ret;
}

static int tegra_hifi_hw_params(struct snd_pcm_substream *substream,
					struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *codec_dai = rtd->dai->codec_dai;
	struct snd_soc_dai *cpu_dai = rtd->dai->cpu_dai;
	struct tegra_i2s_info *info = cpu_dai->private_data;
	int ret = 0;

	tegra_das_set_connection(tegra_das_port_con_id_hifi);
	i2s_set_fifo_format(cpu_dai->id, I2S_FIFO_PACKED);

	pr_info("%s: %s, format=%d, rate=%d, channels=%d\n",
		__func__, substream->stream ? "CAPTURE" : "PLAYBACK",
		params_format(params), params_rate(params), params_channels(params));

	ret = snd_soc_dai_set_fmt(cpu_dai, SND_SOC_DAIFMT_I2S |
				SND_SOC_DAIFMT_NB_NF | SND_SOC_DAIFMT_CBS_CFS);
	if (ret < 0) {
		pr_err("%s: failed to set cpu dai format\n", __func__);
		goto out;
	}

	ret = snd_soc_dai_set_fmt(codec_dai, SND_SOC_DAIFMT_I2S |
				SND_SOC_DAIFMT_NB_NF | SND_SOC_DAIFMT_CBS_CFS);
	if (ret < 0) {
		pr_err("%s: failed to set codec dai format\n", __func__);
		goto out;
	}

	ret = snd_soc_dai_set_sysclk(cpu_dai, TEGRA_I2S_MCLK,
				tegra_i2s_get_mclk(params), SND_SOC_CLOCK_IN);
	if (ret < 0) {
		pr_err("%s: failed to set cpu dai mclk\n", __func__);
		goto out;
	}

	ret = snd_soc_dai_set_sysclk(cpu_dai, TEGRA_I2S_BCLK,
				tegra_i2s_get_bclk(params), SND_SOC_CLOCK_IN);
	if (ret < 0) {
		pr_err("%s: failed to set cpu dai bclk\n", __func__);
		goto out;
	}

	ret = snd_soc_dai_set_sysclk(codec_dai, WM8994_SYSCLK_MCLK1,
				clk_get_rate(info->dap_mclk), SND_SOC_CLOCK_IN);
	if (ret < 0) {
		pr_err("%s: failed to set codec dai mclk\n", __func__);
		goto out;
	}

out:
	return ret;
}

static struct snd_soc_ops tegra_hifi_ops = {
	.hw_params = tegra_hifi_hw_params,
};

static int tegra_hifi_init(struct snd_soc_codec *codec)
{
	int ret;

	ret = tegra_codec_init(codec);
	if (ret < 0)
		pr_err("failed to tegra hifi init\n");

	return ret;
}

static int tegra_bt_hw_params(struct snd_pcm_substream *substream,
					struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *cpu_dai = rtd->dai->cpu_dai;
	int ret = 0;

	tegra_das_set_connection(tegra_das_port_con_id_bt_codec);

	i2s_set_fifo_format(cpu_dai->id, I2S_FIFO_16_LSB);

	ret = snd_soc_dai_set_fmt(cpu_dai, SND_SOC_DAIFMT_DSP_A |
				SND_SOC_DAIFMT_NB_NF | SND_SOC_DAIFMT_CBS_CFS);
	if (ret < 0) {
		pr_err("%s: failed to set cpu dai format\n", __func__);
		goto out;
	}

	ret = snd_soc_dai_set_sysclk(cpu_dai, TEGRA_I2S_MCLK,
				tegra_i2s_get_mclk(params), SND_SOC_CLOCK_IN);
	if (ret < 0) {
		pr_err("%s: failed to set cpu dai mclk\n", __func__);
		goto out;
	}

	ret = snd_soc_dai_set_sysclk(cpu_dai, TEGRA_I2S_BCLK,
				256000, SND_SOC_CLOCK_IN);
	if (ret < 0) {
		pr_err("%s: failed to set cpu dai bclk\n", __func__);
		goto out;
	}

out:
	return ret;
}

static struct snd_soc_ops tegra_bt_ops = {
	.hw_params = tegra_bt_hw_params,
};

static int tegra_bt_init(struct snd_soc_codec *codec)
{
	int ret;

	ret = tegra_codec_init(codec);
	if (ret < 0)
		pr_err("%s: failed to tegra bluetooth init\n", __func__);

	return ret;
}

static struct snd_soc_dai tegra_bt_dai = {
	.name = "Tegra Bluetooth",
	.id = 5,
	.playback = {
		.stream_name = "BT Playback",
		.channels_min = 1,
		.channels_max = 1,
		.rates = SNDRV_PCM_RATE_8000,
		.formats = SNDRV_PCM_FMTBIT_S16_LE,
	},
	.capture = {
		.stream_name = "BT Capture",
		.channels_min = 1,
		.channels_max = 1,
		.rates = SNDRV_PCM_RATE_8000,
		.formats = SNDRV_PCM_FMTBIT_S16_LE,
	 },
};

static struct snd_soc_dai_link tegra_soc_dai[] = {
	{
		.name = "WM8994",
		.stream_name = "WM8994 HiFi",
		.cpu_dai = &tegra_i2s_dai[0],
		.codec_dai = &wm8994_dai[0],
		.init = tegra_hifi_init,
		.ops = &tegra_hifi_ops,
	},
	{
		.name = "Tegra",
		.stream_name = "Tegra Bluetooth",
		.cpu_dai = &tegra_i2s_dai[1],
		.codec_dai = &tegra_bt_dai,
		.init = tegra_bt_init,
		.ops = &tegra_bt_ops,
	},
};

static struct snd_soc_card tegra_snd_soc = {
	.name = "tegra",
	.platform = &tegra_soc_platform,
	.dai_link = tegra_soc_dai,
	.num_links = ARRAY_SIZE(tegra_soc_dai),
};

static struct snd_soc_device tegra_snd_devdata = {
	.card = &tegra_snd_soc,
	.codec_dev = &soc_codec_dev_wm8994,
};

static int __init tegra_init(void)
{
	int ret = 0;

	ret = snd_soc_register_dai(&tegra_bt_dai);
	if (ret != 0) {
		pr_err("%s: failed to register DAI %s\n",
			__func__, tegra_bt_dai.name);
		goto out;
	}

	tegra_snd_device = platform_device_alloc("soc-audio", -1);
	if (!tegra_snd_device) {
		pr_err("%s: failed to allocate soc-audio\n", __func__);
		ret = -ENOMEM;
		goto out;
	}

	platform_set_drvdata(tegra_snd_device, &tegra_snd_devdata);
	tegra_snd_devdata.dev = &tegra_snd_device->dev;
	ret = platform_device_add(tegra_snd_device);
	if (ret) {
		pr_err("%s: audio device could not be added\n", __func__);
		goto out_device_put;
	}

	goto out;

out_device_put:
	platform_device_put(tegra_snd_device);
	tegra_snd_device = 0;
out:
	return ret;
}

static void __exit tegra_exit(void)
{
	platform_device_unregister(tegra_snd_device);
}

module_init(tegra_init);
module_exit(tegra_exit);

/* Module information */
MODULE_DESCRIPTION("Tegra ALSA SoC for LGE Star Tablet");
MODULE_LICENSE("GPL");
