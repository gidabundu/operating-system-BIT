/* shell.h — interactive shell interface */
#ifndef SHELL_H
#define SHELL_H
#include "types.h"

#define SHELL_BUF_SIZE  256
#define SHELL_HISTORY   8

void shell_init(void);
void shell_run(void);       /* blocking main loop */
void shell_process_char(char c);

#endif
