/*
 * audio-tool.c
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

#include "config.h"

#include <stdio.h>
#include <string.h>
#include <libgen.h>

#include "tinyplay.h"
#include "tinycap.h"
#include "tinymix.h"
#include "pulse-generator.h"
#include "tone-generator.h"
#include "save.h"
#include "restore.h"
#include "defaults.h"
#include "config_cmd.h"

/* defined in config.c */
int parse_args(struct audio_tool_config *config, int *argc, char ***argv);
void usage(void);

int main(int argc, char* argv[])
{
	struct audio_tool_config config;
	int ret;
	int tinyplay = 0, tinycap = 0, tinymix = 0;

	/* support backwards-compatible mode with the tinyutils */
	if (strcmp(basename(argv[0]), "tinyplay") == 0) {
		tinyplay = 1;
	} else if (strcmp(basename(argv[0]), "tinycap") == 0) {
		tinycap = 1;
	} else if (strcmp(basename(argv[0]), "tinymix") == 0) {
		tinymix = 1;
	}

	/* redefines argc and argv */
	ret = parse_args(&config, &argc, &argv);

	if (!ret && (argc | tinyplay | tinycap | tinymix)) {
		/* Check for special modes first since argv[0] may be NULL */
		if (tinyplay) {
			ret = tinyplay_main(&config, argc, argv, tinyplay);
		} else if (tinycap) {
			ret = tinycap_main(&config, argc, argv, tinycap);
		} else if (tinymix) {
			ret = tinymix_main(&config, argc, argv, tinymix);

		/* normal modes */
		} else if (strcmp(argv[0], "play") == 0) {
			ret = tinyplay_main(&config, argc, argv, tinyplay);
		} else if (strcmp(argv[0], "cap") == 0
			   || strcmp(argv[0], "capture") == 0) {
			ret = tinycap_main(&config, argc, argv, tinycap);
		} else if (strcmp(argv[0], "mix") == 0) {
			ret = tinymix_main(&config, argc, argv, tinymix);
		} else if (strcmp(argv[0], "pulse") == 0) {
			ret = pulse_generator_main(&config, argc, argv);
		} else if (strcmp(argv[0], "tone") == 0) {
			ret = tone_generator_main(&config, argc, argv);
		} else if (strcmp(argv[0], "save") == 0) {
			ret = save_main(&config, argc, argv);
		} else if (strcmp(argv[0], "restore") == 0) {
			ret = restore_main(&config, argc, argv);
		} else if (strcmp(argv[0], "defaults") == 0) {
			ret = defaults_main(&config, argc, argv);
		} else if (strcmp(argv[0], "config") == 0) {
			ret = config_cmd_main(&config, argc, argv);
		} else {
			usage();
			ret = 1;
		}
	}

	return ret;
}
