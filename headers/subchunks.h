#ifndef _SUBCHUNKS_H_
#define _SUBCHUNKS_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <assert.h>
#include "utils.h"
#include "exit_messages.h"

//#define __FREE_RIFF(r) ({if (r != NULL) free_riff_chunk(&r);})
//#define __FREE_FMT(f) ({if (f != NULL) free_fmt_subchunk(&f);})
//#define __FREE_DATA(d) ({if (d != NULL) free_data_header(&d);})

/**
 * \def INFO_LE
 * \brief Constant for the ASCII of \c "INFO" as \b little \b endian .
 */
#define INFO_LE 0x4F464E49

/**
 * \def __FREE_METADATA(d)
 * \brief	First checks wheter the \c metadata_t pointer \c d is \c NULL and \b if it isn't \b then calls \c free_metadata with the \b address of \c d.
 */
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
 * \pre		\c FILE* \c f is \b not \c NULL.
 */
riff_t get_riff(FILE *file);
//	==================================================

//	==================================================
/**
 * \brief	Reads the \b FMT Subchunk data from the input wave file.
 * \param[in]   file The file that is read.
 * \return	\c fmt_t struct with \c Subchunk1ID , \c Subchunk1Size , \c AudioFormat , \c NumChannels , \c SampleRate , \c ByteRate , \c BlockAlign and \c BitsPerSample .
 * \pre		\c FILE* \c f is \b not \c NULL.
 */
fmt_t get_fmt(FILE *file);
//	==================================================

//	==================================================
/**
 * \brief	Reads the \b Data Subchunk \a header data from the input wave file.
 * \param[in]   file The file that is read.
 * \return	\c data_t struct with \c Subchunk2ID and \c Subchunk2Size .
 * \pre		\c FILE* \c f is \b not \c NULL.
 */
data_t get_datahead(FILE *file, int32_t *start);
//	==================================================

//	==================================================
/**
 * \brief	Reads the \b metadata from the input wave file.
 * \param[in]   file The file that is read.
 * \return	\c metadata_t pointer to a structure that holds all metadata information.
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
 */
riff_t new_riff(uint32_t ChunkSize);
//	==================================================

//	==================================================
/**
 * \brief	Creates a \c fmt_t struct with the data from all fields except the checks, which are auto-calculated.
 * \return	\c fmt_t struct with \c Subchunk1ID , \c Subchunk1Size , \c AudioFormat , \c NumChannels , \c SampleRate , \c ByteRate , \c BlockAlign and \c BitsPerSample .
 */
fmt_t new_fmt(uint32_t Subchunk1Size, uint16_t AudioFormat, uint16_t NumChannels, uint32_t SampleRate, uint16_t BitsPerSample);
//	==================================================

//	==================================================
/**
 * \brief	Creates a \c data_t struct with the given \c Subchunk2Size . 
 * \param[in]   Subchunk2Size The size of the data (which is the size of the subchunk starting after this).
 * \return	\c data_t struct with \c Subchunk2ID and \c Subchunk2Size .
 */
data_t new_datahead(uint32_t Subchunk2Size);
//	==================================================

//void free_riff_chunk(riff_t* r);
//void free_fmt_subchunk(fmt_t* fmt);
//void free_data_header(data_t* d);


//	==================================================
/**
 * \brief	Frees the heap-allocated memory for a structure that held metadata.
 * \param[in]   m The pointer to the memory that will be freed.
 * \pre		\c m is \b not \c NULL .
 */
void free_metadata(metadata_t* m);
//	==================================================

/* Prints and debugs */

//	==================================================
/**
 * \brief	Prints the contents of the \b Riff chunk to the standard output. 
 * \param[in]   r The riff struct to print. 
 */
void print_riff_chunk(riff_t r);
//	==================================================

//	==================================================
/**
 * \brief	Prints the contents of the \b FMT subchunk to the standard output. 
 * \param[in]   fmt The FMT struct to print. 
 */
void print_fmt_subchunk(fmt_t fmt);
//	==================================================

//	==================================================
/**
 * \brief	Prints the contents of the \a header of the \b Data subchunk to the standard output. 
 * \param[in]   d The Data header struct to print. 
 */
void print_data_header(data_t d);
//	==================================================

//	==================================================
/**
 * \brief	Prints the contents of the \b metadata to the standard output. 
 * \param[in]   h The metadata header struct to print. 
 */
void print_metadata(metadata_t h);

/* Input and output */

uint32_t riff_size(riff_t r);
uint16_t audio_format(fmt_t f);
uint32_t audio_size(data_t d);
uint16_t bits_per_sample(fmt_t fmt);
// Returns the number of bytes in the data subchunk that follow this number
//  = NumSamples * NumChannels * BitsPerSample/8

//	==================================================
/**
 * \brief	Returns the size in bytes for the \b entire \b file.
 * \param[in]   f The file whose size will be checked. 
 * \return	\c uint32_t with the size of the entire file in \b bytes.
 */
uint32_t file_size(FILE *f);
//	==================================================


//	==================================================
/**
 * \brief	Returns the size in bytes for the \b full metadata subchunk.
 * \param[in]   m The \c metadata_t pointer to the allocated metadata. 
 * \return	\c uint32_t with the metadata size in \b bytes.
 */
uint32_t metadata_size(metadata_t m);
//	==================================================


/* Operations  */

//	==================================================
/**
 * \brief	Calculates the required riff size given the size of the other subchunks.
 * \param[in]   d   The data_t header struct. 
 * \param[in]   m   The metadata_t header struct. 
 * \return	\c uint32_t with the appropiate size.
 */
uint32_t calc_riff_size(data_t d, metadata_t m);
//	==================================================

void set_riff_size(riff_t r, uint32_t size);

/* Writing to file */

//	==================================================
/**
 * \brief	Writes the entire \b Riff Chunk to the file \c file .
 * \param[out]  file The file that \c RIFF will be written to. 
 * \param[in]   RIFF The Riff struct, containing the information that will be written. 
 */
void write_riff(FILE *file, riff_t RIFF);
//	==================================================

//	==================================================
/**
 * \brief	Writes the entire \b FMT Subchunk to the file \c file .
 * \param[out]  file The file that \c FMT will be written to. 
 * \param[in]   FMT The FMT struct, containing the information that will be written. 
 */
void write_fmt(FILE *file, fmt_t FMT);
//	==================================================

//	==================================================
/**
 * \brief	Writes the \a header for the \b Data Subchunk to the file \c file .
 * \param[out]  file The file that \c d will be written to. 
 * \param[in]   d The data header struct, containing the information that will be written. 
 */
void write_data(FILE *file, data_t d);
//	==================================================

//	==================================================
/**
 * \brief	Appends the \b metadata provided to the file \c file .
 * \param[out]  file The file that \c m will be written to. 
 * \param[in]   m The metadata pointer to a structure that holds all metadata information.
 * \param[in]   write_head Determines if the header for the metadata subchunk is written. If \c false it will only \b append metadata.
 * \pre		If \c write_head is set to \c true there \b cannot be metadata subchunk in the file for it to be valid.
 * \pre		If \c write_head is set to \c false there \b needs to be a header \a already written to the file.
 */
void write_metadata(FILE *file, metadata_t m, bool write_head);

/* Editing existing files */

// void remove_all_metadata(FILE *file, const char *filepath);

#endif // _SUBCHUNKS_H_
