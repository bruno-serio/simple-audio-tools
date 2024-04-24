#include "../headers/subchunks.h"
#include "../headers/utils.h"
#include "../headers/exit_messages.h"

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

struct _metadata_node {
	char code[4];
	unsigned long infoSize;
	char *info;
	struct _metadata_node *R;
	struct _metadata_node *L;
};

struct _metadata_head {
	unsigned short tagsAmount;
	struct _metadata_node *listFirst;
	struct _metadata_node *listLast;
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

metadata_list
create_metadata_node(char code[4], unsigned long infoSize) {
	metadata_list node = malloc(sizeof(struct _metadata_node));
	for (int i=0; i<4; i++)
		node->code[i] = code[i];
	node->infoSize = infoSize;
	node->info = NULL;
	node->L = NULL;
	node->R = NULL;
	return node;
}

metadata_list
append_metadata_list(metadata_list l, metadata_list node) {
	if (l == NULL) 
		return node;

	else if (l->R == NULL) {
		node->L = l;
		l->R = node;
		return node;
	} else
		return append_metadata_list(l->R, node);
}

metadata_head
create_metadata_head_from_list(metadata_list l) {
	metadata_head h = malloc(sizeof(struct _metadata_head));
	h->listFirst = l;
	while (l != NULL && l->R != NULL)
		l = l->R;
	h->listLast = l;
	return h;
}

metadata_head
get_metadata(FILE *file) {
	fseek(file, -8, SEEK_END);
	signed char c = 0;
	unsigned long id = 0;

	for (int maxloop = 0; maxloop < 512 && feof(file) == 0 && id != _INFO_LE; maxloop++) {
		for (c = fgetc(file); feof(file) == 0 && c != 'O'; fseek(file, -2, SEEK_CUR))
			c = fgetc(file);

		if (c == 'O') {
			fseek(file, -4, SEEK_CUR);
			id = read_little_endian(file, 32);
		}
	}

	metadata_list l = NULL;

	if (id == _INFO_LE) {
		while (feof(file) == 0) {
			char tagid[4];
			for (int i=0;i<4;i++)
				tagid[i] = fgetc(file);
			unsigned long infosize = read_little_endian(file, 32);
			metadata_list temp = create_metadata_node(tagid, infosize);
			l = append_metadata_list(l, temp);
		}
		// get all separate tags and create the list
	}

	return create_metadata_head_from_list(l);
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

void
free_metadata_list(metadata_list *l) {
	if (l != NULL) {
		while (*l != NULL && (*l)->sig != NULL)


	}
}

void
free_metadata(metadata_head *head) {
	if (head != NULL) {
		free_metadata_list(&((*head)->listFirst));
		free(*head);
	}
}

/* Input and output */

void
print_riff_chunk(riff_ptr pRIFF) {
	if (pRIFF == NULL) 
		return;

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
	if (pFMT == NULL)
		return;

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
	if (pDataH == NULL)
		return;

	printf("---DATA HEADER\n");

	printf("Subchunk2ID: ");
	for (int i=0; i<4; i++) printf("%c", pDataH->Subchunk2ID[i]);
	printf("\n");

	printf("Subchunk2Size: %ld\n", pDataH->Subchunk2Size);
	return;
}

void
print_metadata_list_ids(metadata_list l) {
	while (l != NULL) {
		printf("%s", l->code);
		l = l->R;
	}
}

unsigned long
get_riff_chunksize(riff_ptr pRIFF) {
	return (pRIFF != NULL) ? pRIFF->ChunkSize : 0;
}

unsigned long
get_sample_rate(fmt_ptr pFMT) {
	return (pFMT != NULL) ? pFMT->SampleRate : 0;
}

unsigned long
get_byte_rate(fmt_ptr pFMT) {
	return (pFMT != NULL) ? pFMT->SampleRate : 0;
}

unsigned short
get_bits_per_sample(fmt_ptr pFMT) {
	return pFMT->BitsPerSample;
}

unsigned long
get_data_size(data_header_ptr pDataH) {
	return pDataH->Subchunk2Size;
}

void
write_riff_chunk(FILE *file, riff_ptr RIFF) {
	fseek(file, 0, SEEK_SET);

	char *tmpstr = "RIFF";
	for (int i=0; i<4; i++)
		fputc(tmpstr[i], file);

	write_little_endian(file, (signed long)(RIFF!=NULL) ? RIFF->ChunkSize : 0, 32);	

	tmpstr = "WAVE";
	for (int i=0; i<4; i++)
		fputc(tmpstr[i], file);
}

void
write_fmt_subchunk(FILE *file, fmt_ptr FMT) {
	fseek(file, 12, SEEK_SET);

	char *tmpstr = "fmt\040";
	for (int i=0; i<4; i++)
		fputc(tmpstr[i], file);

	write_little_endian(file, 16, 32);

	if (FMT != NULL) {
		write_little_endian(file, (unsigned long)FMT->AudioFormat, 16);
		write_little_endian(file, (unsigned long)FMT->NumChannels, 16);
		write_little_endian(file, (unsigned long)FMT->SampleRate, 32);
		write_little_endian(file, (unsigned long)FMT->ByteRate, 32);
		write_little_endian(file, (unsigned long)FMT->BlockAlign, 16);
		write_little_endian(file, (unsigned long)FMT->BitsPerSample, 16);
	} else
		write_little_endian(file, 0, 128);
}

void
write_data_header(FILE *file, data_header_ptr DATA) {
	fseek(file, 36, SEEK_SET);

	char *tmpstr = "data";
	for (int i=0; i<4; i++)
		fputc(tmpstr[i], file);

	write_little_endian(file, (unsigned long)(DATA!=NULL) ? DATA->Subchunk2Size : 0, 32);
}
