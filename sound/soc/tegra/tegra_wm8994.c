/*
 * tegra_wm8994.c  --  Tegra SoC audio for WM8994 audio hub
 *
 * (c) 2011 Nvidia Graphics Pvt. Ltd.
 * (c) 2012 Janne Grunau
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


#include <asm/mach-types.h>

#include <linux/clk.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/regulator/consumer.h>
#ifdef CONFIG_SWITCH
#include <linux/switch.h>
#endif

#include <mach/tegra_wm8994_pdata.h>

#include <linux/mfd/wm8994/registers.h>

#include <sound/core.h>
#include <sound/jack.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>

#include "../codecs/wm8994.h"

#include "tegra_pcm.h"
#include "tegra_asoc_utils.h"

#ifdef CONFIG_ARCH_TEGRA_2x_SOC
#include "tegra20_i2s.h"
#include "tegra20_das.h"
#endif

#include "../codecs/wm8994.h"

extern struct snd_soc_dai tegra_i2s_dai[];
extern struct snd_soc_platform tegra_soc_platform;

static struct snd_soc_jack hs_jack;

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

#define DRV_NAME "tegra-snd-wm8994"

struct tegra_wm8994 {
	struct snd_soc_codec *codec;
	struct tegra_asoc_utils_data util_data;
	struct tegra_wm8994_platform_data *pdata;
	struct regulator *spk_reg;
	struct regulator *dmic_reg;
	int gpio_requested;
#ifdef CONFIG_SWITCH
	int jack_status;
#endif
	enum snd_soc_bias_level bias_level;
};

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
		snd_soc_dapm_disable_pin(&codec->dapm, "Mic Jack");
		snd_soc_dapm_disable_pin(&codec->dapm, "Line Jack");
		snd_soc_dapm_enable_pin(&codec->dapm, "Headphone Jack");
		snd_soc_dapm_disable_pin(&codec->dapm, "Headset Jack");
		break;

	case TEGRA_MIC:
		/* reset = mute headphone */
		snd_soc_dapm_enable_pin(&codec->dapm, "Mic Jack");
		snd_soc_dapm_disable_pin(&codec->dapm, "Line Jack");
		snd_soc_dapm_disable_pin(&codec->dapm, "Headphone Jack");
		snd_soc_dapm_disable_pin(&codec->dapm, "Headset Jack");
		break;

	case TEGRA_LINE:
		snd_soc_dapm_disable_pin(&codec->dapm, "Mic Jack");
		snd_soc_dapm_enable_pin(&codec->dapm, "Line Jack");
		snd_soc_dapm_disable_pin(&codec->dapm, "Headphone Jack");
		snd_soc_dapm_disable_pin(&codec->dapm, "Headset Jack");
		break;

	case TEGRA_HEADSET:
		snd_soc_dapm_disable_pin(&codec->dapm, "Mic Jack");
		snd_soc_dapm_enable_pin(&codec->dapm, "Line Jack");
		snd_soc_dapm_enable_pin(&codec->dapm, "Headphone Jack");
		snd_soc_dapm_disable_pin(&codec->dapm, "Headset Jack");
		break;

	case TEGRA_ALL_ON:
		snd_soc_dapm_enable_pin(&codec->dapm, "Mic Jack");
		snd_soc_dapm_enable_pin(&codec->dapm, "Line Jack");
		snd_soc_dapm_enable_pin(&codec->dapm, "Headphone Jack");
		snd_soc_dapm_enable_pin(&codec->dapm, "Headset Jack");
		break;
	}

	if (tegra_spk_func == TEGRA_SPK_ON)
		snd_soc_dapm_enable_pin(&codec->dapm, "Ext Spk");
	else
		snd_soc_dapm_disable_pin(&codec->dapm, "Ext Spk");
	/* signal a DAPM event */
	snd_soc_dapm_sync(&codec->dapm);
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
static const struct snd_soc_dapm_widget tegra_wm8994_default_dapm_widgets[] = {
	SND_SOC_DAPM_HP("Headphone Jack", NULL),
	SND_SOC_DAPM_SPK("Ext Spk", NULL),
	SND_SOC_DAPM_MIC("Mic Jack", NULL),
	SND_SOC_DAPM_LINE("Line Jack", NULL),
	SND_SOC_DAPM_HP("Headset Jack", NULL),
};

