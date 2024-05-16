#include "../headers/file_creation.h"

FILE*
write_file_header(const char* filename, riff_t r, fmt_t fmt, data_t d) {
	const char *path = get_filepath("../output/", filename);
	FILE *newfile = fopen(path, "wb+");
	__FREE_FILEPATH(path);

	write_riff(newfile, r);
	write_fmt(newfile, fmt);
	write_data(newfile, d);

	return newfile;
}

static int32_t
dummy_s32i(int32_t x) {
	return x;
}

file_slice
slice_audio_file(FILE* f, int32_t start, int32_t end, uint16_t BitsPerSample) {
	if (f == NULL) {
		printf("Can't slice NULL file pointer.\n");
		exit_error(PASSED_NULL_POINTER);
	}	

	if (start == MIN_I32S && end == MIN_I32S) {
		data_t d = get_data_header(f, &start);
		end = (int32_t)audio_size(d); // might need a check for overflow since audio_size(d) returns uint32_t
		__FREE_DATA(d);
	}

	if (BitsPerSample == 0) {
		fmt_t fmt = get_fmt_subchunk(f);
		BitsPerSample = bits_per_sample(fmt);
		__FREE_FMT(fmt);
	}

	return new_fslice(f, end-start, start, BitsPerSample);
}

void
copy_audio(file_slice src, FILE* dest, int32_t (*process)(int32_t)) {
	if (src == NULL)
		exit_error(PASSED_NULL_POINTER);

	if (process == NULL)
		process = &dummy_s32i;	

	FILE* f = fslice_file(src);
	uint16_t bits = fslice_bits(src);
	uint32_t len = fslice_len(src);

	fseek(f, fslice_offset(src), SEEK_SET);
	fseek(dest, 0, SEEK_END);

	for (uint32_t i=0; i<len; i++) {
		int32_t sample = read_little_endian(f, bits);
		write_little_endian(dest, process(sample), bits);
	}
}

void 
append_metadata(FILE* f, const char* path, metadata_t m) {
	riff_t r = get_riff_chunk(f);
	fmt_t fmt = get_fmt_subchunk(f);
	data_t d = get_data_header(f, NULL);
	uint32_t req_riffsize = calc_riff_size(fmt, d, m);

	if (req_riffsize != riff_size(r)) {
		// create new file with correct header and replace current dest file 'f'.
		// set_riff_size(r, req_riffsize);
		
		// ...
		 
		// fclose(f)
		// fclose(newfile)
		// remove(path)
		// rename(tempname, path)
		// fopen(path)
	}

	fseek(f, 0, SEEK_END);
	write_metadata(f, m);
}
