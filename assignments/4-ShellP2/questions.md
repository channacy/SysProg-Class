1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  Using`fork/excevp` allows the copy of the current process to run alongside the original process in comparison to `execvp` which replaces the current process with the new program. The value of `fork` is provided when a separate child process needs to be created and assigns that process a process ID to help manage it.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  The main process prints out an error messaging starting with the prefix "fork" and the shell continues to print the next shell prompt while awaiting the next input command.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**: The execvp() function is given the command and arguments and executes the command by searching through the directories listed in the "PATH" environment variable.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  It allows the parent process to wait for the child process to terminate so if the child process sends an exit status, the parent process can clean up any resources needed. Without calling the wait() function, the child process can become a zombie process and not officially exit from the system.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**: The function WEXITSTATUS() stores information about how the process was terminated via the exit status of a child process and is important because it can provide error handling and allow the parent to perform different actions depending on how the child exited.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  The implementation of build_cmd_buff() handles quoted arguments by going through the command line input (skipping trailing spaces) and checking to see if the token (result of splitting the command line input) is a single quote and also checks if the quote is the leading or ending quote. If a quote is encountered then the in_quotes variable is set to 1 (to represent it is looping through characters inside a quote) and the quote itself is not included in the cmd_buff's argv list. On the other hand, if the character is a space and it is not part of a quote, then break out of the loop. Then, that invalid space character is not considered and a \0 would be assigned to mark the end of processing the token. The next token is then traversed and if it is valid, then it is added to cmd_buff's argv. This was necessary to ensure correct spacing was included in the arguments especially when the input is an echo command with a specific string that has to be output accurately.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**: In this assignment, only one command was being parsed at a time instead of piped commands. Moreover, commands had to be classified as either internal or external through the user of functions that would return the result of the comamnd type. Based on the command type, another function named exec_built_in_cmd() would execute it. In comparison, the last assignment only involved parsing and printing out the comamnd and its arguments. There were a lot of challenges with revising build_cmd_buff() by preserving space within a set of characters surrounded by quotes. This involved determining whether a space is a space outside of quotes or not. 

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  A signal is a lightweight, asynchronous mehod for IPC and notifies a process about an action or event thus serving as fast simple notifcations. Signals can be used to handle user input, errors, or termination of a program so the process can react immediately to these actions.  Other IPC methods involve more complex data structures such as pipes, shared memory, message quotes that cannot always respond as fast to an event. 

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  SIGKILL is a signal used to force a process to terminate right away without any cleanup as a way to respond to any important issues such as an unresponsive process. SIGTERM is also a signal that terminates a process but allows it to clean up resources before exiting. SIGINIT is a signal that is called when the user enters CRTL+C and causes the process to stop right away with the option to perform cleanup of resources or tasks.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  When a process receives SIGSTOP, it signal stops and pauses the process. It cannot be caught, blocked, or ignored as it is designed to not be handled because SIGSTOP's purpose is to suspend the process and then resume later with a SIGCONT signal. This differs from SIGINIT which can be caught and handled.
