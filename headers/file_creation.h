#ifndef _FILE_CREATION_H
#define _FILE_CREATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "subchunks.h"
#include "utils.h"

FILE*
create_new_file(const char *fileName, riff_ptr RIFF, fmt_ptr FMT, data_header_ptr DATA);

#endif // _FILE_CREATION_H
