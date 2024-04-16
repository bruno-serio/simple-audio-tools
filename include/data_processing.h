#ifndef _DATA_PROCESSING_H_
#define _DATA_PROCESSING_H_

#include <stdio.h>

// pasar en vez de 40 un numero imposible y ahi recorrer el archivo hasta encontrar
#define get_abs_peak16_def(data) get_abs_peak16((data), 44)
	
signed short
get_abs_peak16(FILE *file);

#endif // _DATA_PROCESSING_H_
