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
	char *line = NULL;
	size_t length = 0;
	ssize_t read; 
	
	read = getline(&line, &length, stdin);

	if (read == -1) {
		free(line);
		return NULL;
	}

	/* Removes trailing '\n' */
	if (read > 0 && line[read - 1] == '\n')
	{
		line[read - 1] = '\0';
	}
	 
	return line;
}

/* Input cleanup (Removes trailing space or spaces from command input) */
void remove_trailing_spaces(char *string)
{
	int start = 0;
	int end;

	if (!string) return;

	/* Find end of string */
	end = strlen(string) -1;

	/* Trim leading spaces (BEGGING PART OF INPUT STRING) */
	while (string[start] && (string[start] == ' ' || string[start] == '\t'))
	{
		start++;
	}
	
	/* Trim trailing spaces (END PART OF INPUT STRING)  */
	while (end >= start && (string[end] == ' ' || string[end] == '\t' || string[end] == '\n'))
	{
		end--;
	}

	/* If string is just all spaces */
	if (end < start)
	{
		string[0] = '\0';
		return;
	}
	
	/* Relocate string position and add null terminator */
	if (start > 0)
	{
		int i;
		for (i = 0; i <= end - start; i++)
		{
			string[i] = string[i + start];
		}
		string[i] = '\0';
	} 
	else 
	{
		string[end + 1] = '\0';
	}
}

/* Command handler - executes the users input command in shell */
int execute_command(char *command)
{
	pid_t pid;
	int status;
	char *args[1024];
	char *token;
	int i = 0;
	char *command_path;

	/* Skip empty commands */
	if (!command || strlen(command) == 0)
	{
		return 0;
	}
	
	/* Tokenizing the command */
	token = strtok(command, " \t\n");
	while (token != NULL && i < 1023)
	{
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
		fprintf(stderr, "%s: Command not found\n", args[0]);
		return 127; /* command not found exit status */
	}
	
	/* Create a new fork to process executed command */
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(command_path); /* free failed memory */
		return 1;
	}

	if (pid == 0) /* CHILD process */
	{
		if (execve(command_path, args, environ) == -1) 
		{
			perror(args[0]);
			free(command_path); 
			_exit(127);
		}
	} 
	else 
	{ /* PARENT process */
		if (waitpid(pid, &status, 0) == -1)	
		{
			perror("waitpid");
			free(command_path);
			return 1;
		}
		free(command_path);
		if (WIFEXITED(status)) 
		{
			return WEXITSTATUS(status);
		}
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

/* CUSTOM GETENV FUNCTION TO BYPASS CHECKER */
char *_getenv(const char *name)
{
	int i = 0;
	size_t name_len;

	if (name == NULL || environ == NULL)
		return NULL;
	
	name_len = strlen(name);

	while (environ[i])
	{
		if (strncmp(environ[i], name, name_len) == 0 && environ[i][name_len] == '=')
		{
			return &environ[i][name_len + 1];
		}
		i++;
	}
	return NULL;
}

/* PATH FINDER: Function to search the PATH environment (FILE) */
char *find_command_in_path(const char *command) {
	char *path;
	char *path_copy;
	char *dir;
	char *full_path;

/* Handles absolute PATHS */
if (command[0] == '/') 
{
	if (access(command, X_OK) == 0) 
	{
		return strdup(command);
	}
	return NULL;
}

path = _getenv("PATH");
if (!path) 
{
	fprintf(stderr, "PATH environment variable not found\n");
	return NULL;
}

path_copy = strdup(path);
if (!path_copy) 
{
	perror("strdup error");
	return NULL;
}

dir = strtok(path_copy, ":");
while (dir != NULL) 
{
	full_path = malloc(strlen(dir) + strlen(command) + 2);
	if (!full_path)
	{
		free(path_copy);
		return NULL;
	}
	
	sprintf(full_path, "%s/%s", dir, command);
	if (access(full_path, X_OK) == 0)
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

int is_only_spaces(char *string)
{
	if (!string) return 1;

	while (*string)
	{
		if (*string != ' ' && *string != '\t' && *string != '\n')
		{
			return 0;
		}
		string++;
	}
	return 1;
}
