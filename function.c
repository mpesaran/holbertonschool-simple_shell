#include "shell.h"

/**
 * print_prompt - Print shell prompt (interactive bs non-interactive)
 * Return: void
 */

void print_prompt(void)
{
	if (isatty(STDIN_FILENO))
	{
		write(STDOUT_FILENO, "simple_shell$ ", 14);
	}
}

/**
 * read_command - Read the users command input
 * Return: input variable as string
 */
char *read_command(void)
{
	char *input = NULL;
	size_t len = 0;

	if (getline(&input, &len, stdin) == -1)
	{
		if (input)
			free(input);
		return (NULL);
	}
	return (input);
}

/**
 * clean_command - Input command cleanup (Removes trailing space)
 *
 * @cmd : input command as string
 * Return: void
 */
void clean_command(char *cmd)
{
	size_t end, start = 0;

	if (!cmd || strlen(cmd) == 0)
		return;

	while (cmd[start] == ' ' || cmd[start] == '\t')
		start++;

	if (cmd[start] == '\0')
	{
		cmd[0] = '\0';
		return;
	}
	end = strlen(cmd) - 1;
	while (end > start && (cmd[end] == ' ' || cmd[end] == '\t'
			|| cmd[end] == '\n'))
		cmd[end--] = '\0';
}

/**
 * command_handler - executes the users input command in shell
 *
 * @cmd : input command as string type
 * @paths : linked list PATH variable as type path_list
 * @last_status : program status input as integer type
 * Return: status(updated) as integer type
 */
int command_handler(char *cmd, path_list *paths, int *last_status)
{
	char *args[1024], *full_path = NULL, *token;
	int i = 0, status = 0;

	if (!cmd || strlen(cmd) == 0)
		return (0);
	token = strtok(cmd, " \t");
	while (token != NULL && i < 1023)
	{
		args[i++] = token;
		token = strtok(NULL, " \t\n");
	}
	args[i] = NULL;
	if (args[0] == NULL)
	{
		fprintf(stderr, "Error: No command entered\n");
		return (-1);
	}
	if (handle_builtin(args, paths, cmd, last_status) != -1)
		return (0);

	full_path = resolve_command_path(args, paths);
	if (!full_path)
	{
		fprintf(stderr, "%s: 1: %s: not found\n", "./hsh", args[0]);
		return (127);
	}
	status = execute_command(full_path, args);
	free(full_path);
	return (status);
}

/**
 * is_whitespace -function to check input string if only consist of spaces
 *
 * @str: input as string type
 * Return: integer type of 1 if true else 0
 */

int is_whitespace(char *str)
{
	size_t space_num = 0;
	char *input = str;
	int count = 0;

	while (input[count] != '\0')
	{
		if (input[count] == ' ' || input[count] == '\t' ||
				input[count] == '\n')
			space_num++;
		count++;
	}
	if (space_num == strlen(str))
		return (1);
	return (0);
}
