#include "shell.h"
/**
 * _exevp - custom exevp function using execve as base
 * 
 * @file : name of the file/command or path of the command
 * @argv : arrays of commands to be executed
 * Return: 0 is successful else -1
 */

int _execvp(char *file, char *argv[])
{	
	char *path_env, *path_copy, *dir;
	char path[100]; /* array of variables holding path values from env */

	path_env = getenv("PATH");

	if (path_env == NULL)
	{
		perror("Error in environment variable\n");
		return (-1);
	}
	path_copy = strdup(path_env); /* make a copy of path enviroment variable */

	if (path_copy == NULL)
	/*if (!path_copy)*/
	{
		perror("Issue with path environment variable copy\n");
		return (-1);
	}

	dir = strtok(path_copy, ":"); /* tokenize the path value to individual directory*/
	while (dir != NULL)
	{
		sprintf(path, "%s%s", dir, file); /* concentate both path and file/command */
		if (access(path, X_OK) == 0) /* check if path address to a executable file */
		{
			if (execve(path, argv, environ) == -1) /*use execve */
			{
				perror("Issue with execve\n");
				free(path_copy);
			}
		}	
		dir = strtok(NULL, ":"); /* find the next token/dir */
	}
	free(path_env);
	free(path_copy);
	free(dir);
	return (0);

}
