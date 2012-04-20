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
 * oscillator-table.c
 *
 * Utilities generating an accurate waveform to an audio output.
 */

#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#include "oscillator-table.h"

/*#define CHECK_MATH*/

int oscillator_table_render(int16_t *out, struct wave_table *tbl, uint32_t offset,
		uint16_t count, const struct wave_scale wave_scale, uint8_t channels,
		uint16_t vol_frac)
{
	int32_t num;
	const int32_t denom = USHRT_MAX;
	uint32_t tbl_len, wave_len;
	uint32_t k, frame, f_reg, r_reg;
	uint32_t p;
	int16_t val;
	int32_t a, b;
	uint8_t ch;
	char interpolate = 1;
	int16_t shift;
	uint32_t f;
#ifdef CHECK_MATH
	uint32_t ck_p, ck_tbl_len, ck_wave_len;
#endif

	assert( out );
	assert( tbl );
	assert( channels > 0 );

	num = vol_frac;
	tbl_len = tbl->length << wave_scale.sub_shift;
	wave_len = (wave_scale.length << wave_scale.sub_shift) | wave_scale.sub;
#ifdef CHECK_MATH
	ck_tbl_len = (uint64_t)tbl->length << wave_scale.sub_shift;
	ck_wave_len = ((uint64_t)wave_scale.length << wave_scale.sub_shift) | wave_scale.sub;
#endif
	if ( (tbl_len / wave_len) > 4 )
		interpolate = 0;

	for (k=0 ; k < count ; ++k) {
		/* This bit of code is over-optimized for 32-bit processors.
		 * See the "CHECK_MATH" section for grokkable code.
		 * Why?  Because it was a fun challenge.
		 *
		 * Strategy: Break "frame" into 8-bit chunks, and build
		 * up "p", being careful to preserve remainders.
		 */
		assert( (1<<16) > tbl->mask ); /* uncomment code if this fails */
		assert( wave_len == (wave_len & 0xFFFFFF) );
		assert( tbl_len == (tbl_len & 0xFFFFFF) );
		frame = offset + k;
		p = 0;
		f_reg = 0;
		r_reg = 0;
		for (shift=24 ; shift >= 0 ; shift -= 8) {
			f = (frame >> shift) & 0xFF;
			f_reg = f * tbl_len;
			r_reg = (r_reg << 8) + f_reg % wave_len;
			p = (p << 8) + f_reg / wave_len + r_reg / wave_len;
			r_reg %= wave_len;
		}
		p &= tbl->mask;
#ifdef CHECK_MATH
		ck_p = ((uint64_t)(offset + k) * ck_tbl_len) / ck_wave_len;
		ck_p &= tbl->mask;
		assert( p == ck_p );
#endif
		if (interpolate) {
			/* interpolate */
			int32_t water;
			a = tbl->data[p];
			b = tbl->data[p + 1];
			assert( (int32_t)r_reg >= 0 );
			assert( (int32_t)wave_len >= 0 );
			/* water down r_reg and wave_len if they will overflow */
			water = 4;
			while ((r_reg >> water) > 0x7FFF)
				++water;
			val = a + ((b-a) * ((int32_t)r_reg >> water)) / ((int32_t)wave_len >> water);
			if (a < b)
				assert( a <= val && val <= b );
			else
				assert( a >= val && val >= b );
		} else {
			val = tbl->data[p];
		}
		val = (val * num) / denom;
		*out++ = val;
		if (channels >> 1)
			*out++ = val;
		for (ch = 2 ; ch < channels ; ++ch)
			*out++ = val;
	}

	return 0;
}

