#include "shell.h"
/**
 * build_path_list - Builds a linked list of directories from PATH environment
 * @list: Pointer to the path_list structure to populate.
*/
void build_path_list(path_list *list)
{
	char *path_env = getenv("PATH");
	char *token;
	char *path_env_copy = strdup(path_env);

	if (!path_env_copy)
		return;

	token = strtok(path_env_copy, ":");
	while (token != NULL)
	{
		path_node *new_node = malloc(sizeof(path_node));

		if (!new_node)
			exit(EXIT_FAILURE);
		new_node->directory = strdup(token);
		if (!new_node->directory)
		{
			free(new_node);
			exit(EXIT_FAILURE);
		}
		new_node->next = NULL;
		new_node->prev = list->tail;
		if (list->tail)
			list->tail->next = new_node;
		else
			list->head = new_node;

		list->tail = new_node;
		token = strtok(NULL, ":");
	}
	free(path_env_copy);
}
/**
 * find_in_path - Searches for a command in the directories from a path list.
 * @command: Command to search for.
 * @paths: Pointer to the path_list structure containing directories.
 *
 * Return: Full path to the command if found, NULL otherwise.
*/
char *find_in_path(const char *command, path_list *paths)
{
	path_node *current = paths->head;
	char *full_path = NULL;
	size_t len;

	if (command[0] == '/' || strstr(command, "./") || strstr(command, "../"))
	{
		if (access(command, X_OK) == 0)
			return (strdup(command));
		return (NULL);
	}
	while (current)
	{
		len = strlen(current->directory) + strlen(command) + 2;
		full_path = malloc(len);
		if (!full_path)
			exit(EXIT_FAILURE);

		sprintf(full_path, "%s/%s", current->directory, command);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		full_path = NULL;
		current = current->next;
	}
	return (NULL);
}
/**
 * free_path_list - Frees all nodes in the path_list linked list.
 * @list: Pointer to the path_list structure to free
*/
void free_path_list(path_list *list)
{
	path_node *current = list->head;

	while (current)
	{
		path_node *next = current->next;

		free(current->directory);
		free(current);
		current = next;
	}
	list->head = list->tail = NULL;
}
/**
 * execute_command - Executes a command using fork and execvp.
 * @path: Path to the executable command.
 * @args: Arguments to pass to the command.
 *
 * Return: Exit status of the executed command, or -1 on failure.
*/
int execute_command(char *path, char **args)
{
	pid_t PID;
	int status;

	if (!path)
	{
		fprintf(stderr, "%s: command not found\n", args[0]);
		return (127);
	}
	PID = fork();
	if (PID < 0)
	{
		perror("Failed to fork process");
		return (-1);
	}
	else if (PID == 0)
	{
		/* Execute the command in tihe child process */
		if (execvp(path, args) == -1)
		{
			perror("execve");
			exit(127);
		}
	}
	else
	{
		/* Wait for the child process in the parent process */
		do {
			waitpid(PID, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return (WEXITSTATUS(status));
}
