#include "../include/utils.h"
//#include <stdio.h>

signed long 
read_little_endian(FILE *file, unsigned char BitsPerSample) {
	if (BitsPerSample % 8 != 0) {
		printf("Bits per sample provided is not a multiple of 8\n");
		exit(1);
	}
	signed short sample = 0;
	for (int i=0; i<BitsPerSample/8; i++) {
		unsigned char c = 0;
		c = fgetc(file);
		sample += (c << (8*i));
	}
	return sample;
}
