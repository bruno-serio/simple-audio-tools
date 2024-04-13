#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/subchunks.h"
#include "../include/utils.h"
//#include "../include/exit_status.h"
// #include <inttypes.h>

// *****

// pasar al .h cuando cree el de abspeak
// pasar en vez de 40 un numero imposible y ahi recorrer el archivo hasta encontrar
#define get_abs_peak16_def(data) get_abs_peak16((data), 44)
	
// *****

signed short
get_abs_peak16(FILE *file);

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

signed short
get_abs_peak16(FILE *file) {
	signed short max = 0;
	signed short min = 0;

	fmt_ptr FMT = alloc_fmt_subchunk();
	get_fmt_subchunk(file, FMT);

	data_header_ptr DATA = alloc_data_header();
	signed long start = 44;
	get_data_header(file, DATA);

	unsigned long sampleCount = get_data_size(DATA);

	fseek(file, start, SEEK_SET);

	for (unsigned long i=0;i<sampleCount;i++) {
		signed short sample = read_16_bit_sample(file);
		if (sample > max) max = sample;
		if (sample < min) min = sample;
	}

//	printf("MIN: %d (%04x)\nMAX: %d (%04x)\n", min, min, max, max);

	free_fmt_subchunk(FMT);
	free_data_header(DATA);
	return ((max >= (-min)) ? max : -(min+1));
}