/* Tegra machine audio map (connections to the codec pins) */
static const struct snd_soc_dapm_route startablet_audio_map[] = {
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

static const struct snd_kcontrol_new tegra_wm8994_default_controls[] = {
	SOC_ENUM_EXT("Jack Function", tegra_enum[0], tegra_get_jack,
			tegra_set_jack),
	SOC_ENUM_EXT("Speaker Function", tegra_enum[1], tegra_get_spk,
			tegra_set_spk),
	SOC_ENUM_EXT("Digital Audio Switch", tegra_enum[2], tegra_get_das,
			tegra_set_das),
};

static int tegra_codec_init(struct snd_soc_codec *codec)
{
	struct snd_soc_card *card = codec->card;
	struct tegra_wm8994 *machine = snd_soc_card_get_drvdata(card);
	int ret = 0;

	machine->codec = codec;

	/* Jack detection API stuff */
	ret = snd_soc_jack_new(codec, "Headset Jack",
			       SND_JACK_HEADSET, &hs_jack);
	if (ret < 0) {
		pr_err("%s: failed to add new jack\n", __func__);
		goto out;
	}

	ret = snd_soc_jack_add_pins(&hs_jack, ARRAY_SIZE(hs_jack_pins),
				    hs_jack_pins);
	if (ret < 0) {
		pr_err("%s: failed to add jack pins\n", __func__);
		goto out;
	}

	/* Default to HP output */
	tegra_jack_func = TEGRA_HP;
	tegra_spk_func = TEGRA_SPK_ON;
	tegra_das_func = TEGRA_DAS_HIFI;
	tegra_ext_control(codec);
	snd_soc_dapm_sync(&codec->dapm);

out:
	return ret;
}

static int tegra_wm8994_hw_params(struct snd_pcm_substream *substream,
					struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct snd_soc_codec *codec = rtd->codec;
	struct snd_soc_card *card = codec->card;
	struct tegra_wm8994 *machine = snd_soc_card_get_drvdata(card);
	int srate, mclk, i2s_daifmt;
	int err;
	int rate;

	srate = params_rate(params);
	switch (srate) {
	case 8000:
	case 16000:
	case 24000:
	case 32000:
	case 48000:
	case 64000:
	case 96000:
		mclk = 12288000;
		break;
	case 11025:
	case 22050:
	case 44100:
	case 88200:
		mclk = 11289600;
		break;
	default:
		mclk = 12000000;
		break;
	}

	err = tegra_asoc_utils_set_rate(&machine->util_data, srate, mclk);
	if (err < 0) {
		if (!(machine->util_data.set_mclk % mclk))
			mclk = machine->util_data.set_mclk;
		else {
			dev_err(card->dev, "Can't configure clocks\n");
			return err;
		}
	}

	tegra_asoc_utils_lock_clk_rate(&machine->util_data, 1);

	i2s_daifmt = SND_SOC_DAIFMT_NB_NF |
		SND_SOC_DAIFMT_CBS_CFS;

	/* Use DSP mode for mono on Tegra20 */
	if ((params_channels(params) != 2) &&
	    machine_is_startablet())
		i2s_daifmt |= SND_SOC_DAIFMT_DSP_A;
	else
		i2s_daifmt |= SND_SOC_DAIFMT_I2S;

	err = snd_soc_dai_set_fmt(codec_dai, i2s_daifmt);
	if (err < 0) {
		dev_err(card->dev, "codec_dai fmt not set\n");
		return err;
	}

	err = snd_soc_dai_set_fmt(cpu_dai, i2s_daifmt);
	if (err < 0) {
		dev_err(card->dev, "cpu_dai fmt not set\n");
		return err;
	}

	err = snd_soc_dai_set_sysclk(codec_dai, WM8994_SYSCLK_MCLK1, mclk,
					SND_SOC_CLOCK_IN);
	if (err < 0) {
		dev_err(card->dev, "codec_dai clock not set\n");
		return err;
	}

	err = tegra20_das_connect_dac_to_dap(TEGRA20_DAS_DAC_ID_1,
					     TEGRA20_DAS_DAC_SEL_DAP1);
	if (err < 0) {
		dev_err(card->dev, "failed to set dap-dac path\n");
		return err;
	}

	err = tegra20_das_connect_dap_to_dac(TEGRA20_DAS_DAP_ID_1,
					     TEGRA20_DAS_DAP_SEL_DAC1);
	if (err < 0) {
		dev_err(card->dev, "failed to set dac-dap path\n");
		return err;
	}

	/* tegra20_das_connect_dap_to_dac(TEGRA20_DAS_DAP_ID_2, TEGRA20_DAS_DAP_SEL_DAC2); */
	/* tegra20_das_connect_dap_to_dac(TEGRA20_DAS_DAP_ID_3, TEGRA20_DAS_DAP_SEL_DAC2); */
	/* tegra20_das_connect_dap_to_dac(TEGRA20_DAS_DAP_ID_4, TEGRA20_DAS_DAP_SEL_DAC2); */
	/* tegra20_das_connect_dap_to_dac(TEGRA20_DAS_DAP_ID_5, TEGRA20_DAS_DAP_SEL_DAC3); */
	/* tegra20_das_connect_dac_to_dap(TEGRA20_DAS_DAC_ID_2, TEGRA20_DAS_DAC_SEL_DAP4); */
	/* tegra20_das_connect_dac_to_dap(TEGRA20_DAS_DAC_ID_3, TEGRA20_DAS_DAC_SEL_DAP5); */

	return 0;
}

static int tegra_wm8994_init(struct snd_soc_pcm_runtime *rtd)
{
	struct snd_soc_codec *codec = rtd->codec;
	int ret;

	ret = tegra_codec_init(codec);
	if (ret < 0)
		pr_err("failed to tegra hifi init\n");

	return ret;
}

static int tegra_bt_sco_hw_params(struct snd_pcm_substream *substream,
					struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct snd_soc_card *card = rtd->card;
	struct tegra_wm8994 *machine = snd_soc_card_get_drvdata(card);
	int srate, mclk, min_mclk;
	int err;

	srate = params_rate(params);
	switch (srate) {
	case 11025:
	case 22050:
	case 44100:
	case 88200:
		mclk = 11289600;
		break;
	case 8000:
	case 16000:
	case 32000:
	case 48000:
	case 64000:
	case 96000:
		mclk = 12288000;
		break;
	default:
		return -EINVAL;
	}
	min_mclk = 64 * srate;

	err = tegra_asoc_utils_set_rate(&machine->util_data, srate, mclk);
	if (err < 0) {
		if (!(machine->util_data.set_mclk % min_mclk))
			mclk = machine->util_data.set_mclk;
		else {
			dev_err(card->dev, "Can't configure clocks\n");
			return err;
		}
	}

	tegra_asoc_utils_lock_clk_rate(&machine->util_data, 1);

	err = snd_soc_dai_set_fmt(cpu_dai,
				  SND_SOC_DAIFMT_DSP_A |
				  SND_SOC_DAIFMT_NB_NF |
				  SND_SOC_DAIFMT_CBS_CFS);
	if (err < 0) {
		dev_err(card->dev, "cpu_dai fmt not set\n");
		return err;
	}

	err = tegra20_das_connect_dac_to_dap(TEGRA20_DAS_DAC_ID_2,
					     TEGRA20_DAS_DAC_SEL_DAP4);
	if (err < 0) {
		dev_err(card->dev, "failed to set dac-dap path\n");
		return err;
	}

	err = tegra20_das_connect_dap_to_dac(TEGRA20_DAS_DAP_ID_4,
					     TEGRA20_DAS_DAP_SEL_DAC2);
	if (err < 0) {
		dev_err(card->dev, "failed to set dac-dap path\n");
		return err;
	}
	return 0;
}


static int tegra_spdif_hw_params(struct snd_pcm_substream *substream,
					struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_card *card = rtd->card;
	struct tegra_wm8994 *machine = snd_soc_card_get_drvdata(card);
	int srate, mclk, min_mclk;
	int err;

	srate = params_rate(params);
	switch (srate) {
	case 11025:
	case 22050:
	case 44100:
	case 88200:
		mclk = 11289600;
		break;
	case 8000:
	case 16000:
	case 32000:
	case 48000:
	case 64000:
	case 96000:
		mclk = 12288000;
		break;
	default:
		return -EINVAL;
	}
	min_mclk = 128 * srate;

	err = tegra_asoc_utils_set_rate(&machine->util_data, srate, mclk);
	if (err < 0) {
		if (!(machine->util_data.set_mclk % min_mclk))
			mclk = machine->util_data.set_mclk;
		else {
			dev_err(card->dev, "Can't configure clocks\n");
			return err;
		}
	}

	tegra_asoc_utils_lock_clk_rate(&machine->util_data, 1);

	return 0;
}

static int tegra_hw_free(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct tegra_wm8994 *machine = snd_soc_card_get_drvdata(rtd->card);

	tegra_asoc_utils_lock_clk_rate(&machine->util_data, 0);

	return 0;
}

static struct snd_soc_ops tegra_wm8994_ops = {
	.hw_params	= tegra_wm8994_hw_params,
	.hw_free	= tegra_hw_free,
};

static struct snd_soc_ops tegra_spdif_ops = {
	.hw_params	= tegra_spdif_hw_params,
	.hw_free	= tegra_hw_free,
};

static struct snd_soc_ops tegra_wm8994_bt_sco_ops = {
	.hw_params	= tegra_bt_sco_hw_params,
	.hw_free	= tegra_hw_free,
};

static struct switch_dev tegra_wm8994_headset_switch = {
	.name = "h2w",
};

static struct snd_soc_dai_link tegra_wm8994_dai[] = {
	{
		.name = "WM8994",
		.stream_name = "WM8994 PCM",
		.codec_name = "wm8994-codec",
		.platform_name = "tegra-pcm-audio",
		.cpu_dai_name = "tegra20-i2s.0",
		.codec_dai_name = "wm8994-aif1",
		.init = tegra_wm8994_init,
		.ops = &tegra_wm8994_ops,
	},
	{
		.name = "SPDIF",
		.stream_name = "SPDIF PCM",
		.codec_name = "spdif-dit.0",
		.platform_name = "tegra-pcm-audio",
		.cpu_dai_name = "tegra20-spdif",
		.codec_dai_name = "dit-hifi",
		.ops = &tegra_spdif_ops,
	},
	{
		.name = "BT-SCO",
		.stream_name = "BT SCO PCM",
		.codec_name = "spdif-dit.1",
		.platform_name = "tegra-pcm-audio",
		.cpu_dai_name = "tegra20-i2s.1",
		.codec_dai_name = "dit-hifi",
		.ops = &tegra_wm8994_bt_sco_ops,
	},
};

static struct snd_soc_card snd_soc_tegra_wm8994 = {
	.name = "tegra-wm8994",
	.dai_link = tegra_wm8994_dai,
	.num_links = ARRAY_SIZE(tegra_wm8994_dai),
};

static __devinit int tegra_wm8994_driver_probe(struct platform_device *pdev)
{
	struct snd_soc_card *card = &snd_soc_tegra_wm8994;
	struct tegra_wm8994 *machine;
	struct tegra_wm8994_platform_data *pdata;
	int ret;

	pdata = pdev->dev.platform_data;
	if (!pdata) {
		dev_err(&pdev->dev, "No platform data supplied\n");
		return -EINVAL;
	}

	machine = kzalloc(sizeof(struct tegra_wm8994), GFP_KERNEL);
	if (!machine) {
		dev_err(&pdev->dev, "Can't allocate tegra_wm8994 struct\n");
		return -ENOMEM;
	}

	machine->pdata = pdata;

	ret = tegra_asoc_utils_init(&machine->util_data, &pdev->dev);
	if (ret)
		goto err_free_machine;

#ifdef CONFIG_SWITCH
	/* Addd h2w swith class support */
	ret = switch_dev_register(&tegra_wm8994_headset_switch);
	if (ret < 0)
		goto err_fini_utils;
#endif

	card->dev = &pdev->dev;
	platform_set_drvdata(pdev, card);
	snd_soc_card_set_drvdata(card, machine);

	card->controls = tegra_wm8994_default_controls;
	card->num_controls = ARRAY_SIZE(tegra_wm8994_default_controls);

	card->dapm_widgets = tegra_wm8994_default_dapm_widgets;
	card->num_dapm_widgets = ARRAY_SIZE(tegra_wm8994_default_dapm_widgets);

	card->dapm_routes = startablet_audio_map;
	card->num_dapm_routes = ARRAY_SIZE(startablet_audio_map);

	ret = snd_soc_register_card(card);
	if (ret) {
		dev_err(&pdev->dev, "snd_soc_register_card failed (%d)\n",
			ret);
		goto err_unregister_switch;
	}

	if (!card->instantiated) {
		ret = -ENODEV;
		dev_err(&pdev->dev, "snd_soc_register_card failed, card "
			"not instantiated (%d)\n",
			ret);
		goto err_unregister_card;
	}

	return 0;

err_unregister_card:
	snd_soc_unregister_card(card);
err_unregister_switch:
#ifdef CONFIG_SWITCH
	switch_dev_unregister(&tegra_wm8994_headset_switch);
#endif
err_fini_utils:
	tegra_asoc_utils_fini(&machine->util_data);
err_free_machine:
	kfree(machine);
	return ret;
}

static int __devexit tegra_wm8994_driver_remove(struct platform_device *pdev)
{
	struct snd_soc_card *card = platform_get_drvdata(pdev);
	struct tegra_wm8994 *machine = snd_soc_card_get_drvdata(card);

	snd_soc_unregister_card(card);

	tegra_asoc_utils_fini(&machine->util_data);

#ifdef CONFIG_SWITCH
	switch_dev_unregister(&tegra_wm8994_headset_switch);
#endif
	kfree(machine);

	return 0;
}

static struct platform_driver tegra_wm8994_driver = {
	.driver = {
		.name = DRV_NAME,
		.owner = THIS_MODULE,
		.pm = &snd_soc_pm_ops,
	},
	.probe = tegra_wm8994_driver_probe,
	.remove = __devexit_p(tegra_wm8994_driver_remove),
};

static int __init tegra_wm8994_modinit(void)
{
	return platform_driver_register(&tegra_wm8994_driver);
}

static void __exit tegra_wm8994_modexit(void)
{
	platform_driver_unregister(&tegra_wm8994_driver);
}
module_init(tegra_wm8994_modinit);
module_exit(tegra_wm8994_modexit);

/* Module information */
MODULE_DESCRIPTION("Tegra+WM8994 machine ASoC driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:" DRV_NAME);
