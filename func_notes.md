# Function.c Notes

#### My code with detailed notes of why I have written what i've written. The brute force code i've trialed and errored. 😅

```C++
void print_prompt(void) 
{
	if (isatty(STDIN_FILENO))
	{
		printf("simple_shell$ ");
		fflush(stdout);
	}
}

// isatty       => checks the type of input source (text vs file input)
// STDIN        => standard INPUT represents the input from the users keyboard whilst running in a terminal. 
// fflush       => This function will "flush" the buffer and print immediately the data thats stored to its destination eg. 'printf'
```

```C++
char *read_input(void) 
{
	char *input_line = NULL;
	size_t buffer_length = 0;

	ssize_t read_command = getline(&input_line, &buffer_length, stdin);

	if (read_command == -1)
	{
		free(input_line);
		return NULL;
	}

	return input_line;
}

// *input_line          => pointer to character array (aka string) once passed through 'getline' it will dynamically be allocated
// buffer_length        => initial size of the buffer used to store 'input_line'. Once passed through 'getline' it will allocate memory as needed.
// getline              => function with parameters ('input_line' 'buffer_length' 'stdin') which basically reads the line of input from the standard input (stdin)
// read_command         => if value is positive integer then SUCCESS if negative integer then FAILURE. [0 = SUCCESS] [-1 = FAILURE]
// free                 => if FAILURE then free memory to prevent memory leak
```


```C++
void trailing_input(char *input_trail) 
{
	if (input_trail)
	{
		input_trail[strcspn(input_trail, "\n")] = 0;
	}
}

// *input_trail                         => is the array of characters (the string aka the command the user inputs into shell)
// strcspn(input, "\n")                 => calculates (locates in the array) where the "\n" is located
// input[strcspn(input, "\n")] = 0;     => replaces the "\n" with '\0' (= 0), which marks the end of the input

```


```C++
int command_handler(char *command) 
{
	pid_t PID;
	int status;
	char *args[2];
	extern char **environ;

	// Preparation arguments for EXECVE
	args[0] = command;
	args[1] = NULL;

	if (!command || strlen(command) == 0)
	{
		return 0;
	}
        // IF command pointer is NULL or string length (strlen) is empty then return 0 (== 0). which means no command was processed. This handles invalid inputs

	PID = fork();

	if (PID < 0)
	{
		perror("Failed to fork process");
		return -1;
	}
        // If PID is less than '0' then print error message(perror)
	else if (PID == 0)
	{
		if (execve(command, args, environ) == -1)
		{
			fprintf(stderr, "%s: Command not found in PATH\n", command);
			exit(EXIT_FAILURE);
		}
                // execve(command, args, environ): executes specified *command and args and environment variables (if any)
                // if successful it will not return the function
                // if failure  (== -1) it will return -1 and EXIT. 
	}
		else
		{
			do
			{
				waitpid(PID, &status, WUNTRACED);
			} while (!WIFEXITED(status) && !WIFSIGNALED(status));
                // waitpid      => waits for child process with PID (stores in PID variable)
                // updates the status variable with childs exit status or signal info. 
                // while loop   => condition indicates that either status being exit or signal termination will be TRUE. 
		}
		
	return 0;
        // once child process finishes to return function. 
}

```