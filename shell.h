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
 * struct path_node - linked list storing for PATH directory
 *
 * @directory: directory variable of type string
 * @next: pointer to next node
 * @prev: pointer to previous node
 * Description : data structure for storing path directories
 */

typedef struct path_node
{
	char *directory;
	struct path_node *next;
	struct path_node *prev;
} path_node;

/**
 * struct path_list - link list for pointers of head or trail
 *
 * @head: pointer to the head/start of the list
 * @tail: pointer to the tail/end of the list
 * Description : link list for storing address of the head or tail of the list
 */
typedef struct path_list
{
	path_node *head;
	path_node *tail;
} path_list;

/* Function Prototypes */
void print_prompt(void);
char *read_command(void);
void clean_command(char *input);
int command_handler(char *command, path_list *paths, int *last_status);
int is_whitespace(char *str);
char *_getenv(const char *name);
void build_path_list(path_list *list);
char *find_in_path(const char *command, path_list *paths);
void free_path_list(path_list *list);
int execute_command(char *path, char **args);
void print_env(void);
void exit_shell(char **args, path_list *paths, char *cmd, int *last_status);
int handle_builtin(char **args, path_list *paths, char *cmd, int *last_status);
char *resolve_command_path(char **args, path_list *paths);
void append_path_node(path_list *list, const char *directory);

#endif
