#include "shells.h"

int main (void)
{	
	char *input; /* input from terminal */
	char *argv[100]; /* execve arguments */

	while (1) /* neverending loop for prompt */
	{
		printf("$ ");
		input = getline_process();
		strtok_process();	

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
			continue;
		}
	}
	if (buffer[nread - 1] == '\n') /* remove newline at the end */
	{
		buffer[nread - 1] == '\0';
	}
	return (buffer);
}

void strtok_process(char *input, char **argv)
{
	char *token;
	int count = 0;

	token = strtok(input, " "); /* start tokenizing with space as delimiter */
	while (token != NULL) 
	{	count = count + 1;
        	argv[count] = token; /* store token into argv[count] */
        	token = strtok(NULL, " "); /* continue tokenizing 
    	}

} 
