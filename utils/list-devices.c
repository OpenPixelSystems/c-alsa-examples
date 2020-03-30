/**
 * @file main.c
 * @brief  ALSA tool which lists the PCM devices
 * @author Bram Vlerick <bram.vlerick@openpixelsystems.org>
 * @version
 * @date 2020-03-30
 *
 * @note: Inspired by: https://stackoverflow.com/questions/6866103/finding-all-the-devices-i-can-use-to-play-pcm-with-alsa
 */

#include <stdio.h>
#include <signal.h>
#include <stdbool.h>
#include <alloca.h>

#include <alsa/asoundlib.h>

int main()
{
	void **hints;
	/* Find names associated with PCM devices */
	int err = snd_device_name_hint(-1, "pcm", (void ***)&hints);

	if (err < 0) {
		return -1;
	}

	char **n = (char **)hints;
	while (*n != NULL) {
		/* Get there actual name */
		char *name = snd_device_name_get_hint(*n, "NAME");
		if (name != NULL) {
			printf("%s\n", name);
			free(name);
		}
		n++;
	}
	snd_device_name_free_hint(hints);
}
