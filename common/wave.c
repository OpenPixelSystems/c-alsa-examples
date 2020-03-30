/**
 * @file snd-wave.c
 * @brief  WAVE helper library
 * @author Bram Vlerick <bram.vlerick@openpixelsystems.org>
 * @version v1.0
 * @date 2019-01-28
 */

#include "wave.h"

int wave_header_parse(FILE *fp, struct wave_header_t *hdr)
{
	int error;

	if (!fp) {
		return -1;
	}

	fseek(fp, 0L, SEEK_END);
	int fsize = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	error = fread((void *)hdr, sizeof(struct wave_header_t), 1, fp);
	if (error < 0) {
		return -1;
	}

	/* Swap Big-endian words to Little-endian */
	hdr->chunk_id = __bswap_32(hdr->chunk_id);
	hdr->format = __bswap_32(hdr->format);
	hdr->subchunk1_id = __bswap_32(hdr->subchunk1_id);
	hdr->subchunk2_id = __bswap_32(hdr->subchunk2_id);

	/* Verify chunks and subchunks */

	if (hdr->chunk_id != WAVE_CHUNK_ID) {
		return -1;
	}
	if (hdr->format != WAVE_FMT) {
		return -1;
	}
	if (hdr->subchunk1_id != WAVE_SUBCHUNK1_ID) {
		return -1;
	}
	if (hdr->subchunk2_id != WAVE_SUBCHUNK2_ID) {
		return -1;
	}
	if ((hdr->chunk_size + WAVE_CHUNK_OFFSET) != fsize) {
		return -1;
	}
	if (hdr->audio_fmt == WAVE_FORMAT_PCM && hdr->subchunk1_size != WAVE_SUBCHUNK1_PCM_LEN) {
		return -1;
	}

	printf("Chunk id: 0x%x\n", hdr->chunk_id);
	printf("Format  : 0x%x\n", hdr->format);
	printf("Sub1 id : 0x%x\n", hdr->subchunk1_id);
	printf("Sub2 id : 0x%x\n", hdr->subchunk2_id);

	printf("Chunk len : %u\n", hdr->chunk_size);
	printf("Sub1 len  : %u\n", hdr->subchunk1_size);
	printf("Sub2 len  : %u\n", hdr->subchunk2_size);

	printf("Audio fmt : %s\n", hdr->audio_fmt == 1 ? "PCM" : "Other");
	printf("Samplerate: %u\n", hdr->samplerate);
	printf("Channels  : %d\n", hdr->nr_channels);
	printf("Align     : %u\n", hdr->block_align);
	printf("Bits P/S  : %d\n", hdr->bits_per_sample);
	printf("Byterate  : %u\n", hdr->byterate);

	return 0;
}
