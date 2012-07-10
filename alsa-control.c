/*
 * alsa-control.c
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

#include "alsa-control.h"
#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <errno.h>

#include <sound/asound.h>

#define log_fmt(x) x
#define LOGE(msg, ...) fprintf(stderr, log_fmt(msg) "\n", ##__VA_ARGS__)
#ifdef LOG_NDEBUG
#define LOGV(msg, ...) fprintf(stderr, log_fmt(msg) "\n", ##__VA_ARGS__)
#else
#define LOGV(msg, ...) ((void)0)
#endif

/* \brief Returns the path to a card's control device (/dev/snd/controlC#)
 *
 * \param card - The index of the sound card.
 * \param dest - Pointer to a string buffer.
 * \param dest_len - Maximum size of memory at dest.  Should be larger
 * than sizeof(SND_CONTROL_TEMPLATE) by a few bytes (16 to 32 bytes).
 *
 * \return 0 on succes.  Negative error code on failure.
 *
 *   -EINVAL: The destination string was not large enough.
 */
static int card_control_path(int card, char *dest, size_t dest_len)
{
	snprintf(dest, dest_len, SND_CONTROL_TEMPLATE, card);
	return 0;
}

/* \brief returns the maximum number of cards that could be plugged in.
 *
 * \return The maximum number of cards that could be plugged in.  This
 * will be one more than the maximum index.
 */
int ah_card_max_count()
{
	/* XXX This is being set arbitrarily.  Need to ask kernel. */
	return AH_MAX_CARD_COUNT;
}

/* \brief returns the number of cards currently registered with ALSA
 *
 * \return The number of cards registered with ALSA.  If there are no
 * cards returns a zero.  Note that in some cases you may have a card
 * number greater than the return of this function.  (E.g. if there
 * were 4 cards and cards 1 and 2 were unplugged.
 */
int ah_card_count()
{
        DIR *dir;
        struct dirent *de;
        int count = 0;

        dir = opendir(ALSA_DEVICE_DIRECTORY);
        if (dir == 0)
		return 0;

        do {
		de = readdir(dir);
		if (!de)
			break;
		if (0 == strncmp(SND_CONTROL_FILE_PREFIX, de->d_name, strlen(SND_CONTROL_FILE_PREFIX)))
			++count;
        } while(1);

	closedir(dir);

	if (count > ah_card_max_count())
		count = ah_card_max_count();

        return count;
}

/* \brief Returns the name of sound card (by number)
 *
 * \param card - The sound card index.
 * \param str - Pointer to location where string will be written.
 * \param strlen - Amound of memory available at str
 *
 * \return 0 on success.  A negative error code on error.
 */
int ah_card_get_name(int card, char *str, size_t strlen)
{
	snd_ctl_card_info_t info;
	int fd, rv = 0;

	fd = ah_control_open(card, O_RDONLY);
	if (fd < 0) {
		LOGE("ah_control_open() failed");
		rv = -errno;
		goto end;
	}

	rv = ah_card_get_info(fd, &info);
	if (rv) {
		LOGE("error %d in getting card info", errno);
		rv = -errno;
		goto card_err;
	}
	LOGV("card %d info->name = %s", card, info.id);
	strncpy(str, (char*)info.id, strlen);
card_err:
	ah_control_close(fd);
end:
        return rv;
}

/* \brief Return the index of the card with id name, if exists.
 *
 * \param name - name of card that is being searched for.
 *
 * \return - The card index, or a negative error code:
 *
 *    ENODEV - Device does not exits.
 */
int ah_card_find_by_name(const char* name)
{
	char cur[256] = "";
        int cards = ah_card_count();
        int k;
        int match = 0;
	LOGV("%s() looking for %s", __func__, name);
        for (k = 0 ; k < cards ; ++k) {
		ah_card_get_name(k, cur, sizeof(cur));
		LOGV("Comparing to '%s'", cur);
		if ( 0 == strcmp(cur, name) ) {
			match = 1;
			break;
		}
        }

        if (match)
		return k;

        return -ENODEV;
}

/* \brief Grabs the card info structure from the kernel.
 *
 * \param card - The sound card index
 * \param info - Pointer to card info structure.  The kernel's
 * structure will be copied to this memory location.
 *
 * \return 0 on success.  Otherwise a negative error code.
 * Error codes are the same as may be returned by open(2) and
 * ioctl(2).
 */
int ah_card_get_info(int fd, snd_ctl_card_info_t *info)
{
        int e, rv = 0;
        e = ioctl(fd, SNDRV_CTL_IOCTL_CARD_INFO, info);
        if(e)
		rv = -errno;
        return rv;
}

/* \brief Open up the control device for the specified card.
 *
 * \param card - The index of the card to use.
 * \param mode - The mode with which to open the card.  Same as for
 * open(2).
 *
 * Note: In fact, this function is really just a front from open(2).
 *
 * \return A file descriptor (int) for the device on success.  On
 * error, returns the same errors as open(2).
 */
int ah_control_open(int card, int mode)
{
	char control_fn[sizeof(SND_CONTROL_TEMPLATE) + 32] = "";
	int fd;
	card_control_path(card, control_fn, sizeof(control_fn));
	LOGV("Trying to open %s (card = %d)", control_fn, card);
	fd = open(control_fn, mode);
	return fd;
}

/* \brief Close the control device.
 *
 * \param fd - File descriptor to control device.
 *
 * \return 0 on success.  Returns same errors as close(2)
 */
int ah_control_close(int fd)
{
	return close(fd);
}
