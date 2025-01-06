# Simple Shell Notes

## main.c
#### Purpose: Entry point for the simple shell program.

`main` function:
- Initializes the `paths` linked list to hold the directories in the `PATH` environment variable.
- Continuously reads commands from the user, processes them, and executes them in a loop.
- Exits gracefully when the user signals an end-of-file (EOF) or when a `NULL` command is read.

#### Key Functions:

- `isatty(STDIN_FILENO)`: Checks if the input is from an interactive terminal.
- `print_prompt()`: Displays the shell prompt when in interactive mode.
- `read_command()`: Reads user input.
- `is_whitespace(command_line)`: Checks if the input is only whitespace.
- `command_handler(command_line, &paths, &status)`: Processes and executes the command.
- Frees allocated memory (`command_line`, `paths`) before exiting.

## function.c
#### Purpose: Core functionality for handling commands.

1. `rint_prompt`:
- Displays the prompt (simple_shell$ ) in interactive mode.
2. `read_command`:
- Reads a line of input using getline. If EOF is encountered, returns NULL.
3. `clean_command`:
- Trims leading and trailing whitespace from the command input.
4. `command_handler`:
- Parses and executes the user's command:
    - Splits the command into arguments.
    - Checks for built-in commands (exit, env) using handle_builtin.
    - Resolves the command's path and executes it using execute_command.
5. `is_whitespace`:
- Returns `1` if the input string contains only whitespace, otherwise 0.

## helper_function.c
#### Purpose: Handles built-in commands and utility functions.

1. `print_env`:
- Prints the shell's environment variables.
2. `_getenv`:
- Retrieves the value of a given environment variable.
3. `exit_shell`:
- Exits the shell, freeing resources. Optionally takes an exit code as an argument.
4. `handle_builtin`:
- Executes built-in commands (exit or env). Returns 0 on success, -1 otherwise.
5. `resolve_command_path`:
- Searches for the executable in the PATH environment variable. If not found, handles absolute or relative paths directly.

## path_functions.c
#### Purpose: Manages the linked list of directories in the PATH.

1. `append_path_node`:
- Adds a new directory to the paths linked list.
2. `build_path_list`:
- Parses the PATH environment variable into a linked list of directories.
3. `find_in_path`:
- Searches for an executable in the directories from the PATH list.
4. `free_path_list`:
- Frees all nodes in the paths linked list.
5. `execute_command`:
- Creates a child process using fork and executes a command with execvp. The parent process waits for the child to finish.

## shell.h
#### Purpose: Contains shared definitions and prototypes for the shell.

1. **Structures**:
- `path_node`: Represents a node in the linked list storing a directory.
- `path_list`: Stores pointers to the head and tail of the linked list.
2. **Prototypes**:
- Defines all functions used across the shell program.

## Flow of Execution:
1. **Startup**:
- `build_path_list` initializes the paths linked list with directories from the PATH environment variable.
2. **Shell Loop**:
- Prints the prompt (`print_prompt`).
- Reads user input (`read_command`).
- Cleans up input (`clean_command`) and ignores empty or whitespace-only commands (`is_whitespace`).
3. **Command Processing**:
- Built-in commands (exit, env) are handled by handle_builtin.
- For other commands, resolve_command_path determines the executable path.
- The command is executed using execute_command.
4. **Termination**:
- Frees allocated memory and exits with the last command's status.ain.c
Purpose: Entry point for the simple shell program.

