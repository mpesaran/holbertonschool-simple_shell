#include "shell.h"

/* add int is_AllSpace(char *s); */

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
	/* added for space detection */
	/**
	*if (is_AllSpace(input_line))
	*{
		return (NULL);
	*}
	*/
 
	if (strcmp(input_line, "exit\n") == 0) /* exit command */
	{
		/*printf("Exiting\n"); for debugging */
		exit(1);

	}

	return (input_line);
}

/* Input cleanup (Removes trailing space) */
void trailing_input(char *input_trail)
{
	size_t end, start;

	if (!input_trail || strlen(input_trail) == 0)
		return;

	start = 0;
	while (input_trail[start] == ' ' || input_trail[start] == '\t')
	{
        	start++;
    	}
	if (input_trail[start] =='\0')
	{
		input_trail[0] = '\0';
		return;
	}
	end = strlen(input_trail) - 1;
	while (end > start && (input_trail[end] == ' ' || input_trail[end] == '\t' || input_trail[end] == '\n'))
	{
        	input_trail[end] = '\0';
        	end--;
	}
}

/* Command handler - executes the users input command in shell */
int command_handler(char *command)
{
	pid_t PID;
	int status;
	char *args[1024];
	char *envp[] = {NULL};
	char *token;
	int i = 0;
	
	if (!command || strlen(command) == 0)
	{
		return (0);
	}
	token = strtok(command, " \t");
	while (token != NULL && i < 1023)
	{
        	args[i++] = token;
        	token = strtok(NULL, " \t\n");
	}
	args[i] = NULL; 
	
	if (args[0] == NULL)
	{
		fprintf(stderr, "Error: No command entered\n");
		return (-1);
	}
	if (access(args[0], X_OK) != 0)
	{
		fprintf(stderr, "%s: Command not found\n", args[0]);
		return(-1);
	}
	
	PID = fork();
	if (PID < 0)
	{
		perror("Failed to fork process");
		return (-1);
	}
	else if (PID == 0)
	{
		if (execve(args[0], args, envp)== -1)
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
	return WEXITSTATUS(status);
}

/**
 * is_AllSpace - function to check input string is all charcters of  spaces
 *
 * @s: input string
 * Return: 1 if true else 0
 */

int is_AllSpace(char *s)
{
	char *input_line = s;
	int count = 0;
	size_t space_num = 0;

	while (input_line[count] != '\0')
	{	/* check for space, tab and newline */
		if (input_line[count] == ' ' || input_line[count] == '\t' || input_line[count] == '\n')
		{
			space_num++;
		}
		count++;	
	}
	if (space_num == strlen(s))
	{
		return (1); /* true as input all space */	
	}
	return (0);
}
