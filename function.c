#include "shell.h"

// Print shell promt (interactive vs non-interactive
void print_prompt(void) {
	printf("simple_shell$ ");
	fflush(stdout);
}

// read the users command input
char *read_input(void) {
	char *input_line = NULL;
	size_t buffer_length = 0;

	ssize_t read_command = getline(&input_line, &buffer_length, stdin);

	if (read_command == -1)
	{
		free(input_line);
		return NULL;
	}

	return input_line;
	
}

// Remove trailing space from input
// -> remove_trail()

// Executes string type commands
// -> command_handler()

