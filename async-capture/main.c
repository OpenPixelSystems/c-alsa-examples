/**
 * @file main.c
 * @brief  ALSA simple async capture example
 * @author Bram Vlerick <bram.vlerick@openpixelsystems.org>
 * @version
 * @date 2020-03-30
 */

#include <stdio.h>
#include <signal.h>
#include <stdbool.h>
#include <alloca.h>

#include <alsa/asoundlib.h>

#define NR_SAMPLES 256
#define NR_CHANNELS 1
#define FMT_SIZE 2

bool wait_for_async = false;

void _record_callback(snd_async_handler_t *pcm_callback)
{
	printf("Eeeeeh\n");
	wait_for_async = true;
}

int main()
{
	int error = 0;
	unsigned int nr_channels = NR_CHANNELS;
	unsigned int sample_rate = 16000;
	unsigned long period_size = NR_SAMPLES;
	snd_pcm_t *handle = NULL;
	snd_pcm_sw_params_t *sw_params = NULL;
	snd_pcm_hw_params_t *hw_params = NULL;

	/* Open the default alsa sound device in capture mode */
	error = snd_pcm_open(&handle, "pulse", SND_PCM_STREAM_CAPTURE, 0);
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

	snd_pcm_uframes_t buffer_size = 1024;
	snd_pcm_hw_params_set_buffer_size_near (handle, hw_params, &buffer_size);

	/* Push ALSA HW params */
	if ((error = snd_pcm_hw_params(handle, hw_params)) < 0) {
		printf("Failed to set HW params\n");
		goto cleanup;
	}

	snd_pcm_sw_params_set_start_threshold(handle, sw_params, buffer_size - period_size);
	snd_pcm_sw_params_set_avail_min(handle, sw_params, period_size);
	snd_pcm_sw_params(handle, sw_params); //!< Set SW params

	snd_pcm_sw_params_free (sw_params); //!< Cleanup

	snd_pcm_prepare(handle); //!< Prepare the PCM device for playback or capture

	snd_async_handler_t *async_handler;
	if ((error = snd_async_add_pcm_handler(&async_handler, handle, _record_callback, NULL)) < 0) {
		printf("Failed to add PCM handler! ASync not supported \n");
		goto cleanup;
	}

	snd_pcm_start(handle); //!< Start capture

	printf("Capture setup succesful\n");

	for (int i = 0; i < 100; i++) {
		/* Note: ALSA Reads/Writes in number of samples! */
		/* char read_buffer[NR_SAMPLES * NR_CHANNELS * FMT_SIZE]; */
		/* int readn = snd_pcm_readi(handle, read_buffer, NR_SAMPLES); */
		printf("hmmmm\n");
		while (!wait_for_async) {
			usleep(100);
		}
		printf("Done\n");
		wait_for_async = false;
	}
	printf("Bye bye!\n");

cleanup:
	if (handle) {
		snd_pcm_close(handle);
	}
	return error;
}
