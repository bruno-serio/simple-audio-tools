#include <stdio.h>
#include <stdlib.h>

#include "../include/get_subchunks.h"
//#include "../include/exit_status.h"
// #include <inttypes.h>

// *****

signed long
get_abs_peak(FILE *file);

void
validate_audio_format(FILE *file);

void
print_riff_chunk(riff_chunk_t *riffChunk);

void
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

	//get_abs_peak(audioFile);


	fclose(audioFile);
	return 0;
}

// *****

signed long
get_abs_peak(FILE *file) {
	signed long peak = 0;
	fmt_subchunk_t FMT;
	get_fmt_subchunk(file, &FMT);

	fseek(file, 44, SEEK_SET);

	return peak;
}

void
print_riff_chunk(riff_chunk_t *riffChunk) {
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
	return;
}
/*
void
validate_audio_format(FILE *file) {
	unsigned char riffcomparison[4]= {'R', 'I', 'F', 'F'};
	unsigned char wavecomparison[4]= {'W', 'A', 'V', 'E'};

	fseek(file, 0, SEEK_SET);
	for  (int i=0; i<4; i++) if (riffcomparison[i] != fgetc(file)) return E_INVALID_AUDIO_FORMAT;

	fseek(file, 8, SEEK_SET);
	for  (int i=0; i<4; i++) if (wavecomparison[i] != fgetc(file)) return E_INVALID_AUDIO_FORMAT;

//	fseek(file, 16, SEEK_SET);
//	if (0x10 != fgetc(file)) return E_INVALID_AUDIO_FORMAT; //0x10 es el formato que tiene el test. 0x01 es pcm (sin compresion)

	return E_SUCCESS;
}*/
