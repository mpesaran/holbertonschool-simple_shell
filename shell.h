#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* Global Variables */
extern char **environ;

/* Function Prototypes */
void print_prompt(void);
char *read_input(void);
void trailing_input(char *input_trail);
int command_handler(char *command);


#endif
