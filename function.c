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
	char *command_path = NULL;

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

	/* Normalize function if it contains '/' in path */
	if (strchr(args[0], '/') != NULL)
	{
		char *normalized = normalize_path(args[0]);
		if (normalized)
			{
				if (access(normalized, X_OK) == 0)
				{
					command_path = normalized;
				}
			else
			{
				free(normalized);
				fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
				return 127;
			}		
		}
		else 
		{
			fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
			return 127;
		}
	}
	else
	{
		command_path = find_command_in_path(args[0]);
	}

	/* Does the command exit? */
	if (!command_path)
	{
		fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
		return 127; /* Command not found status */
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
			fprintf(stderr, "./hsh: 1: %s not found\n", args[0]);
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
	char *full_path = malloc(strlen(directory) + strlen(command) + 2); /* '+2' is for '/' and '\0' */
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
		return strdup(command);
	return NULL;
}

path = _getenv("PATH");
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
while (dir != NULL) 
{
	full_path = combine_path(dir, command);
	if (!full_path)
	{
		free(path_copy);
		return NULL;
	}
	
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

/* Normalize Paths */
char *normalize_path(const char *path)
{
	char *normalized_buffer = malloc(PATH_MAX);
	char *current_component;
	char **path_components;
	int component_count = 0;
	int final_component_index;
	char *final_path;

	if (!normalized_buffer) return NULL;

	/* Copy path (cp) to work with it */
	strncpy(normalized_buffer, path, PATH_MAX - 1);
	normalized_buffer[PATH_MAX - 1] = '\0';

	/* Allocated memory for path components */
	path_components = malloc(sizeof(char*) * PATH_MAX);
	if (!path_components)
	{
		free(normalized_buffer);
		return NULL;
	}

	/* Break apart the path into components */
	current_component = strtok(normalized_buffer, "/");
	while (current_component != NULL && component_count < PATH_MAX -1)
	{
		if (strcmp(current_component, ".") == 0) 
		{
			/* Skip current directory marker '.' */
		} 
		else if (strcmp(current_component, "..") == 0)
		{ 
			/* Go up one directory level by reducing component count */	
			if (component_count > 0)
				component_count--; 
			}
			else
			{
				/* Store valid path components */
				path_components[component_count++] = current_component;
			}
			current_component = strtok(NULL, "/");
		}
	
	/* Allocate memory for final path */
	final_path = malloc(PATH_MAX);
	if (!final_path)
	{
		free(path_components);
		free(normalized_buffer);
		return NULL;
	}

	/* (FAIL SAFE OPTION) Start with root location if original path was absolute */
	if (path[0] == '/')
	{
		strcpy(final_path, "/");
	} 
	else
	{
		final_path[0] = '\0';
	}
	
	/* Rebuild path with components */
	for (final_component_index = 0; final_component_index < component_count; final_component_index++)
	{
		if (final_component_index > 0) 
		strcat(final_path, "/");
		strcat(final_path, path_components[final_component_index]);
	}
	
	/* Free allocated memory */
	free(path_components);
	free(normalized_buffer);
	return final_path;
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
