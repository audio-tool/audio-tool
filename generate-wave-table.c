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
 * generate-wave-table.c
 *
 * Creates a static wave-table that can be used later as a look-up table
 * for synthesis.
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

typedef enum {
	DATA_TYPE_NONE = 0,
	DATA_TYPE_S16,
	DATA_TYPE_S32,
} data_t;

typedef enum {
	WAVE_TYPE_SINE = 1,
	WAVE_TYPE_COSINE,
	WAVE_TYPE_SQUARE,
	WAVE_TYPE_TRIANGLE,
	WAVE_TYPE_SAW,
} wave_t;

struct signal_spec {
	data_t type;
	wave_t wave;
	unsigned length;
};

typedef int (*table_generator_func)(struct signal_spec *spec,
				    unsigned offset, unsigned count, void *buf);
typedef int (*table_output_func)(void *buf, data_t type, unsigned count); 

#define FS_S16 0x7FFF
#define FS_S32 0x7FFFFFFF

int square_wave_generator(struct signal_spec *spec, unsigned offset,
			  unsigned count, void *buf)
{
	int16_t s16, *s16ptr;
	int32_t s32, *s32ptr;
	unsigned midpoint, k, end;

	assert( spec );

	if ((offset + count) > spec->length) {
		fprintf(stderr, "Warning: generating square wave beyond "
			"the predetermined length.\n");
	}
	
	midpoint = spec->length / 2;
	if (midpoint > offset)
		midpoint -= offset;
	else
		midpoint = 0;
	end = spec->length;
	if (end > offset)
		end -= offset;
	else
		end = 0;

	if (spec->type == DATA_TYPE_S16) {
		s16ptr = (int16_t*)buf;
		for (k = 0 ; k < count ; ++k) {
			if (k < midpoint) {
				s16 = FS_S16;
			} else {
				s16 = -FS_S16;
			}
			s16ptr[k] = s16;
		}
	} else if (spec->type == DATA_TYPE_S32) {
		s32ptr = (int32_t*)buf;
		for (k = 0 ; k < count ; ++k) {
			if (k < midpoint) {
				s32 = FS_S32;
			} else {
				s32 = -FS_S32;
			}
			s32ptr[k] = s32;
		}
	} else {
		assert( 0 );
	}

	return 0;
}

int sine_wave_generator(struct signal_spec *spec, unsigned offset,
			  unsigned count, void *buf)
{
	int16_t s16, *s16ptr;
	int32_t s32, *s32ptr;
	double value, angle;
	unsigned k;

	assert( spec );

	if (spec->type == DATA_TYPE_S16) {
		s16ptr = (int16_t*)buf;
		for (k = 0 ; k < count ; ++k) {
			angle = 2.0 * M_PI * ((double)(offset + k)) / ((double)spec->length);
			value = sin(angle) * FS_S16;
			s16 = round(value);
			s16ptr[k] = s16;
		}
	} else if (spec->type == DATA_TYPE_S32) {
		s32ptr = (int32_t*)buf;
		for (k = 0 ; k < count ; ++k) {
			angle = 2.0 * M_PI * ((double)(offset + k)) / ((double)spec->length);
			value = sin(angle) * FS_S32;
			s32 = round(value);
			s32ptr[k] = s32;
		}
	} else {
		assert( 0 );
	}

	return 0;
}

int triangle_wave_generator(struct signal_spec *spec, unsigned offset,
			  unsigned count, void *buf)
{
	int16_t s16, *s16ptr;
	int32_t s32, *s32ptr;
	double value;
	unsigned k;

	assert( spec );

	if (spec->type == DATA_TYPE_S16) {
		s16ptr = (int16_t*)buf;
		for (k = 0 ; k < count ; ++k) {
			value = 4.0 * ((double)(offset + k)) / ((double)spec->length);
			if (value <= 1.0) {
				/* NO-OP */
			} else if (value <= 3.0) {
				value = 2.0 - value;
			} else {
				value = value - 4.0;
			}
			s16 = round(value * FS_S16);
			s16ptr[k] = s16;
		}
	} else if (spec->type == DATA_TYPE_S32) {
		s32ptr = (int32_t*)buf;
		for (k = 0 ; k < count ; ++k) {
			value = 4.0 * ((double)(offset + k)) / ((double)spec->length);
			if (value <= 1.0) {
				/* NO-OP */
			} else if (value <= 3.0) {
				value = 2.0 - value;
			} else {
				value = value - 4.0;
			}
			s32 = round(value * FS_S32);
			s32ptr[k] = s32;
		}
	} else {
		assert( 0 );
	}

	return 0;
}

