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

// Command handler - executes the users input command in shell
int command_handler(char *command) {
	pid_t PID;
	int status;
	char *args[2];
	extern char **environ;

	// Preparation arguments for EXECVE
	args[0] = command;
	args[1] = NULL;

	if (!command || strlen(command) == 0)
	{
		return 0;
	}

	PID = fork();

	if (PID < 0)
	{
		perror("Failed to fork process");
		return -1;
	} else if (PID == 0)
	{
		if (execve(command, args, environ) == -1)
		{
			fprintf(stderr, "%s: Command not found in PATH\n", command);
			exit(EXIT_FAILURE);
		}
		}
	return 0;
}

/**
* main - Entry point for simple shell
* Return: Always '0'
*/
int main(void) {
	char *command_line;

	while (1)
	{
		print_prompt();

		command_line = read_input();

		// EOF
		if (command_line == NULL)
		{
			printf("\n");
			break;
		}

		trailing_input(command_line);

		command_handler(command_line);

		free(command_line);	
	}
	
	return 0;
}
 