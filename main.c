#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


/**
* main - Entry point for simple shell
* Return: Always '0'
*/
int main(void)
{
	char *line = NULL;
	size_t line_length = 0;
	ssize_t char_count;
	/*
	 * @line	-> Pointer to store input line/text
	 * @line_length 	-> Dynamic memory allocation (auto allocates memory for @line input)
	 * @char_count	-> Number of characters reda by 'getline'
	 */

	char_count = getline(&line, &line_length, stdin);
	
	return 0;
}
