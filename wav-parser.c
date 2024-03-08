#include <stdio.h>
#include <stdlib.h>
// #include <inttypes.h>

enum _exit_status {
	E_SUCCESS = 0,
	E_FAIL = 1,
	E_COULDNT_ALLOCATE_MEMORY = 2,
	E_COULDNT_OPEN_FILE = 3,
	E_UNEXPECTED_NULL_POINTER = 4,
	E_INVALID_AUDIO_FORMAT = 5,
	E_UNEXPECTED_VALUE = 6
};

typedef enum _exit_status exit_t;

// *****

typedef struct _riff_chunk {
	char ChunkID[4];
	unsigned long ChunkSize;
	char Format[4];
} riff_chunk_t;

typedef struct _fmt_subchunk {
	char Subchunk1ID[4];
	unsigned long Subchunk1Size;
	unsigned short AudioFormat;
	unsigned short NumChannels;
	unsigned long SampleRate;
	unsigned long ByteRate;
	unsigned short BlockAlign;
	unsigned short BitsPerSample;
} fmt_subchunk_t;

/*
typedef struct _data16bit {
	signed short sample[2];
	struct _data16bit *next;
} data16bit_t;

typedef struct _data_header {
	char Subchunk2ID[4];
	unsigned long Subchunk2Size;
	data16bit_t *data;
} data_header_t;
 */
	
// *****

exit_t
get_riff_chunk(FILE *file, riff_chunk_t *dataChunk);

exit_t
get_fmt_subchunk(FILE *file, fmt_subchunk_t *fmtChunk);

/*
exit_t
get_data_subchunk(FILE *file, data_subchunk_t *dataChunk);
*/

signed long
get_abs_peak(FILE *file, fmt_subchunk_t fmtSubchunk);

exit_t
validate_audio_format(FILE *file);

void
exit_code_handler(exit_t exitCode);
	
exit_t
print_riff_chunk(riff_chunk_t *riffChunk);

exit_t
print_fmt_chunk(fmt_subchunk_t *fmtChunk);

// *****

int main(int argc, char *argv[]) {
	FILE *audioFile = fopen("testWavFile.wav", "rb");
	riff_chunk_t audioFileRIFF;
	fmt_subchunk_t audioFileFMT;

	get_riff_chunk(audioFile, &audioFileRIFF);
	get_fmt_subchunk(audioFile, &audioFileFMT);

	print_riff_chunk(&audioFileRIFF);
	print_fmt_chunk(&audioFileFMT);

	fclose(audioFile);
	return 0;
}

// *****

exit_t
get_riff_chunk(FILE *file, riff_chunk_t *dataChunk) {
	unsigned long sizeOut = 0;
	
	for (int i=0; i<4; i++) dataChunk->ChunkID[i] = fgetc(file);
	
	for (int i=0; i<4; i++) {
		unsigned char iByteOfSize = fgetc(file);
		sizeOut += iByteOfSize << (i*8);
	}
	
	dataChunk->ChunkSize = sizeOut;

	for (int i=0; i<4; i++) dataChunk->Format[i] = fgetc(file);

	return 0;
}

exit_t
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
		return 6;
	}

	if (fmtChunk->ByteRate != (fmtChunk->SampleRate * fmtChunk->NumChannels * fmtChunk->BitsPerSample/8)) {
		return 6;
	}

	return 0;
}

exit_t
print_riff_chunk(riff_chunk_t *riffChunk) {
	printf("---RIFF CHUNK\n");
	printf("ChunkID: ");
	for (int i=0; i<4; i++) printf("%c", riffChunk->ChunkID[i]);
	printf("\n");

	printf("ChunkSize: %ld\n", riffChunk->ChunkSize);

	printf("Format: ");
	for (int i=0; i<4; i++) printf("%c", riffChunk->Format[i]);
	printf("\n");

	return 0;
}

exit_t
print_fmt_chunk(fmt_subchunk_t *fmtChunk) {
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
	return 0;
}

exit_t
validate_audio_format(FILE *file) {
	unsigned char riffcomparison[4]= {'R', 'I', 'F', 'F'};
	unsigned char wavecomparison[4]= {'W', 'A', 'V', 'E'};

	fseek(file, 0, SEEK_SET);
	for  (int i=0; i<4; i++) if (riffcomparison[i] != fgetc(file)) return E_INVALID_AUDIO_FORMAT;

	fseek(file, 8, SEEK_SET);
	for  (int i=0; i<4; i++) if (wavecomparison[i] != fgetc(file)) return E_INVALID_AUDIO_FORMAT;

/**
 *	fseek(file, 16, SEEK_SET);
 *	if (0x10 != fgetc(file)) return E_INVALID_AUDIO_FORMAT; //0x10 es el formato que tiene el test. 0x01 es pcm (sin compresion)
 */
	return E_SUCCESS;
}

void
exit_code_handler(exit_t exitCode) {
	if (exitCode == 0) return;

	switch (exitCode) {
		case 1: 
			printf("An error has ocurred. Exit code 1.\n");
			exit(1);
			break;
		case 2:
			printf("Couldn't allocate memory. Exit code 2.\n");
			exit(2);
			break;
		case 3:
			printf("Couldn't open file. Exit code 3.\n");
			exit(3);
			break;
		case 4:
			printf("Received an unexpected NULL pointer. Exit code 4.\n");
			exit(4);
			break;
	}
	return;
}
