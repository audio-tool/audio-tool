/* Copyright (c) 2011, Gabriel M. Beddingfield <gabrbedd@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * tone-generator.c
 *
 * Utility for generating an accurate waveform to an audio output.
 */

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <tinyalsa/asoundlib.h>
#include "oscillator-table.h"

#include "config.h"
#include "tone-generator.h"

/* LOAD ALL THE WAVE TABLES
 *
 * WAVE TABLE ASSUMPTIONS:
 *
 *  - All lengths are a power of 2 (8, 16, 32, 64, ...)
 *
 *  - The wave tables are sufficiently large such that
 *    interpolation is not necessary.  For example, at a
 *    sample rate of 48000 Hz, a wave table length 4096
 *    should be sufficient for all frequencies above
 *    24 Hz (48000/4096 = 11.72 Hz).
 */
static int16_t g_table_square_wave_data[] = {
#include "table_square.c"
};

static int16_t g_table_sine_wave_data[] = {
#include "table_sine.c"
};

static int16_t g_table_triangle_wave_data[] = {
#include "table_triangle.c"
};

static int16_t g_table_sawtooth_wave_data[] = {
#include "table_sawtooth.c"
};

static struct wave_table g_wave_tables[] = {
	DECLARE_TABLE("square", g_table_square_wave_data),
	DECLARE_TABLE("sine", g_table_sine_wave_data),
	DECLARE_TABLE("triangle", g_table_triangle_wave_data),
	DECLARE_TABLE("sawtooth", g_table_sawtooth_wave_data),
	{ 0 }
};

static int check_wave_tables()
{
	assert( sizeof(g_table_square_wave_data) / sizeof(int16_t) <= 0xFFFF );
	assert( sizeof(g_table_sine_wave_data) / sizeof(int16_t) <= 0xFFFF );
	assert( sizeof(g_table_triangle_wave_data) / sizeof(int16_t) <= 0xFFFF );
	assert( sizeof(g_table_sawtooth_wave_data) / sizeof(int16_t) <= 0xFFFF );

	return 0;
}

struct tone_generator_config {
	int card;
	int device;
	struct wave_table *wave_table;
	struct wave_scale wave_scale;
	struct pcm_config pcm_config;
	uint32_t duration;
	int16_t volume; /* binary fraction / USHRT_MAX */
	uint32_t chan_mask;
};

static int inner_main(struct tone_generator_config config)
{
	struct pcm_config *pcm_config = &config.pcm_config;
	struct pcm *pcm;
	unsigned pos;
	int16_t *buf;

	pcm = pcm_open(0, 0, PCM_OUT, pcm_config);
	if (!pcm) {
		fprintf(stderr, "Could not open sound card\n");
		fprintf(stderr, "%s\n", pcm_get_error(pcm));
		return 1;
	}
	if (!pcm_is_ready(pcm)) {
		fprintf(stderr, "Sound card not ready\n");
		fprintf(stderr, "%s\n", pcm_get_error(pcm));
		return 1;
	}

	buf = (int16_t*)calloc(sizeof(int16_t),
			       pcm_config->period_size * pcm_config->channels);
	if (!buf) {
		fprintf(stderr, "Could not allocate memory for buffer\n");
		return 1;
	}

	for (pos=0 ; (!config.duration || (pos < config.duration)) ; pos += pcm_config->period_size) {
		oscillator_table_render(buf,
			config.wave_table,
			pos,
			pcm_config->period_size,
			config.wave_scale,
			pcm_config->channels,
			config.chan_mask, /* write to all channels */
			config.volume);
		if (pcm_write(pcm,
			      buf,
			      pcm_config->channels * pcm_config->period_size * sizeof(int16_t))) {
			fprintf(stderr, "Error writing to sound card\n");
			fprintf(stderr, "%s\n", pcm_get_error(pcm));
		}
	}

	pcm_close(pcm);

	return 0;
}

static void usage()
{
	struct wave_table *ptr;

	printf("Usage: audio-tool [options] tone <wave_type> <frequency> [<vol_db>]\n");
	printf("\n");
	printf("wave_type:\n");
	for (ptr=g_wave_tables ; ptr->name ; ++ptr) {
		printf("    %s\n", ptr->name);
	}
	printf("frequency: non-negative real number\n");
	printf("vol_db: (optional) Volume attenuation in dB FS (implied negative, must be >= 0, default=0)\n");
}

int tone_generator_main(const struct audio_tool_config *at_config, int argc, char* argv[])
{
	struct tone_generator_config config = {
		.card = 0,
		.device = 0,
		.chan_mask = ~0,
	};
	struct pcm_config pcm_config;
	struct wave_table *ptr, *table;
	struct wave_scale wave_scale;
	double freq;
	char *arg_wave_type, *arg_freq, *arg_voldb;
	double tmp;

	if ((argc < 3) || (argc > 4)) {
		usage();
		return 1;
	}

	if (check_wave_tables())
		return 1;

	arg_wave_type = argv[1];
	arg_freq = argv[2];
	if (argc > 3)
		arg_voldb = argv[3];
	else
		arg_voldb = "0";

	/* Set sane defaults */
	memset(&pcm_config, 0, sizeof(struct pcm_config));
	pcm_config.format = PCM_FORMAT_S16_LE;

	config.device = at_config->device;
	config.card = at_config->card;
	pcm_config.period_size = at_config->period_size;
	pcm_config.period_count = at_config->num_periods;
	pcm_config.rate = at_config->rate;
	pcm_config.channels = at_config->channels;
	config.chan_mask = at_config->channel_mask;
	config.duration = at_config->duration * pcm_config.rate;

	for (ptr = g_wave_tables ; ptr->name ; ++ptr) {
		if (strcmp(arg_wave_type, ptr->name) == 0) {
			table = ptr;
			assert( IS_POWER_OF_TWO(table->length) );
			assert( table->mask == table->length - 1 );
			break;
		}
	}
	if (ptr->name == 0) {
		fprintf(stderr, "Invalied wave_type parameter\n");
		return 1;
	}

	tmp = atof(arg_freq);
	if (tmp < 10.0) {
		fprintf(stderr, "Error: frequency must be > 10Hz\n");
		return 1;
	}
	freq = tmp;

	tmp = atof(arg_voldb);
	if (tmp < 0 ) {
		fprintf(stderr, "Volume attenuation must be greater than 0 dB FS\n");
		return 1;
	}
	/* Convert db to fraction */
	tmp = -tmp;
	tmp = pow(10.0, tmp/10.0);
	config.volume = (unsigned short) (tmp * ((double)USHRT_MAX));

	tmp = ((double)pcm_config.rate) / freq;
	wave_scale.length = tmp;
	tmp = (tmp - wave_scale.length) * 0xFFF;
	wave_scale.sub = tmp;
	wave_scale.sub_den = 0xFFF;
	wave_scale.sub_shift = 12;

	/* This restriction prevents overflows in render()
	 */
	{
		uint16_t bits = 0;
		while ((1<<bits) < table->length) ++bits;
		if (wave_scale.sub_shift + bits > 24) {
			fprintf(stderr, "bits(wave_scale) + bits(table.length) "
				" must be less than or equal to 24\n");
			return 1;
		}
	}

	memcpy(&config.pcm_config, &pcm_config, sizeof(pcm_config));
	memcpy(&config.wave_scale, &wave_scale, sizeof(wave_scale));
	config.wave_table = table;

	return inner_main(config);

	return 0;
}
