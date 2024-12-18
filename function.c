#include "shell.h"

/* Print shell prompt (interactive bs non-interactive) */
void print_prompt(void)
{
	if (isatty(STDIN_FILENO))
	{
		write(STDOUT_FILENO, "simple_shell$ ", 14);
	}
}

/* Read the users command input */
char *read_input(void)
{
	char *input_line = NULL;
	size_t buffer_length = 0;
	ssize_t read_command; 
	
	read_command = getline(&input_line, &buffer_length, stdin);

	if (read_command == -1)
	{
		if (input_line)
			free(input_line);
		return (NULL);
	}

	return (input_line);
}


/* Input cleanup (Removes trailing space) */
void trim_input(char *input)
{
	char *start = input;
	char *end;

	while (*start == ' ' || *start == '\n' || *start == '\t')
		start++;
	
	if (start != input)
		memmove(input, start, strlen(start) + 1);
	
	end = input + strlen(input) - 1;
	while (end > input && (*end == ' ' || *end == '\n' || *end == '\t'))
		end--; 

	*(end + 1) = '\0';
}

/* Command handler - executes the users input command in shell */
int command_handler(char *command)
{
	pid_t PID;
	int status;
	int i = 0;
	char *token;
	char *args[100];
	char *envp[] = {NULL};

	if (!command || strlen(command) == 0)
	{
		return (0);
	}

	token = strtok(command, " \t");
	while (token != NULL)
	{
		args[i] = token;
		i++;
		token = strtok(NULL, " \t");
	}
	args[i] = NULL;

		if (access(command, X_OK) != 0)
		{
			fprintf(stderr, "%s: Command not found\n", command);
			return (-1);
		}

	PID = fork();
	if (PID < 0)
	{
		perror("Failed to fork process");
		return(-1);
	}
	else if (PID == 0)
	{
		if (execve(args[0], args, envp) == -1)
		{
			perror("execve ERROR");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		waitpid(PID, &status, 0);
	}
	return WEXITSTATUS(status);
}
