/*
 * alsa-control.h
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

#ifndef __ALSA_CONTROL_H__
#define __ALSA_CONTROL_H__

#include "alsa-global.h"

#include <string.h>
#include <time.h>
/*#include <linux/time.h>*/
#include <sound/asound.h>

/* Typedefs for kernel structs.  See <sound/asound.h>
 */
typedef struct snd_ctl_card_info snd_ctl_card_info_t;
typedef struct snd_ctl_elem_id snd_ctl_elem_id_t;
typedef struct snd_ctl_elem_info snd_ctl_elem_info_t;
typedef struct snd_ctl_elem_list snd_ctl_elem_list_t;
typedef struct snd_ctl_elem_value snd_ctl_elem_value_t;

int ah_card_max_count();
int ah_card_count();
int ah_card_get_name(int card, char *str, size_t strlen);
int ah_card_find_by_name(const char* name);

int ah_control_open(int card, int mode);
int ah_control_close(int fd);
int ah_card_get_info(int fd, snd_ctl_card_info_t *info);


#endif /* __ALSA_CONTROL_H__ */
