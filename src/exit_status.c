#include "../include/exit_status.h"

enum _exit_status {
	E_SUCCESS = 0,
	E_FAIL = 1,
	E_COULDNT_ALLOCATE_MEMORY = 2,
	E_COULDNT_OPEN_FILE = 3,
	E_UNEXPECTED_NULL_POINTER = 4,
	E_INVALID_AUDIO_FORMAT = 5,
	E_UNEXPECTED_VALUE = 6
};

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
