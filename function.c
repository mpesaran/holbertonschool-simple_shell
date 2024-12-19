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
char *get_user_input(void)
{
	char *input_line = NULL;
	size_t bufsize = 0;
	ssize_t characters; 
	
	characters = getline(&input_line, &bufsize, stdin);

	if (characters == -1) {
		free(input_line);
		return NULL;
	}
 
	return (input_line);
}

/* Input cleanup (Removes trailing space or spaces from command input) */
void remove_trailing_spaces(char *string)
{
	int i;
	
	if (!string)
		return;

	/* Removes trailing space (BEFORE STRING)*/
	i = strlen(string) - 1;
	while (i >= 0 && (string[i] == ' ' || string[i] == '\t' || string[i] == '\n')) {
		string[i] = '\0';
		i--;
	}
	
	/* Removes leading space (AFTER STRING) */
	i = 0;
	while (string[i] == ' ' || string[i] == '\t') {
		i++;
	}
	
	if (i > 0) {
		memmove(string, string + i, strlen(string + i) + 1);
	}	
}

/* Command handler - executes the users input command in shell */
int execute_command(char *command)
{
	pid_t pid;
	int status;
	char *args[1024];
	int i = 0;
	char *token;
	char *command_path;
	
	if (!command || strlen(command) == 0)
	{
		return 0;
	}

	/* Tokenizing the command */
	token = strtok(command, " \t\n");
	while (token != NULL && i < 1023) {
        args[i++] = token;
        token = strtok(NULL, " \t\n");
	}
	args[i] = NULL; 
	
	/* No command entered do this */
	if (args[0] == NULL)
	{
		return 0;
	}

	/* Find command in PATH */
	command_path = find_command_in_path(args[0]);
	if (!command_path)
	{
		fprintf(stderr, "%s: Command not found", args[0]);
		return 127; /* command not found exit status */
	}
	
	/* Create a new fork to process executed command */
	pid = fork();
	if (pid < 0)
	{
		perror("Failed to fork process");
		free(command_path); /* free failed memory */
		return 1;
	}
	if (pid == 0) /* CHILD process */
	{
		if (execve(command_path, args, environ)== -1) {
				perror(args[0]);
				free(command_path); 
				exit(127);
		}
	} else { /* PARENT process */
		waitpid(pid, &status, 0);
		free(command_path);

		if (WIFEXITED(status)) {
			return WEXITSTATUS(status);
		}
		return 1;		
	}
	return 0;
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
	char *path;
	char *path_copy;
	char *dir;
	char *full_path;
	struct stat buffer;

/* IF command contains '/' execute as direct PATH */
if (strchr(command, '/') != NULL)
{
	if (stat(command, &buffer) == 0)
	{
		return strdup(command);
	}
	return NULL;
}

path = getenv("PATH");
if (!path)
{
	return NULL;
}

path_copy = strdup(path);
if (!path_copy)
{
	return NULL;
}

dir = strtok(path_copy, ":");
while (dir)
{
	full_path = malloc(strlen(dir) + strlen(command) + 2);
	if (!full_path)
	{
		free(path_copy);
		return NULL;
	}
	sprintf(full_path, "%s/%s", dir, command);

	if (stat(full_path, &buffer) == 0 && (buffer.st_mode & S_IXUSR))
	{
		free(path_copy);
		return full_path;
	}
	
	free(full_path);
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

int is_only_spaces(char *string) {
	while (*string)
	{
		if (*string != ' ' && *string != '\t' && *string != '\n') {
			return 0;
		}
		string++;
	}
	return 1;
}
