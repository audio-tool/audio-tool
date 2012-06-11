/*
 * card-omap-common-4-5.h
 *
 * Copyright (C) 2012 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef __AUDIO_TOOL_CARD_OMAP_COMMON_4_5_H__
#define __AUDIO_TOOL_CARD_OMAP_COMMON_4_5_H__
#else
#error This file should be treated as code and not as a header.
#endif

/*
 * Code common between cards SDP4430 and OMAP45
 *
 * When matching ABE versions, the ABE FW version is attached.
 *
 * Symbols are not versioned until versioning is required.
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <tinyalsa/asoundlib.h>

#include "alsa-control.h"
#include "mixer_cache.h"

static struct audio_tool_mixer_control_info g_defaults_common[] = {
	{
		.id = -1,
		.name = "DL1 Equalizer",
		.type = MIXER_CTL_TYPE_ENUM,
		.num_values = 1,
		.value.enumerated = {
			"Flat response",
			"",
			"",
			"",
		},
	},
	{
		.id = -1,
		.name = "DL2 Left Equalizer",
		.type = MIXER_CTL_TYPE_ENUM,
		.num_values = 1,
		.value.enumerated = {
			"High-pass 0dB",
			"",
			"",
			"",
		},
	},
	{
		.id = -1,
		.name = "DL2 Right Equalizer",
		.type = MIXER_CTL_TYPE_ENUM,
		.num_values = 1,
		.value.enumerated = {
			"High-pass 0dB",
			"",
			"",
			"",
		},
	},
	{
		.id = -1,
		.name = "Sidetone Equalizer",
		.type = MIXER_CTL_TYPE_ENUM,
		.num_values = 1,
		.value.enumerated = {
			"Flat response",
			"",
			"",
			"",
		},
	},
	{
		.id = -1,
		.name = "AMIC Equalizer",
		.type = MIXER_CTL_TYPE_ENUM,
		.num_values = 1,
		.value.enumerated = {
			"High-pass 0dB",
			"",
			"",
			"",
		},
	},
	{
		.id = -1,
		.name = "DMIC Equalizer",
		.type = MIXER_CTL_TYPE_ENUM,
		.num_values = 1,
		.value.enumerated = {
			"High-pass 0dB",
			"",
			"",
			"",
		},
	},
	{
		.id = -1,
		.name = "DL1 Media Playback Volume",
		.type = MIXER_CTL_TYPE_INT,
		.num_values = 1,
		.value.integer = {
			118,
		},
	},
	{
		.id = -1,
		.name = "DL1 Tones Playback Volume",
		.type = MIXER_CTL_TYPE_INT,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "DL1 Voice Playback Volume",
		.type = MIXER_CTL_TYPE_INT,
		.num_values = 1,
		.value.integer = {
			120,
		},
	},
	{
		.id = -1,
		.name = "DL1 Capture Playback Volume",
		.type = MIXER_CTL_TYPE_INT,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "VXREC Media Volume",
		.type = MIXER_CTL_TYPE_INT,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "VXREC Tones Volume",
		.type = MIXER_CTL_TYPE_INT,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "VXREC Voice DL Volume",
		.type = MIXER_CTL_TYPE_INT,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "VXREC Voice UL Volume",
		.type = MIXER_CTL_TYPE_INT,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "AUDUL Media Volume",
		.type = MIXER_CTL_TYPE_INT,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "AUDUL Tones Volume",
		.type = MIXER_CTL_TYPE_INT,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "AUDUL Voice UL Volume",
		.type = MIXER_CTL_TYPE_INT,
		.num_values = 1,
		.value.integer = {
			120,
		},
	},
	{
		.id = -1,
		.name = "AUDUL Voice DL Volume",
		.type = MIXER_CTL_TYPE_INT,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "SDT UL Volume",
		.type = MIXER_CTL_TYPE_INT,
		.num_values = 1,
		.value.integer = {
			101,
		},
	},
	{
		.id = -1,
		.name = "SDT DL Volume",
		.type = MIXER_CTL_TYPE_INT,
		.num_values = 1,
		.value.integer = {
			120,
		},
	},
	{
		.id = -1,
		.name = "DMIC1 UL Volume",
		.type = MIXER_CTL_TYPE_INT,
		.num_values = 2,
		.value.integer = {
			120,
			120,
		},
	},
	{
		.id = -1,
		.name = "DMIC2 UL Volume",
		.type = MIXER_CTL_TYPE_INT,
		.num_values = 2,
		.value.integer = {
			120,
			120,
		},
	},
	{
		.id = -1,
		.name = "DMIC3 UL Volume",
		.type = MIXER_CTL_TYPE_INT,
		.num_values = 2,
		.value.integer = {
			120,
			120,
		},
	},
	{
		.id = -1,
		.name = "AMIC UL Volume",
		.type = MIXER_CTL_TYPE_INT,
		.num_values = 2,
		.value.integer = {
			120,
			120,
		},
	},
	{
		.id = -1,
		.name = "BT UL Volume",
		.type = MIXER_CTL_TYPE_INT,
		.num_values = 2,
		.value.integer = {
			120,
			120,
		},
	},
	{
		.id = -1,
		.name = "DL1 Mono Mixer",
		.type = MIXER_CTL_TYPE_BOOL,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "AUDUL Mono Mixer",
		.type = MIXER_CTL_TYPE_BOOL,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "DL1 MM_EXT Switch",
		.type = MIXER_CTL_TYPE_BOOL,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "DL1 BT_VX Switch",
		.type = MIXER_CTL_TYPE_BOOL,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "Sidetone Mixer Capture",
		.type = MIXER_CTL_TYPE_BOOL,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "Sidetone Mixer Playback",
		.type = MIXER_CTL_TYPE_BOOL,
		.num_values = 1,
		.value.integer = {
			1,
		},
	},
	{
		.id = -1,
		.name = "Capture Mixer Tones",
		.type = MIXER_CTL_TYPE_BOOL,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "Capture Mixer Voice Playback",
		.type = MIXER_CTL_TYPE_BOOL,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "Capture Mixer Voice Capture",
		.type = MIXER_CTL_TYPE_BOOL,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "Capture Mixer Media Playback",
		.type = MIXER_CTL_TYPE_BOOL,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "Voice Capture Mixer Tones Playback",
		.type = MIXER_CTL_TYPE_BOOL,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "Voice Capture Mixer Media Playback",
		.type = MIXER_CTL_TYPE_BOOL,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "Voice Capture Mixer Capture",
		.type = MIXER_CTL_TYPE_BOOL,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "DL1 Mixer Tones",
		.type = MIXER_CTL_TYPE_BOOL,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "DL1 Mixer Voice",
		.type = MIXER_CTL_TYPE_BOOL,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "DL1 Mixer Capture",
		.type = MIXER_CTL_TYPE_BOOL,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "DL1 Mixer Multimedia",
		.type = MIXER_CTL_TYPE_BOOL,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "MUX_VX1",
		.type = MIXER_CTL_TYPE_ENUM,
		.num_values = 1,
		.value.enumerated = {
			"None",
			"",
			"",
			"",
		},
	},
	{
		.id = -1,
		.name = "MUX_VX0",
		.type = MIXER_CTL_TYPE_ENUM,
		.num_values = 1,
		.value.enumerated = {
			"None",
			"",
			"",
			"",
		},
	},
	{
		.id = -1,
		.name = "MUX_UL11",
		.type = MIXER_CTL_TYPE_ENUM,
		.num_values = 1,
		.value.enumerated = {
			"None",
			"",
			"",
			"",
		},
	},
	{
		.id = -1,
		.name = "MUX_UL10",
		.type = MIXER_CTL_TYPE_ENUM,
		.num_values = 1,
		.value.enumerated = {
			"None",
			"",
			"",
			"",
		},
	},
	{
		.id = -1,
		.name = "MUX_UL07",
		.type = MIXER_CTL_TYPE_ENUM,
		.num_values = 1,
		.value.enumerated = {
			"None",
			"",
			"",
			"",
		},
	},
	{
		.id = -1,
		.name = "MUX_UL06",
		.type = MIXER_CTL_TYPE_ENUM,
		.num_values = 1,
		.value.enumerated = {
			"None",
			"",
			"",
			"",
		},
	},
	{
		.id = -1,
		.name = "MUX_UL05",
		.type = MIXER_CTL_TYPE_ENUM,
		.num_values = 1,
		.value.enumerated = {
			"None",
			"",
			"",
			"",
		},
	},
	{
		.id = -1,
		.name = "MUX_UL04",
		.type = MIXER_CTL_TYPE_ENUM,
		.num_values = 1,
		.value.enumerated = {
			"None",
			"",
			"",
			"",
		},
	},
	{
		.id = -1,
		.name = "MUX_UL03",
		.type = MIXER_CTL_TYPE_ENUM,
		.num_values = 1,
		.value.enumerated = {
			"None",
			"",
			"",
			"",
		},
	},
	{
		.id = -1,
		.name = "MUX_UL02",
		.type = MIXER_CTL_TYPE_ENUM,
		.num_values = 1,
		.value.enumerated = {
			"None",
			"",
			"",
			"",
		},
	},
	{
		.id = -1,
		.name = "MUX_UL01",
		.type = MIXER_CTL_TYPE_ENUM,
		.num_values = 1,
		.value.enumerated = {
			"None",
			"",
			"",
			"",
		},
	},
	{
		.id = -1,
		.name = "MUX_UL00",
		.type = MIXER_CTL_TYPE_ENUM,
		.num_values = 1,
		.value.enumerated = {
			"None",
			"",
			"",
			"",
		},
	},
	{
		.id = -1,
		.name = "Capture Preamplifier Volume",
		.type = MIXER_CTL_TYPE_INT,
		.num_values = 2,
		.value.integer = {
			1,
			1,
		},
	},
	{
		.id = -1,
		.name = "Capture Volume",
		.type = MIXER_CTL_TYPE_INT,
		.num_values = 2,
		.value.integer = {
			4,
			4,
		},
	},
	{
		.id = -1,
		.name = "Aux FM Volume",
		.type = MIXER_CTL_TYPE_INT,
		.num_values = 2,
		.value.integer = {
			3,
			3,
		},
	},
	{
		.id = -1,
		.name = "Headset Playback Volume",
		.type = MIXER_CTL_TYPE_INT,
		.num_values = 2,
		.value.integer = {
			15,
			15,
		},
	},
	{
		.id = -1,
		.name = "Handsfree Playback Volume",
		.type = MIXER_CTL_TYPE_INT,
		.num_values = 2,
		.value.integer = {
			26,
			26,
		},
	},
	{
		.id = -1,
		.name = "Earphone Playback Volume",
		.type = MIXER_CTL_TYPE_INT,
		.num_values = 1,
		.value.integer = {
			14,
		},
	},
	{
		.id = -1,
		.name = "Headset Power Mode",
		.type = MIXER_CTL_TYPE_ENUM,
		.num_values = 1,
		.value.enumerated = {
			"Low-Power",
			"",
			"",
			"",
		},
	},
	{
		.id = -1,
		.name = "Earphone Playback Switch",
		.type = MIXER_CTL_TYPE_BOOL,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "Headset Right Playback",
		.type = MIXER_CTL_TYPE_ENUM,
		.num_values = 1,
		.value.enumerated = {
			"Off",
			"",
			"",
			"",
		},
	},
	{
		.id = -1,
		.name = "Headset Left Playback",
		.type = MIXER_CTL_TYPE_ENUM,
		.num_values = 1,
		.value.enumerated = {
			"Off",
			"",
			"",
			"",
		},
	},
	{
		.id = -1,
		.name = "Handsfree Right Playback",
		.type = MIXER_CTL_TYPE_ENUM,
		.num_values = 1,
		.value.enumerated = {
			"HF DAC",
			"",
			"",
			"",
		},
	},
	{
		.id = -1,
		.name = "Handsfree Left Playback",
		.type = MIXER_CTL_TYPE_ENUM,
		.num_values = 1,
		.value.enumerated = {
			"HF DAC",
			"",
			"",
			"",
		},
	},
	{
		.id = -1,
		.name = "Analog Right Capture Route",
		.type = MIXER_CTL_TYPE_ENUM,
		.num_values = 1,
		.value.enumerated = {
			"Off",
			"",
			"",
			"",
		},
	},
	{
		.id = -1,
		.name = "Analog Left Capture Route",
		.type = MIXER_CTL_TYPE_ENUM,
		.num_values = 1,
		.value.enumerated = {
			"Off",
			"",
			"",
			"",
		},
	},
	{
		.id = -1,
		.name = "TWL6040 Power Mode",
		.type = MIXER_CTL_TYPE_ENUM,
		.num_values = 1,
		.value.enumerated = {
			"Low-Power",
			"",
			"",
			"",
		},
	},
};

/* The AUX switch hasn't really been associated to an ABE FW version,
 * so we check for it specifically.
 */
