#include "shell.h"

/* Functions dealing with PATH manipulation and searching */

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

/* Connects directory path and command name into a string with '/' */
char *combine_path(const char *directory, const char *command) 
{	
	char *full_path = malloc(strlen(directory) + strlen(command) + 2); 
	/* '+2' is for '/' and '\0' */
	if (!full_path) /* Memory allocation checker */
	{
		perror("malloc error");
		exit(EXIT_FAILURE);
	}
	sprintf(full_path, "%s/%s", directory, command);
	return full_path;
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
