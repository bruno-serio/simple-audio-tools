#include "../include/utils.h"
//#include <stdio.h>

signed long 
read_little_endian(FILE *file, unsigned char bits) {
	if (bits % 8 != 0) {
		printf("The number of bits is not a multiple of 8\n");
		exit(1);
	}
	signed long sample = 0;
	for (int i=0; i<bits/8; i++) {
		unsigned char c = 0;
		c = fgetc(file);
		sample += (c << (8*i));
	}
	return sample;
}

void
write_little_endian(FILE *fileOut, signed long n, unsigned char bits) {
	if (fileOut == NULL) {
		printf("fileOut NULL pointer\n");
		exit(1);
	}
	if (bits % 8 != 0) {
		printf("The number of bits is not a multiple of 8\n");
		exit(1);
	}

	for (int i=0; i<bits/8; i++) {
		fputc((int)((n >> (8*i)) & 0xff), fileOut);
	}
}
