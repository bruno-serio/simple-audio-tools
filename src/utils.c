#include "../include/utils.h"
#include "../include/exit_messages.h"

signed long 
read_little_endian(FILE *file, unsigned char bits) {
	if (bits % 8 != 0)
		exit_error(NOT_8_BIT_MULT);
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
		exit_error(PASSED_NULL_POINTER);
	if (bits % 8 != 0)
		exit_error(NOT_8_BIT_MULT);

	for (int i=0; i<bits/8; i++) {
		fputc((int)((n >> (8*i)) & 0xff), fileOut);
	}
}
