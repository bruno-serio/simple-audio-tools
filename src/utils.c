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

signed long 
read_big_endian(FILE *file, unsigned char bits) {
	if (bits % 8 != 0)
		exit_error(NOT_8_BIT_MULT);
	char n[bits/8];
	for (int i=0; i<bits/8; i++)
		n[i] = fgetc(file);
	signed long sample = 0; 
	for (int i=0; i<bits/8; i++) 
		sample += (n[i] << 8*(bits/8 - i - 1));

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
	if (dir == NULL || fileName == NULL)
		exit_error(PASSED_NULL_POINTER);

	size_t size = strlen(dir) + strlen(fileName) + 1;

	char *filePath = malloc(size);

	if (filePath == NULL)
		exit_error(MEM_ALLOC_FAILED);
	
	memset(filePath, '\0', size);
	strcpy(filePath, dir);
	strcat(filePath, fileName);

	return (const char*)filePath;
}

void
free_filepath(const char *fp) {
	free((char*)fp);
}
