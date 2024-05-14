#include "../headers/subchunks.h"
#include "../headers/data_processing.h"
#include "../headers/utils.h"

int16_t
get_abs_peak(FILE *file, uint16_t BitsPerSample) {
	int16_t max = 0;
	int16_t min = 0;
	int32_t start = 0; 

	fmt_t fmt = get_fmt_subchunk(file);
	data_t d = get_data_header(file, &start);
	uint32_t sampleCount = audio_size(d);
	if (BitsPerSample == DEF_BITSPERSAMPLE) BitsPerSample = bits_per_sample(fmt);

	fseek(file, start, SEEK_SET);

	for (uint32_t i=0;i<sampleCount;i++) {
		int16_t sample = (int16_t)read_sample(file, BitsPerSample);
		if (sample > max) max = sample;
		if (sample < min) min = sample;
	}

	free_fmt_subchunk(fmt);
	free_data_header(d);
	return ((max >= (-min)) ? max : -(min+1));
}
