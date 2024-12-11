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

