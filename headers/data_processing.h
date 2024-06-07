#ifndef _DATA_PROCESSING_H_
#define _DATA_PROCESSING_H_

#include <stdio.h>
#include <stdbool.h>
#include "subchunks.h"
#include "utils.h"
#include "exit_messages.h"

#define DEF_BITSPERSAMPLE 0

#define __GET_ABS_PEAK(data) get_abs_peak((data), DEF_BITSPERSAMPLE)
#define __FREE_FSLICE(s, b) ({if (s != NULL) free_fslice(&s, b);})
	
typedef struct _file_slice* file_slice;

file_slice new_fslice(FILE* file, uint32_t length, int32_t offset, uint16_t BitsPerSample);
void free_fslice(file_slice* s, bool closeFile);

FILE* fslice_file(file_slice s);
uint32_t fslice_len(file_slice s);
int32_t fslice_offset(file_slice s);
uint16_t fslice_bits(file_slice s);

int32_t get_abs_peak(FILE *file, uint16_t BitsPerSample);
float normalize_ratio(int32_t peak, uint16_t BitsPerSample);  

int32_t scale_sample(int32_t sample, float ratio);  
int32_t offset_sample(int32_t sample, int32_t offset, uint16_t BitsPerSample);	

void write_scaled_fslice(file_slice fs, float ratio, FILE* out);	// TO BE DONE

#endif // _DATA_PROCESSING_H_
