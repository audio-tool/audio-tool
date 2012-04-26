/*
 * module.h
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

#ifndef __AUDIO_TOOL_MODULE_H__
#define __AUDIO_TOOL_MODULE_H__

struct audio_tool_module;
struct audio_tool_board_module;
struct audio_tool_mixer_cache;

#ifndef __GNUC__
#error The module interface requires GCCs constructor/destructor attributes.
#endif

#define __init __attribute__((constructor))
#define __exit __attribute__((destructor))

int audio_tool_module_register(struct audio_tool_module *mod);

/* Board module access */
struct audio_tool_module* audio_tool_get_module(int module_type,
	const char* optional_name);

/* Module types */
#define AUDIO_TOOL_MOD_TYPE_CARD	1

#define AUDIO_TOOL_MOD_TYPE_MAX		AUDIO_TOOL_MOD_TYPE_CARD

struct audio_tool_module {
	int type;
	const char* name;

	/* Required: return non-zero if the module supports the current device */
	int (*probe)(void);
};

struct audio_tool_card_module {
	int type;
	const char* name;

	/* Required: return zero if the module supports the current device */
	int (*probe)(void);

	/* Required: Takes an existing mixer_cache and populates
	 * it with default values.  Returns 0 on success.
	 */
	int (*get_mixer_defaults)(struct audio_tool_mixer_cache *cache);
};

#endif /* __OMAP_AUDIO_TOOL_MODULE_H__ */

