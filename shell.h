#ifndef SHELL_H
#define SHELL_H

/* Define PATH_MAX (Max memory count of characters) */
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include <limits.h>

/* Global Variables */
extern char **environ;


/* Function Prototypes */
void print_prompt(void);
char *get_user_input(void);
void remove_trailing_spaces(char *string);
int execute_command(char *command);
char *combine_path(const char *directory, const char *command);
char *_getenv(const char *name);
char *find_command_in_path(const char *command);
int is_only_spaces(char *string);
char *normalize_path(const char *path);

#endif
