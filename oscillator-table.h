/* Copyright (c) 2012, Gabriel M. Beddingfield <gabrbedd@gmail.com>
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
#ifndef __LIBGABE_OSCILLATOR_TABLE_H__
#define __LIBGABE_OSCILLATOR_TABLE_H__

#define IS_POWER_OF_TWO(x) ((0) == ((x) & ((x)-1)))

struct wave_table {
	const char* name;
	uint16_t length; /* Must be a power of two */
	uint16_t mask;   /* = length - 1 */
	int16_t *data;
};

struct wave_scale {
	uint16_t length;
	uint16_t sub;       /* sub-frames, binary fraction with
			       denominator = ((1 << sub_shift) - 1)*/
	uint16_t sub_den;   /* must be (power of 2)-1 */
	uint16_t sub_shift; /* denominator shift, sub_den = (1 << sub_shift) - 1 */
};

#define STATIC_ARRAY_SIZE(ra) (sizeof(ra)/sizeof(ra[0]))

#define DECLARE_TABLE(t_name, t_data)					\
	{								\
		.name = (t_name),					\
		.length = STATIC_ARRAY_SIZE(t_data),			\
		.mask = STATIC_ARRAY_SIZE(t_data) - 1,			\
		.data = (t_data),					\
	}

int oscillator_table_render(void *out, struct wave_table *tbl, uint32_t offset,
		uint16_t count, const struct wave_scale wave_scale, uint8_t channels,
		uint32_t channel_mask, uint16_t vol_frac, int bits);

#endif /* __LIBGABE_OSCILLATOR_TABLE_H__ */

