#include <stdio.h>
#include <stdlib.h>

#include "../include/get_subchunks.h"
//#include "../include/exit_status.h"
// #include <inttypes.h>

// *****

signed long
get_abs_peak(FILE *file);

// *****

int main(int argc, char *argv[]) {
	FILE *audioFile = fopen("../testWavFile.wav", "rb");
	//get_abs_peak(audioFile);


	fclose(audioFile);
	return 0;
}

// *****

signed long
get_abs_peak(FILE *file) {
	signed long peak = 0;
	fmt_subchunk_t FMT;
	get_fmt_subchunk(file, &FMT);

	fseek(file, 44, SEEK_SET);

	return peak;
}
