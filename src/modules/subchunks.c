#include "../../headers/subchunks.h"

typedef struct _metadata_node *metadata_list;

struct _metadata_node {
	char code[4];
	uint32_t infoSize;
	char *info;
	struct _metadata_node *L;
	struct _metadata_node *R;
};

struct _metadata_head {
	uint16_t tagsAmount;
	uint32_t metadataSize;
	struct _metadata_node *first;
	struct _metadata_node *last;
};

/* Memory allocation and freeing */
/*
static riff_t
alloc_riff_chunk() {
	riff_t riff = NULL;
	riff = malloc(sizeof(struct _riff_chunk));
	return riff;
}

static fmt_t
alloc_fmt_subchunk() {
	fmt_t fmt = NULL;
	fmt = malloc(sizeof(struct _fmt_subchunk));
	return fmt;
}

static data_t
alloc_data_header() {
	data_t d = NULL;
	d = malloc(sizeof(struct _data_header));
	return d;
}
*/

riff_t
get_riff(FILE *file) {
	riff_t r;
	for (int i=0; i<4; i++) r.ChunkID[i] = fgetc(file);
	r.ChunkSize = read_little_endian(file, 32);
	for (int i=0; i<4; i++) r.Format[i] = fgetc(file);
	return r;
}

fmt_t
get_fmt(FILE *file) {
	fmt_t fmt;
	fseek(file, 12, SEEK_SET);

	for (int i=0; i<4; i++) fmt.Subchunk1ID[i] = fgetc(file);

	fmt.Subchunk1Size = (uint32_t)read_little_endian(file, 32);
	fmt.AudioFormat = (uint16_t)(read_little_endian(file, 16));
	fmt.NumChannels = (uint16_t)(read_little_endian(file, 16));
	fmt.SampleRate = (uint32_t)read_little_endian(file, 32);
	fmt.ByteRate = (uint32_t)read_little_endian(file, 32);
	fmt.BlockAlign = (uint16_t)(read_little_endian(file, 16));
	fmt.BitsPerSample = (uint16_t)(read_little_endian(file, 16));

	// CHECKS
	if (fmt.BlockAlign != (fmt.NumChannels * fmt.BitsPerSample/8))
		exit_error(BLOCK_ALIGN_TEST_FAIL);

	if (fmt.ByteRate != (fmt.SampleRate * fmt.NumChannels * fmt.BitsPerSample/8))
		exit_error(BYTE_RATE_TEST_FAIL);

	return fmt;
}

data_t
get_datahead(FILE *file, int32_t *start) {
	data_t d;
	int32_t dataStart = 35;

	fseek(file, 36, SEEK_SET);

	do {
		dataStart++;
		d.Subchunk2ID[0] = fgetc(file);
	} while (d.Subchunk2ID[0] != 'd');

	for (int i=1; i<4; i++) d.Subchunk2ID[i] = fgetc(file);
	dataStart += 4;

	d.Subchunk2Size = read_little_endian(file, 32);

	if (start != NULL) *start = dataStart;
	return d;
}

static metadata_list
new_metadata_node(char *code, uint32_t infoSize, char *info) {
	metadata_list node = malloc(sizeof(struct _metadata_node));
	for (int i=0; i<4; i++)
		node->code[i] = code[i];
	node->infoSize = infoSize;
	node->info = info;
	node->L = NULL;
	node->R = NULL;
	return node;
}

metadata_t
new_metadata_head() {
	metadata_t m = malloc(sizeof(struct _metadata_head));
	if (m == NULL)
		exit_error(MEM_ALLOC_FAILED);
	m->tagsAmount = 0;
	m->metadataSize = 12;
	m->first = m->last = NULL;
	return m;
}

void
add_metadata_tag(metadata_t md, char *code, uint32_t infoSize, char *info) {
	if (md == NULL)
		exit_error(PASSED_NULL_POINTER);

	metadata_list tag = new_metadata_node(code, infoSize, info);
	tag->L = md->last;
	if (md->last != NULL)
		md->last->R = tag;
	else
		md->first = tag;
	md->last = tag;
	md->metadataSize += 8 + infoSize;
	md->tagsAmount++;
}

