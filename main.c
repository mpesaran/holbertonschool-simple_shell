#include "shell.h"

/**
* main - Entry point for simple shell
* Return: Always '0'
*/
int main(void) {
	char *command_line;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			print_prompt();
		
		command_line = get_user_input();

		/* EOF */
		if (command_line == NULL)
		{
			if	(isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			free(command_line); /* for valgrind */
			break;
		}

		/* skip processing IF only spaces or no input */
		if (is_only_spaces(command_line) == 1)
		{	
			free(command_line); /* for valgrind */
			break;
		}


		remove_trailing_spaces(command_line);

		/* Execute command */
		if (strlen(command_line) > 0)
			execute_command(command_line);

		free(command_line);	
	}

	return 0;
}
 
