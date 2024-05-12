#ifndef _FILE_CREATION_H
#define _FILE_CREATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "subchunks.h"
#include "utils.h"

FILE*
create_new_file(const char *fileName, riff_ptr RIFF, fmt_ptr FMT);

void
copy_audio_data(FILE *source, FILE *dest);


#endif // _FILE_CREATION_H
