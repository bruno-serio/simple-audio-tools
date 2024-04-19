#include "../include/file_creation.h"

signed int
create_file_from_header(riff_ptr RIFF, fmt_ptr FMT, data_header_ptr DATA) {

	// open and create file
	// write headers
	// return offset
	

	// rm this
	print_riff_chunk(RIFF);
	print_fmt_subchunk(FMT);
	print_data_header(DATA);
	return 0;
}

