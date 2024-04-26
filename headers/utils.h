#ifndef _UTILS_H_
#define _UTILS_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define read_sample(f, b) read_little_endian(f, b)

signed long
read_little_endian(FILE *file, unsigned char bits);

signed long
read_big_endian(FILE *file, unsigned char bits);

void
write_little_endian(FILE *fileOut, signed long n, unsigned char bits);

const char*
get_filepath(const char *dir, const char *fileName);

void
free_filepath(const char *fp);

/*
signed long
read_sl_little_endian(FILE *file, unsigned char bits);

signed long
read_ul_little_endian(FILE *file, unsigned char bits);

void
write_sl_little_endian(FILE *fileOut, signed long n, unsigned char bits);

void
write_ul_little_endian(FILE *fileOut, signed long n, unsigned char bits);

*/
#endif // _UTILS_H_
