#include "shells.h"

int main (void)
{	
	char input[120];

	while (1) /* neverending loop for prompt */
	{
		printf("$ ");
		input = getline_process();
		strtok();	
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
	
	return (buffer);
}

