#include "shell.h"

/* add int is_AllSpace(char *s); */

/* Print shell prompt (interactive bs non-interactive) */
void print_prompt(void)
{
	if (isatty(STDIN_FILENO))
	{
		printf("simple_shell$");
		fflush(stdout);
	}
}

/* Read the users command input */
char *read_input(void)
{
	char *input_line = NULL;
	size_t buffer_length = 0;
<<<<<<< HEAD

	ssize_t read_command = getline(&input_line, &buffer_length, stdin);

=======
	ssize_t read_command; 
	
	read_command = getline(&input_line, &buffer_length, stdin);
>>>>>>> origin/is_AllSpace
	if (read_command == -1)
	{
		
		if (feof(stdin)) /* end of file */
		{
			free(input_line);
			printf("End of file /Ctrl D detected.. Exiting\n");
			exit (1);
		}
		else
		{	
			perror("Error in getline\n");	
		}
			
		free(input_line);
		return (NULL);
	}
<<<<<<< HEAD
	if (strcmp(input_line, "exit\n") == 0) /* exit command */
	{
		printf("Exiting\n");
		exit(1);
=======
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
>>>>>>> origin/is_AllSpace

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
<<<<<<< HEAD
	char *args[2];
	
	char *token;
	int count = 0;

	/* Preparation arguments for EXECVE */
	/*args[0] = command; */
	/*args[1] = NULL; */

	/*args[100];*/
	
	/* Tokenize */
	token = strtok(command, " ");
	while (token != NULL) 
	{
		args[count++] = token;   
		token = strtok(NULL, " ");
	}
	args[count] = NULL;

=======
	char *args[1024];
	char *envp[] = {NULL};
	char *token;
	int i = 0;
	
>>>>>>> origin/is_AllSpace
	if (!command || strlen(command) == 0)
	{
		return (0);
	}
<<<<<<< HEAD

=======
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
	
>>>>>>> origin/is_AllSpace
	PID = fork();

	if (PID < 0)
	{
		perror("Failed to fork process");
		return (-1);
	}
	else if (PID == 0)
	{
<<<<<<< HEAD
		if (execvp(command, args) == -1)
		/*if (execve(args[0], args, environ) == -1)*/
		{
			fprintf(stderr, "%s: %s Command not found in PATH\n", command, strerror(errno));
			exit(EXIT_FAILURE);
		}
=======
		if (execve(args[0], args, envp)== -1)
			{
				perror("execve");
				exit(EXIT_FAILURE);
			}
>>>>>>> origin/is_AllSpace
	}
	else
		{
			do { 
				waitpid(PID, &status, WUNTRACED);
			} while (!WIFEXITED(status) && !WIFSIGNALED(status));
			/*wait(NULL);*/
		}
	return (0);
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

	/*free(input_line);*/
	return (0);
}
