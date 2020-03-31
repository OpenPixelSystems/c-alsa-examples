/**
 * @file main.c
 * @brief  ALSA simple playback example
 * @author Bram Vlerick <bram.vlerick@openpixelsystems.org>
 * @version
 * @date 2020-03-30
 */

#include <stdio.h>
#include <signal.h>
#include <stdbool.h>
#include <alloca.h>

#include <alsa/asoundlib.h>

#include "wave.h"

#define NR_SAMPLES 256
#define NR_CHANNELS 2
#define FMT_SIZE 2

int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("./alsa-playback <file>\n");
		return -1;
	}

	int error = 0;
	char *write_buffer = NULL;
	unsigned int nr_channels = NR_CHANNELS;
	unsigned int sample_rate = 16000;
	unsigned long period_size = NR_SAMPLES;
	snd_pcm_t *handle = NULL;
	snd_pcm_sw_params_t *sw_params = NULL;
	snd_pcm_hw_params_t *hw_params = NULL;

	/* Open the default alsa sound device in capture mode */
	error = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
	if (error < 0) {
		printf("Failed to open PCM device %s\n", snd_strerror(error));
		goto cleanup;
	}

	/* Allocate HW and SW params structure */
	snd_pcm_sw_params_malloc(&sw_params);
	snd_pcm_sw_params_current(handle, sw_params);

	snd_pcm_hw_params_alloca(&hw_params);

	/* Setup HW params for all possible parameters */
	if (snd_pcm_hw_params_any(handle, hw_params) < 0) {
		printf("Failed to retrieve HW params\n");
		goto cleanup;
	}

	/* Setup the ALSA interal parameters */
	if ((error = snd_pcm_hw_params_set_access(handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
		printf("ERROR: Can't set interleaved mode. %s\n", snd_strerror(error));
		goto cleanup;
	}

	if ((error = snd_pcm_hw_params_set_format(handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
		printf("ERROR: Can't set format. %s\n", snd_strerror(error));
		goto cleanup;
	}

	if ((error = snd_pcm_hw_params_set_channels(handle, hw_params, nr_channels)) < 0) {
		printf("ERROR: Can't set channels number. %s\n", snd_strerror(error));
		goto cleanup;
	}
	if ((error = snd_pcm_hw_params_set_rate_near(handle, hw_params, &sample_rate, 0)) < 0) {
		printf("ERROR: Can't set rate. %s\n", snd_strerror(error));
		goto cleanup;
	}

	if ((error = snd_pcm_hw_params_set_period_size_near(handle, hw_params, &period_size, 0)) < 0) {
		printf("Error: Can't set period size\n");
		return -1;

	}

	/* Push ALSA HW params */
	if ((error = snd_pcm_hw_params(handle, hw_params)) < 0) {
		printf("Failed to set HW params\n");
		goto cleanup;
	}

	printf("Capture setup succesful\n");

	FILE *wfp = NULL;
	wfp = fopen(argv[1], "r"); //!< Insecure unchecked argument here. Bad idea in proper software!
	if (!wfp) {
		printf("Failed to open wav header\n");
		return -1;
	}

	struct wave_header_t wheader;
	if ((error = wave_header_parse(wfp, &wheader)) < 0) {

	}

	write_buffer = (char *)malloc(wheader.subchunk2_size);
	if (!write_buffer) {
		fclose(wfp);
		return -1;
	}

	int n = fread(write_buffer, sizeof(char), wheader.subchunk2_size, wfp);
	if (n < 0) {
		printf("Failed to read data\n");
		goto cleanup;
	}

	/* Note: This will only play until the last buffers which equals the buffes size */
	/* So this might drop the last few samples */
	/* Also this loop does handle underruns properly. It's just tries to recover the ALSA state */
	int nr_buffers = wheader.subchunk2_size / (NR_SAMPLES * NR_CHANNELS * FMT_SIZE);
	for (int i = 0; i < nr_buffers; i++) {
		/* Note: ALSA Reads/Writes in number of samples! */
		int writen = snd_pcm_writei(handle, &write_buffer[i * (NR_SAMPLES * NR_CHANNELS * FMT_SIZE)], NR_SAMPLES);
		if (writen < 0) {
			/* Recover the ALSA internal state if an error occurse */
			int recover = snd_pcm_recover(handle, writen, 0);
			if (recover < 0) {
				printf("Failed to recover ALSA state\n");
				goto cleanup;
			}
			printf("ALSA State recoverd\n");
		}
		/* printf("written %d samples\n", writen); */
	}
	printf("Bye bye!\n");

cleanup:
	if (handle) {
		snd_pcm_close(handle);
	}
	if (write_buffer) {
		free(write_buffer);
	}
	return error;
}
