#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

/* Global Variables */
extern char **environ;


/**
 * struct path_list - create structure for PATH variable in env
 *
 * @path: string variable of PATH value
 * @next: pointer to next node of struct path_list
*/

struct path_list {
	char *path;
	struct path_list *next;
};

typedef struct path_list path_t;

/* Function Prototypes */
void print_prompt(void);
char *read_input(void);
void trailing_input(char *input_trail);
int command_handler(char *command);
int is_AllSpace(char *s);
int _execvp(char *file, char *argv[]);
path_t *get_path(path_t *head);
void free_path(path_t *head);
char *matched_path(path_t *head, char *cmd);


#endif
