#ifndef _SUBCHUNKS_H_
#define _SUBCHUNKS_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <assert.h>
#include "utils.h"
#include "exit_messages.h"

#define INFO_LE 0x4F464E49

//#define __FREE_RIFF(r) ({if (r != NULL) free_riff_chunk(&r);})
//#define __FREE_FMT(f) ({if (f != NULL) free_fmt_subchunk(&f);})
//#define __FREE_DATA(d) ({if (d != NULL) free_data_header(&d);})
#define __FREE_METADATA(d) ({if (d != NULL) free_metadata(&d);})
		
typedef struct {
	char ChunkID[4];
	uint32_t ChunkSize; //  This is the size of the entire file in bytes minus 8 bytes for
			    //  the two fields not included in this count: ChunkID and ChunkSize.
	char Format[4];
} riff_t;

typedef struct {
	char Subchunk1ID[4];
	uint32_t Subchunk1Size;
	uint16_t AudioFormat;
	uint16_t NumChannels;
	uint32_t SampleRate;
	uint32_t ByteRate;
	uint16_t BlockAlign;
	uint16_t BitsPerSample;
} fmt_t;

typedef struct {
	char Subchunk2ID[4];
	uint32_t Subchunk2Size;
} data_t;

typedef struct _metadata_head *metadata_t;

/* Memory allocation and freeing */

//	==================================================
/**
 * \brief	Reads the \b Riff Chunk data from the input wave file.
 * \param[in]   file The file that is read.
 * \return	\c riff_t struct with \c ChunkID , \c ChunkSize and \c Format.
 * \headerfile	subchunks.h "SUBCHUNKS header"
 * \pre		\c FILE* \c f is \b not \c NULL.
 */
riff_t get_riff(FILE *file);
//	==================================================

//	==================================================
/**
 * \brief	Reads the \b FMT Subchunk data from the input wave file.
 * \param[in]   file The file that is read.
 * \return	\c fmt_t struct with \c Subchunk1ID , \c Subchunk1Size , \c AudioFormat , \c NumChannels , \c SampleRate , \c ByteRate , \c BlockAlign and \c BitsPerSample .
 * \headerfile	subchunks.h "SUBCHUNKS header"
 * \pre		\c FILE* \c f is \b not \c NULL.
 */
fmt_t get_fmt(FILE *file);
//	==================================================

//	==================================================
/**
 * \brief	Reads the \b Data Subchunk \a header data from the input wave file.
 * \param[in]   file The file that is read.
 * \return	\c data_t struct with \c Subchunk2ID and \c Subchunk2Size .
 * \headerfile	subchunks.h "SUBCHUNKS header"
 * \pre		\c FILE* \c f is \b not \c NULL.
 */
data_t get_datahead(FILE *file, int32_t *start);
//	==================================================

//	==================================================
/**
 * \brief	Reads the \b metadata from the input wave file.
 * \param[in]   file The file that is read.
 * \return	\c metadata_t pointer to a structure that holds all metadata information.
 * \headerfile	subchunks.h "SUBCHUNKS header"
 * \pre		\c FILE* \c f is \b not \c NULL.
 * \post	\b Heap-allocates memory.
 */
metadata_t get_metadata(FILE *file);
//	==================================================

//	==================================================
/**
 * \brief	Creates a \c riff_t struct with the given \c ChunkSize .
 * \param[in]   ChunkSize The size of the entire file in bytes \c -8.
 * \return	\c riff_t struct with \c ChunkID , \c ChunkSize and \c Format.
 * \headerfile	subchunks.h "SUBCHUNKS header"
 */
riff_t new_riff(uint32_t ChunkSize);
//	==================================================

//	==================================================
/**
 * \brief	Creates a \c fmt_t struct with the data from all fields except the checks, which are auto-calculated.
 * \return	\c fmt_t struct with \c Subchunk1ID , \c Subchunk1Size , \c AudioFormat , \c NumChannels , \c SampleRate , \c ByteRate , \c BlockAlign and \c BitsPerSample .
 * \headerfile	subchunks.h "SUBCHUNKS header"
 */
fmt_t new_fmt(uint32_t Subchunk1Size, uint16_t AudioFormat, uint16_t NumChannels, uint32_t SampleRate, uint16_t BitsPerSample);
//	==================================================

//	==================================================
/**
 * \brief	Creates a \c data_t struct with the given \c Subchunk2Size . 
 * \param[in]   Subchunk2Size The size of the data (which is the size of the subchunk starting after this).
 * \return	\c data_t struct with \c Subchunk2ID and \c Subchunk2Size .
 * \headerfile	subchunks.h "SUBCHUNKS header"
 */
data_t new_datahead(uint32_t Subchunk2Size);

//void free_riff_chunk(riff_t* r);
//void free_fmt_subchunk(fmt_t* fmt);
//void free_data_header(data_t* d);
void free_metadata(metadata_t* m);

/* Prints and debugs */

void print_riff_chunk(riff_t r);
void print_fmt_subchunk(fmt_t fmt);
void print_data_header(data_t d);
void print_metadata(metadata_t h);

/* Input and output */

uint32_t riff_size(riff_t r);
uint16_t audio_format(fmt_t f);
uint32_t file_size(FILE *f);
// uint32_t fmt_size(fmt_t t);
uint32_t audio_size(data_t d);
uint32_t metadata_size(metadata_t m);

// uint32_t sample_rate(fmt_t fmt);
// uint32_t byte_rate(fmt_t fmt);
uint16_t bits_per_sample(fmt_t fmt);

// Returns the number of bytes in the data subchunk that follow this number
//  = NumSamples * NumChannels * BitsPerSample/8

/* Operations  */

uint32_t calc_riff_size(fmt_t fmt, data_t d, metadata_t m);
void set_riff_size(riff_t r, uint32_t size);

/* Writing to file */

void write_riff(FILE *file, riff_t RIFF);
void write_fmt(FILE *file, fmt_t FMT);
void write_data(FILE *file, data_t d);
void write_metadata(FILE *file, metadata_t m, bool write_head);

/* Editing existing files */

// void remove_all_metadata(FILE *file, const char *filepath);

#endif // _SUBCHUNKS_H_
