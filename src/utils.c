#include "../headers/utils.h"
#include "../headers/exit_messages.h"

int32_t
read_little_endian(FILE *file, uint8_t bits) {
	if (bits % 8 != 0)
		exit_error(NOT_8_BIT_MULT);
	int32_t sample = 0;
	for (int i=0; i<bits/8; i++)
		sample += ((uint8_t)fgetc(file) << (8*i));

	return sample;
}

int32_t
read_big_endian(FILE *file, uint8_t bits) {
	if (bits % 8 != 0)
		exit_error(NOT_8_BIT_MULT);
	int8_t n[bits/8];
	for (int i=0; i<bits/8; i++)
		n[i] = fgetc(file);
	int32_t sample = 0; 
	for (int i=0; i<bits/8; i++) 
		sample += (n[i] << 8*(bits/8 - i - 1));

	return sample;
}
void
write_little_endian(FILE *fileOut, int32_t n, uint8_t bits) {
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
