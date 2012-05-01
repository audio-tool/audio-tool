/*
 * card-sdp4430.c
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

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <tinyalsa/asoundlib.h>

#include "module.h"
#include "alsa-control.h"
#include "mixer_cache.h"

#define CARD_SDP4430_CARD_NAME "SDP4430"

#include "card-omap-common-4-5.h"


static int probe(void)
{
	int card = -ENODEV;

	card = ah_card_find_by_name(CARD_SDP4430_CARD_NAME);

	if (card >= 0)
		return 0;

	return ENODEV;

}

static struct audio_tool_card_module g_sdp4430_mod = {
	.type = AUDIO_TOOL_MOD_TYPE_CARD,
	.name = CARD_SDP4430_CARD_NAME,
	.probe = probe,
	.get_mixer_defaults = get_mixer_defaults,
	.get_fe_be_names = get_fe_be_names,
	.config = config,
};

static void __init init(void)
{
	int ret;

	ret = audio_tool_module_register((struct audio_tool_module*)&g_sdp4430_mod); 
	if (ret) {
		fprintf(stderr, "Error: could not register module %s (%s)\n",
			g_sdp4430_mod.name, strerror(ret));
	}
}

