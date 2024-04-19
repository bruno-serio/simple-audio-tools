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
		//printf("%0x %0x %0x\n", 0x11&0x11, 0x11&0x10, 0x11&0x00);
		//printf("%0x\n", (int)(0x11&(n >> (8*i))));
		//fputc((int)(char)(0x11&(n >> (8*i))), fileOut);
		//int c = 0x11 & (n >> (8*i));
		//printf("%0x", c);
		//fputc(c, fileOut);
		fputc((int)((n >> (8*i)) & 0x00000011), fileOut);
	}
	// fputc
	//(n >> (8*i)) & 0x00000011
}
