#include "shell.h"

/* add int is_only_spaces(char *s); */

/* Print shell prompt (interactive bs non-interactive) */
void print_prompt(void)
{
	if (isatty(STDIN_FILENO))
	{
		write(STDOUT_FILENO, "simple_shell$ ", 14);
	}
}

/* Read the users command input */
char *get_user_input(void)
{
	char *input_line = NULL;
	size_t buffer_size = 0;
	ssize_t bytes_read; 
	
	bytes_read = getline(&input_line, &buffer_size, stdin);
	if (bytes_read == -1)
	{
		if (input_line)
			free(input_line);
		return (NULL);
	}
 
	return (input_line);
}

/* Input cleanup (Removes trailing space or spaces from command input) */
void remove_trailing_spaces(char *input)
{
	size_t end, start;

	if (!input || strlen(input) == 0)
		return;

	start = 0;
	while (input[start] == ' ' || input[start] == '\t')
	{
        	start++;
    	}
	if (input[start] =='\0')
	{
		input[0] = '\0';
		return;
	}
	end = strlen(input) - 1;
	while (end > start && (input[end] == ' ' || input[end] == '\t' || input[end] == '\n'))
	{
        	input[end] = '\0';
        	end--;
	}
}

/* Command handler - executes the users input command in shell */
int execute_command(char *command)
{
	pid_t PID;
	int status;
	char *args[1024];
	char *envp[] = {NULL};
	char *token;
	int i = 0;
	char *command_path;
	
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
	
	/* No command entered do this */
	if (args[0] == NULL)
	{
		fprintf(stderr, "Error: No command entered\n");
		return (-1);
	}

	/* Find command in PATH */
	command_path = find_command_in_path(args[0]);
	if (!command_path)
	{
		/* printf("%s: Testing PATH_FINDER func error"); */
		fprintf(stderr, "%s: Command not found in PATH variable", args[0]);
		return (-1);
	}
	
	/* Check if command exists and can be run */
	if (access(args[0], X_OK) != 0)
	{
		perror("access");
		fprintf(stderr, "%s: Command not found\n", args[0]);
		return(-1);
	}
	
	/* Create a new fork to process executed command */
	PID = fork();
	if (PID < 0)
	{
		perror("Failed to fork process");
		free(command_path); /* free failed memory */
		return (-1);
	}
	else if (PID == 0)
	{
		if (execve(command_path, args, envp)== -1)
			{
				/* printf("%s: Chilld enviroment PATH FORK ERROR point"); */
				perror("execve failure");
				free(command_path); 
				exit(EXIT_FAILURE);
			}
	}
	else
	{
		do {
			waitpid(PID, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	free(command_path);
	return WEXITSTATUS(status);
}

/* Connects directory path and command name into a string with '/' */
char *combine_path(const char *directory, const char *command) 
{	
	char *full_path = malloc(strlen(directory) + strlen(command) + 2);
	if (!full_path) /* Memory allocation checker */
	{
		perror("malloc error");
		exit(EXIT_FAILURE);
	}
	sprintf(full_path, "%s/%s", directory, command);
	return full_path;
}

/* PATH FINDER: Function to search the PATH environment (FILE) */
char *find_command_in_path(const char *command) {
	char *path = getenv("PATH");
	char *path_copy = strdup(path);
	char *dir;
	struct stat buffer;

	printf("Searching for '%s' in PATH: %s\n", command, path);

	dir = strtok(path_copy, ":");
	while (dir) 
	{ /* loops through the directories in PATH file */
		char *full_path_command = combine_path(dir, command);
		if (stat(full_path_command, &buffer) == 0 && (buffer.st_mode & S_IXUSR)) 
		{
			free(path_copy);
			return full_path_command;
		}
		free(full_path_command);
		dir = strtok(NULL, ":");
	}
	free(path_copy);
	return NULL;
}

/**
 * is_only_spaces - function to check input string is all charcters of  spaces
 *
 * @s: input string
 * Return: 1 if true else 0
 */

int is_only_spaces(char *input)
{
	int i = 0;

	while (input[i] != '\0')
	{
		if (input[i] != ' ' && input[i] != '\t' && input[i] != '\n')
		{
			return (0);
		}
		i++;
	}
	return(1);
}
