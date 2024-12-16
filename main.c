#include "shell.h"

/**
* main - Entry point for simple shell
* Return: Always '0'
*/
int main(void) {
	char *command_line, *command;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			print_prompt();
		
		command_line = read_input();

		/* EOF */
		if (command_line == NULL)
		{
			if	(isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		trailing_input(command_line);
		command = strtok(command_line, " ");
		
		while (command != NULL)
		{
			if (strlen(command) > 0)
				command_handler(command);
			command = strtok(NULL, " ");
		}
		
		free(command_line);	
	}
	
	return 0;
}
 
