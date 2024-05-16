#include "../headers/data_processing.h"

struct _file_slice {
	FILE* file;
	uint32_t length;
	int32_t offset;
	uint16_t BitsPerSample;
};

file_slice 
new_fslice(FILE* file, uint32_t length, int32_t offset, uint16_t BitsPerSample) {
	file_slice s = malloc(sizeof(struct _file_slice));
	if (s == NULL)
		exit_error(MEM_ALLOC_FAILED);

	s->file = file;
	s->length = length;
	s->offset = offset;
	s->BitsPerSample = BitsPerSample;

	return s;
}

void 
free_fslice(file_slice* s, bool closeFile) {
	if (closeFile)
		fclose((*s)->file);
	free(*s);
	*s = NULL;
}


FILE*
fslice_file(file_slice s) {
	if (s == NULL)
		exit_error(PASSED_NULL_POINTER);
	return s->file;
}

uint32_t 
fslice_len(file_slice s) {
	if (s == NULL)
		exit_error(PASSED_NULL_POINTER);
	return s->length;
}

int32_t 
fslice_offset(file_slice s) {
	if (s == NULL)
		exit_error(PASSED_NULL_POINTER);
	return s->offset;
}

uint16_t 
fslice_bits(file_slice s) {
	if (s == NULL)
		exit_error(PASSED_NULL_POINTER);
	return s->BitsPerSample;
}

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

	__FREE_FMT(fmt);
	__FREE_DATA(d);
	return ((max >= (-min)) ? max : -(min+1));
}
