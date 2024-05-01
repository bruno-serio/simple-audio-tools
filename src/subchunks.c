#include "../headers/subchunks.h"
#include "../headers/utils.h"
#include "../headers/exit_messages.h"

struct _riff_chunk {
	char ChunkID[4];
	uint32_t ChunkSize;
	char Format[4];
};

struct _fmt_subchunk {
	char Subchunk1ID[4];
	uint32_t Subchunk1Size;
	uint16_t AudioFormat;
	uint16_t NumChannels;
	uint32_t SampleRate;
	uint32_t ByteRate;
	uint16_t BlockAlign;
	uint16_t BitsPerSample;
};

struct _data_header {
	char Subchunk2ID[4];
	uint32_t Subchunk2Size;
};

struct _metadata_node {
	char code[4];
	uint32_t infoSize;
	char *info;
	struct _metadata_node *R;
	struct _metadata_node *L;
};

struct _metadata_head {
	uint16_t tagsAmount;
	uint32_t metadataSize;
	struct _metadata_node *listFirst;
	struct _metadata_node *listLast;
};

/* Memory allocation and freeing */

static riff_ptr
alloc_riff_chunk() {
	riff_ptr riff = NULL;
	riff = malloc(sizeof(struct _riff_chunk));
	return riff;
}

static fmt_ptr
alloc_fmt_subchunk() {
	fmt_ptr fmt = NULL;
	fmt = malloc(sizeof(struct _fmt_subchunk));
	return fmt;
}

static data_header_ptr
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
	pFMT->Subchunk1Size = (uint32_t)read_little_endian(file, 32);
	pFMT->AudioFormat = (uint16_t)(read_little_endian(file, 16));
	pFMT->NumChannels = (uint16_t)(read_little_endian(file, 16));
	pFMT->SampleRate = (uint32_t)read_little_endian(file, 32);
	pFMT->ByteRate = (uint32_t)read_little_endian(file, 32);
	pFMT->BlockAlign = (uint16_t)(read_little_endian(file, 16));
	pFMT->BitsPerSample = (uint16_t)(read_little_endian(file, 16));

	// CHECKS
	if (pFMT->BlockAlign != (pFMT->NumChannels * pFMT->BitsPerSample/8))
		exit_error(BLOCK_ALIGN_TEST_FAIL);

	if (pFMT->ByteRate != (pFMT->SampleRate * pFMT->NumChannels * pFMT->BitsPerSample/8))
		exit_error(BYTE_RATE_TEST_FAIL);

	return pFMT;
}

data_header_ptr
get_data_header(FILE *file, int32_t *start) {
	data_header_ptr pDataH = alloc_data_header();
	int32_t dataStart = 35;

	fseek(file, 36, SEEK_SET);

	do {
		dataStart++;
		pDataH->Subchunk2ID[0] = fgetc(file);
	} while (pDataH->Subchunk2ID[0] != 'd');

	for (int i=1; i<4; i++) pDataH->Subchunk2ID[i] = fgetc(file);
	dataStart += 4;

	pDataH->Subchunk2Size = read_little_endian(file, 32);

	if (start != NULL) *start = dataStart;
	return pDataH;
}

metadata_list
create_metadata_node(char *code, uint32_t infoSize, char *info) {
	metadata_list node = malloc(sizeof(struct _metadata_node));
	for (int i=0; i<4; i++)
		node->code[i] = code[i];
	node->infoSize = infoSize;
	node->info = info;
	node->L = NULL;
	node->R = NULL;
	return node;
}

metadata_list
append_metadata_list(metadata_list *l, metadata_list node) {
	if (l == NULL || *l == NULL) 
		return node;

	else if ((*l)->R == NULL) {
		node->L = *l;
		(*l)->R = node;
		return node;
	} else {
		metadata_list dummy = *l;
		while (dummy->R != NULL)
			dummy = dummy->R;
		return append_metadata_list(&dummy, node);
	}
}

metadata_head
create_metadata_head_from_list(metadata_list l) {
	metadata_head h = malloc(sizeof(struct _metadata_head));

	h->metadataSize = 0;
	h->listFirst = l;

	while (l != NULL && l->R != NULL) {
		h->metadataSize += 16 + l->infoSize;
		l = l->R;
	}
	h->metadataSize += 16 + l->infoSize;
	h->listLast = l;
	return h;
}

