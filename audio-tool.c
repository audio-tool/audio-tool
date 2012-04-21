/*
 * Copyright (C) 2012 Texas Instruments
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "config.h"

#include <stdio.h>
#include <string.h>

#include "tinyplay.h"
#include "tinycap.h"
#include "tinymix.h"
#include "pulse-generator.h"

/* defined in config.c */
int parse_args(struct audio_tool_config *config, int *argc, char ***argv);
void usage(void);

int main(int argc, char* argv[])
{
	struct audio_tool_config config;
	int ret;

	/* redefines argc and argv */
	ret = parse_args(&config, &argc, &argv);

	if (!ret && argc) {
		if (strcmp(argv[0], "play") == 0) {
			ret = tinyplay_main(&config, argc, argv);
		} else if (strcmp(argv[0], "cap") == 0
			   || strcmp(argv[0], "capture") == 0) {
			ret = tinycap_main(&config, argc, argv);
		} else if (strcmp(argv[0], "mix") == 0) {
			ret = tinymix_main(&config, argc, argv);
		} else if (strcmp(argv[0], "pulse") == 0) {
			ret = pulse_generator_main(&config, argc, argv);
		} else {
			usage();
			ret = 1;
		}
	}

	return ret;
}
