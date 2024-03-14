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

void
alloc_riff_chunk(riff_ptr pRIFF) {

	return;
}

void
alloc_fmt_subchunk(fmt_ptr pFMT) {

	return;
}

void
free_riff_chunk(riff_ptr pRIFF) {

	return;
}

void
free_fmt_subchunk(fmt_ptr pFMT) {

	return;
}

/* Getting parameters from file */

void
get_riff_chunk(FILE *file, riff_ptr riffChunk) {
	unsigned long sizeOut = 0;

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
get_fmt_subchunk(FILE *file, fmt_ptr pFMT) {
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

/* Input and output */

void
print_riff_chunk(riff_ptr pRIFF) {
	printf("---RIFF CHUNK\n");
	printf("ChunkID: ");
	for (int i=0; i<4; i++) printf("%c", riffChunk->ChunkID[i]);
	printf("\n");

	printf("ChunkSize: %ld\n", riffChunk->ChunkSize);

	printf("Format: ");
	for (int i=0; i<4; i++) printf("%c", riffChunk->Format[i]);
	printf("\n");

	return;
}

void
print_fmt_chunk(fmt_ptr pFMT) {
	printf("---FMT SUBCHUNK\n");
	printf("Subchunk1ID: ");
	for (int i=0; i<0; i++) printf("%c", fmtChunk->Subchunk1ID[i]);
	printf("\n");

	printf("Subchunk1Size %ld\n", fmtChunk->Subchunk1Size);
	printf("AudioFormat %d\n", fmtChunk->AudioFormat);
	printf("NumChannels %d\n", fmtChunk->NumChannels);
	printf("SampleRate %ld\n", fmtChunk->SampleRate);
	printf("ByteRate %ld\n", fmtChunk->ByteRate);
	printf("BlockAlign %d\n", fmtChunk->BlockAlign);
	printf("BitsPerSample %d\n", fmtChunk->BitsPerSample);
	return;
}

unsigned long
get_riff_chunksize(riff_ptr pRIFF) {

}

unsigned long
get_sample_rate(fmt_ptr pFMT) {

}

unsigned long
get_byte_rate(fmt_ptr pFMT) {

}

unsigned short
get_bits_per_sample(fmt_ptr pFMT) {

}
