#ifndef _GET_SUBCHUNKS_H_
#define _GET_SUBCHUNKS_H_

#include <stdio.h>

struct _riff_chunk {
	char ChunkID[4];
	unsigned long ChunkSize;
	char Format[4];
};

struct _fmt_subchunk {
	char Subchunk1ID[4];
	unsigned long Subchunk1Size;
	unsigned short AudioFormat;
	unsigned short NumChannels;
	unsigned long SampleRate;
	unsigned long ByteRate;
	unsigned short BlockAlign;
	unsigned short BitsPerSample;
};

typedef struct _riff_chunk riff_chunk_t;
typedef struct _fmt_subchunk fmt_subchunk_t;

void
get_riff_chunk(FILE *file, riff_chunk_t *dataChunk);

void
get_fmt_subchunk(FILE *file, fmt_subchunk_t *fmtChunk);

#endif // _GET_SUBCHUNKS_H_
