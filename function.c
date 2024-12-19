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
	char *find_command_in_PATH;
	
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
	
	find_command_in_PATH = path_finder(args[0]);
		if (args[0] == NULL)
		{
			fprintf(stderr, "Error: No command entered\n");
			return (-1);
		}

	if (!find_command_in_PATH)
	{
		/* printf("%s: Testing PATH_FINDER func error"); */
		fprintf(stderr, "%s: Command not found in PATH variable", args[0]);
		return -1;
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
		free(find_command_in_PATH); /* free failed memory */
		return (-1);
	}
	else if (PID == 0)
	{
		if (execve(find_command_in_PATH, args, envp)== -1)
			{
				/* printf("%s: Chilld enviroment PATH FORK ERROR point"); */
				perror("execve failure");
				free(find_command_in_PATH); 
				exit(EXIT_FAILURE);
			}
	}
	else
	{
		do {
			waitpid(PID, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	free(find_command_in_PATH);
	return WEXITSTATUS(status);
}

/* Connects directory path and command name into a string with '/' */
char *concat_path(const char *directory_path, const char *command_name) {
	char *full_path = malloc(strlen(directory_path) + strlen(command_name) + 2);
	if (!full_path) /* Memory allocation checker */
	{
		perror("malloc error");
		exit(EXIT_FAILURE);
	}
	sprintf(full_path, "%s/%s", directory_path, command_name);
	return full_path;
}

/* PATH FINDER: Function to search the PATH environment (FILE) */
char *path_finder(const char *command_finder) {
	char *path_environment_var = getenv("PATH");
	char *copy_path = strdup(path_environment_var);
	char *directory_spliter = strtok(copy_path, ":"); /* Split PATH directories with ":' */
	struct stat buffer;

	while (directory_spliter) { /* loops through the directories in PATH file */
		char *full_path_command = concat_path(directory_spliter, command_finder);
		if (stat(full_path_command, &buffer) == 0 && (buffer.st_mode & S_IXUSR)) {
			free(copy_path);
			return full_path_command;
		}
		free(full_path_command);
		directory_spliter = strtok(NULL, ":");
	}
	free(copy_path);
	return NULL;
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
