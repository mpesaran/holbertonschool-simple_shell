#include "shell.h"

/**
* main - Entry point for simple shell
* Return: Always '0'
*/

int main(void)
{
	char *command_line;
	path_list paths = {NULL, NULL};
	int status = 0;

	build_path_list(&paths);
	while (1)
	{
		if (isatty(STDIN_FILENO))
			print_prompt();
		
		command_line = read_command();

		if (command_line == NULL)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			free(command_line);
			break;
		}
		if (is_whitespace(command_line) == 1)
                {
                        free(command_line);/* for valgrind */
                        continue;
                }
		clean_command(command_line);

		if (strlen(command_line) > 0)
			status = command_handler(command_line, &paths, &status);
		
		free(command_line);
	}
	free_path_list(&paths);
	return (status);
}
