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

typedef void (*write_to_buffer_func_t)(void *out, uint16_t frame, uint8_t channels,
				       uint8_t channel, int16_t value);

inline void write_s16_to_s8(void *out, uint16_t frame, uint8_t channels,
				       uint8_t channel, int16_t value)
{
	int8_t *dest = out;
	dest[frame * channels + channel] = value >> 8;
}

inline void write_s16_to_s16(void *out, uint16_t frame, uint8_t channels,
				       uint8_t channel, int16_t value)
{
	int16_t *dest = out;
	dest[frame * channels + channel] = value;
}

inline void write_s16_to_s24(void *out, uint16_t frame, uint8_t channels,
				       uint8_t channel, int16_t value)
{
	union s24_t {
		int32_t v;
		unsigned char c[4];
	} val;
	int64_t tmp = value;
	unsigned char *d;

	/* scale 0x7FFF to 0x7FFFFF */
	tmp = (tmp * 0x7FFFFF) / 0x7FFF;
	assert( tmp <= (int64_t)0x7FFFFF );
	assert( (tmp >= 0) || (-tmp <= (int64_t)0x7FFFFF ) );
	val.v = tmp;
	d = out;
	d += 3 * (frame * channels + channel);
	d[0] = val.c[0];
	d[1] = val.c[1];
	d[2] = val.c[2];
}

inline void write_s16_to_s32(void *out, uint16_t frame, uint8_t channels,
				       uint8_t channel, int16_t value)
{
	int32_t *dest = out;
	int64_t tmp = value;
	int32_t v;

	/* scale 0x7FFF to 0x7FFFFFFF */
	tmp = (tmp * 0x7FFFFFFF) / 0x7FFF;
	assert( tmp <= (int64_t)0x7FFFFFFF );
	assert( (tmp >= 0) || (-tmp <= (int64_t)0x7FFFFFFF ) );
	v = tmp;
	dest[frame * channels + channel] = v;
}

/**
 * \brief render an oscillator table to an output buffer, using the freq specified
 *
 * \param out The output buffer (PCM16)
 * \param tbl The source oscillator table
 * \param offset the frame offset of the wave as rendered on the output
 * \param count the number of frames to render this cycle
 * \param wave_scale specification of the desired output wavelength
 * \param channels number of output channels in the buffer (range: [1,32])
 * \param clannel_mask mask indicating which channels to write to
 * \param vol_frac An integer fraction for attenuating the signal
 * \param format output bits (should be 8, 16, 24, or 32)
 *
 * \returns non-zero on error.
 */
int oscillator_table_render(void *out, struct wave_table *tbl, uint32_t offset,
		uint16_t count, const struct wave_scale wave_scale, uint8_t channels,
		uint32_t channel_mask, uint16_t vol_frac, int bits)
{
	int32_t num;
	const int32_t denom = USHRT_MAX;
	uint32_t tbl_len, wave_len;
	uint32_t k, frame, f_reg, r_reg;
	uint32_t p;
	int16_t val;
	int32_t a, b;
	uint8_t ch;
	int32_t chbit;
	char interpolate = 1;
	int16_t shift;
	uint32_t f;
#ifdef CHECK_MATH
	uint32_t ck_p, ck_tbl_len, ck_wave_len;
#endif
	write_to_buffer_func_t write_to_buffer;

	assert( out );
	assert( tbl );
	assert( channels > 0 );
	assert( 0 == (bits % 8) );
	assert( bits > 0 );
	assert( bits <= 32 );

	switch (bits) {
	case 8: write_to_buffer = write_s16_to_s8; break;
	case 16: write_to_buffer = write_s16_to_s16; break;
	case 24: write_to_buffer = write_s16_to_s24; break;
	case 32: write_to_buffer = write_s16_to_s32; break;
	}

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
		for (ch = 0, chbit=1 ; ch < channels ; ++ch, chbit <<= 1) {
			if ( channel_mask & chbit )
				write_to_buffer(out, k, channels, ch, val);
			else
				write_to_buffer(out, k, channels, ch, 0);
		}
	}

	return 0;
}

