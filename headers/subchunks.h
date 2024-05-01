#ifndef _SUBCHUNKS_H_
#define _SUBCHUNKS_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define _INFO_LE 0x4F464E49

typedef struct _riff_chunk *riff_ptr;
typedef struct _fmt_subchunk *fmt_ptr;
typedef struct _data_header *data_header_ptr;
typedef struct _metadata_node *metadata_list;
typedef struct _metadata_head *metadata_head;

/* Memory allocation and freeing */

riff_ptr
get_riff_chunk(FILE *file);

fmt_ptr
get_fmt_subchunk(FILE *file);

data_header_ptr
get_data_header(FILE *file, int32_t *start);

metadata_head
get_metadata(FILE *file);

void
free_riff_chunk(riff_ptr pRIFF);

void
free_fmt_subchunk(fmt_ptr pFMT);

void
free_data_header(data_header_ptr pDataH);

void
free_metadata(metadata_head *head);

/* Prints and debugs */

void
print_riff_chunk(riff_ptr pRIFF);

void
print_fmt_subchunk(fmt_ptr pFMT);

void
print_data_header(data_header_ptr pDataH);

void
print_metadata(metadata_head h);

/* Input and output */

uint32_t
get_riff_chunksize(riff_ptr pRIFF);

uint32_t
get_sample_rate(fmt_ptr pFMT);

uint32_t
get_byte_rate(fmt_ptr pFMT);

uint16_t
get_bits_per_sample(fmt_ptr pFMT);

// Returns the number of bytes in the data subchunk that follow this number
//  = NumSamples * NumChannels * BitsPerSample/8
uint32_t
get_data_size(data_header_ptr pDataH);

/* Writing to file */

void
write_riff_chunk(FILE *file, riff_ptr RIFF);

void
write_fmt_subchunk(FILE *file, fmt_ptr FMT);

void
write_data_header(FILE *file, data_header_ptr DATA);

/* Editing existing files */

void
remove_all_metadata(FILE *file, const char *filepath, metadata_head h);

#endif // _SUBCHUNKS_H_
