#include "../headers/subchunks.h"
#include "../headers/data_processing.h"
#include "../headers/utils.h"

signed short
get_abs_peak(FILE *file, unsigned short BitsPerSample) {
	signed short max = 0;
	signed short min = 0;
	signed long start = 0; 

	fmt_ptr FMT = get_fmt_subchunk(file);
	data_header_ptr DATA = get_data_header(file, &start);
	unsigned long sampleCount = get_data_size(DATA);
	if (BitsPerSample == DEF_BITSPERSAMPLE) BitsPerSample = get_bits_per_sample(FMT);

	fseek(file, start, SEEK_SET);

	for (unsigned long i=0;i<sampleCount;i++) {
		signed short sample = (signed short)read_sample(file, BitsPerSample);
		if (sample > max) max = sample;
		if (sample < min) min = sample;
	}

	free_fmt_subchunk(FMT);
	free_data_header(DATA);
	return ((max >= (-min)) ? max : -(min+1));
}
