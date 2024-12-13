#include "shells.h"

char *getline_process(void);
void strtok_process(char *input, char **argv);
void execve_process(char **argv);

int main (void)
{	
	char *input; /* input from terminal */
	char *argv[100]; /* execve arguments */
	/*int flag; */

	while (1) /* neverending loop for prompt */
	{
		printf("$ ");
		input = getline_process();
		strtok_process(input, argv);	
		execve_process(argv);
	}
	return (0);
}

/**
 * getline_process - function to read a line of text from terminal stdin
 *
 * @void : 
 * Return: char *
 */

char *getline_process(void)
{
	char *buffer = NULL;
	int nread;
	size_t length = 0;

	nread = getline(&buffer, &length, stdin); 
	if (nread == -1)
	{
		if (feof(stdin)) /* end of file */
		{
			printf("End of file /Ctrl D detected.. Exiting\n");
			exit (1);
		}
		else
		{
			perror("Error in getline\n");
			/*exit (1);*/
			/*continue;*/
		}
	}
	if (buffer[nread - 1] == '\n') /* remove newline at the end */
	{
		buffer[nread - 1] = '\0';
	}
	return (buffer);
}

/**
 * strtok - function to tokenize the input
 *
 * @input : string input from terminal
 * @argv : argument array
 * Return: void
 */

void strtok_process(char *input, char **argv)
{
	char *token;
	int count = 0;

	token = strtok(input, " "); /* start tokenizing with space as delimiter */
	while (token != NULL) 
	{	/*count = count + 1; */
		argv[count] = token; /* store token into argv[count] */
        	/*token = strtok(NULL, " ");  continue tokenizing */
		printf("Token is %s\n", token); /*debugging */
		printf("argc[%d] is %s\n", count, argv[count]); /*debugging */
    		count++;
		token  = strtok(NULL, " ");
		/*count++; */
	}
	argv[count] = NULL; /* terminate last argument */
	printf("last count is %d\n", count); /* for debugging */
	return;	
} 

/**
 * execve - function to create a child pid and execute the command
 *
 * @argv : argument array
 * Return: void
 */

void execve_process(char **argv)
{	
	extern char **environ; /* environmental variable from system */
	/*int flag; */
	int status;

	pid_t child_pid;

	child_pid = fork(); /* create a child process */
	
	if (child_pid < 0)
	{
		perror("Error in child pid");
		exit (1);
	}
	
	if (child_pid == 0) /* if 0, child_pid  */
	{	
		/*flag = execve(argv[0], argv, environ); */
		printf("Environ is %s\n", *environ); /*for debugging */
		printf("argv[0] is %s\n", argv[0]); /* for debugging */
		printf("argv[1] is %s\n", argv[1]); /* for debugging */
		/*flag = execve(argv[0], argv, NULL); */
		/*if (flag == -1)  issue with execve */
		
		if (execve(argv[0], argv, NULL) == -1)
		{
			perror("Error in execve\n");
			exit(1);
		}
	}	
	else /* parent pid is */
	{
		wait(&status); /* wait for child_pid to finish */
	}
	return;
}
