#include "../headers/utils.h"
#include "../headers/exit_messages.h"

signed long 
read_little_endian(FILE *file, unsigned char bits) {
	if (bits % 8 != 0)
		exit_error(NOT_8_BIT_MULT);
	signed long sample = 0;
	for (int i=0; i<bits/8; i++)
		sample += ((unsigned char)fgetc(file) << (8*i));

	return sample;
}

void
write_little_endian(FILE *fileOut, signed long n, unsigned char bits) {
	if (fileOut == NULL)
		exit_error(PASSED_NULL_POINTER);
	if (bits % 8 != 0)
		exit_error(NOT_8_BIT_MULT);

	for (int i=0; i<bits/8; i++)
		fputc((int)((n >> (8*i)) & 0xff), fileOut);
}

const char*
get_filepath(const char *dir, const char *fileName) {
	char *filePath = malloc(strlen(dir) + strlen(fileName) + 1);

	if (filePath == NULL)
		exit_error(MEM_ALLOC_FAILED);

	memset(filePath, '\0', strlen(filePath));
	strcpy(filePath, dir);
	strcat(filePath, fileName);

	return (const char*)filePath;
}
