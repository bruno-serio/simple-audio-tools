#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/subchunks.h"
#include "../include/data_processing.h"
#include "../include/utils.h"
// #include <inttypes.h>

// *****

// *****

// *****

int main(int argc, char *argv[]) {
	char directory[14] = "../wav-files/";

	for (int fileN=1; fileN<argc; fileN++) {
		char filePath[48];
		signed long dataStart;

		memset(filePath, '\0', sizeof(filePath));
		strcpy(filePath, directory);
		strcat(filePath,argv[fileN]);

	//	FILE *audioFile = fopen("../wav-files/sinewave.wav", "rb");
		FILE *audioFile = fopen(filePath, "rb");

		riff_ptr RIFF = get_riff_chunk(audioFile);
		fmt_ptr FMT = get_fmt_subchunk(audioFile);
		data_header_ptr dataHeader = get_data_header(audioFile, &dataStart);

	//	print_riff_chunk(RIFF);
	//	print_fmt_subchunk(FMT);
	//	print_data_header(dataHeader);

		printf("Peak: %d\n", get_abs_peak_def(audioFile));

		fclose(audioFile);
		free_riff_chunk(RIFF);
		free_fmt_subchunk(FMT);	
		free_data_header(dataHeader);
	}

	return 0;
}

// *****


