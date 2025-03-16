# Simple Shell

## Overview
This project is a simple command-line shell implemented in C. It can execute system commands, handle built-in commands like `cd` and `pwd`, and parse user input efficiently. The shell provides a basic interactive interface for running commands similar to the Unix/Linux terminal.

## Features
- Executes system commands using `execvp()`
- Supports built-in commands:
  - `cd <directory>`: Changes the working directory
  - `pwd`: Prints the current working directory
- Handles multiple arguments for commands
- Implements a simple command parsing system
- Provides a user-friendly prompt (`$ `)

## How It Works
1. The shell continuously reads user input.
2. It parses the input into tokens (command and arguments).
3. If the command is built-in (`cd`, `pwd`), it executes the appropriate function.
4. If the command is an external program, the shell forks a child process and uses `execvp()` to execute it.
5. The parent process waits for the child process to complete before prompting for the next command.

## Code Structure
- **`parse_input()`**: Tokenizes the user input into an array of arguments.
- **`eval_commands()`**: Executes system commands using `fork()` and `execvp()`.
- **`builtin_impl_cd()`**: Implements the `cd` command.
- **`builtin_impl_pwd()`**: Implements the `pwd` command.
- **`refresh_cwd()`**: Updates the current working directory.
- **`is_builtin()`**: Checks if a command is a built-in command.
- **`execute_builtin()`**: Calls the appropriate function for built-in commands.
- **`main()`**: Runs the main shell loop, handling user input and command execution.

## Compilation & Execution
### **Compiling the Shell**
To compile the shell, use:
```sh
gcc shell.c -o shell
```

### **Running the Shell**
To start the shell, run:
```sh
./shell
```

### **Using the Shell**
- Run system commands like `ls`, `echo`, `cat`, etc.
- Use `cd <directory>` to change directories.
- Use `pwd` to print the current directory.
- Use `Ctrl+D` or `Ctrl+C` to exit.

## Example Usage
```sh
$ pwd
/home/user
$ ls
(total output)
$ cd Documents
$ pwd
/home/user/Documents
$ echo "Hello, World!"
Hello, World!
$ exit
```

## Future Enhancements
- Add support for I/O redirection (`>`, `<`)
- Implement piping (`|`)
- Support background execution (`&`)
- Improve error handling and signal handling

