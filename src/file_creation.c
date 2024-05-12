#include "../headers/file_creation.h"

FILE*
create_new_file(const char *fileName, riff_ptr RIFF, fmt_ptr FMT) {
	const char *filePath = get_filepath("../output/", fileName);
	FILE *newFile = fopen(filePath, "wb+");

	write_riff_chunk(newFile, RIFF);
	write_fmt_subchunk(newFile, FMT);
	write_def_data_header(newFile);

	free_filepath(filePath);
	//fopen(, "ab+")

	return newFile;
}

void
copy_audio_data(FILE *source, FILE *dest) {
	int32_t start = 0;
	fmt_ptr FMT = get_fmt_subchunk(source);
	data_header_ptr DATA = get_data_header(source, &start);
	uint32_t sampleCount = get_data_size(DATA);
	uint16_t BitsPerSample = get_bits_per_sample(FMT);		

	fseek(source, start, SEEK_SET);
	fseek(dest, 0, SEEK_END);

	write_little_endian(dest, sampleCount, 32);

	for (uint32_t i=0;i<sampleCount/BitsPerSample;i++) {
		int32_t sample = read_little_endian(source, BitsPerSample);
		write_little_endian(dest, sample, BitsPerSample);
	}
}
