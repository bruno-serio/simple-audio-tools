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

	const char* sourcePath = get_filepath("../wav-files/", argv[1]);
	FILE* wavFile = fopen(sourcePath, "rb");
	fmt_t FMT = get_fmt(wavFile);
	int32_t audioOffset = 0;
	/*data_t DATA = */get_datahead(wavFile, &audioOffset);
	uint32_t audioSize = file_size(wavFile);
	//uint32_t audioOffset = audio_start(wavFile);	//audio_start() to be implemented.
	file_slice wavSlice = new_fslice(wavFile, audioSize, audioOffset, FMT.BitsPerSample);
	
	int32_t wavPeak = get_abs_peak(wavFile, FMT.BitsPerSample);
	float scalingRatio = normalize_ratio(wavPeak, FMT.BitsPerSample);

	const char* destPath = get_filepath("../output/", argv[1]);
	FILE* outFile = fopen(destPath, "rb");

	// copy RIFF, FMT, DATA HEADER

	write_scaled_fslice(wavSlice, scalingRatio, outFile);		//scale_fslice() to be implemented

	// copy METADATA

	__FREE_FSLICE(wavSlice, false);
	fclose(wavFile);
	fclose(outFile);
	__FREE_FILEPATH(sourcePath);
	return 0;
}
