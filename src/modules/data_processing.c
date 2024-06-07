#include "../../headers/data_processing.h"

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

int32_t
get_abs_peak(FILE *file, uint16_t BitsPerSample) {
	int16_t max = 0;
	int16_t min = 0;
	int32_t start = 0; 

	fmt_t fmt = get_fmt(file);
	data_t d = get_datahead(file, &start);
	uint32_t sampleCount = audio_size(d);
	if (BitsPerSample == DEF_BITSPERSAMPLE) BitsPerSample = bits_per_sample(fmt);

	fseek(file, start, SEEK_SET);

	for (uint32_t i=0;i<sampleCount;i++) {
		int16_t sample = (int16_t)read_sample(file, BitsPerSample);
		if (sample > max) max = sample;
		if (sample < min) min = sample;
	}

	return ((max >= (-min)) ? max : -(min+1));
}

float normalize_ratio(int32_t peak, uint16_t BitsPerSample) {
	int32_t max = 1;
	switch (BitsPerSample) {
		case 8:  max = 0x7f;		// 011111111
		   	 break;
		case 16: max = 0x7fff;		// 0111111111111111
			 break;
		case 24: max = 0x7fffff;	// 011111111111111111111111
			 break;
		case 32: max = 0x7fffffff;	// 01111111111111111111111111111111
			 break;
		default:
			 exit_error(NOT_8_BIT_MULT);
	}
	return (float)(peak/max);
}

int32_t scale_sample(int32_t sample, float ratio)  {
	return sample*ratio;
}
int32_t offset_sample(int32_t sample, int32_t offset, uint16_t BitsPerSample) {
	int32_t max = 1;
	int32_t min = 1;
	switch (BitsPerSample) {
		case 8:  max = 0x7f;		// 01111111
			 min = 0x80;		// 10000000
		   	 break;
		case 16: max = 0x7fff;		// 0111111111111111
			 min = 0x8000;		// 1000000000000000
			 break;
		case 24: max = 0x007fffff;
			 min = 0xff800000;
			 break;
		case 32: max = 0x7fffffff;	// 01111111111111111111111111111111
			 min = 0x80000000;	// 10000000000000000000000000000000
			 break;
		default:
			 exit_error(NOT_8_BIT_MULT);
	}
	
	if (offset > 0)
		return (sample >= (max - offset)) ? max : sample + offset;
	else
		return (sample <= (min - offset)) ? min : sample + offset;
}

void
scale_fslice(file_slice f, float ratio) {
}