metadata_t
get_metadata(FILE *file) {
	fseek(file, -8, SEEK_END);
	signed char c = 0;
	uint32_t id = 0;
	int32_t length = 0;

	for (int maxloop = 0; maxloop < 12 && feof(file) == 0 && id != INFO_LE; maxloop++) {
		for (c = fgetc(file); feof(file) == 0 && c != 'O'; fseek(file, -2, SEEK_CUR)) {
			c = fgetc(file);
			length++;
		}
		if (c == 'O') {
			fseek(file, -2, SEEK_CUR);
			id = read_little_endian(file, 32);
		}
	}

	metadata_t m = NULL; 

	if (id == INFO_LE) {
		m = new_metadata_head();
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

			add_metadata_tag(m, tagid, infosize, tagInfo);
			fseek(file, infosize, SEEK_CUR);
		}
	}
	return m;
}

riff_t
new_riff(uint32_t ChunkSize) {
	riff_t r;
	char *id = "RIFF";
	for (int i=0; i<4; i++)
		r.ChunkID[i] = id[i];
	r.ChunkSize = ChunkSize;
	return r;
}

fmt_t
new_fmt(uint32_t Subchunk1Size, uint16_t AudioFormat, uint16_t NumChannels, uint32_t SampleRate, uint16_t BitsPerSample) {
	fmt_t fmt;
	char *id = "fmt\040";
	for (int i=0; i<4; i++)
		fmt.Subchunk1ID[i] = id[i];

	fmt.Subchunk1Size = Subchunk1Size;
	fmt.AudioFormat = AudioFormat;
	fmt.NumChannels = NumChannels;
	fmt.SampleRate = SampleRate;
	fmt.BitsPerSample = BitsPerSample;
	
	fmt.BlockAlign = fmt.NumChannels * fmt.BitsPerSample/8;
	fmt.ByteRate = fmt.SampleRate * fmt.NumChannels * fmt.BitsPerSample/8;

	return fmt;
}

data_t new_datahead(uint32_t Subchunk2Size) {
	data_t d;
	char *id = "data";
	
	for (int i=0; i<4; i++)
		d.Subchunk2ID[i] = id[i];
	d.Subchunk2Size = Subchunk2Size;

	return d;
}

/*
void
free_riff_chunk(riff_t* r) {
	free(*r);
	*r = NULL;
	return;
}

void
free_fmt_subchunk(fmt_t* fmt) {
	free(*fmt);
	*fmt = NULL;
	return;
}

void
free_data_header(data_t* d) {
	free(*d);
	*d = NULL;
	return;
}
*/

void
free_metadata(metadata_t *m) {
	metadata_list l = (*m)->first;
	while (l != NULL) {
		metadata_list del = l;
		l = l->R;
		if (del->info)
			free(del->info);
		free(del);
		del = NULL;
	}
	free(*m);
	*m = NULL;
}

/* Input and output */

void
print_riff_chunk(riff_t r) {
	//if (r == NULL) 
	//	return;

	printf("---RIFF CHUNK\n");
	printf("ChunkID: ");
	for (int i=0; i<4; i++) printf("%c", r.ChunkID[i]);
	printf("\n");

	printf("ChunkSize: %" PRIu32 "\n", r.ChunkSize);

	printf("Format: ");
	for (int i=0; i<4; i++) printf("%c", r.Format[i]);
	printf("\n");

	return;
}

void
print_fmt_subchunk(fmt_t fmt) {
	//if (fmt == NULL)
	//	return;

	printf("---FMT SUBCHUNK\n");
	printf("Subchunk1ID: ");
	for (int i=0; i<4; i++) printf("%c", fmt.Subchunk1ID[i]);
	printf("\n");

	printf("Subchunk1Size %" PRIu32 "\n", fmt.Subchunk1Size);
	printf("AudioFormat %" PRIu16 "\n", fmt.AudioFormat);
	printf("NumChannels %" PRIu16 "\n", fmt.NumChannels);
	printf("SampleRate %" PRIu32 "\n", fmt.SampleRate);
	printf("ByteRate %" PRIu32 "\n", fmt.ByteRate);
	printf("BlockAlign %" PRIu16 "\n", fmt.BlockAlign);
	printf("BitsPerSample %" PRIu16 "\n", fmt.BitsPerSample);
	return;
}

void
print_data_header(data_t d) {
	//if (d == NULL)
	//	return;

	printf("---DATA HEADER\n");

	printf("Subchunk2ID: ");
	for (int i=0; i<4; i++) printf("%c", d.Subchunk2ID[i]);
	printf("\n");

	printf("Subchunk2Size: %" PRIu32 "\n", d.Subchunk2Size);
	return;
}

