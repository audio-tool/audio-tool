/*
 * mixer_cache.c
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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <tinyalsa/asoundlib.h>

#include "mixer_cache.h"

int mixer_cache_init(struct audio_tool_mixer_cache *cache)
{
	if (!cache)
		return EINVAL;

	memset(cache, 0, sizeof(struct audio_tool_mixer_cache));

	return 0;
}

void mixer_cache_deinit(struct audio_tool_mixer_cache *cache)
{
	if (!cache)
		return;
	if (!cache->ctrls)
		return;
	free(cache->ctrls);
}

int mixer_cache_populate(struct audio_tool_mixer_cache *cache, struct mixer *mixer)
{
	struct mixer_ctl *ctl;
	struct audio_tool_mixer_control_info *cur;
	const char* name;
	size_t count, n, v;
	int tmp;

	if (!cache)
		return EINVAL;

	if (!mixer)
		return EINVAL;

	if (cache->ctrls)
		free(cache->ctrls);

	count = mixer_get_num_ctls(mixer);
	cache->count = count;

	if (count) {
		cache->ctrls = calloc(count, sizeof(struct audio_tool_mixer_control_info));
		if (! cache->ctrls)
			return -ENOMEM;
	}

	for (n = 0, cur = cache->ctrls ; n < count ; ++n, ++cur) {
		ctl = mixer_get_ctl(mixer, n);
		if (!ctl)
			return ENODEV;
		name = mixer_ctl_get_name(ctl);
		if (!name)
			return ENODEV;

		cur->id = n;
		cur->type = mixer_ctl_get_type(ctl);
		strcpy(cur->name, name);
		cur->num_values = mixer_ctl_get_num_values(ctl);
		if (cur->num_values > MAX_NUM_VALUES)
			cur->num_values = MAX_NUM_VALUES;
		for (v = 0 ; v < cur->num_values ; ++v) {
			switch (cur->type) {
			case MIXER_CTL_TYPE_BOOL:
			case MIXER_CTL_TYPE_INT:
				cur->value.integer[v] = mixer_ctl_get_value(ctl, v);
				break;
			case MIXER_CTL_TYPE_ENUM:
				tmp = mixer_ctl_get_value(ctl, v);
				name = mixer_ctl_get_enum_string(ctl, tmp);
				strcpy(cur->value.enumerated[v], name);
				break;
			case MIXER_CTL_TYPE_BYTE:
				cur->value.byte[v] = mixer_ctl_get_value(ctl, v);
				break;
			case MIXER_CTL_TYPE_INT64:
				cur->value.integer64[v] = mixer_ctl_get_value(ctl, v);
				break;
			default:
				(void)0;
			}
		}
	}

	return 0;
}

int mixer_cache_apply(struct audio_tool_mixer_cache *cache, struct mixer *mixer)
{
	struct audio_tool_mixer_control_info *cur;
	struct mixer_ctl *ctl;
	int n, k, ret=0;

	for (n = 0 ; n < cache->count ; ++n) {
		cur = &cache->ctrls[n];
		ctl = mixer_get_ctl(mixer, n);
		for (k = 0 ; k < cur->num_values ; ++k) {
			switch (cur->type) {
			case MIXER_CTL_TYPE_BOOL:
			case MIXER_CTL_TYPE_INT:
				mixer_ctl_set_value(ctl, k, cur->value.integer[k]);
				break;
			case MIXER_CTL_TYPE_ENUM:
				mixer_ctl_set_enum_by_string(ctl, cur->value.enumerated[k]);
				break;
			case MIXER_CTL_TYPE_BYTE:
				mixer_ctl_set_value(ctl, k, cur->value.byte[k]);
				break;
			case MIXER_CTL_TYPE_INT64:
				mixer_ctl_set_value(ctl, k, cur->value.integer64[k]);
				break;
			default:
				ret = 1;
			}
		}
	}

	return ret;
}

int mixer_cache_get_id_by_name(struct audio_tool_mixer_cache *cache, const char* name)
{
	struct audio_tool_mixer_control_info *cur;

	if (!cache)
		return -EINVAL;
	if (!name)
		return -EINVAL;

	for (cur = cache->ctrls ; cur < (cache->ctrls + cache->count) ; ++cur) {
		if (strcmp(cur->name, name) == 0)
			return cur->id;
	}

	return -ENODEV;
}

void mixer_cache_reset_touch(struct audio_tool_mixer_cache *cache)
{
	struct audio_tool_mixer_control_info *cur;

	if (!cache)
		return;

	for (cur = cache->ctrls ; cur < (cache->ctrls + cache->count) ; ++cur) {
		cur->touch = 0;
	}
}

void mixer_cache_touch(struct audio_tool_mixer_cache *cache, int id)
{
	if (!cache)
		return;
	if (id < 0)
		return;
	if (id > cache->count)
		return;

	cache->ctrls[id].touch = 1;
}

int mixer_cache_audit_touch(struct audio_tool_mixer_cache *cache, int verbose)
{
	struct audio_tool_mixer_control_info *cur;
	int ret = 0;

	if (!cache)
		return EINVAL;

	for (cur = cache->ctrls ; cur < (cache->ctrls + cache->count) ; ++cur) {
		if (!cur->touch) {
			ret = 1;
			if (verbose) {
				fprintf(stderr, "Warning: Control #%d (%s) was not touched\n",
					cur->id, cur->name);
			}
		}
	}

	return ret;
}

