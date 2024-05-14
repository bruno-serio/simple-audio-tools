#include "../headers/file_creation.h"

FILE*
write_file(const char* filename, riff_t r, fmt_t fmt, data_t d, metadata_t m) {
	const char *path = get_filepath("../output/", filename);
	FILE *newfile = fopen(path, "wb+");
	free_filepath(path);

	write_riff(newfile, r);
	write_fmt(newfile, fmt);
	write_data(newfile, d);
	write_metadata(newfile, m);

	return newfile;
}
/*
FILE* create_new_file(const char *fileName, riff_t r, fmt_t fmt) {
	const char *filePath = get_filepath("../output/", fileName);
	FILE *newFile = fopen(filePath, "wb+");
	free_filepath(filePath);
	return newFile;
}

void
copy_audio_data(FILE *source, FILE *dest) {
	int32_t start = 0;
	fmt_t fmt = get_fmt_subchunk(source);
	data_t DATA = get_data_header(source, &start);
	uint32_t sampleCount = audio_size(DATA);
	uint16_t BitsPerSample = bits_per_sample(fmt);		

	fseek(source, start, SEEK_SET);
	fseek(dest, 0, SEEK_END);

	write_little_endian(dest, sampleCount, 32);

	for (uint32_t i=0;i<sampleCount/BitsPerSample;i++) {
		int32_t sample = read_little_endian(source, BitsPerSample);
		write_little_endian(dest, sample, BitsPerSample);
	}
}
*/
