#include "../headers/file_creation.h"

FILE*
write_file_header(const char* filename, riff_t r, fmt_t fmt, data_t d) {
	const char *path = get_filepath("../output/", filename);
	FILE *newfile = fopen(path, "wb+");
	free_filepath(path);

	write_riff(newfile, r);
	write_fmt(newfile, fmt);
	write_data(newfile, d);

	return newfile;
}

void
copy_audio(FILE* src, FILE* dest, int32_t (*process)(int32_t)) {
}

void 
append_metadata(FILE* f, metadata_t m) {
}
