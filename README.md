# Simple Shell Project

## Description 

This project is a simple UNIX command line interpreter implemented in C language.\
Goal to create a basic shell that can execute commands, handle arguments from the command line\
Perform fundamental shell functions.

## Features

- Dsiplays a command prompt
- Executes basic command-line instructions
- Handles `PATH` resolutions
- Supports built-in commands like `exit` and `env`
- Work in interavtive and non-interative mode.

## Requirements

- Ubuntu 20.04 LTS +
- GCC Command with specific flags
- Follow Betty coding Style

## Compilation

```
gcc -Wall -Werror -Wextra -pedantic - std=gnu89 *.c -o hsh
```

## Usage

### Interactive Mode

```
$ ./hsh
($) /bin/ls
file_1 file_2 file_3
($) exit
```

### Non-Interactive Mode
```
$ echo "/bin/ls" | ./hsh
file_1 file_2 file_3
```

## Files

- `main.c`: Main shell program file (Entry Point)
- `shell.h`: Header file with list of function prototypes
- `prompt.c`: Prompt handling functions
- `execute.c`: Command execution logic

## Authors

See AUTHORS file for list of contributors.

## Limitations

- Does not support advanced shell features
- limited to custom built-in command support
- Minimal error handling

## Learning Objectives

- Understand process creation and management
- Implement basic system calls
- Practice Basic C programming
- Learn shell internals