static struct audio_tool_mixer_control_info g_defaults_aux_switch[] = {
	{
		.id = -1,
		.name = "Aux Left Playback Switch",
		.type = MIXER_CTL_TYPE_BOOL,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "Aux Right Playback Switch",
		.type = MIXER_CTL_TYPE_BOOL,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
};

static struct audio_tool_mixer_control_info g_defaults_0951[] = {
	{
		.id = -1,
		.name = "DL2 Media Playback Volume",
		.type = MIXER_CTL_TYPE_INT,
		.num_values = 1,
		.value.integer = {
			118,
		},
	},
	{
		.id = -1,
		.name = "DL2 Tones Playback Volume",
		.type = MIXER_CTL_TYPE_INT,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "DL2 Voice Playback Volume",
		.type = MIXER_CTL_TYPE_INT,
		.num_values = 1,
		.value.integer = {
			120,
		},
	},
	{
		.id = -1,
		.name = "DL2 Capture Playback Volume",
		.type = MIXER_CTL_TYPE_INT,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "DL2 Mono Mixer",
		.type = MIXER_CTL_TYPE_BOOL,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "DL1 PDM Switch",
		.type = MIXER_CTL_TYPE_BOOL,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "DL2 Mixer Tones",
		.type = MIXER_CTL_TYPE_BOOL,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "DL2 Mixer Voice",
		.type = MIXER_CTL_TYPE_BOOL,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "DL2 Mixer Capture",
		.type = MIXER_CTL_TYPE_BOOL,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "DL2 Mixer Multimedia",
		.type = MIXER_CTL_TYPE_BOOL,
		.num_values = 1,
		.value.integer = {
			1,
		},
	},
};

static struct audio_tool_mixer_control_info g_defaults_0956[] = {
	{
		.id = -1,
		.name = "DL1 PDM_DL2 Switch",
		.type = MIXER_CTL_TYPE_BOOL,
		.num_values = 1,
		.value.integer = {
			1,
		},
	},
	{
		.id = -1,
		.name = "DL1 PDM_DL1 Switch",
		.type = MIXER_CTL_TYPE_BOOL,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "PLL Selection",
		.type = MIXER_CTL_TYPE_ENUM,
		.num_values = 1,
		.value.enumerated = {
			"Low-Power",
			"",
			"",
			"",
		},
	},
	{
		.id = -1,
		.name = "AUXR Playback Switch",
		.type = MIXER_CTL_TYPE_BOOL,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "AUXL Playback Switch",
		.type = MIXER_CTL_TYPE_BOOL,
		.num_values = 1,
		.value.integer = {
			0,
		},
	},
	{
		.id = -1,
		.name = "Vibra Right Playback",
		.type = MIXER_CTL_TYPE_ENUM,
		.num_values = 1,
		.value.enumerated = {
			"Input FF",
			"",
			"",
			"",
		},
	},
	{
		.id = -1,
		.name = "Vibra Left Playback",
		.type = MIXER_CTL_TYPE_ENUM,
		.num_values = 1,
		.value.enumerated = {
			"Input FF",
			"",
			"",
			"",
		},
	},
};

#define ABE_API_NULL 0L
#define ABE_API_0951 951L
#define ABE_API_0956 956L

static int g_abe_api = ABE_API_NULL;

static struct audio_tool_mixer_cache g_card_mix_defaults = {
	.count = 0,
	.ctrls = 0,
};

static char* g_playback_frontends[] = {
	"Multimedia",
	"Voice",
	"Tones",
	"MultimediaLP",
	0,
};

static char* g_playback_backends[] = {
	"Headset",
	"Handsfree",
	"Earpiece",
	"Bluetooth",
	0,
};

static char *g_capture_frontends[] = {
	"Multimedia",
	"Multimedia2",
	"Voice",
	0,
};

static char *g_capture_backends[] = {
	"HeadsetMic",
	"OnboardMic",
	"Aux/FM",
	"DMic0",
	"DMic1",
	"DMic2",
	"Bluetooth",
	"Echo",
	0,
};

struct route_setting
{
	char *ctl_name;
	int intval;
	char *strval;
};

#define RS_INT(str, val) { .ctl_name = str, .intval = val }
#define RS_ENUM(str, val) { .ctl_name = str, .strval = val }
#define RS_NULL { 0, 0, 0, }

/***************************************************************************
 * PLAYBACK ROUTE SETTINGS
 ***************************************************************************
 */

/* For Frontend := { Multimedia, MultimediaLP }
 * For Backend := { Headset, Earpiece, Bluetooth }
 */
static struct route_setting g_playback_multimedia_accessory_mix[] = {
	RS_INT("Sidetone Mixer Playback", 1),
	RS_INT("SDT DL Volume", 120),
	RS_INT("DL1 Mixer Multimedia", 1),
	RS_INT("DL1 Media Playback Volume", 118),
	RS_NULL,
};

/* For Frontend := { Multimedia, MultimediaLP }
 * For Backend := { Handsfree }
 */
static struct route_setting *g_playback_multimedia_handsfree_mix = 0;
static struct route_setting g_playback_multimedia_handsfree_mix_0951[] = {
	RS_INT("DL2 Mixer Multimedia", 1),
	RS_INT("DL2 Media Playback Volume", 118),
	RS_NULL,
};
static struct route_setting g_playback_multimedia_handsfree_mix_0956[] = {
	RS_INT("Sidetone Mixer Playback", 1),
	RS_INT("SDT DL Volume", 120),
	RS_INT("DL1 Mixer Multimedia", 1),
	RS_INT("DL1 Media Playback Volume", 118),
	RS_NULL,
};

/* For Frontend := { Voice }
 * For Backend := { Headset, Earpiece, Bluetooth }
 */
static struct route_setting g_playback_voice_accessory_mix[] = {
	RS_INT("Sidetone Mixer Playback", 1),
	RS_INT("SDT DL Volume", 120),
	RS_INT("DL1 Mixer Voice", 1),
	RS_INT("DL1 Voice Playback Volume", 118),
	RS_NULL,
};

/* For Frontend := { Voice }
 * For Backend := { Handsfree }
 */
static struct route_setting *g_playback_voice_handsfree_mix = 0;
static struct route_setting g_playback_voice_handsfree_mix_0951[] = {
	RS_INT("DL2 Mixer Voice", 1),
	RS_INT("DL2 Voice Playback Volume", 118),
	RS_NULL,
};
static struct route_setting g_playback_voice_handsfree_mix_0956[] = {
	RS_INT("Sidetone Mixer Playback", 1),
	RS_INT("SDT DL Volume", 120),
	RS_INT("DL1 Mixer Voice", 1),
	RS_INT("DL1 Voice Playback Volume", 118),
	RS_NULL,
};

/* For Frontend := { Tones }
 * For Backend := { Headset, Earpiece, Bluetooth }
 */
static struct route_setting g_playback_tones_accessory_mix[] = {
	RS_INT("Sidetone Mixer Playback", 1),
	RS_INT("SDT DL Volume", 120),
	RS_INT("DL1 Mixer Tones", 1),
	RS_INT("DL1 Tones Playback Volume", 118),
	RS_NULL,
};

/* For Frontend := { Tones }
 * For Backend := { Handsfree }
 */
static struct route_setting *g_playback_tones_handsfree_mix = 0;
static struct route_setting g_playback_tones_handsfree_mix_0951[] = {
	RS_INT("DL2 Mixer Tones", 1),
	RS_INT("DL2 Tones Playback Volume", 118),
	RS_NULL,
};
static struct route_setting g_playback_tones_handsfree_mix_0956[] = {
	RS_INT("Sidetone Mixer Playback", 1),
	RS_INT("SDT DL Volume", 120),
	RS_INT("DL1 Mixer Tones", 1),
	RS_INT("DL1 Tones Playback Volume", 118),
	RS_NULL,
};

static struct route_setting *g_playback_be_headset_mix = 0;
static struct route_setting g_playback_be_headset_mix_0951[] = {
	RS_INT("DL1 PDM Switch", 1),
	RS_ENUM("Headset Left Playback", "HS DAC"),
	RS_ENUM("Headset Right Playback", "HS DAC"),
	RS_INT("Headset Playback Volume", 13),
	RS_NULL,
};
static struct route_setting g_playback_be_headset_mix_0956[] = {
	RS_INT("DL1 PDM_DL1 Switch", 1),
	RS_ENUM("Headset Left Playback", "HS DAC"),
	RS_ENUM("Headset Right Playback", "HS DAC"),
	RS_INT("Headset Playback Volume", 13),
	RS_NULL,
};

static struct route_setting *g_playback_be_earpiece_mix = 0;
static struct route_setting g_playback_be_earpiece_mix_0951[] = {
	RS_INT("DL1 PDM Switch", 1),
	RS_INT("Earphone Playback Switch", 1),
	RS_INT("Earphone Playback Volume", 13),
	RS_NULL,
};
static struct route_setting g_playback_be_earpiece_mix_0956[] = {
	RS_INT("DL1 PDM_DL1 Switch", 1),
	RS_INT("Earphone Playback Switch", 1),
	RS_INT("Earphone Playback Volume", 13),
	RS_NULL,
};

static struct route_setting g_playback_be_bluetooth_mix[] = {
	RS_INT("DL1 BT_VX Switch", 1),
	RS_INT("BT UL Volume", 120),
	RS_NULL,
};

static struct route_setting *g_playback_be_handsfree_mix = 0;
static struct route_setting g_playback_be_handsfree_mix_0951[] = {
	RS_ENUM("Handsfree Left Playback", "HF DAC"),
	RS_ENUM("Handsfree Right Playback", "HF DAC"),
	RS_INT("Handsfree Playback Volume", 23),
	RS_NULL,
};
static struct route_setting g_playback_be_handsfree_mix_0956[] = {
	RS_INT("DL1 PDM_DL2 Switch", 1),
	RS_ENUM("Handsfree Left Playback", "HF DAC"),
	RS_ENUM("Handsfree Right Playback", "HF DAC"),
	RS_INT("Handsfree Playback Volume", 23),
	RS_NULL,
};

/***************************************************************************
 * CAPTURE ROUTE SETTINGS
 ***************************************************************************
 */

static struct route_setting g_capture_multimedia_amic_mix[] = {
	RS_ENUM("MUX_UL00", "AMic0"),
	RS_ENUM("MUX_UL01", "AMic1"),
	RS_ENUM("MUX_UL02", "None"),
	RS_ENUM("MUX_UL03", "None"),
	RS_ENUM("MUX_UL04", "None"),
	RS_ENUM("MUX_UL05", "None"),
	RS_ENUM("MUX_UL06", "None"),
	RS_ENUM("MUX_UL07", "None"),
	RS_NULL,
};

static struct route_setting g_capture_multimedia_dmic0_mix[] = {
	RS_ENUM("MUX_UL00", "DMic0L"),
	RS_ENUM("MUX_UL01", "DMic0R"),
	RS_ENUM("MUX_UL02", "None"),
	RS_ENUM("MUX_UL03", "None"),
	RS_ENUM("MUX_UL04", "None"),
	RS_ENUM("MUX_UL05", "None"),
	RS_ENUM("MUX_UL06", "None"),
	RS_ENUM("MUX_UL07", "None"),
	RS_NULL,
};

static struct route_setting g_capture_multimedia_dmic1_mix[] = {
	RS_ENUM("MUX_UL00", "DMic1L"),
	RS_ENUM("MUX_UL01", "DMic1R"),
	RS_ENUM("MUX_UL02", "None"),
	RS_ENUM("MUX_UL03", "None"),
	RS_ENUM("MUX_UL04", "None"),
	RS_ENUM("MUX_UL05", "None"),
	RS_ENUM("MUX_UL06", "None"),
	RS_ENUM("MUX_UL07", "None"),
	RS_NULL,
};

static struct route_setting g_capture_multimedia_dmic2_mix[] = {
	RS_ENUM("MUX_UL00", "DMic2L"),
	RS_ENUM("MUX_UL01", "DMic2R"),
	RS_ENUM("MUX_UL02", "None"),
	RS_ENUM("MUX_UL03", "None"),
	RS_ENUM("MUX_UL04", "None"),
	RS_ENUM("MUX_UL05", "None"),
	RS_ENUM("MUX_UL06", "None"),
	RS_ENUM("MUX_UL07", "None"),
	RS_NULL,
};

static struct route_setting g_capture_multimedia_bluetooth_mix[] = {
	RS_ENUM("MUX_UL00", "BT Left"),
	RS_ENUM("MUX_UL01", "BT Right"),
	RS_ENUM("MUX_UL02", "None"),
	RS_ENUM("MUX_UL03", "None"),
	RS_ENUM("MUX_UL04", "None"),
	RS_ENUM("MUX_UL05", "None"),
	RS_ENUM("MUX_UL06", "None"),
	RS_ENUM("MUX_UL07", "None"),
	RS_NULL,
};

static struct route_setting g_capture_multimedia_echo_mix[] = {
	RS_ENUM("MUX_UL00", "Echo Left"),
	RS_ENUM("MUX_UL01", "Echo Right"),
	RS_ENUM("MUX_UL02", "None"),
	RS_ENUM("MUX_UL03", "None"),
	RS_ENUM("MUX_UL04", "None"),
	RS_ENUM("MUX_UL05", "None"),
	RS_ENUM("MUX_UL06", "None"),
	RS_ENUM("MUX_UL07", "None"),
	RS_NULL,
};

static struct route_setting g_capture_multimedia2_amic_mix[] = {
	RS_ENUM("MUX_UL10", "AMic0"),
	RS_ENUM("MUX_UL11", "AMic1"),
	RS_NULL,
};

static struct route_setting g_capture_multimedia2_dmic0_mix[] = {
	RS_ENUM("MUX_UL10", "DMic0L"),
	RS_ENUM("MUX_UL11", "DMic0R"),
	RS_NULL,
};

static struct route_setting g_capture_multimedia2_dmic1_mix[] = {
	RS_ENUM("MUX_UL10", "DMic1L"),
	RS_ENUM("MUX_UL11", "DMic1R"),
	RS_NULL,
};

static struct route_setting g_capture_multimedia2_dmic2_mix[] = {
	RS_ENUM("MUX_UL10", "DMic2L"),
	RS_ENUM("MUX_UL11", "DMic2R"),
	RS_NULL,
};

static struct route_setting g_capture_multimedia2_bluetooth_mix[] = {
	RS_ENUM("MUX_UL10", "BT Left"),
	RS_ENUM("MUX_UL11", "BT Right"),
	RS_NULL,
};

static struct route_setting g_capture_multimedia2_echo_mix[] = {
	RS_ENUM("MUX_UL10", "Echo Left"),
	RS_ENUM("MUX_UL11", "Echo Right"),
	RS_NULL,
};

static struct route_setting g_capture_voice_amic_mix[] = {
	RS_INT("Voice Capture Mixer Capture", 1),
	RS_ENUM("MUX_VX0", "AMic0"),
	RS_ENUM("MUX_VX1", "AMic1"),
	RS_NULL,
};

static struct route_setting g_capture_voice_dmic0_mix[] = {
	RS_INT("Voice Capture Mixer Capture", 1),
	RS_ENUM("MUX_VX0", "DMic0L"),
	RS_ENUM("MUX_VX1", "DMic0R"),
	RS_NULL,
};

static struct route_setting g_capture_voice_dmic1_mix[] = {
	RS_INT("Voice Capture Mixer Capture", 1),
	RS_ENUM("MUX_VX0", "DMic1L"),
	RS_ENUM("MUX_VX1", "DMic1R"),
	RS_NULL,
};

static struct route_setting g_capture_voice_dmic2_mix[] = {
	RS_INT("Voice Capture Mixer Capture", 1),
	RS_ENUM("MUX_VX0", "DMic2L"),
	RS_ENUM("MUX_VX1", "DMic2R"),
	RS_NULL,
};

static struct route_setting g_capture_voice_bluetooth_mix[] = {
	RS_INT("Voice Capture Mixer Capture", 1),
	RS_ENUM("MUX_VX0", "BT Left"),
	RS_ENUM("MUX_VX1", "BT Right"),
	RS_NULL,
};

static struct route_setting g_capture_voice_echo_mix[] = {
	RS_INT("Voice Capture Mixer Capture", 1),
	RS_ENUM("MUX_VX0", "Echo Left"),
	RS_ENUM("MUX_VX1", "Echo Right"),
	RS_NULL,
};

static struct route_setting g_capture_be_headsetmic_mix[] = {
	RS_INT("AMIC UL Volume", 120),
	RS_ENUM("Analog Left Capture Route", "Headset Mic"),
	RS_ENUM("Analog Right Capture Route", "Headset Mic"),
	RS_INT("Capture Preamplifier Volume", 1),
	RS_INT("Capture Volume", 4),
	RS_NULL,
};

static struct route_setting g_capture_be_onboardmic_mix[] = {
	RS_INT("AMIC UL Volume", 120),
	RS_ENUM("Analog Left Capture Route", "Main Mic"),
	RS_ENUM("Analog Right Capture Route", "Sub Mic"),
	RS_INT("Capture Preamplifier Volume", 1),
	RS_INT("Capture Volume", 4),
	RS_NULL,
};

static struct route_setting g_capture_be_auxfm_mix[] = {
	RS_INT("AMIC UL Volume", 120),
	RS_ENUM("Analog Left Capture Route", "Aux/FM Left"),
	RS_ENUM("Analog Right Capture Route", "Aux/FM Right"),
	RS_INT("Capture Preamplifier Volume", 1),
	RS_INT("Capture Volume", 4),
	RS_NULL,
};

static struct route_setting g_capture_be_dmic0_mix[] = {
	RS_INT("DMIC1 UL Volume", 140),
	RS_NULL,
};

static struct route_setting g_capture_be_dmic1_mix[] = {
	RS_INT("DMIC2 UL Volume", 140),
	RS_NULL,
};

static struct route_setting g_capture_be_dmic2_mix[] = {
	RS_INT("DMIC3 UL Volume", 140),
	RS_NULL,
};

static struct route_setting g_capture_be_bluetooth_mix[] = {
	RS_INT("BT UL Volume", 120),
	RS_INT("DL1 BT_VX Switch", 1),
	RS_NULL,
};

static struct route_setting g_capture_be_echo_mix[] = {
	RS_INT("Echo Mixer DL1", 1),
	RS_INT("Echo Mixer DL2", 1),
	RS_INT("Echo DL1 Volume", 120),
	RS_INT("Echo DL2 Volume", 120),
	RS_NULL,
};

/* side effects: initializes g_abe_api and g_card_mix_defaults */
static int detect_abe_api(struct audio_tool_mixer_cache *cache)
{
	int abe_api = ABE_API_0951;
	int has_aux_switch = 0;
	int count = 0;
	int n;

	/* Detect if using 09.56 API */
	for (n = 0 ; n < cache->count ; ++n) {
		if (0 == strcmp("DL1 PDM_DL2 Switch", cache->ctrls[n].name)) {
			abe_api = ABE_API_0956;
			break;
		}
	}

	/* Detect if have aux switch */
	for (n = 0 ; n < cache->count ; ++n) {
		if (0 == strcmp("Aux Left Playback Switch", cache->ctrls[n].name)) {
			has_aux_switch = 1;
			break;
		}
	}

	count = sizeof(g_defaults_common) / sizeof(g_defaults_common[0]);
	if (has_aux_switch) {
		count += sizeof(g_defaults_aux_switch) / sizeof(g_defaults_aux_switch[0]);
	}
	switch (abe_api) {
	case ABE_API_0951:
		count += sizeof(g_defaults_0951) / sizeof(g_defaults_0951[0]);
		break;
	case ABE_API_0956:
		count += sizeof(g_defaults_0956) / sizeof(g_defaults_0956[0]);
		break;
	default:
		assert(0);
	}

	g_card_mix_defaults.ctrls = calloc(count, sizeof(struct audio_tool_mixer_control_info));
	memcpy(g_card_mix_defaults.ctrls, &g_defaults_common, sizeof(g_defaults_common));
	n = sizeof(g_defaults_common) / sizeof(g_defaults_common[0]);
	if (has_aux_switch) {
		memcpy(g_card_mix_defaults.ctrls + n, &g_defaults_aux_switch,
		       sizeof(g_defaults_aux_switch));
		n += sizeof(g_defaults_aux_switch) / sizeof(g_defaults_aux_switch[0]);
	}
	switch (abe_api) {
	case ABE_API_0951:
		memcpy(g_card_mix_defaults.ctrls + n, &g_defaults_0951, sizeof(g_defaults_0951));
		break;
	case ABE_API_0956:
		memcpy(g_card_mix_defaults.ctrls + n, &g_defaults_0956, sizeof(g_defaults_0956));
		break;
	default:
		assert(0);
	}

	g_card_mix_defaults.count = count;
	g_abe_api = abe_api;


	/* initialize static id's */
	for (n = 0 ; n < g_card_mix_defaults.count ; ++n) {
		g_card_mix_defaults.ctrls[n].id = n;
	}

	/* Set up the routes */
	switch (abe_api) {
	case ABE_API_0951:
		g_playback_multimedia_handsfree_mix = g_playback_multimedia_handsfree_mix_0951;
		g_playback_voice_handsfree_mix = g_playback_voice_handsfree_mix_0951;
		g_playback_tones_handsfree_mix = g_playback_tones_handsfree_mix_0951;
		g_playback_be_headset_mix = g_playback_be_headset_mix_0951;
		g_playback_be_earpiece_mix = g_playback_be_earpiece_mix_0951;
		g_playback_be_handsfree_mix = g_playback_be_handsfree_mix_0951;
		break;
	case ABE_API_0956:
		g_playback_multimedia_handsfree_mix = g_playback_multimedia_handsfree_mix_0956;
		g_playback_voice_handsfree_mix = g_playback_voice_handsfree_mix_0956;
		g_playback_tones_handsfree_mix = g_playback_tones_handsfree_mix_0956;
		g_playback_be_headset_mix = g_playback_be_headset_mix_0956;
		g_playback_be_earpiece_mix = g_playback_be_earpiece_mix_0956;
		g_playback_be_handsfree_mix = g_playback_be_handsfree_mix_0956;
		break;
	default:
		assert(0);
	}

	return 0;
}

static int get_mixer_defaults(struct audio_tool_mixer_cache *cache)
{
	struct audio_tool_mixer_cache *defs = &g_card_mix_defaults;
	int m, n;
	int ret = 0;

	if (g_abe_api == ABE_API_NULL)
		detect_abe_api(cache);

	mixer_cache_reset_touch(defs);
	mixer_cache_reset_touch(cache);

	for (m = 0 ; m < cache->count ; ++m) {
		n = mixer_cache_get_id_by_name(defs,
			cache->ctrls[m].name);
		if (n < 0) {
			fprintf(stderr, "Warning: No default defined for %s\n", cache->ctrls[m].name);
			ret = 1;
			continue;
		}
		if (cache->ctrls[m].type != defs->ctrls[n].type) {
			fprintf(stderr, "Warning: type mismatch on %s\n", cache->ctrls[m].name);
			ret = 1;
			continue;
		}
		memcpy(&cache->ctrls[m].value, &defs->ctrls[n].value,
		       sizeof(g_defaults_common[0].value));

		mixer_cache_touch(cache, m);
		mixer_cache_touch(defs, n);
	}

	ret = mixer_cache_audit_touch(cache, 1) ? 1 : ret;
	ret = mixer_cache_audit_touch(defs, 1) ? 1 : ret;

	return ret;
}

static int set_route_by_array(struct mixer *mixer, struct route_setting *route,
                              int enable)
{
    struct mixer_ctl *ctl;
    unsigned int i, j;

    /* Go through the route array and set each value */
    i = 0;
    while (route[i].ctl_name) {
        ctl = mixer_get_ctl_by_name(mixer, route[i].ctl_name);
        if (!ctl)
            return -EINVAL;

        if (route[i].strval) {
            if (enable)
                mixer_ctl_set_enum_by_string(ctl, route[i].strval);
            else
                mixer_ctl_set_enum_by_string(ctl, "Off");
        } else {
            /* This ensures multiple (i.e. stereo) values are set jointly */
            for (j = 0; j < mixer_ctl_get_num_values(ctl); j++) {
                if (enable)
                    mixer_ctl_set_value(ctl, j, route[i].intval);
                else
                    mixer_ctl_set_value(ctl, j, 0);
            }
        }
        i++;
    }

    return 0;
}

static int get_fe_be_names(int direction, char ***fes, char ***bes)
{
	if (direction == AUDIO_DIRECTION_PLAYBACK) {
		*fes = g_playback_frontends;
		*bes = g_playback_backends;
	} else {
		assert(direction == AUDIO_DIRECTION_CAPTURE);
		*fes = g_capture_frontends;
		*bes = g_capture_backends;
	}
	return 0;
}

#define FE_P_MM 0
#define FE_P_VX 1
#define FE_P_TONES 2
#define FE_P_MMLP 3

#define FE_C_MM 0
#define FE_C_MM2 1
#define FE_C_VX 2

#define BE_P_HS 0
#define BE_P_HF 1
#define BE_P_EP 3
#define BE_P_BT 4

#define BE_C_HSMIC 0
#define BE_C_OBMIC 1
#define BE_C_AUXFM 2
#define BE_C_DMIC0 3
#define BE_C_DMIC1 4
#define BE_C_DMIC2 5
#define BE_C_BT 6
#define BE_C_ECHO 7

static int config_playback(struct mixer *mixer, const char* fe,
	const char* be, int enable, int *optional_port)
{
	int f, b;
	int ret = 0;
	int port;

	if (0 == strcmp(fe, "Multimedia")) {
		f = FE_P_MM;
		port = 0;
	} else if (0 == strcmp(fe, "Voice")) {
		f = FE_P_VX;
		port = 2;
	} else if (0 == strcmp(fe, "Tones")) {
		f = FE_P_TONES;
		port = 3;
	} else if (0 == strcmp(fe, "MultimediaLP")) {
		f = FE_P_MMLP;
		port = 6;
	} else {
		return EINVAL;
	}

	if (optional_port)
		*optional_port = port;

	if (0 == strcmp(be, "Headset")) {
		b = BE_P_HS;
	} else if (0 == strcmp(be, "Handsfree")) {
		b = BE_P_HF;
	} else if (0 == strcmp(be, "Earpiece")) {
		b = BE_P_EP;
	} else if (0 == strcmp(be, "Bluetooth")) {
		b = BE_P_BT;
	} else {
		return EINVAL;
	}

	switch (f) {
	case FE_P_MM:
	case FE_P_MMLP:
		if (b == BE_P_HF) {
			ret = set_route_by_array(mixer,
					g_playback_multimedia_handsfree_mix, enable);
		} else {
			ret = set_route_by_array(mixer,
					g_playback_multimedia_accessory_mix, enable);
		}
		break;
	case FE_P_VX:
		if (b == BE_P_HF) {
			ret = set_route_by_array(mixer,
					g_playback_voice_handsfree_mix, enable);
		} else {
			ret = set_route_by_array(mixer,
					g_playback_voice_accessory_mix, enable);
		}
		break;
	case FE_P_TONES:
		if (b == BE_P_HF) {
			ret = set_route_by_array(mixer,
					g_playback_tones_handsfree_mix, enable);
		} else {
			ret = set_route_by_array(mixer,
					g_playback_tones_accessory_mix, enable);
		}
		break;
	default:
		assert(0);
	}

	if (ret)
		return ret;

	switch (b) {
	case BE_P_HS:
		ret = set_route_by_array(mixer,
				g_playback_be_headset_mix, enable);
		break;
	case BE_P_HF:
		ret = set_route_by_array(mixer,
				g_playback_be_handsfree_mix, enable);
		break;
	case BE_P_EP:
		ret = set_route_by_array(mixer,
				g_playback_be_earpiece_mix, enable);
		break;
	case BE_P_BT:
		ret = set_route_by_array(mixer,
				g_playback_be_bluetooth_mix, enable);
		break;
	default:
		assert(0);
	}

	return ret;
}

static int config_capture(struct mixer *mixer, const char* fe,
	const char* be, int enable, int *optional_port)
{
	int f, b;
	int ret = 0;
	int port;

	if (0 == strcmp(fe, "Multimedia")) {
		f = FE_C_MM;
		port = 0;
	} else if (0 == strcmp(fe, "Multimedia2")) {
		f = FE_C_MM2;
		port = 1;
	} else if (0 == strcmp(fe, "Voice")) {
		f = FE_C_VX;
		port = 2;
	} else {
		return EINVAL;
	}

	if (0 == strcmp(be, "HeadsetMic")) {
		b = BE_C_HSMIC;
	} else if (0 == strcmp(be, "OnboardMic")) {
		b = BE_C_OBMIC;
	} else if (0 == strcmp(be, "Aux/FM")) {
		b = BE_C_AUXFM;
	} else if (0 == strcmp(be, "DMic0")) {
		b = BE_C_DMIC0;
	} else if (0 == strcmp(be, "DMic1")) {
		b = BE_C_DMIC1;
	} else if (0 == strcmp(be, "DMic2")) {
		b = BE_C_DMIC2;
	} else if (0 == strcmp(be, "Bluetooth")) {
		b = BE_C_BT;
	} else if (0 == strcmp(be, "Echo")) {
		b = BE_C_ECHO;
	} else {
		return EINVAL;
	}

	switch (f) {
	case FE_C_MM:
		switch (b) {
		case BE_C_HSMIC:
		case BE_C_OBMIC:
		case BE_C_AUXFM:
			ret = set_route_by_array(mixer,
					g_capture_multimedia_amic_mix, enable);
			break;
		case BE_C_DMIC0:
			ret = set_route_by_array(mixer,
					 g_capture_multimedia_dmic0_mix, enable);
			break;
		case BE_C_DMIC1:
			ret = set_route_by_array(mixer,
					 g_capture_multimedia_dmic1_mix, enable);
			break;
		case BE_C_DMIC2:
			ret = set_route_by_array(mixer,
					 g_capture_multimedia_dmic2_mix, enable);
			break;
		case BE_C_BT:
			ret = set_route_by_array(mixer,
					 g_capture_multimedia_bluetooth_mix, enable);
			break;
		case BE_C_ECHO:
			ret = set_route_by_array(mixer,
					g_capture_multimedia_echo_mix, enable);
			break;
		}
		break;
	case FE_C_MM2:
		switch (b) {
		case BE_C_HSMIC:
		case BE_C_OBMIC:
		case BE_C_AUXFM:
			ret = set_route_by_array(mixer,
					 g_capture_multimedia2_amic_mix, enable);
			break;
		case BE_C_DMIC0:
			ret = set_route_by_array(mixer,
					 g_capture_multimedia2_dmic0_mix, enable);
			break;
		case BE_C_DMIC1:
			ret = set_route_by_array(mixer,
					 g_capture_multimedia2_dmic1_mix, enable);
			break;
		case BE_C_DMIC2:
			ret = set_route_by_array(mixer,
					 g_capture_multimedia2_dmic2_mix, enable);
			break;
		case BE_C_BT:
			ret = set_route_by_array(mixer,
					 g_capture_multimedia2_bluetooth_mix, enable);
			break;
		case BE_C_ECHO:
			ret = set_route_by_array(mixer,
					g_capture_multimedia2_echo_mix, enable);
			break;
		}
		break;
	case FE_C_VX:
		switch (b) {
		case BE_C_HSMIC:
		case BE_C_OBMIC:
		case BE_C_AUXFM:
			ret = set_route_by_array(mixer,
					 g_capture_voice_amic_mix, enable);
			break;
		case BE_C_DMIC0:
			ret = set_route_by_array(mixer,
					 g_capture_voice_dmic0_mix, enable);
			break;
		case BE_C_DMIC1:
			ret = set_route_by_array(mixer,
					 g_capture_voice_dmic1_mix, enable);
			break;
		case BE_C_DMIC2:
			ret = set_route_by_array(mixer,
					 g_capture_voice_dmic2_mix, enable);
			break;
		case BE_C_BT:
			ret = set_route_by_array(mixer,
					 g_capture_voice_bluetooth_mix, enable);
			break;
		case BE_C_ECHO:
			ret = set_route_by_array(mixer,
					g_capture_voice_echo_mix, enable);
			break;
		}
		break;
	default:
		assert(0);
	}

	if (ret)
		return ret;

	switch (b) {
	case BE_C_HSMIC:
		ret = set_route_by_array(mixer,
				g_capture_be_headsetmic_mix, enable);
		break;
	case BE_C_OBMIC:
		ret = set_route_by_array(mixer,
				g_capture_be_onboardmic_mix, enable);
		break;
	case BE_C_AUXFM:
		ret = set_route_by_array(mixer,
				g_capture_be_auxfm_mix, enable);
		break;
	case BE_C_DMIC0:
		ret = set_route_by_array(mixer,
				g_capture_be_dmic0_mix, enable);
		break;
	case BE_C_DMIC1:
		ret = set_route_by_array(mixer,
				g_capture_be_dmic1_mix, enable);
		break;
	case BE_C_DMIC2:
		ret = set_route_by_array(mixer,
				g_capture_be_dmic2_mix, enable);
		break;
	case BE_C_BT:
		ret = set_route_by_array(mixer,
				g_capture_be_bluetooth_mix, enable);
		break;
	case BE_C_ECHO:
		ret = set_route_by_array(mixer,
				g_capture_be_echo_mix, enable);
		break;
	default:
		assert(0);
	}

	return ret;
}

static int config(struct mixer *mixer, int direction, const char* fe,
	const char* be, int enable, int *optional_port)
{
	if (g_abe_api == ABE_API_NULL) {
		struct audio_tool_mixer_cache cache;
		mixer_cache_init(&cache);
		if(mixer_cache_populate(&cache, mixer)) {
			fprintf(stderr, "Error: could not populate mixer cache for card\n");
			return ENODEV;
		}
		if(detect_abe_api(&cache)) {
			fprintf(stderr, "Error: could not detect ABE version\n");
			return ENODEV;
		}
		mixer_cache_deinit(&cache);
	}

	if (direction == AUDIO_DIRECTION_PLAYBACK) {
		return config_playback(mixer, fe, be, enable, optional_port);
	} else {
		return config_capture(mixer, fe, be, enable, optional_port);
	}
}