metadata_head
get_metadata(FILE *file) {
	fseek(file, -8, SEEK_END);
	signed char c = 0;
	uint32_t id = 0;
	int32_t length = 0;

	for (int maxloop = 0; maxloop < 12 && feof(file) == 0 && id != _INFO_LE; maxloop++) {
		for (c = fgetc(file); feof(file) == 0 && c != 'O'; fseek(file, -2, SEEK_CUR)) {
			c = fgetc(file);
			length++;
		}
		if (c == 'O') {
			fseek(file, -2, SEEK_CUR);
			id = read_little_endian(file, 32);
		}
	}

	metadata_list l = NULL;
	if (id == _INFO_LE) {
		while (feof(file) == 0 && length > 0) {
			char tagid[4];
			for (int i=0;i<4;i++) {
				tagid[i] = fgetc(file);
			}
			uint32_t infosize = read_little_endian(file, 32);
			length -= (8 + infosize);

			char *tagInfo = malloc((infosize+1)*sizeof(char));
			memset(tagInfo, '\0', infosize+1);
			for (uint32_t i=0; i<infosize; i++)
				tagInfo[i] = fgetc(file);

			metadata_list temp = create_metadata_node(tagid, infosize, tagInfo);
			l = append_metadata_list(&l, temp);
			fseek(file, infosize, SEEK_CUR);
		}
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
		while (*l != NULL) {
			metadata_list rm = *l;
			*l = (*l)->R;
			free(rm->info);
			free(rm);
		}
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

	printf("ChunkSize: %" PRIu32 "\n", pRIFF->ChunkSize);

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

	printf("Subchunk1Size %" PRIu32 "\n", pFMT->Subchunk1Size);
	printf("AudioFormat %" PRIu16 "\n", pFMT->AudioFormat);
	printf("NumChannels %" PRIu16 "\n", pFMT->NumChannels);
	printf("SampleRate %" PRIu32 "\n", pFMT->SampleRate);
	printf("ByteRate %" PRIu32 "\n", pFMT->ByteRate);
	printf("BlockAlign %" PRIu16 "\n", pFMT->BlockAlign);
	printf("BitsPerSample %" PRIu16 "\n", pFMT->BitsPerSample);
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

	printf("Subchunk2Size: %" PRIu32 "\n", pDataH->Subchunk2Size);
	return;
}

void
print_metadata(metadata_head h) {
	if (h != NULL) {
		printf("Metadata size: %" PRIu32 "\n", h->metadataSize);
		metadata_list l = h->listFirst;
		while (l != NULL) {
			for (int i=0; i<4; i++)
				printf("%c",l->code[i]);
			printf(": %s\n", l->info);
			l = l->R;
		}
	}
}

uint32_t
get_riff_chunksize(riff_ptr pRIFF) {
	return (pRIFF != NULL) ? pRIFF->ChunkSize : 0;
}

uint32_t
get_sample_rate(fmt_ptr pFMT) {
	return (pFMT != NULL) ? pFMT->SampleRate : 0;
}

uint32_t
get_byte_rate(fmt_ptr pFMT) {
	return (pFMT != NULL) ? pFMT->SampleRate : 0;
}

uint16_t
get_bits_per_sample(fmt_ptr pFMT) {
	return pFMT->BitsPerSample;
}

uint32_t
get_data_size(data_header_ptr pDataH) {
	return pDataH->Subchunk2Size;
}

void
write_riff_chunk(FILE *file, riff_ptr RIFF) {
	fseek(file, 0, SEEK_SET);

	char *tmpstr = "RIFF";
	for (int i=0; i<4; i++)
		fputc(tmpstr[i], file);

	write_little_endian(file, (int32_t)(RIFF!=NULL) ? RIFF->ChunkSize : 0, 32);	

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
		write_little_endian(file, (uint32_t)FMT->AudioFormat, 16);
		write_little_endian(file, (uint32_t)FMT->NumChannels, 16);
		write_little_endian(file, (uint32_t)FMT->SampleRate, 32);
		write_little_endian(file, (uint32_t)FMT->ByteRate, 32);
		write_little_endian(file, (uint32_t)FMT->BlockAlign, 16);
		write_little_endian(file, (uint32_t)FMT->BitsPerSample, 16);
	} else
		write_little_endian(file, 0, 128);
}

void
write_data_header(FILE *file, data_header_ptr DATA) {
	fseek(file, 36, SEEK_SET);

	char *tmpstr = "data";
	for (int i=0; i<4; i++)
		fputc(tmpstr[i], file);

	write_little_endian(file, (uint32_t)(DATA!=NULL) ? DATA->Subchunk2Size : 0, 32);
}
