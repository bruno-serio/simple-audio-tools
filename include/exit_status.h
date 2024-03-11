#ifndef _EXIT_STATUS_H_
#define _EXIT_STATUS_H_

typedef enum _exit_status exit_t;

/**
 * \brief	Prints some information about error exit codes to the standard output.
 * \date	2024
 */
void
exit_code_handler(exit_t exitCode);
	
#endif // _EXIT_STATUS_H_
