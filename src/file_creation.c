#include "../headers/file_creation.h"

FILE*
create_new_file(const char *fileName, riff_ptr RIFF, fmt_ptr FMT, data_header_ptr DATA) {
	const char *filePath = get_filepath("../output/", fileName);
	FILE *newFile = fopen(filePath, "wb+");

	write_riff_chunk(newFile, RIFF);
	write_fmt_subchunk(newFile, FMT);
	write_data_header(newFile, DATA);

	fclose(newFile);
	//fopen(, "ab+")

	return NULL;
}

