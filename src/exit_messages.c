#include "../headers/exit_messages.h"

void
exit_error (exit_message_t code) {

	switch (code) {
		case SUCCESS:
			printf("The operation was successful!\n");
			break;

		case NOT_8_BIT_MULT:
			printf("The number of bits is not a multiple of 8.\n");
			break;

		case PASSED_NULL_POINTER:
			printf("Caught invalid NULL pointer.\n");
			break;
		
		case BLOCK_ALIGN_TEST_FAIL:
			printf("Block Align test failed.\n");
			break;

		case BYTE_RATE_TEST_FAIL:
			printf("Byte Rate test failed.\n");
			break;
		
		case MEM_ALLOC_FAILED:
			printf("Memory allocation failed.\n");
			break;
		
		case UNEXPECTED_ERROR:
		default:
			printf("An unexpected error has ocurred!\n");
			printf("Exiting with code 1.\n");
			exit(1);
			break;
		}
	printf("Exiting with code %d.\n", code);
	exit(code);
}
