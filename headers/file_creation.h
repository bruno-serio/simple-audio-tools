#ifndef _FILE_CREATION_H
#define _FILE_CREATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "subchunks.h"
#include "utils.h"

FILE* write_file(const char* path, riff_t r, fmt_t fmt, data_t d, metadata_t m);

/*
FILE* create_new_file(const char *fileName, riff_t r, fmt_t fmt);
void copy_audio_data(FILE *source, FILE *dest);
*/

#endif // _FILE_CREATION_H
