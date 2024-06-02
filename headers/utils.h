#ifndef _UTILS_H_
#define _UTILS_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "exit_messages.h"

#define read_sample(f, b) read_little_endian(f, b)
#define __FREE_FILEPATH(p) ({if (p != NULL) free_filepath(&p);})

int32_t read_little_endian(FILE *file, uint8_t bits);
int32_t read_big_endian(FILE *file, uint8_t bits);

float read_f_little_endian(FILE *file);
//float read_f_big_endian(FILE *file, uint8_t bits);

void write_little_endian(FILE *fileOut, int32_t n, uint8_t bits);

const char* get_filepath(const char *dir, const char *fileName);
void free_filepath(const char **fp);

/*
signed long read_sl_little_endian(FILE *file, unsigned char bits);
signed long read_ul_little_endian(FILE *file, unsigned char bits);
void write_sl_little_endian(FILE *fileOut, signed long n, unsigned char bits);
void write_ul_little_endian(FILE *fileOut, signed long n, unsigned char bits);
*/
#endif // _UTILS_H_
