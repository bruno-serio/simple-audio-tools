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
		long dataStart;

		memset(filePath, '\0', sizeof(filePath));
		strcpy(filePath, directory);
		strcat(filePath,argv[fileN]);

	//	FILE *audioFile = fopen("../wav-files/sinewave.wav", "rb");
		FILE *audioFile = fopen(filePath, "rb");

		riff_ptr RIFF = alloc_riff_chunk();
		get_riff_chunk(audioFile, RIFF);

		fmt_ptr FMT = alloc_fmt_subchunk();
		get_fmt_subchunk(audioFile, FMT);

		data_header_ptr dataHeader = alloc_data_header();
		dataStart = get_data_header(audioFile, dataHeader);
		printf("%ld", dataStart);

	//	print_riff_chunk(RIFF);
	//	print_fmt_subchunk(FMT);
	//	print_data_header(dataHeader);
		//get_abs_peak(audioFile);

	//	printf("\n\n");
		printf("Peak: %d\n", get_abs_peak16(audioFile));

		fclose(audioFile);
		free_riff_chunk(RIFF);
		free_fmt_subchunk(FMT);	
		free_data_header(dataHeader);
	}

	return 0;
}

// *****


