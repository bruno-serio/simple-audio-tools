#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>

#include "../../headers/subchunks.h"
#include "../../headers/data_processing.h"
#include "../../headers/utils.h"
#include "../../headers/exit_messages.h"

int main(int argc, char* argv[]) {
	if (argc < 2) 
		return 1;

	const char* filePath = get_filepath("../wav-files/", argv[1]);
	FILE* wavFile = fopen(filePath, "rb");
	fmt_t FMT = get_fmt(wavFile);
	uint32_t audioSize = file_size(wavFile);
	uint32_t audioOffset = audio_start(wavFile);	//audio_start() to be implemented.
	
	file_slice wavSlice = new_fslice(wavFile, audioSize, audioOffset, FMT.BitsPerSample);

	int32_t wavPeak = get_abs_peak(wavFile, FMT.BitsPerSample);
	float scalingRatio = normalize_ratio(wavPeak, FMT.BitsPerSample);

	scale_fslice(wavSlice, scalingRatio);		//scale_fslice() to be implemented

	fclose(wavFile);
	__FREE_FSLICE(wavSlice, false);
	__FREE_FILEPATH(filePath);
	return 0;
}
