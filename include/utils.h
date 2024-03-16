#ifndef _UTILS_H_
#define _UTILS_H_
#include <stdio.h>

signed short
read_16_bit_sample(FILE *file);

signed long
read_24_bit_sample(FILE *file);

signed long 
read_32_bit_sample(FILE *file);

#endif // _UTILS_H_
