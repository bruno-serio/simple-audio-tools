#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/subchunks.h"
#include "../headers/data_processing.h"
#include "../headers/utils.h"
#include "../headers/exit_messages.h"
#include "../headers/file_creation.h"
// #include <inttypes.h>

// *****

// *****

// *****

int main(int argc, char *argv[]) {
	char directory[14] = "../wav-files/";

	for (int fileN=1; fileN<2/*argc*/; fileN++) {
		char filePath[48];
		signed long dataStart;

		memset(filePath, '\0', sizeof(filePath));
		strcpy(filePath, directory);
		strcat(filePath,argv[fileN]);

		FILE *audioFile = fopen(filePath, "rb");

		if (argc>1) {
			create_new_file(argv[fileN+1], NULL, NULL, NULL);	
			/*char fileOut[48];
			memset(fileOut, '\0', sizeof(fileOut));
			strcpy(fileOut, "./");
			strcat(fileOut,argv[fileN+1]);
			printf("out: %s\n", fileOut);
			FILE *audioOut = fopen(fileOut, "wb");
			write_little_endian(audioOut, 0x5af6130a, 32);*/
		}

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


