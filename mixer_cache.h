/*
 * mixer_cache.h
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

#ifndef __AUDIO_TOOL_MIXER_CACHE_H__
#define __AUDIO_TOOL_MIXER_CACHE_H__

#include <stdint.h>
#include <tinyalsa/asoundlib.h>

/* In kernel this is typically 44 */
#define AUDIO_TOOL_MIX_CTL_NAME_MAX 128

struct audio_tool_mixer_control_info;
struct audio_tool_mixer_cache;

/* Returns 0 on success, errno on failure*/
int mixer_cache_init(struct audio_tool_mixer_cache *cache);
void mixer_cache_deinit(struct audio_tool_mixer_cache *cache);
/* Returns 0 on success, errno on failure */
int mixer_cache_populate(struct audio_tool_mixer_cache *cache, struct mixer *mixer);
/* Returns control id (>=0), or negative errno on failure */
int mixer_cache_get_id_by_name(struct audio_tool_mixer_cache *cache, const char* name);
void mixer_cache_reset_touch(struct audio_tool_mixer_cache *cache);
void mixer_cache_touch(struct audio_tool_mixer_cache *cache, int id);
int mixer_cache_audit_touch(struct audio_tool_mixer_cache *cache, int verbose);
int mixer_cache_apply(struct audio_tool_mixer_cache *cache, struct mixer *mixer);

#define MAX_NUM_VALUES 8

struct audio_tool_mixer_control_info {
	unsigned id;
	char name[AUDIO_TOOL_MIX_CTL_NAME_MAX];
	enum mixer_ctl_type type;
	unsigned num_values;
	union {
		long integer[128];
		int64_t integer64[64];
		char enumerated[MAX_NUM_VALUES][64];
		unsigned char byte[512];
	} value;

	int touch; /* For use with full-list processing */
};

struct audio_tool_mixer_cache {
	size_t count;
	struct audio_tool_mixer_control_info *ctrls;
};

#endif /* __OMAP_AUDIO_TOOL_MIXER_CACHE_H__ */

