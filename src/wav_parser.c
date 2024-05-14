#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "../headers/subchunks.h"
#include "../headers/data_processing.h"
#include "../headers/utils.h"
#include "../headers/exit_messages.h"
#include "../headers/file_creation.h"

// *****

// *****

// *****

int main(int argc, char *argv[]) {
	char directory[14] = "../wav-files/";

	for (int fileN=1; fileN<2; fileN++) { // argc in place of 2
		char filePath[48];
		int32_t dataStart;

		memset(filePath, '\0', sizeof(filePath));
		strcpy(filePath, directory);
		strcat(filePath,argv[fileN]);

		FILE *audioFile = fopen(filePath, "rb");
		FILE *newFile = NULL;
	
		riff_t RIFF = get_riff_chunk(audioFile);
		print_riff_chunk(RIFF);
		fmt_t FMT = get_fmt_subchunk(audioFile);
		print_fmt_subchunk(FMT);
		data_t dataHeader = get_data_header(audioFile, &dataStart);
		print_data_header(dataHeader);
		metadata_t mdhead = get_metadata(audioFile);


		if (argc>1) {
			// newFile = create_new_file(argv[fileN+1], RIFF, FMT);
			// copy_audio_data(audioFile, newFile);
		}

		print_metadata(mdhead);

		printf("Peak: %d\n", get_abs_peak_def(audioFile));

		fclose(audioFile);
		fclose(newFile);
		free_riff_chunk(RIFF);
		free_fmt_subchunk(FMT);	
		free_data_header(dataHeader);
		if (mdhead != NULL)
			free_metadata(&mdhead);
	}
	return 0;
}

// *****


