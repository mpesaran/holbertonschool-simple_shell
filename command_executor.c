#include "shell.h"

/* Core command execution for functionality */

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
