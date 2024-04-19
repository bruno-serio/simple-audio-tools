#ifndef _DATA_PROCESSING_H_
#define _DATA_PROCESSING_H_

#include <stdio.h>

#define DEF_BITSPERSAMPLE 0
#define get_abs_peak_def(data) get_abs_peak((data), DEF_BITSPERSAMPLE)
	
signed short
get_abs_peak(FILE *file, unsigned short BitsPerSample);

#endif // _DATA_PROCESSING_H_
