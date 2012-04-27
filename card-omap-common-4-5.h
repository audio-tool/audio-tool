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

/*
 * Code common between cards SDP4430 and OMAP45
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <tinyalsa/asoundlib.h>

#include "alsa-control.h"
#include "mixer_cache.h"

static struct audio_tool_mixer_control_info g_defaults[] = {
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
		.name = "DL2 Mono Mixer",
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
		.name = "DL1 PDM Switch",
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

static struct audio_tool_mixer_cache g_card_mix_defaults = {
	.count = sizeof(g_defaults) / sizeof(g_defaults[0]),
	.ctrls = (struct audio_tool_mixer_control_info*)&g_defaults,
};

static int get_mixer_defaults(struct audio_tool_mixer_cache *cache)
{
	struct audio_tool_mixer_cache *defs = &g_card_mix_defaults;
	size_t m, n;
	int ret = 0;

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
		       sizeof(g_defaults[0].value));

		mixer_cache_touch(cache, m);
		mixer_cache_touch(defs, n);
	}

	ret = mixer_cache_audit_touch(cache, 1) ? ret : 1;
	ret = mixer_cache_audit_touch(defs, 1) ? ret : 1;

	return ret;
}

#endif /* __AUDIO_TOOL_CARD_OMAP_COMMON_4_5_H__ */
