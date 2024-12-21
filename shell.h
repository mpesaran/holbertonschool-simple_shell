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

/* Data structure for storing path directories */
typedef struct path_node
{
	char *directory;
	struct path_node *next;
	struct path_node *prev;
} path_node;

typedef struct path_list
{
	path_node *head;
	path_node *tail;
} path_list;

/* Function Prototypes */
void print_prompt(void);
char *read_input(void);
void trailing_input(char *input_trail);
int command_handler(char *command, path_list *paths);
int is_AllSpace(char *s);
char *_getenv(const char *name);
void build_path_list(path_list *list);
char *find_in_path(const char *command, path_list *paths);
void free_path_list(path_list *list);
int execute_command(char *path, char **args);
void print_env(void);
#endif
