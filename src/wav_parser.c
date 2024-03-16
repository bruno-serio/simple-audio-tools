#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/subchunks.h"
//#include "../include/exit_status.h"
// #include <inttypes.h>

// *****

signed long
get_abs_peak(FILE *file);

// *****

int main(int argc, char *argv[]) {
	char directory[13] = "../wav-files/";

	for (int fileN=1; fileN<argc; fileN++) {
		char filePath[48];
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
		get_data_header(audioFile, dataHeader);

		print_riff_chunk(RIFF);
		print_fmt_subchunk(FMT);
		print_data_header(dataHeader);
		//get_abs_peak(audioFile);

		fclose(audioFile);
		free_riff_chunk(RIFF);
		free_fmt_subchunk(FMT);	

	}

	return 0;
}

// *****

signed long
get_abs_peak(FILE *file) {
	signed long peak = 0;
	fmt_ptr FMT = alloc_fmt_subchunk();
	get_fmt_subchunk(file, FMT);

	fseek(file, 44, SEEK_SET);

	return peak;
}
