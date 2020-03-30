#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#include <alsa/asoundlib.h>

static char card[64] = "default";

static long convert_from_percent(int val, long min, long max)
{
	return ceil((val) * ((max) - (min)) * 0.01 + (min));
}

static int convert_to_percent(long val, long min, long max)
{
	long range = max - min;
	int tmp;

	if (range == 0)
		return 0;

	val -= min;
	tmp = rint((double)val/(double)range *100);
	return tmp;
}

int main(int argc, char **argv)
{
	int error = 0;
	static snd_mixer_t *handle = NULL;
	snd_mixer_elem_t *elem;
	snd_mixer_selem_id_t *sid;

	snd_mixer_selem_id_alloca(&sid);
	snd_mixer_selem_id_set_index(sid, 0);
	snd_mixer_selem_id_set_name(sid, "Master");

	error = snd_mixer_open(&handle, 0);
	if (error < 0) {
		printf("Failed to open mixer\n");
		goto cleanup;
	}

	error = snd_mixer_attach(handle, card);
	if (error < 0) {
		printf("Failed to attach mixer\n");
		goto cleanup;
	}
	printf("Mixer attached successful\n");

	error = snd_mixer_selem_register(handle, NULL, NULL);
	if (error < 0) {
		printf("Failed to register mixer\n");
		goto cleanup;
	}

	error = snd_mixer_load(handle);
	if (error < 0) {
		printf("Failed to load mixer\n");
		goto cleanup;
	}

	elem = snd_mixer_find_selem(handle, sid);
	if (!elem) {
		printf("Element not found\n");
		goto cleanup;
	}

	long vol_max, vol_min, vol_curr;

	/* Get the mixer min max values */
	snd_mixer_selem_get_playback_volume_range(elem, &vol_min, &vol_max);

	/* Get currect volume */
	snd_mixer_selem_get_playback_volume(elem, 0, &vol_curr);

	/* Gradually increase volume from 0 to 50% */
	for (int i = 0; i <= 50; i++) {
		long back_to_val = convert_from_percent(i, vol_min, vol_max);
		printf("step %d: %ld\n", i, back_to_val);
		snd_mixer_selem_set_playback_volume(elem, 0, back_to_val);
		snd_mixer_selem_set_playback_volume(elem, 1, back_to_val);
		sleep(1);
	}

cleanup:
	snd_mixer_close(handle);
	return error;
}
