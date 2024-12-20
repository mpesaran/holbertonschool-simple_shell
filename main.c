#include "shell.h"

/**
* main - Entry point for simple shell
* Return: Always '0'
*/

int main(void)
{
	char *command_line;
	path_list paths = {NULL, NULL};
	
	build_path_list(&paths);

	while (1)
	{
		if (isatty(STDIN_FILENO))
			print_prompt();
		
		command_line = read_input();

		if (command_line == NULL)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			free(command_line);
			break;
		}
		if (is_AllSpace(command_line) == 1)
                {
                        free(command_line);/* for valgrind */
                        break;
                }
		trailing_input(command_line);

		if (strlen(command_line) > 0)
			command_handler(command_line, &paths);
		free(command_line);
	}
	free_path_list(&paths);
	return (0);
}
