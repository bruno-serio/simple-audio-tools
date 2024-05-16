#ifndef _FILE_CREATION_H
#define _FILE_CREATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "subchunks.h"
#include "data_processing.h"
#include "utils.h"

#define MIN_I32S -2147483648
#define __SLICE_AUDIO_FILE(f) slice_audio_file(f, MIN_I32S, MIN_I32S, 0)

FILE* write_file_header(const char* path, riff_t r, fmt_t fmt, data_t d);
file_slice slice_audio_file(FILE* f, int32_t start, int32_t end, uint16_t BitsPerSample);
void copy_audio(file_slice src, FILE* dest, int32_t (*process)(int32_t));
void append_metadata(FILE* f, const char* path, metadata_t m);  // ######################### TO BE DONE

#endif // _FILE_CREATION_H
