#ifndef _SUBCHUNKS_H_
#define _SUBCHUNKS_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct _riff_chunk *riff_ptr;
typedef struct _fmt_subchunk *fmt_ptr;
typedef struct _data_header *data_header_ptr;

/* Memory allocation and freeing */

riff_ptr
get_riff_chunk(FILE *file);

fmt_ptr
get_fmt_subchunk(FILE *file);

data_header_ptr
get_data_header(FILE *file, signed long *start);

void
free_riff_chunk(riff_ptr pRIFF);

void
free_fmt_subchunk(fmt_ptr pFMT);

void
free_data_header(data_header_ptr pDataH);


/* Prints and debugs */

void
print_riff_chunk(riff_ptr pRIFF);

void
print_fmt_subchunk(fmt_ptr pFMT);

void
print_data_header(data_header_ptr pDataH);

/* Input and output */

unsigned long
get_riff_chunksize(riff_ptr pRIFF);

unsigned long
get_sample_rate(fmt_ptr pFMT);

unsigned long
get_byte_rate(fmt_ptr pFMT);

unsigned short
get_bits_per_sample(fmt_ptr pFMT);

// Returns the number of bytes in the data subchunk that follow this number
//  = NumSamples * NumChannels * BitsPerSample/8
unsigned long
get_data_size(data_header_ptr pDataH);

/* Writing to file */

void
write_riff_chunk(FILE *file, riff_ptr RIFF);

void
write_fmt_subchunk(FILE *file, fmt_ptr FMT);

void
write_data_header(FILE *file, data_header_ptr DATA);

#endif // _SUBCHUNKS_H_
