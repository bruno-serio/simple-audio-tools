#ifndef _EXIT_MESSAGES_H_
#define _EXIT_MESSAGES_H_
#include <stdio.h>
#include <stdlib.h>

enum EXIT {
	UNEXPECTED_ERROR,
	NOT_8_BIT_MULT,
	PASSED_NULL_POINTER
};

typedef enum EXIT exit_message_t;

void
exit_error (exit_message_t code);

#endif // _EXIT_MESSAGES_H_
