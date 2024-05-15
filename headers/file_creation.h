#ifndef _FILE_CREATION_H
#define _FILE_CREATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "subchunks.h"
#include "utils.h"

FILE* write_file_header(const char* path, riff_t r, fmt_t fmt, data_t d);
void copy_audio(FILE* src, FILE* dest, int32_t (*processing)(int32_t));
void append_metadata(FILE* f, metadata_t m);

#endif // _FILE_CREATION_H
