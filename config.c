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
#include "cmdline.h"

#include <assert.h>

int parse_args(struct audio_tool_config *conf, int *argc, char*** argv)
{
	int ret;
	struct gengetopt_args_info args_info;

	assert(conf);

	ret = cmdline_parser(*argc, *argv, &args_info);

	if (!ret) {
		conf->card = args_info.card_arg;
		conf->device = args_info.device_arg;
		conf->period_size = args_info.periods_arg;
		conf->num_periods = args_info.num_periods_arg;
		conf->duration = args_info.time_arg;
		conf->channels = args_info.channels_arg;
		conf->channel_mask = (uint32_t) args_info.channel_mask_arg;
		conf->bits = args_info.bits_arg;
		conf->rate = args_info.rate_arg;

		*argc = args_info.inputs_num;
		*argv = args_info.inputs;
	}

	return ret;
}