void
print_metadata(metadata_t h) {
	printf("---METADATA\n");
	if (h != NULL) {
		printf("Metadata size: %" PRIu32 "\n", h->metadataSize);
		metadata_list l = h->first;
		while (l != NULL) {
			for (int i=0; i<4; i++)
				printf("%c",l->code[i]);
			printf(": %s\n", l->info);
			l = l->R;
		}
	}
}

/* Input and output */

uint32_t 
riff_size(riff_t r) {
	return r.ChunkSize;
}

uint16_t 
audio_format(fmt_t f) {
	return f.AudioFormat;
}

uint32_t
audio_size(data_t d) {
	return d.Subchunk2Size;
}

uint16_t
bits_per_sample(fmt_t fmt) {
	return fmt.BitsPerSample;
}

uint32_t 
file_size(FILE *f) {
	riff_t r = get_riff(f);
	uint32_t size = 8 +riff_size(r);
	//__FREE_RIFF(r);
	return size;
}

// uint32_t fmt_size(fmt_t t) {
// }

uint32_t 
metadata_size(metadata_t m) {
	return (m != NULL) ? m->metadataSize : 0;
}

// uint32_t
// sample_rate(fmt_t fmt) {
// 	return (fmt != NULL) ? fmt->SampleRate : 0;
// }

// int32_t
// byte_rate(fmt_t fmt) {
// 	return (fmt != NULL) ? fmt->SampleRate : 0;
// }

/* Operations */

uint32_t
calc_riff_size(fmt_t fmt, data_t d, metadata_t m) {
	return (36 + audio_size(d) + metadata_size(m));
}

void 
set_riff_size(riff_t r, uint32_t size) {
	r.ChunkSize = size;
}

/* Writing to file */

void
write_riff(FILE *file, riff_t r) {
	assert(file != NULL);
	//assert(r != NULL);

	fseek(file, 0, SEEK_SET);

	char *tmpstr = "RIFF";
	for (int i=0; i<4; i++)
		fputc(tmpstr[i], file);

	write_little_endian(file, (int32_t)r.ChunkSize, 32);	

	tmpstr = "WAVE";
	for (int i=0; i<4; i++)
		fputc(tmpstr[i], file);
}

void
write_fmt(FILE *file, fmt_t fmt) {
	assert(file != NULL);
	//assert(fmt != NULL);

	fseek(file, 12, SEEK_SET);

	char *tmpstr = "fmt\040";
	for (int i=0; i<4; i++)
		fputc(tmpstr[i], file);

	write_little_endian(file, 16, 32);

	write_little_endian(file, (uint32_t)fmt.AudioFormat, 16);
	write_little_endian(file, (uint32_t)fmt.NumChannels, 16);
	write_little_endian(file, (uint32_t)fmt.SampleRate, 32);
	write_little_endian(file, (uint32_t)fmt.ByteRate, 32);
	write_little_endian(file, (uint32_t)fmt.BlockAlign, 16);
	write_little_endian(file, (uint32_t)fmt.BitsPerSample, 16);
}

void
write_data(FILE *file, data_t d) {
	assert(file != NULL);
	//assert(d != NULL);

	fseek(file, 36, SEEK_SET);

	char *tmpstr = "data";
	for (int i=0; i<4; i++)
		fputc(tmpstr[i], file);

	write_little_endian(file, d.Subchunk2Size, 32);
}

void
write_metadata(FILE *file, metadata_t m, bool write_head) {
	fseek(file, 0, SEEK_END);

	if (write_head) {
		char* list = "LIST";
		for (int i=0; i<4; i++)
			fputc(list[i], file);

		for (int i=0; i<4; i++)
			fputc(0x00, file);

		char* info = "INFO";
		for (int i=0; i<4; i++)
			fputc(info[i], file);
	}
	
	if (!m)	return;

	metadata_list rec = m->first;
	for (int i=0; !rec && i<m->tagsAmount; i++) {
		for (int j=0; j<4; j++)
			fputc(rec->code[j], file);
		write_little_endian(file, rec->infoSize, 32);
		for (uint32_t j=0; j<rec->infoSize && rec->info[j] != '\0'; j++)
			fputc(rec->info[j], file);
		rec = rec->R;
	}
}

/* Editing existing files */

// void
// remove_all_metadata(FILE *file, const char *filepath) {
// }
