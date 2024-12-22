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
        if (input_trail[start] == '\0')
        {
                input_trail[0] = '\0';
                return;
        }
        end = strlen(input_trail) - 1;
        while (end > start && (input_trail[end] == ' ' || input_trail[end] == '\t'
                                || input_trail[end] == '\n'))
        {
                input_trail[end] = '\0';
		end--;
	}
}
void exit_handler(char **args, path_list *paths, char *command_line, int *last_status)
{
    int exit_code = *last_status;
     if (args[1] != NULL)
    {
        exit_code = atoi(args[1]);
        if (exit_code <= -1)
        {
            fprintf(stderr, "exit: Illegal number: %s\n", args[1]);
            exit_code = 2; 
        }
    }

    free_path_list(paths);
    free(command_line);
    exit(exit_code);
}
/* Command handler - executes the users input command in shell */
int command_handler(char *command, path_list *paths, int *last_status)
{
	char *args[1024];
	char *full_path = NULL;
	char *token;
	int i = 0;
	int status = 0;

	if (!command || strlen(command) == 0)
		return (0);
	token = strtok(command, " \t");
	while (token != NULL && i < 1023)
	{
		args[i++] = token;
		token = strtok(NULL, " \t\n");
	}
	args[i] = NULL;
	if (args[0] == NULL)
	{
		fprintf(stderr, "Error: No command entered\n");
		return (-1);
	}
	if (strcmp(args[0], "env") == 0)
	{
		print_env();
		return (0);
	}
	if (strcmp(args[0], "exit") == 0)
	{
		exit_handler(args, paths, command, last_status);
	}
	if (!_getenv("PATH") || strlen(_getenv("PATH")) == 0)
    	{
        	if (strchr(args[0], '/'))
        	{
            		if (access(args[0], X_OK) == 0)
            		{
                		full_path = strdup(args[0]);
            		}
            		else
            		{
                		fprintf(stderr, "%s: No such file or directory\n", args[0]);
                		return (127);
            		}
        	}
    	}
    	else
    	{
        	full_path = find_in_path(args[0], paths);
    	}
	if (!full_path)
	{
		if (paths)
			free_path_list(paths);
		fprintf(stderr, "%s: 1: %s: not found\n", "./hsh", args[0]);
		if (command)
			free(command);
		exit(127);
	}
	/* Execute the command */
	status = execute_command(full_path, args);
	*last_status = status;	
	free(full_path);
	return (status);
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
	{       /* check for space, tab and newline */
		if (input_line[count] == ' ' || input_line[count] == '\t'
                                || input_line[count] == '\n')
                {
                        space_num++;
                }
                count++;
        }
        if (space_num == strlen(s))
        {
                return (1);/* true as input all space */
        }
        /*free(input_line);*/
        return (0);
}

/**
 * _getenv - Retrieves the value of an environment variable.
 * @name: variable name in environment
 *
 * Return: value corresponding to the provided variable name, NULL if not found
 */
char *_getenv(const char *name)
{
        size_t name_len = strlen(name);
        char **env = environ;

        while (*env)
        {
                if (strncmp(*env, name, name_len) == 0 && (*env)[name_len] == '=')
                {
                        return (*env + name_len + 1);
                }
                env++;
        }

        return (NULL);
}
