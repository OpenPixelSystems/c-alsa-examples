/**
 * @file wave.h
 * @brief  WAVE helper library
 * @author Bram Vlerick <bram.vlerick@openpixelsystems.org>
 * @version v1.0
 * @date 2019-01-28
 */

#ifndef _WAVE_HELPER_H_
#define _WAVE_HELPER_H_

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <byteswap.h>

#define WAVE_HDR_LEN 44                 //!< Standard wave header length
#define WAVE_CHUNK_OFFSET 8             //!< Offeset for first chuck
#define WAVE_SUBCHUNK1_PCM_LEN 16       //!< PCM Chunk length
#define WAVE_CHUNK_ID 0x52494646        //!< "RIFF" ASCII
#define WAVE_FMT 0x57415645             //!< "WAVE" ASCII
#define WAVE_SUBCHUNK1_ID 0x666D7420    //!< "fmt " ASCII
#define WAVE_SUBCHUNK2_ID 0x64617461    //!< "data" ASCII
#define WAVE_FORMAT_PCM 1               //!< WAVE Format constant

/* --------------------------------------------------------------------------*/
/**
 * @brief  WAVE header structure.
 * @note Only PCM support implemented at this moment
 * no other formats are supported
 */
/* --------------------------------------------------------------------------*/
struct wave_header_t {
	uint32_t	chunk_id;               //!< Should contain "RIFF" (BE)
	uint32_t	chunk_size;             //!< Chunksize (LE)
	uint32_t	format;                 //!< Should contain "WAVE" (BE)
	uint32_t	subchunk1_id;           //!< Should contain "fmt " (BE)
	uint32_t	subchunk1_size;         //!< Subchunk size (LE)
	uint16_t	audio_fmt;              //!< PCM = 1, Different from 1 is compressed in some way (LE)
	uint16_t	nr_channels;            //!< Number of channels
	uint32_t	samplerate;             //!< Samplerate (LE)
	uint32_t	byterate;               //!< Samplerate * # channels * fmt (bits per sample / 8) (LE)
	uint16_t	block_align;            //!< Number of bytes for 1 sample (LE)
	uint16_t	bits_per_sample;        //!< 8, 16,... (LE)
	uint32_t	subchunk2_id;           //!< Should be "data" (BE)
	uint32_t	subchunk2_size;         //!< Data size (LE)
};

/* --------------------------------------------------------------------------*/
/**
 * @brief  Parse a WAVE header
 *
 * @param fp A FILE ptr to the WAVE file
 * @param hdr Pointer to an allocated header which will be filled
 *
 * @returns   -1 if failed, 0 if succesfull
 */
/* --------------------------------------------------------------------------*/
int wave_header_parse(FILE *fp, struct wave_header_t *hdr);

#endif                          /* WAVE_HELPER_H_ */
