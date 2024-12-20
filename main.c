#include "shell.h"

/**
* main - Entry point for simple shell
* Return: Always '0'
*/
int main(void)
{
	char *command_line;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			print_prompt();
		}
		
		command_line = get_user_input();

		if (command_line == NULL)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			free(command_line); /* for valgrind */
			break;
		}
		
		if (!is_only_spaces(command_line))
		{
			execute_command(command_line);
		}
		free(command_line);
	}
	return 0;
}
