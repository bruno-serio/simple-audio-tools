#include "../include/subchunks.h"
#include "../include/utils.h"
#include "../include/exit_messages.h"

struct _riff_chunk {
	char ChunkID[4];
	unsigned long ChunkSize;
	char Format[4];
};

struct _fmt_subchunk {
	char Subchunk1ID[4];
	unsigned long Subchunk1Size;
	unsigned short AudioFormat;
	unsigned short NumChannels;
	unsigned long SampleRate;
	unsigned long ByteRate;
	unsigned short BlockAlign;
	unsigned short BitsPerSample;
};

struct _data_header {
	char Subchunk2ID[4];
	unsigned long Subchunk2Size;
};

/* Memory allocation and freeing */

riff_ptr
alloc_riff_chunk() {
	riff_ptr riff = NULL;
	riff = malloc(sizeof(struct _riff_chunk));
	return riff;
}

fmt_ptr
alloc_fmt_subchunk() {
	fmt_ptr fmt = NULL;
	fmt = malloc(sizeof(struct _fmt_subchunk));
	return fmt;
}

data_header_ptr
alloc_data_header() {
	data_header_ptr pDataH = NULL;
	pDataH = malloc(sizeof(struct _data_header));
	return pDataH;
}

void
free_riff_chunk(riff_ptr pRIFF) {
	free(pRIFF);
	pRIFF = NULL;
	return;
}

void
free_fmt_subchunk(fmt_ptr pFMT) {
	free(pFMT);
	pFMT = NULL;
	return;
}

void
free_data_header(data_header_ptr pDataH) {
	free(pDataH);
	pDataH = NULL;
	return;
}

/* Getting parameters from file */

riff_ptr
get_riff_chunk(FILE *file) {
	riff_ptr pRIFF = alloc_riff_chunk();
	for (int i=0; i<4; i++) pRIFF->ChunkID[i] = fgetc(file);
	pRIFF->ChunkSize = read_little_endian(file, 32);
	for (int i=0; i<4; i++) pRIFF->Format[i] = fgetc(file);
	return pRIFF;
}

fmt_ptr
get_fmt_subchunk(FILE *file) {
	fmt_ptr pFMT = alloc_fmt_subchunk();
	fseek(file, 12, SEEK_SET);

	for (int i=0; i<4; i++) pFMT->Subchunk1ID[i] = fgetc(file);
	pFMT->Subchunk1Size = (unsigned long)read_little_endian(file, 32);
	pFMT->AudioFormat = (unsigned short)(read_little_endian(file, 16));
	pFMT->NumChannels = (unsigned short)(read_little_endian(file, 16));
	pFMT->SampleRate = (unsigned long)read_little_endian(file, 32);
	pFMT->ByteRate = (unsigned long)read_little_endian(file, 32);
	pFMT->BlockAlign = (unsigned short)(read_little_endian(file, 16));
	pFMT->BitsPerSample = (unsigned short)(read_little_endian(file, 16));

	// CHECKS
	if (pFMT->BlockAlign != (pFMT->NumChannels * pFMT->BitsPerSample/8))
		exit_error(BLOCK_ALIGN_TEST_FAIL);

	if (pFMT->ByteRate != (pFMT->SampleRate * pFMT->NumChannels * pFMT->BitsPerSample/8))
		exit_error(BYTE_RATE_TEST_FAIL);

	return pFMT;
}

data_header_ptr
get_data_header(FILE *file, signed long *start) {
	data_header_ptr pDataH = alloc_data_header();
	signed long dataStart = 35;

	fseek(file, 36, SEEK_SET);

	do {
		dataStart++;
		pDataH->Subchunk2ID[0] = fgetc(file);
	} while (pDataH->Subchunk2ID[0] != 'd');

	for (int i=1; i<4; i++) pDataH->Subchunk2ID[i] = fgetc(file);
	dataStart += 4;

	pDataH->Subchunk2Size = read_little_endian(file, 32);

	if (start != NULL && *start != 0) *start = dataStart;
	return pDataH;
}

/* Input and output */

void
print_riff_chunk(riff_ptr pRIFF) {
	if (pRIFF == NULL) return;

	printf("---RIFF CHUNK\n");
	printf("ChunkID: ");
	for (int i=0; i<4; i++) printf("%c", pRIFF->ChunkID[i]);
	printf("\n");

	printf("ChunkSize: %ld\n", pRIFF->ChunkSize);

	printf("Format: ");
	for (int i=0; i<4; i++) printf("%c", pRIFF->Format[i]);
	printf("\n");

	return;
}

void
print_fmt_subchunk(fmt_ptr pFMT) {
	printf("---FMT SUBCHUNK\n");
	printf("Subchunk1ID: ");
	for (int i=0; i<4; i++) printf("%c", pFMT->Subchunk1ID[i]);
	printf("\n");

	printf("Subchunk1Size %ld\n", pFMT->Subchunk1Size);
	printf("AudioFormat %d\n", pFMT->AudioFormat);
	printf("NumChannels %d\n", pFMT->NumChannels);
	printf("SampleRate %ld\n", pFMT->SampleRate);
	printf("ByteRate %ld\n", pFMT->ByteRate);
	printf("BlockAlign %d\n", pFMT->BlockAlign);
	printf("BitsPerSample %d\n", pFMT->BitsPerSample);
	return;
}

void
print_data_header(data_header_ptr pDataH) {
	printf("---DATA HEADER\n");

	printf("Subchunk2ID: ");
	for (int i=0; i<4; i++) printf("%c", pDataH->Subchunk2ID[i]);
	printf("\n");

	printf("Subchunk2Size: %ld\n", pDataH->Subchunk2Size);
	return;
}

unsigned long
get_riff_chunksize(riff_ptr pRIFF) {
	return pRIFF->ChunkSize;
}

unsigned long
get_sample_rate(fmt_ptr pFMT) {
	return pFMT->SampleRate;
}

unsigned long
get_byte_rate(fmt_ptr pFMT) {
	return pFMT->SampleRate;
}

unsigned short
get_bits_per_sample(fmt_ptr pFMT) {
	return pFMT->BitsPerSample;
}

unsigned long
get_data_size(data_header_ptr pDataH) {
	return pDataH->Subchunk2Size;
}
