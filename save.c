/*
 * save.c
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

#include <stdio.h>
#include <tinyalsa/asoundlib.h>

#include "config.h"
#include "save.h"

int save_main(const struct audio_tool_config *config, int argc, char **argv)
{
	struct mixer *mixer;
	int card = config->card;
	char *filename = 0;
	FILE *dest;
	unsigned count, n;

	if (argc != 2) {
		printf("Usage: audio-tool save <filename>\n");
		return 1;
	}

	filename = argv[1];

	mixer = mixer_open(card);
	if (!mixer) {
		printf("Could not open mixer for card %d\n", card);
		return 1;
	}

	dest = fopen(filename, "wt");
	if (!dest) {
		printf("Could not open file %s for writing\n", filename);
		return 1;
	}

	fprintf(dest, "# Format: CTL_NAME<tab>CTL_TYPE<tab>NUM_VALS<tab>VAL1"
		"<tab>VAL2...\n");
	count = mixer_get_num_ctls(mixer);
	for (n = 0 ; n < count ; ++n) {
		struct mixer_ctl *ctl;
		const char *ctl_name, *ctl_type;
		enum mixer_ctl_type type;
		unsigned val, num_vals, k;

		ctl = mixer_get_ctl(mixer, n);
		ctl_name = mixer_ctl_get_name(ctl);
		ctl_type = mixer_ctl_get_type_string(ctl);
		num_vals = mixer_ctl_get_num_values(ctl);
		type = mixer_ctl_get_type(ctl);
		fprintf(dest, "%s\t%s\t%u", ctl_name, ctl_type, num_vals);

		for (k = 0 ; k < num_vals ; ++k) {
			switch(type) {
			case MIXER_CTL_TYPE_INT:
			case MIXER_CTL_TYPE_BOOL:
			case MIXER_CTL_TYPE_BYTE:
				fprintf(dest, "\t%u", mixer_ctl_get_value(ctl, k));
				break;
			case MIXER_CTL_TYPE_ENUM:
				val = mixer_ctl_get_value(ctl, k);
				fprintf(dest, "\t%s", mixer_ctl_get_enum_string(ctl, val));
				break;
			default:
				fprintf(dest, "\t#N/A");
			}
		}
		fprintf(dest, "\n");
	}

	fclose(dest);
	mixer_close(mixer);

	return 0;
}

