#include "../include/utils.h"
#include <stdio.h>

signed short
read_16_bit_sample(FILE *file) {
	signed short sample = 0;
	for (int i=0; i<2; i++) {
		signed short c = fgetc(file);
		sample += (c << (8*i));
	}
	return sample;
}

signed long
read_24_bit_sample(FILE *file) {
	signed short sample = 0;
	for (int i=0; i<3; i++) {
		signed long c = fgetc(file);
		sample += (c << (8*i));
	}
	return sample;
}

signed long
read_32_bit_sample(FILE *file) {
	signed short sample = 0;
	for (int i=0; i<4; i++) {
		signed long c = fgetc(file);
		sample += (c << (8*i));
	}
	return sample;
}