int sawtooth_wave_generator(struct signal_spec *spec, unsigned offset,
			  unsigned count, void *buf)
{
	int16_t s16, *s16ptr;
	int32_t s32, *s32ptr;
	double value;
	unsigned k;

	assert( spec );

	if (spec->type == DATA_TYPE_S16) {
		s16ptr = (int16_t*)buf;
		for (k = 0 ; k < count ; ++k) {
			value = 2.0 * ((double)(offset + k)) / ((double)spec->length) - 1.0;
			s16 = round(value * FS_S16);
			s16ptr[k] = s16;
		}
	} else if (spec->type == DATA_TYPE_S32) {
		s32ptr = (int32_t*)buf;
		for (k = 0 ; k < count ; ++k) {
			value = 2.0 * ((double)(offset + k)) / ((double)spec->length) - 1.0;
			s32 = round(value * FS_S32);
			s32ptr[k] = s32;
		}
	} else {
		assert( 0 );
	}

	return 0;
}

int stdout_c_header_table(void *buf, data_t type, unsigned count)
{
	int16_t *s16ptr = (int16_t*)buf;
	int32_t *s32ptr = (int32_t*)buf;
	unsigned k;

	assert( buf );

	if (type == DATA_TYPE_S16) {
		for (k=0 ; k<count ; ++k) {
			fprintf(stdout, "0x%04hx,\n", s16ptr[k]);
		}
	} else if (type == DATA_TYPE_S32) {
		for (k=0 ; k<count ; ++k) {
			fprintf(stdout, "0x%08x,\n", s32ptr[k]);
		}
	} else {
		assert( 0 );
	}

	return 0;
}

struct wave_type {
	const char* name;
	table_generator_func generator;
	wave_t wave_id;
};

static struct wave_type g_types[] = {
	{
		.name = "square",
		.generator = square_wave_generator,
	},
	{
		.name = "sine",
		.generator = sine_wave_generator,
	},
	{
		.name = "triangle",
		.generator = triangle_wave_generator,
	},
	{
		.name = "sawtooth",
		.generator = sawtooth_wave_generator,
	},
	{ 0 }
};

#define BUFSIZE 4096
#define UNSIGNED_MAX (~0U)

int main(int argc, char *argv[])
{
	table_generator_func gen = square_wave_generator;
	table_output_func out = stdout_c_header_table;
	struct signal_spec spec = {
		.type = DATA_TYPE_S16,
		.wave = WAVE_TYPE_SQUARE,
		.length = 512,
	};
	char buf[BUFSIZE];
	unsigned k, count;
	long tmp;
	struct wave_type *ptr;
	char *arg_wave, *arg_length, *arg_format;

	if (argc < 4) {
		printf("Usage: generate-wave-table <wave_type> <length> <format>\n");
		printf("wave_types:\n");
		for (ptr = g_types ; ptr->name != 0 ; ++ptr) {
			printf("    %s\n", ptr->name);
		}
		printf("length: a positive integer\n");
		printf("format: S16 or S32\n");
		printf("All output is to stdout in a format suitable for C files.\n");
		printf("Data generated is in the native byte order.\n");
		return 0;
	}

	arg_wave = argv[1];
	arg_length = argv[2];
	arg_format = argv[3];

	for (ptr = g_types ; ptr->name != NULL ; ++ptr) {
		if (strcmp(arg_wave, ptr->name) == 0) {
			spec.wave = ptr->wave_id;
			gen = ptr->generator;
			break;
		}
	}
	if (ptr->name == NULL) {
		fprintf(stderr, "Error: invalid wave type %s\n", arg_wave);
		return 1;
	}

	tmp = atol(arg_length);
	if (tmp < 8) {
		fprintf(stderr, "Error: wave length must be at least 8 "
			"samples\n");
		return 1;
	}
	if (tmp > UINT_MAX) {
		fprintf(stderr, "Error: wave length must fit into an "
			"unsigned integer\n");
		return 1;
	}
	spec.length = tmp;

	if (0 == strcmp(arg_format, "S16")) {
		spec.type = DATA_TYPE_S16;
	} else if (0 == strcmp(arg_format, "S32")) {
		spec.type = DATA_TYPE_S32;
	} else {
		fprintf(stderr, "Error: unsupported format %s "
			"(must be 'S16' or 'S32')\n", arg_format);
		return 1;
	}

	memset(buf, 0, sizeof(buf));
	count = 0;
	for (k=0 ; k<spec.length ; k += count) {
		count = BUFSIZE;
		switch (spec.type) {
		case DATA_TYPE_S16:
			count /= 2;
			break;
		case DATA_TYPE_S32:
			count /= 4;
			break;
		default:
			assert(0);
		}
		if (k + count > spec.length)
			count = spec.length - k;
		gen(&spec, k, count, buf);
		out(buf, spec.type, count);
	}

	return 0;
}
