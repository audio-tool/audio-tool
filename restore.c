/*
 * restore.c
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

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <tinyalsa/asoundlib.h>

#include "config.h"
#include "restore.h"
#include "mixer_cache.h"

#define BUFSIZE (1L << 15)

static int read_line(char *dest, size_t max, const char *src, size_t size, size_t *offset)
{
	size_t n;

	for (n = 0 ; (n < max) && (src[*offset] != '\n') && (*offset < size) ; ++n, *offset += 1) {
		dest[n] = src[*offset];
	}

	if (n < max)
		dest[n] = '\0';
	else
		dest[n-1] = '\0';

	if ((*offset < size) && (src[*offset] == '\n'))
		*offset += 1;

	return n;
}

/* Notes:
 *   - param 'line' is modified by this function
 *   - param 'db' has mixer_cache_touch() called on it.
 */
static void process_line(char* line, struct audio_tool_mixer_cache *db, struct mixer *mixer)
{
	size_t length = strlen(line);
	size_t pos = 0, sep;
	const char* sep_ptr;
	enum field_t { FNAME=0, FTYPE, FCOUNT, FVALS } field = FNAME;
	unsigned control_id, val, val_no;
	const char *ctl_type;
	unsigned file_count, ctl_count;
	struct mixer_ctl *ctl;

	if (!length)
		return;

	if (line[0] == '#')
		return;

	control_id = -1;
	val_no = 0;
	while (pos < length) {
		sep_ptr = strstr(&line[pos], "\t");
		if (sep_ptr)
			sep = (size_t) (sep_ptr - line);
		else
			sep = length;
		if (sep < length)
			line[sep] = '\0';
		switch(field) {
		case FNAME:
			control_id = mixer_cache_get_id_by_name(db, &line[pos]);
			if (control_id == -1) {
				printf("Error: could not find control %s\n", &line[pos]);
				pos = length;
			}
			ctl = mixer_get_ctl(mixer, control_id);
			break;
		case FTYPE:
			ctl_type = mixer_ctl_get_type_string(ctl);
			if (0 != strcmp(ctl_type, &line[pos])) {
				printf("Error: type mismatch for control #%d: file=%s card=%s\n",
				       control_id, &line[pos], ctl_type);
				pos = length;
			}
			break;
		case FCOUNT:
			ctl_count = mixer_ctl_get_num_values(ctl);
			file_count = atoi(&line[pos]);
			if (ctl_count != file_count) {
				printf("Error: mismatch in the count of control #%d's values: "
				       "file=%d card=%d\n", control_id, file_count, ctl_count);
				pos = length;
			}
			break;
		case FVALS:
			if (0 == strcmp("#N/A", &line[pos])) {
				pos = length;
			} else if (0 == strcmp("ENUM", ctl_type)) {
				mixer_ctl_set_enum_by_string(ctl, &line[pos]);
			} else {
				val = atoi(&line[pos]);
				mixer_ctl_set_value(ctl, val_no, val);
			}
			++val_no;
			break;
		}

		if (field != FVALS)
			++field;

		pos = sep + 1;
	}
	mixer_cache_touch(db, control_id);
}

int restore_main(const struct audio_tool_config *config, int argc, char **argv)
{
	struct mixer *mixer;
	int card = config->card;
	int ret = 0;
	char *filename = 0;
	char buf[BUFSIZE];
	int fd;
	char *src;
	size_t cursor;
	struct stat fd_stat;
	struct audio_tool_mixer_cache db;

	if (argc != 2) {
		printf("Usage: audio-tool restore <filename>\n");
		return 1;
	}

	filename = argv[1];

	mixer = mixer_open(card);
	if (!mixer) {
		printf("Could not open mixer for card %d\n", card);
		return 1;
	}

	fd = open(filename, O_RDONLY);
	if (fd == -1) {
		printf("Could not open file %s for reading (error=%s)\n", filename, strerror(errno));
		return 1;
	}

	if(-1 == fstat(fd, &fd_stat)) {
		printf("Could not stat file %s\n", filename);
		return 1;
	}

	src = mmap(NULL, fd_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (src == MAP_FAILED) {
		printf("Could not mmap the file %s (error=%s)\n", filename, strerror(errno));
		return 1;
	}

	if (mixer_cache_init(&db)) {
		printf("Could not initialize mixer cache\n");
		return 1;
	}
	if (mixer_cache_populate(&db, mixer)) {
		printf("Could not populate mixer cache\n");
		return 1;
	}

	mixer_cache_reset_touch(&db);
	cursor = 0;
	while(read_line(buf, BUFSIZE, src, fd_stat.st_size, &cursor)) {
		process_line(buf, &db, mixer);
	}
	ret = mixer_cache_audit_touch(&db, 1);

	munmap(src, fd_stat.st_size);
	close(fd);
	mixer_close(mixer);

	return ret;
}

