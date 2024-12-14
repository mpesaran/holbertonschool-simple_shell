#include "shell.h"

/* Function Prototype */

// Print shell prompt (interactive bs non-interactive)
void print_prompt(void) {
	if (isatty(STDIN_FILENO)){
	printf("simple_shell$ ");
	fflush(stdout);
	}
}

// Read the users command input
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

// Input cleanup (Removes trailing space)
void trailing_input(char *input_trail) {
	if (input_trail)
	{
		input_trail[strcspn(input_trail, "\n")] = 0;
	}
	
}

/**
* main - Entry point for simple shell
* Return: Always '0'
*/
int main(void) {
	char *line;

	while (1)
	{
		print_prompt();

		line = read_input();

		// EOF
		if (line == NULL)
		{
			printf("\n");
			break;
		}

		trailing_input(line);
		
	}
	
	return 0;
}
 