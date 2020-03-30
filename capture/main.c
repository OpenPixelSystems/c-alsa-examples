#include <stdio.h>
#include <alloca.h>

#include <alsa/asoundlib.h>

int main()
{
	int error = 0;
	unsigned int nr_channels = 2;
	unsigned int sample_rate = 16000;
	snd_pcm_t *handle = NULL;
	snd_pcm_sw_params_t *sw_params = NULL;
	snd_pcm_hw_params_t *hw_params = NULL;

	/* Open the default alsa sound device in capture mode */
	error = snd_pcm_open(&handle, "default", SND_PCM_STREAM_CAPTURE, 0);
	if (error < 0) {
		printf("Failed to open PCM device %s\n", snd_strerror(error));
		goto cleanup;
	}

	/* Allocate HW and SW params structure */
	snd_pcm_hw_params_alloca(&hw_params);
	snd_pcm_sw_params_malloc(&sw_params);

	/* Setup HW params for all possible parameters */
	if (snd_pcm_hw_params_any(handle, hw_params) < 0) {
		printf("Failed to retrieve HW params\n");
		goto cleanup;
	}

	/* Retrieve current SW params */
	if (snd_pcm_sw_params_current(handle, sw_params) < 0) {
		printf("Failed to retrieve current SW params\n");
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

	if ((error = snd_pcm_hw_params(handle, hw_params)) < 0) {
		printf("Failed to set HW params\n");
		goto cleanup;
	}

	printf("Capture setup succesful\n");

cleanup:
	if (handle) {
		snd_pcm_close(handle);
	}
	return error;
}
