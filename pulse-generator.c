/* pulse-generator.c
**
** Copyright 2011, The Android Open Source Project
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of The Android Open Source Project nor the names of
**       its contributors may be used to endorse or promote products derived
**       from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY The Android Open Source Project ``AS IS'' AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED. IN NO EVENT SHALL The Android Open Source Project BE LIABLE
** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
** SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
** CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
** LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
** OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
** DAMAGE.
*/

#include <tinyalsa/asoundlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <time.h>
#include <signal.h>

volatile int running = 1;

void sigint_handler(int sig)
{
    running = 0;
}

struct interval_tracker {
    struct timespec last;
    struct timespec now;
    size_t count;
    double sum_ms;
    double sum2_ms;
    double max_ms;
    double min_ms;
    int initd;
};

void ivt_init(struct interval_tracker *it)
{
    memset(it, 0, sizeof(struct interval_tracker));
}

void ivt_lap(struct interval_tracker *it)
{
    double elapsed;
    clock_gettime(CLOCK_MONOTONIC, &it->now);
    if (it->initd) {
        it->count++;
        elapsed = (it->now.tv_sec - it->last.tv_sec) * 1000000.0
            + (it->now.tv_nsec - it->last.tv_nsec) / 1000.0;
        it->sum_ms += elapsed;
        it->sum2_ms += elapsed*elapsed;
        if (it->count == 1) {
            it->max_ms = it->min_ms = elapsed;
        } else {
            if (elapsed > it->max_ms)
                it->max_ms = elapsed;
            if (elapsed < it->min_ms)
                it->min_ms = elapsed;
        }
        printf("now=%ld dt=%08u max=%08u min=%08u\n",
               1000000 * (it->now.tv_sec % 100) + it->now.tv_nsec / 1000,
               (unsigned)(elapsed+.5), (unsigned)(it->max_ms+.5), (unsigned)(it->min_ms+.5));
    }
    it->last = it->now;
    it->initd = 1;
}

double ivt_average(struct interval_tracker *it)
{
    if (it->count <= 0)
        return 0.0;
    return it->sum_ms / (double)(it->count);
}
               
#define PULSE_AT_FRONT  (1<<0)
#define PULSE_AT_END    (1<<1)
#define PULSE_AT_MIDDLE (1<<2)

struct play_pulses_params {
    unsigned int card;
    unsigned int device;
    unsigned int channels;
    unsigned int rate;
    unsigned int bits;
    unsigned int period_size;
    unsigned int period_count;
    unsigned int pulse_position;
};

void play_pulses(struct play_pulses_params *params);

int main(int argc, char **argv)
{
    struct play_pulses_params params = {
        .card = 0,
        .device = 0,
        .channels = 2,
        .rate = 48000,
        .bits = 16,
        .period_size = 1024,
        .period_count = 4,
        .pulse_position = PULSE_AT_FRONT,
    };
	    
    /* parse command line arguments */
    argv += 1;
    while (*argv) {
        if (strcmp(*argv, "-d") == 0) {
            argv++;
            if (*argv) params.device = atoi(*argv);
        }
        if (strcmp(*argv, "-p") == 0) {
            argv++;
            if (*argv) params.period_size = atoi(*argv);
        }
        if (strcmp(*argv, "-n") == 0) {
            argv++;
            if (*argv) params.period_count = atoi(*argv);
        }
        if (strcmp(*argv, "-D") == 0) {
            argv++;
            if (*argv) params.card = atoi(*argv);
        }
	if (strcmp(*argv, "-c") == 0) {
            argv++;
            if (*argv) params.channels = atoi(*argv);
        }
        if (strcmp(*argv, "-r") == 0) {
            argv++;
            if (*argv) params.rate = atoi(*argv);
        }
        if (strcmp(*argv, "-b") == 0) {
            argv++;
            if (*argv) params.bits = atoi(*argv);
        }
        if (strcmp(*argv, "-P") == 0) {
            argv++;
            if (*argv) params.pulse_position = atoi(*argv);
        }
        if (*argv) argv++;
    }

    signal(SIGINT, sigint_handler);
    play_pulses(&params);

    return 0;
}

void play_pulses(struct play_pulses_params *params)
{
    struct pcm_config config;
    struct pcm *pcm;
    struct interval_tracker it;
    char *buffer;
    int size;
    int num_read;

    ivt_init(&it);

    config.channels = params->channels;
    config.rate = params->rate;
    config.period_size = params->period_size;
    config.period_count = params->period_count;
    if (params->bits == 32)
        config.format = PCM_FORMAT_S32_LE;
    else if (params->bits == 16)
        config.format = PCM_FORMAT_S16_LE;
    config.start_threshold = 0;
    config.stop_threshold = 0;
    config.silence_threshold = 0;

    pcm = pcm_open(params->card, params->device, PCM_OUT, &config);
    if (!pcm || !pcm_is_ready(pcm)) {
        fprintf(stderr, "Unable to open PCM device %u (%s)\n",
                params->device, pcm_get_error(pcm));
        return;
    }

    size = pcm_get_buffer_size(pcm);
    buffer = malloc(size);
    if (!buffer) {
        fprintf(stderr, "Unable to allocate %d bytes\n", size);
        free(buffer);
        pcm_close(pcm);
        return;
    }

    printf("Playing sample: %u ch, %u hz, %u bit\n",
           params->channels, params->rate, params->bits);

    int k, stat;
    int interrupt_tol = 20 + 1000000 * params->period_size / params->rate;
    int pulse_position = params->pulse_position;
    pcm_start(pcm);
    do {
        memset(buffer, 0, size);
        num_read = 0;
        num_read = size;
        if (pulse_position & PULSE_AT_FRONT) {
            for( k = 0 ; (k < size) && (k < 4) ; ++k ) {
                buffer[k] = 0x7f;
            }
        }
        if (pulse_position & PULSE_AT_MIDDLE) {
            /* not supported */
        }
        if (pulse_position & PULSE_AT_END) {
            for( k = size-1 ; (k > 0) && (k >= (size-4)) ; --k ) {
                buffer[k] = 0x7f;
            }
        }

	stat = pcm_wait(pcm, interrupt_tol /* ms timeout */);
	switch (stat) {
	case 0: /* timeout */
            printf("timeout");
            break;
	case 1:
            ivt_lap(&it);
            if (pcm_write(pcm, buffer, num_read)) {
                fprintf(stderr, "Error playing sample\n");
                num_read = 0;
            }
            num_read = 1;
            break;
	case -EINTR:
            printf("wait was interrupted\n");
            break;
	case -EPIPE:
            printf("xrun\n");
            break;
	case -ESTRPIPE:
            printf("state suspended\n");
            break;
	case -ENODEV:
            printf("disconnected\n");
            num_read = 0;
            break;
	case -EIO:
            printf("EIEIO\n");
            num_read = 0;
            break;
	default:
            printf("unknown error\n");
	}

    } while (running && (num_read > 0));

    pcm_stop(pcm);

    printf("avg=%g max=%g min=%g\n",
           ivt_average(&it), it.max_ms, it.min_ms);

    free(buffer);
    pcm_close(pcm);
}
