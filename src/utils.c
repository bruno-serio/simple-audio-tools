#include "../include/utils.h"

signed short
read_16_bit_sample(FILE *file) {
	signed short sample = 0;
	for (int i=0; i<2; i++) {
		char c = fgetc(file);
		sample += (c << (8*i));
	}
	return sample;
}

signed long
read_24_bit_sample(FILE *file) {
	signed short sample = 0;
	for (int i=0; i<3; i++) {
		char c = fgetc(file);
		sample += (c << (8*i));
	}
	return sample;
}

signed long
read_32_bit_sample(FILE *file) {
	signed short sample = 0;
	for (int i=0; i<4; i++) {
		char c = fgetc(file);
		sample += (c << (8*i));
	}
	return sample;
}
