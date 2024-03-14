#ifndef _SUBCHUNKS_H_
#define _SUBCHUNKS_H_

#include <stdio.h>

//typedef struct _riff_chunk riff_chunk_t;
//typedef struct _fmt_subchunk fmt_subchunk_t;

typedef _riff_chunk *riff_ptr;
typedef _fmt_subchunk *fmt_ptr;

/* Memory allocation and freeing */

void
alloc_riff_chunk(riff_ptr pRIFF);

void
alloc_fmt_subchunk(fmt_ptr pFMT);

void
free_riff_chunk(riff_ptr pRIFF);

void
free_fmt_subchunk(fmt_ptr pFMT);

/* Getting parameters from file */

void
get_riff_chunk(FILE *file, riff_ptr pRIFF);

void
get_fmt_subchunk(FILE *file, fmt_ptr pFMT);

/* Input and output */

void
print_riff_chunk(riff_ptr pRIFF);

void
print_fmt_chunk(fmt_ptr pFMT);

unsigned long
get_riff_chunksize(riff_ptr pRIFF);

unsigned long
get_sample_rate(fmt_ptr pFMT);

unsigned long
get_byte_rate(fmt_ptr pFMT);

unsigned short
get_bits_per_sample(fmt_ptr pFMT);

#endif // _SUBCHUNKS_H_
