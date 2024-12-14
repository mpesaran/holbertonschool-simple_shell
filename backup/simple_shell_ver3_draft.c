#include "shells.h"

extern char **environ; /* environmental variable from system */

char *getline_process(void);
void strtok_process(char *input, char **argv);
void execve_process(char **argv);
void non_interactive_process(int ac, char *av[]);

int main (void)
/*int main (int argc, char *argv[], char *env[]) */
/*int main (int argc, char *argv[])*/
{	
	char *input; /* input from terminal */
	char *argv_local[1024]; /* execve arguments */
	/*int flag; */
	/*non_interactive_process(argc, argv); */

	while (1) /* neverending loop for prompt */
	{	
		if (isatty(STDIN_FILENO) != 0) 
		{	printf("($) ");
			input = getline_process();
			strtok_process(input,argv_local);	
			execve_process(argv_local);
		}
		else
		{	
			printf(" hsh ");
			input = getline_process();
			printf("Pipe detected\n"); /*debugging */
			printf("input piple is %s\n", input); /* debuggiong */
			strtok_process(input,argv_local);
                        printf("argv_local [0] is %s\n ", argv_local[0]);
			printf("argv_local [1] is %s\n", argv_local[1]);
			execve_process(argv_local);
			exit (1);
		}
	}
	return (0);
}


void non_interactive_process(int ac, char *av[])
{
	printf("ac %d, av[0] is %s\n", ac, av[0]);
	printf("Testing\n");
	if (ac < 1)
	{
		printf("Error\n");
		exit(1);
	}
	else
	{
		if (execve(av[0],av, environ) == -1)
                {
                        perror("Error in execve\n");
                        exit(1);
                }
	}

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
	if (strcmp(buffer, "exit") == 0)
	{
		printf("Exiting\n");
		exit (1);

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
	{	/* count = count + 1; */
		argv[count] = token; /* store token into argv[count] */
        	/* token = strtok(NULL, " ");  continue tokenizing */
		/* printf("Token is %s\n", token); debugging */
		/* printf("argc[%d] is %s\n", count, argv[count]); debugging */
    		count++;
		token  = strtok(NULL, " ");
		/*count++; */
	}
	argv[count] = NULL; /* terminate last argument */
	/* printf("last count is %d\n", count);  for debugging */
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
	/*extern char **environ;  environmental variable from system */
	/*int flag; */
	/*char *envList[] = {"HOME=/root", "PATH=/bin/", NULL }; user defined env */
	
	int status;
	char path_name[100];

	pid_t child_pid;

	child_pid = fork(); /* create a child process */
	
	if (child_pid < 0)
	{
		perror("Error in child pid");
		exit (1);
	}
	/*temp = strcat("/bin/",argv[0]); causing core dumped error*/
	sprintf(path_name, "../bin/%s", argv[0]); /* concentate */
	/*printf("path name is %s\n", path_name); debugging */
	if (child_pid == 0) /* if 0, child_pid  */
	{	
		/*flag = execve(argv[0], argv, environ); */
		/* printf("Environ is %s\n", *environ); for debugging */
		/* printf("argv[0] is %s\n", argv[0]);  for debugging */
		/* printf("argv[1] is %s\n", argv[1]);  for debugging */
		/*flag = execve(argv[0], argv, NULL); */
		/*if (flag == -1)  issue with execve */
		
		/* if (execve(argv[0], argv, NULL) == -1) */
		/* if (execve(argv[0], argv, envList) == -1) */
		/* if (execve(path_name, argv, NULL) == -1) */
		if (execve(path_name, argv, environ) == -1)
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