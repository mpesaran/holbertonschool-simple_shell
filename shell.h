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


/**** Function Prototypes ****/

/* Input_handlers.c - Related to user input processing */
void print_prompt(void);
char *get_user_input(void);
void remove_trailing_spaces(char *string);
int is_only_spaces(char *string);

/* Command_executor.c - Core command executing functionality */
int execute_command(char *command);

/* Path_handlers.c - Dealing with PATH manipulation and searching */
char *find_command_in_path(const char *command);
char *combine_path(const char *directory, const char *command);
char *normalize_path(const char *path);

/* env_handlers.c - Environment-related functions */
char *_getenv(const char *name);
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

#endif
