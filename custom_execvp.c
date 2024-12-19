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


path_t *get_path(path_t *head)
{
	char *path_env, *path_copy, *dir;
	/*path_t *head = NULL;  create a pointer to path_t locally */
	path_t *new_node = (path_t *)malloc(sizeof(path_t)); /* new node if head is NULL */
	path_t *temp; /* temp path_t structure , no need for malloc as will copy from *head */
	
	path_env = getenv("PATH"); /* original variable from getenv */	
	if (path_env == NULL)
        {
                perror("Error in environment variable\n");
                free(new_node);
		/*exit (1);*/
		/*return (-1);*/
		return (NULL);
        }
	/*path_copy = strcpy(path_env);  create local path_env for data processing */
	path_copy = strdup(path_env);
	
	if (path_copy == NULL)
        {
                perror("Issue with path environment variable copy\n");
                free(new_node);
		return (NULL);
		/*exit (1);*/
		/*return (-1);*/
        }
	dir = strtok(path_copy, ":"); /* copy dir value before : delimiter */
	while (dir != NULL)
	{	
		if (head == NULL) /* empty head node thus create 1st node */
		{	
			head = new_node;
			head->path = dir;
			head->next = NULL;
		}
		else
		{
			temp = head; /* temp now hold *head  value to keep head safe */
			while (temp->next != NULL)
			{
				temp = temp->next; /* stroll to temp->next till its the end */
			}
			new_node->path = dir;
			new_node->next = NULL;
			temp->next = new_node;
		}
		dir = strtok(NULL, ":");
	}
	return (temp);
}
	
