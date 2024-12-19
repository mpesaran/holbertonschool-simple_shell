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

/**
 * get_path - function to populate directory value from PATH environemnt
 *
 * @head : linked list input
 * Return: linked list path_t
 */

/*path_t *get_path(path_t *head) */
path_t *get_path(void)
{
	char *path_env, *path_copy, *dir;
	/*path_t *head = NULL;  create a pointer to path_t locally */
	/*path_t *new_node = (path_t *)malloc(sizeof(path_t));  new node if head is NULL */
	/*path_t *temp;  temp path_t structure , no need for malloc as will copy from *head */
	path_t  *head = NULL;
	path_t *new_node;
	path_t *tail = NULL;

	path_env = getenv("PATH"); /* original variable from getenv */	
	if (path_env == NULL)
        {
                perror("Error in environment variable\n");
                /*free(new_node); */
		/*exit (1);*/
		/*return (-1);*/
		return (NULL);
        }
	/*path_copy = strcpy(path_env);  create local path_env for data processing */
	path_copy = strdup(path_env);
	
	/**
	 * if (path_copy == NULL)
        *{
        *       perror("Issue with path environment variable copy\n");
        *      free(new_node);
	*	return (NULL);
	*	exit (1);
	*	return (-1);
        */
	
	dir = strtok(path_copy, ":");  /*copy dir value before : delimiter */
	while (dir != NULL)
	{	
		new_node = create_list(dir); 
		/*if (head == NULL)  empty head node thus create 1st node */
		if (new_node == NULL)
		{	
			free(head);
			return (NULL);

			/*head = new_node; */
			/*head->path = dir; */
			/*head->next = NULL; */
		}
		if (head == NULL)
		{
			head = new_node;
		        tail = new_node;
			/* tail = NULL;  this will caused segmentation error and exit */	
			/* temp = head;  temp now hold *head  value to keep head safe */
			/*while (temp->next != NULL) */
				/* temp = temp->next; stroll to temp->next till its the end */
			
			/*new_node->path = dir; */
			/*new_node->next = NULL; */
			/*temp->next = new_node; */
		}
		else
		{
			tail->next = new_node;
			tail = new_node;
		}
		dir = strtok(NULL, ":");
	}
	free(path_copy); /* for valgrind */
	return (head);
}

/**
 * create_list - function to create a list of new node
 * @dir : input string
 * Return: new node;
 */ 

path_t *create_list(char *dir)
{
	path_t *new_node = (path_t *)malloc(sizeof(path_t));
	if (new_node == NULL)
	{
		perror("Error in creating node\n");
		free(new_node);
		return (NULL);
	}
	new_node->path = dir;  /* duplicate */
	new_node->next = NULL;
	return (new_node);
}
/**
 * free_path - function to free the linked list
 *
 * @head : linked list input consisting of path variable
 * Return: void
 */

void free_path(path_t *head)
{
    path_t *temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
	free(temp->path);
        free(temp);  /* Free each node */
    }
}

/**
 * matched_path - function to search if command is executable and its return path 
 * @head : linked list input consisting of path variable
 * @cmd : command to be searched
 * Return: NULL if failed else path of the command
 */
char* matched_path(path_t *head, char *cmd)
{
	/*char *dir ;  */
	/*char path[100];*/
	char *full_path = NULL;

	/*path_t *temp = head;*/
	path_t *current = head;

	/*while (head != NULL)*/
	while (current)
	{
		/*temp = head; */
		/*dir = temp->path; */
		/*sprintf(path, "%s%s", dir, cmd);  concentate both dir and cmd to path */
		full_path = (char *)malloc(strlen(current->path) + strlen(cmd) + 2); /* +2 for '/' and '\0' */
		if (full_path == NULL)
		{
			perror("Full path error \n");
			free(full_path);
			return (NULL);
		}
		
		sprintf(full_path, "%s/%s", current->path, cmd);

		if (access(full_path, X_OK) == 0) /* find the path if executable */
		{	
			/*memset(path, 0, 100);  clear path array to prevent garbage*/
			/*strcpy(path, dir);  copy dir to path */
			/*strcat(path, cmd);  concatenate cmd to path */
			
			return(full_path);
		}
		free(full_path); /* for valgrind */
		/*head = head->next; */
		current = current->next;
	}
	return (NULL);
}


