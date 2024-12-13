# Function.c Notes

#### My code with detailed notes of why I have written what i've written. The brute force code i've trialed and errored. 😅

```
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

/*
 * function_name - Does something i'm yet to code
 * @command_name: The command to execute once i've given it a name
 * Return: '0' for success and '-1' for error (failure)
 */

int command_handler (char *input)
{
        pid_t PID;
        int child_status;
        char *args[2];
        /*
         * @PID         -> Store Process ID
         * @child_status-> Store Child Process Status (Sucess vs Failure)         * @args        -> Argument array for execve (input + NULL terminator to complete the input) (0 = Input, 1 = NULL)
         */

        input[strcspn(input, "\n")] = 0;

        return 0;
}
```
