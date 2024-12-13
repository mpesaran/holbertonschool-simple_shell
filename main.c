#include "shell.h"

// Function Prototype
void print_prompt(void) {
	if (isatty(STDIN_FILENO)){
	printf("simple_shell$ ");
	fflush(stdout);
	}
}

/**
* main - Entry point for simple shell
* Return: Always '0'
*/
int main(void) {
	char *line;

	while (1)
	{
		print_prompt();
	}
	
	return 0;
}
