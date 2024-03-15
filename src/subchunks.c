#include "../include/subchunks.h"
//#include "../include/exit_status.h"

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

/* Getting parameters from file */

void
get_riff_chunk(FILE *file, riff_ptr pRIFF) {
	unsigned long sizeOut = 0;

	for (int i=0; i<4; i++) pRIFF->ChunkID[i] = fgetc(file);
	
	for (int i=0; i<4; i++) {
		unsigned char iByteOfSize = fgetc(file);
		sizeOut += iByteOfSize << (i*8);
	}
	
	pRIFF->ChunkSize = sizeOut;

	for (int i=0; i<4; i++) pRIFF->Format[i] = fgetc(file);

	return;
}

void
get_fmt_subchunk(FILE *file, fmt_ptr pFMT) {
	unsigned long sizeOut = 0;

	fseek(file, 12, SEEK_SET);

	// SUBCHUNK1 ID - fmt
	for (int i=0; i<4; i++) pFMT->Subchunk1ID[i] = fgetc(file);

	// SUBCHUNK1 SIZE
	for (int i=0; i<4; i++) {
		unsigned char iByteOfSize = fgetc(file);
		sizeOut += iByteOfSize << (i*8);
	}
	pFMT->Subchunk1Size = sizeOut;

	// AUDIO FORMAT
	unsigned short tempShort = 0;
	for (int i=0; i<2; i++) {
		unsigned char iByte = fgetc(file);
		tempShort += iByte << (i*8);
	}
	pFMT->AudioFormat = tempShort;

	// NUM CHANNELS
	tempShort = 0;
	for (int i=0; i<2; i++) {
		unsigned char iByte = fgetc(file);
		tempShort += iByte << (i*8);
	}
	pFMT->NumChannels = tempShort;

	// SAMPLE RATE
	sizeOut = 0;
	for (int i=0; i<4; i++) {
		unsigned char iByteOfSize = fgetc(file);
		sizeOut += iByteOfSize << (i*8);
	}
	pFMT->SampleRate = sizeOut;

	// BYTE RATE
	sizeOut = 0;
	for (int i=0; i<4; i++) {
		unsigned char iByteOfSize = fgetc(file);
		sizeOut += iByteOfSize << (i*8);
	}
	pFMT->ByteRate = sizeOut;

	// BLOCK ALIGN
	tempShort = 0;
	for (int i=0; i<2; i++) {
		unsigned char iByte = fgetc(file);
		tempShort += iByte << (i*8);
	}
	pFMT->BlockAlign = tempShort;

	// BITS PER SAMPLE
	tempShort = 0;
	for (int i=0; i<2; i++) {
		unsigned char iByte = fgetc(file);
		tempShort += iByte << (i*8);
	}
	pFMT->BitsPerSample = tempShort;

	// CHECKS
	if (pFMT->BlockAlign != (pFMT->NumChannels * pFMT->BitsPerSample/8)) {
		printf("error 6\n");
		return;
	}

	if (pFMT->ByteRate != (pFMT->SampleRate * pFMT->NumChannels * pFMT->BitsPerSample/8)) {
		printf("error 6\n");
		return;
	}

	return;
}

/* Input and output */

void
print_riff_chunk(riff_ptr pRIFF) {
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
