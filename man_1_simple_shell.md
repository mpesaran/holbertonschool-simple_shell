# Name
hsh - a simple Unix shell with basic features and built-in commands


# Synopsis
```
./hsh
```

# Description
hsh is a basic Unix shell that reads and executes user commands. 
It handles simple commands, supports arguments, and can run executables from directories in the $PATH environment variable. 
The shell prompts with `$` and executes each command in a new process. 
If a command does not exist, it prints an error and waits for a new command. 


# Built-in commands include:
- `exit`: Exits the shell.
- `env`: Displays the current environment variables.


# Features
- Supports commands with arguments (e.g., `ls -l`, `echo "Hello"`).
- Uses `fork()` and `execve()` to run commands.
- Handles the `PATH` environment variable to find executables.
- Implements built-in commands (`exit`, `env`).
- Handles errors when a command doesn't exist or fails to execute.
- Displays a prompt (`$`) after executing a command.
- Supports the Ctrl+D to exit gracefully.
- Handles multiple commands with arguments.
- Performs checks before calling `fork()` to ensure the command exists.


# Exit Status
0: If the command is successfully executed.
1: If an error occurs, such as an invalid command or an execution failure.


# Examples
To start the shell, run:
```       
$ ./hsh
```
#### Example of a valid command with arguments:
```       
$ ls -l
$ echo "Hello World"
```
#### Example of using a built-in command:
```       
$ env
$ exit
```

Pressing Ctrl+D will exit the shell.


# Authors
Developed by Mahsa, Mao, and Okky.

# SEE ALSO
sh(1), bash(1), env(1), exit(1)

