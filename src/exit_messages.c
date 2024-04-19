#include "../include/exit_messages.h"

void
exit_error (exit_message_t code) {

	switch (code) {
		case NOT_8_BIT_MULT:
			printf("The number of bits is not a multiple of 8.\n");
			break;

		case PASSED_NULL_POINTER:
			printf("Caught invalid NULL pointer.\n");
			break;

		case UNEXPECTED_ERROR:
		default:
			printf("An unexpected error has ocurred!\n");
			break;
		}
	printf("Exiting with code %d\n", code);
	exit(code);
}
