#include "../include/subchunks.h"
#include "../include/data_processing.h"
#include "../include/utils.h"

signed short
get_abs_peak16(FILE *file) {
	signed short max = 0;
	signed short min = 0;

	fmt_ptr FMT = alloc_fmt_subchunk();
	get_fmt_subchunk(file, FMT);

	data_header_ptr DATA = alloc_data_header();
	signed long start = get_data_header(file, DATA);

	unsigned long sampleCount = get_data_size(DATA);

	fseek(file, start, SEEK_SET);

	for (unsigned long i=0;i<sampleCount;i++) {
		signed short sample = read_16_bit_sample(file);
		if (sample > max) max = sample;
		if (sample < min) min = sample;
	}

	free_fmt_subchunk(FMT);
	free_data_header(DATA);
	return ((max >= (-min)) ? max : -(min+1));
}
