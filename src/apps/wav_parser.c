#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "../../headers/subchunks.h"
#include "../../headers/data_processing.h"
#include "../../headers/utils.h"
#include "../../headers/exit_messages.h"

int main(int argc, char *argv[]) {
	if (argc != 2) 
		return 1;

	char directory[14] = "../wav-files/";

	char filePath[48];
	int32_t dataStart;

	memset(filePath, '\0', sizeof(filePath));
	strcpy(filePath, directory);
	strcat(filePath, argv[1]);

	FILE *audioFile = fopen(filePath, "rb");

	riff_t RIFF = get_riff(audioFile);
	fmt_t FMT = get_fmt(audioFile);
	data_t dataHeader = get_datahead(audioFile, &dataStart);
	metadata_t mdhead = get_metadata(audioFile);

	print_riff_chunk(RIFF);
	print_fmt_subchunk(FMT);
	print_data_header(dataHeader);
	print_metadata(mdhead);

	printf("Peak: %d\n", __GET_ABS_PEAK(audioFile));

	fclose(audioFile);
	//__FREE_RIFF(RIFF);
	//__FREE_FMT(FMT);	
	//__FREE_DATA(dataHeader);
	__FREE_METADATA(mdhead);
	return 0;
}
