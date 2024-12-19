#include "shell.h"

/* Functions related to user input processing */

/* Print shell prompt (interactive bs non-interactive) */
void print_prompt(void)
{
	if (isatty(STDIN_FILENO))
	{
		write(STDOUT_FILENO, "simple_shell$ ", 14);
	}
}

/* Read the users command input */
char *get_user_input(void)
{
	char *line = NULL;
	size_t length = 0;
	ssize_t read; 
	
	read = getline(&line, &length, stdin);

	if (read == -1) {
		free(line);
		return NULL;
	}

	/* Removes trailing '\n' */
	if (read > 0 && line[read - 1] == '\n')
	{
		line[read - 1] = '\0';
	}
	 
	return line;
}

/* Input cleanup (Removes trailing space or spaces from command input) */
void remove_trailing_spaces(char *string)
{
	int start = 0;
	int end;

	if (!string) return;

	/* Find end of string */
	end = strlen(string) -1;

	/* Trim leading spaces (BEGGING PART OF INPUT STRING) */
	while (string[start] && (string[start] == ' ' || string[start] == '\t'))
	{
		start++;
	}
	
	/* Trim trailing spaces (END PART OF INPUT STRING)  */
	while (end >= start && (string[end] == ' ' || string[end] == '\t' || string[end] == '\n'))
	{
		end--;
	}

	/* If string is just all spaces */
	if (end < start)
	{
		string[0] = '\0';
		return;
	}
	
	/* Relocate string position and add null terminator */
	if (start > 0)
	{
		int i;
		for (i = 0; i <= end - start; i++)
		{
			string[i] = string[i + start];
		}
		string[i] = '\0';
	} 
	else 
	{
		string[end + 1] = '\0';
	}
}

/**
 * is_only_spaces - function to check input string is all charcters of  spaces
 *
 * @s: input string
 * Return: 1 if true else 0
 */

int is_only_spaces(char *string)
{
	if (!string) return 1;

	while (*string)
	{
		if (*string != ' ' && *string != '\t' && *string != '\n')
		{
			return 0;
		}
		string++;
	}
	return 1;
}