#include "shell.h"

/**
 * print_env - function to print the environmental variable, environ
 *
 * Return: void
 */
void print_env(void)
{
	char **env = environ;

	while (*env)
	{
		printf("%s\n", *env);
		env++;
	}
}

/**
 * _getenv - Retrieves the value of an environment variable.
 *
 * @name: variable name in environment
 * Return: value corresponding to the provided variable name, NULL if not found
 */
char *_getenv(const char *name)
{
	size_t name_len = strlen(name);
	char **env = environ;

	while (*env)
	{
		if (strncmp(*env, name, name_len) == 0 && (*env)[name_len] == '=')
			return (*env + name_len + 1);
		env++;
	}
	return (NULL);
}

/**
 * exit_shell - execute exit process
 *
 * @args: command argument input
 * @paths: linked list PATH variable input as type path_list
 * @cmd: command input
 * @last_status: last program status input as int type
 * Return: void
 */

void exit_shell(char **args, path_list *paths, char *cmd, int *last_status)
{
	int exit_code = *last_status;

	if (args[1] != NULL)
	{
		exit_code = atoi(args[1]);
		if (exit_code <= -1)
		{
			fprintf(stderr, "exit: Illegal number: %s\n", args[1]);
			exit_code = 2;
		}
	}
	free_path_list(paths);
	free(cmd);
	exit(exit_code);
}

/**
 * handle_builtin- handle builtin functions of exit and env
 *
 * @args: command argument input
 * @paths: linked list PATH variable input as type path_list
 * @cmd: command input
 * @last_status: last program status input as int type
 * Return: 0 if successful else -1
 */

int handle_builtin(char **args, path_list *paths, char *cmd, int *last_status)
{
	if (strcmp(args[0], "exit") == 0)
	{
		exit_shell(args, paths, cmd, last_status);
	}
	if (strcmp(args[0], "env") == 0)
	{
		print_env();
		return (0);
	}
	return (-1);
}

/**
 * resolve_command_path - search for path variable of the  input argument
 *
 * @args: command argument input
 * @paths: linked list PATH variable input as type path_list
 * Return: matched path variable as type string
 */
char *resolve_command_path(char **args, path_list *paths)
{
	char *path_env = _getenv("PATH");
	char *full_path = NULL;

	if (!path_env || strlen(path_env) == 0)
	{
		if (strchr(args[0], '/'))
		{
			if (access(args[0], X_OK) == 0)
				full_path = strdup(args[0]);
			else
			{
				fprintf(stderr, "%s: No such file or directory\n", args[0]);
				return (NULL);
			}
		}
	}
	else
		full_path = find_in_path(args[0], paths);
	return (full_path);
}
