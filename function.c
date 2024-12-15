#include "shell.h"

/* Print shell prompt (interactive bs non-interactive) */
void print_prompt(void)
{
	if (isatty(STDIN_FILENO))
	{
		printf("simple_shell$ ");
		fflush(stdout);
	}
}

/* Read the users command input */
char *read_input(void)
{
	char *input_line = NULL;
	size_t buffer_length = 0;

	ssize_t read_command = getline(&input_line, &buffer_length, stdin);

	if (read_command == -1)
	{
		free(input_line);
		return (NULL);
	}

	return (input_line);
}

/* Input cleanup (Removes trailing space) */
void trailing_input(char *input_trail)
{
	if (input_trail)
	{
		input_trail[strcspn(input_trail, "\n")] = 0;
	}
}

/* Command handler - executes the users input command in shell */
int command_handler(char *command)
{
	pid_t PID;
	int status;
	char *args[2];

	/* Preparation arguments for EXECVE */
	args[0] = command;
	args[1] = NULL;

	if (!command || strlen(command) == 0)
	{
		return (0);
	}

	PID = fork();

	if (PID < 0)
	{
		perror("Failed to fork process");
		return (-1);
	}
	else if (PID == 0)
	{
		if (execve(command, args, environ) == -1)
		{
			fprintf(stderr, "%s: Command not found in PATH\n", command);
			exit(EXIT_FAILURE);
		}
	}
		else
		{
			do {
				waitpid(PID, &status, WUNTRACED);
			} while (!WIFEXITED(status) && !WIFSIGNALED(status));
		}
	return (0);
}
