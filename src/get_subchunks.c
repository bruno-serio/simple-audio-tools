#include "../include/get_subchunks.h"
//#include "../include/exit_status.h"

void
get_riff_chunk(FILE *file, riff_chunk_t *dataChunk) {
	unsigned long sizeOut = 0;
	printf("llegue aca carajo\n");	
	for (int i=0; i<4; i++) dataChunk->ChunkID[i] = fgetc(file);
	
	for (int i=0; i<4; i++) {
		unsigned char iByteOfSize = fgetc(file);
		sizeOut += iByteOfSize << (i*8);
	}
	
	dataChunk->ChunkSize = sizeOut;

	for (int i=0; i<4; i++) dataChunk->Format[i] = fgetc(file);

	return;
}

void
get_fmt_subchunk(FILE *file, fmt_subchunk_t *fmtChunk) {
	unsigned long sizeOut = 0;

	fseek(file, 12, SEEK_SET);

	// SUBCHUNK1 ID - fmt
	for (int i=0; i<4; i++) fmtChunk->Subchunk1ID[i] = fgetc(file);

	// SUBCHUNK1 SIZE
	for (int i=0; i<4; i++) {
		unsigned char iByteOfSize = fgetc(file);
		sizeOut += iByteOfSize << (i*8);
	}
	fmtChunk->Subchunk1Size = sizeOut;

	// AUDIO FORMAT
	unsigned short tempShort = 0;
	for (int i=0; i<2; i++) {
		unsigned char iByte = fgetc(file);
		tempShort += iByte << (i*8);
	}
	fmtChunk->AudioFormat = tempShort;

	// NUM CHANNELS
	tempShort = 0;
	for (int i=0; i<2; i++) {
		unsigned char iByte = fgetc(file);
		tempShort += iByte << (i*8);
	}
	fmtChunk->NumChannels = tempShort;

	// SAMPLE RATE
	sizeOut = 0;
	for (int i=0; i<4; i++) {
		unsigned char iByteOfSize = fgetc(file);
		sizeOut += iByteOfSize << (i*8);
	}
	fmtChunk->SampleRate = sizeOut;

	// BYTE RATE
	sizeOut = 0;
	for (int i=0; i<4; i++) {
		unsigned char iByteOfSize = fgetc(file);
		sizeOut += iByteOfSize << (i*8);
	}
	fmtChunk->ByteRate = sizeOut;

	// BLOCK ALIGN
	tempShort = 0;
	for (int i=0; i<2; i++) {
		unsigned char iByte = fgetc(file);
		tempShort += iByte << (i*8);
	}
	fmtChunk->BlockAlign = tempShort;

	// BITS PER SAMPLE
	tempShort = 0;
	for (int i=0; i<2; i++) {
		unsigned char iByte = fgetc(file);
		tempShort += iByte << (i*8);
	}
	fmtChunk->BitsPerSample = tempShort;

	// CHECKS
	if (fmtChunk->BlockAlign != (fmtChunk->NumChannels * fmtChunk->BitsPerSample/8)) {
		printf("error 6\n");
		return;
	}

	if (fmtChunk->ByteRate != (fmtChunk->SampleRate * fmtChunk->NumChannels * fmtChunk->BitsPerSample/8)) {
		printf("error 6\n");
		return;
	}

	return;
}
