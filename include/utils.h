#ifndef _UTILS_H_
#define _UTILS_H_
#include <stdio.h>
#include <stdlib.h>

#define read_sample(f, b) read_little_endian(f, b)

signed long
read_little_endian(FILE *file, unsigned char BitsPerSample);

/*
signed long
read_sl_little_endian(FILE *file, unsigned char BitsPerSample);

signed long
read_sl_little_endian(FILE *file, unsigned char BitsPerSample);
*/
#endif // _UTILS_H_
