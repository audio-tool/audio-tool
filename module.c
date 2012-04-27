/*
 * module.c
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

#include "module.h"

struct module_node {
	struct module_node *next;
	struct audio_tool_module *module;
};

struct module_node *g_module_list = 0;

static void module_list_push_back(struct module_node *list, struct module_node *node)
{
	if (!list)
		return;

	while (list->next)
		list = list->next;

	list->next = node;
}

int audio_tool_module_register(struct audio_tool_module *mod)
{
	struct module_node *node;

	if (!mod)
		return EINVAL;

	node = calloc(1, sizeof(struct module_node));

	if (!node)
		return ENOMEM;

	node->module = mod;

	if (!g_module_list) {
		g_module_list = node;
	} else {
		module_list_push_back(g_module_list, node);
	}

	return 0;
}

struct audio_tool_module* audio_tool_get_module(int module_type,
	const char* optional_name)
{
	struct module_node *node = g_module_list;

	if (!module_type || module_type > AUDIO_TOOL_MOD_TYPE_MAX)
		return NULL;

	for ( ; node ; node = node->next ) {
		if (node->module->type != module_type)
			continue;

		if (optional_name) {
			if(!strcmp(node->module->name, optional_name))
				break;
		} else {
			if (!node->module->probe())
				break;
		}
	}

	if (node)
		return node->module;

	return 0;
}
