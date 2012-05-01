/*
 * config_cmd.c
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

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <tinyalsa/asoundlib.h>

#include "config.h"
#include "config_cmd.h"
#include "module.h"
#include "alsa-control.h"

#define BUFSIZE 512

static void usage(int card)
{
	char **p_fes, **p_bes, **c_fes, **c_bes;
	char cardname[BUFSIZE];
	struct audio_tool_card_module *mod;
	int ret;

	ret = ah_card_get_name(card, cardname, BUFSIZE);
	if (ret) {
		fprintf(stderr, "Error: couldn't get name for card %d (%s)\n",
			card, strerror(-ret));
		return;
	}

	mod = (struct audio_tool_card_module*)audio_tool_get_module(
		AUDIO_TOOL_MOD_TYPE_CARD, cardname);
	if (!mod) {
		fprintf(stderr, "Error: couldn't get module for card %d\n",
			card);
		return;
	}

	ret = mod->get_fe_be_names(AUDIO_DIRECTION_PLAYBACK, &p_fes, &p_bes);
	if (ret) {
		fprintf(stderr, "Error: couldn't get listing of FE's/BE's for card\n");
		return;
	}
	ret = mod->get_fe_be_names(AUDIO_DIRECTION_CAPTURE, &c_fes, &c_bes);
	if (ret) {
		fprintf(stderr, "Error: couldn't get listing of FE's/BE's for card\n");
		return;
	}

	printf("Usage: audio-tool config <play|cap> <frontend> <backend> <1|0|disable|enable>\n");
	printf("\n");
	printf("For card %d, the options are:\n", card);
	printf("PLAYBACK (play):\n");
	printf("  Frontends:");
	while (*p_fes) {
		printf(" %s", *p_fes);
		++p_fes;
	}
	printf("\n");
	printf("  Backends:");
	while (*p_bes) {
		printf(" %s", *p_bes);
		++p_bes;
	}
	printf("\n");
	printf("CAPTURE (cap):\n");
	mod->get_fe_be_names(AUDIO_DIRECTION_CAPTURE, &c_fes, &c_bes);
	printf("  Frontends:");
	while (*c_fes) {
		printf(" %s", *c_fes);
		++c_fes;
	}
	printf("\n");
	printf("  Backends:");
	while (*c_bes) {
		printf(" %s", *c_bes);
		++c_bes;
	}
	printf("\n");
	printf("\n");
	printf("To see options for a different card, use the -D option\n");
}

int config_cmd_main(const struct audio_tool_config *config, int argc, char **argv)
{
	struct mixer *mixer;
	struct audio_tool_card_module *mod;
	char *dir_arg, *fe_arg, *be_arg, *enable_arg;
	char **fes, **bes;
	char cardname[BUFSIZE];
	int card = config->card;
	int ret = 0;
	int direction;
	int port = 0;
	int enable;

	if (argc < 5) {
		usage(card);
		return 0;
	}

	dir_arg = argv[1];
	fe_arg = argv[2];
	be_arg = argv[3];
	enable_arg = argv[4];

	if (0 == strcmp(dir_arg, "play")) {
		direction = AUDIO_DIRECTION_PLAYBACK;
	} else if (0 == strcmp(dir_arg, "cap")) {
		direction = AUDIO_DIRECTION_CAPTURE;
	} else {
		fprintf(stderr, "Error: %s is not 'play' or 'cap'\n", dir_arg);
		usage(card);
		return 1;
	}

	if ((0 == strcmp(enable_arg, "1")) || (0 == strcmp(enable_arg, "enable"))) {
		enable = 1;
	} else if ((0 == strcmp(enable_arg, "0")) || (0 == strcmp(enable_arg, "disable"))) {
		enable = 0;
	} else {
		fprintf(stderr, "Error: '%s' is not '0', '1', 'disable', or 'enable'\n",
			enable_arg);
		return 1;
	}

	ret = ah_card_get_name(card, cardname, BUFSIZE);
	if (ret) {
		fprintf(stderr, "Error: couldn't get name for card %d (%s)\n",
			card, strerror(-ret));
		return ret;
	}

	mod = (struct audio_tool_card_module*)audio_tool_get_module(
		AUDIO_TOOL_MOD_TYPE_CARD, cardname);
	if (!mod) {
		fprintf(stderr, "Error: couldn't get module for card %d\n",
			card);
		return ret;
	}

	ret = EINVAL;
	if (direction == AUDIO_DIRECTION_PLAYBACK) {
		ret = mod->get_fe_be_names(AUDIO_DIRECTION_PLAYBACK, &fes, &bes);
	} else {
		ret = mod->get_fe_be_names(AUDIO_DIRECTION_CAPTURE, &fes, &bes);
	}
	if (ret) {
		fprintf(stderr, "Error: could not get frontends and backends for this device\n");
		return ret;
	}

	/* check that user args are in fe/be lists */
	while (*fes) {
		if (0 == strcmp(*fes, fe_arg))
			break;
		++fes;
	}
	if (!*fes) {
		fprintf(stderr, "Error: '%s' is not a supported frontend\n", fe_arg);
		return 1;
	}
	while (*bes) {
		if (0 == strcmp(*bes, be_arg))
			break;
		++bes;
	}
	if (!*bes) {
		fprintf(stderr, "Error: '%s' is not a supported backend\n", be_arg);
		return 1;
	}

	mixer = mixer_open(card);
	if (!mixer) {
		fprintf(stderr, "Error: could not open mixer for card %d\n", card);
		return 1;
	}

	ret = mod->config(mixer, direction, fe_arg, be_arg, enable, &port);

	if (ret) {
		fprintf(stderr, "Error: could not configure path (%s)\n", strerror(-ret));
	} else {
		printf("Path configured. Use card %d port %d\n", card, port);
	}

	return ret;
}

