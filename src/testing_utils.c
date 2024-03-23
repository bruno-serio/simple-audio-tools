#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/utils.h"

int main (int argc, char **argv) {
	char directory[14] = "../wav-files/";

	if (argc<2) exit(1);

	char filePath[48];

	memset(filePath, '\0', sizeof(filePath));
	strcpy(filePath, directory);
	strcat(filePath,argv[1]);

	FILE *audioFile = fopen(filePath, "rb");

	for (int i=0; i<10; i++) {
		signed short num = read_16_bit_sample(audioFile);
		printf("%d, %2x\n", num, num);
	}

	fclose(audioFile);

	return 0;
}
