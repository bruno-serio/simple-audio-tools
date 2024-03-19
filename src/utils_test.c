// todo
// check read functions
#include <stdio.h>
#include "../include/utils.h"

int main(int argc, char *argv[]) {
	FILE *file = fopen("in", "rb");

	for (int i=0; i<10; i++) printf("%d", read_16_bit_sample(file));

	return 0;
}
