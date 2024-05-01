#ifndef _EXIT_MESSAGES_H_
#define _EXIT_MESSAGES_H_
#include <stdio.h>
#include <stdlib.h>

enum EXIT {
	SUCCESS,
	UNEXPECTED_ERROR,
	NOT_8_BIT_MULT,
	PASSED_NULL_POINTER,
	BLOCK_ALIGN_TEST_FAIL,
	BYTE_RATE_TEST_FAIL,
	MEM_ALLOC_FAILED,
	COULDNT_REMOVE_FILE,
	COULDNT_RENAME_FILE
};

typedef enum EXIT exit_message_t;

void
exit_error (exit_message_t code);

#endif // _EXIT_MESSAGES_H_
