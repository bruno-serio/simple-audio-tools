#ifndef _FILE_CREATION_H
#define _FILE_CREATION_H

#include <stdio.h>
#include <stdlib.h>
#include "subchunks.h"

// returns offset for writing data
signed int
create_file_from_header(riff_ptr RIFF, fmt_ptr FMT, data_header_ptr DATA);

#endif // _FILE_CREATION_H
