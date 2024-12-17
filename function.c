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
	char *args[2];
	char *envp[] = {NULL};
	char *token;

	if (!command || strlen(command) == 0)
	{
		return (0);
	}

	token = strtok(command, "\n");
	while (token != NULL)
	{
		if (access(command, X_OK) != 0)
		{
			fprintf(stderr, "%s: Command not found\n", token);
			token = strtok(NULL, "\n ");
			continue;;
		}
	
		PID = fork();
		if (PID < 0)
		{
			perror("Failed to fork process");
			return (-1);
		}
		else if (PID == 0)
		{
		/* Preparation arguments for EXECVE */
			args[0] = token;
			args[1] = NULL;
	
			if (execve(token, args, envp)== -1)
			{	
				perror("execve");
				exit(EXIT_FAILURE);
			}
	}
	else
	{
		do {
			waitpid(PID, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	token = strtok(NULL, "\n ");
}
return(0);
}
