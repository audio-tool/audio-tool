/*
 * defaults.c
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
#include <tinyalsa/asoundlib.h>

#include "defaults.h"
#include "config.h"
#include "alsa-control.h"
#include "mixer_cache.h"
#include "module.h"

#define BUFSIZE 512

int defaults_main(const struct audio_tool_config *config, int argc, char **argv)
{
	struct audio_tool_card_module *card_mod;
	struct mixer *mixer;
	struct audio_tool_mixer_cache cache;
	char buf[BUFSIZE] = "";
	int card = config->card;
	int cards = ah_card_count();
	int ret = 0;

	if (card < 0 || card >= cards) {
		fprintf(stderr, "Error: card %d does not exist\n", card);
		ret = 1;
		goto end;
	}

	ret = ah_card_get_name(card, buf, BUFSIZE);
	if (ret) {
		fprintf(stderr, "Error: can't get name of card %d (%s)",
			card, strerror(-ret));
		ret = 1;
		goto end;
	}

	card_mod = (struct audio_tool_card_module*)audio_tool_get_module(
		AUDIO_TOOL_MOD_TYPE_CARD, buf);
	if (!card_mod) {
		fprintf(stderr, "Error: could not find the defaults for this card (%s)\n",
			buf);
		ret = 1;
		goto end;
	}

	mixer = mixer_open(card);
	if (!mixer) {
		fprintf(stderr, "Error: could not open mixer device (%s)\n",
			strerror(errno));
		ret = 1;
		goto end;
	}

	ret = mixer_cache_init(&cache);
	if (ret) {
		fprintf(stderr, "Error: could not initialize the mixer cache (%s)\n",
			strerror(ret));
		ret = 1;
		goto mixer_cache_err;
	}

	ret = mixer_cache_populate(&cache, mixer);
	if (ret) {
		fprintf(stderr, "Error: could not populate the mixer cache (%s)\n",
			strerror(-ret));
		ret = 1;
		goto mixer_cache_pop_err;
	}

	if (card_mod->get_mixer_defaults(&cache)) {
		fprintf(stderr, "Warning: mixer defaults mismatched\n");
	}

	if (mixer_cache_apply(&cache, mixer)) {
		fprintf(stderr, "Error: could not apply mixer setting\n");
		ret = 1;
		goto mixer_cache_pop_err;
	}

mixer_cache_pop_err:
	mixer_cache_deinit(&cache);
mixer_cache_err:
	mixer_close(mixer);
end:

	return ret;
}
