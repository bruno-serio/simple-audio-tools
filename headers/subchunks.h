#ifndef _SUBCHUNKS_H_
#define _SUBCHUNKS_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>

#define _INFO_LE 0x4F464E49

#define __FREE_RIFF(r) ({if (r != NULL) free_riff_chunk(&r);})
#define __FREE_FMT(f) ({if (f != NULL) free_fmt_subchunk(&f);})
#define __FREE_DATA(d) ({if (d != NULL) free_data_header(&d);})

typedef struct _riff_chunk *riff_t;
typedef struct _fmt_subchunk *fmt_t;
typedef struct _data_header *data_t;
typedef struct _metadata_head *metadata_t;

/* Memory allocation and freeing */

riff_t get_riff(FILE *file);
fmt_t get_fmt(FILE *file);
data_t get_datahead(FILE *file, int32_t *start);
metadata_t get_metadata(FILE *file);

riff_t new_riff(uint32_t ChunkSize);
fmt_t new_fmt(uint32_t Subchunk1Size, uint16_t AudioFormat, uint16_t NumChannels, uint32_t SampleRate, uint16_t BitsPerSample);
data_t new_datahead(uint32_t Subchunk2Size);

void free_riff_chunk(riff_t* r);
void free_fmt_subchunk(fmt_t* fmt);
void free_data_header(data_t* d);
void free_metadata(metadata_t* m);

/* Prints and debugs */

void print_riff_chunk(riff_t r);
void print_fmt_subchunk(fmt_t fmt);
void print_data_header(data_t d);
void print_metadata(metadata_t h);

/* Input and output */

uint32_t riff_size(riff_t r);
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
void write_metadata(FILE *file, metadata_t m);  // ######################### TO BE DONE

/* Editing existing files */

// void remove_all_metadata(FILE *file, const char *filepath);

#endif // _SUBCHUNKS_H_
