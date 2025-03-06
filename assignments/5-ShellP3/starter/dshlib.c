#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 *
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 *
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 *
 *   Also, use the constants in the dshlib.h in this code.
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 *
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 *
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 *
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int exec_local_cmd_loop()
{
    char *cmd_buff;
    command_list_t clist;
    int rc = 0;

    while (1)
    {
        printf("%s", SH_PROMPT);
        // Read user commmand input into cmd_buff before parsing
        cmd_buff = (char *)malloc(SH_CMD_MAX * sizeof(char));
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL)
        {
            printf("\n");
            break;
        }

        // Remove the trailing newline and spaces from cmd_buff
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        char *end = cmd_buff + strlen(cmd_buff) - 1;
        while (end >= cmd_buff && isspace(*end))
        {
            *end = '\0';
            end--;
        }

        // If no command is provided, then print warning
        if (cmd_buff[0] == '\0')
        {
            printf(CMD_WARN_NO_CMD);
        }
        else
        {
            // Parse through cmd_buff and load into clist, a list of commands
            rc = build_cmd_list(cmd_buff, &clist);
            if (rc == -3)
            {
                printf("error: limited to %i length size for exe and %i for arg\n", EXE_MAX, ARG_MAX);
            }
            else if (rc == -2)
            {
                printf("error: limited to %i commands\n", SH_CMD_MAX);
            }
            else
            {
                // Get the first command
                cmd_buff_t *current_cmd = &clist.commands[0];
                char *command = current_cmd->argv[0];

                int result = match_command(command);
                if (result != 3)
                {
                    // execute built in command
                    int exec_result = exec_built_in_cmd(&clist.commands[0]);
                    if (exec_result == -7)
                    {
                        printf("exiting...\n");
                        return OK;
                    }
                }
                else if (result == 3)
                {
                    if (strcmp("|", command) == 0) {
                        printf("bash: syntax error near unexpected token `|'");
                    }
                    // If external command does not have pipes, then just execute that one command
                    else if (clist.num == 1)
                    {
                        exec_cmd(&clist.commands[0]);
                    }
                    else
                    {
                        // Execute external commands with pipes
                        execute_pipeline(&clist);
                    }
                }
            }
            
            free_cmd_list(&clist);
            free(cmd_buff);
        }
    }

    return OK;
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff)
{
    char *start = cmd_line;
    char *cmd_copy;
    char *arg_token;
    int in_quotes = 0;

    // Initalize fields in cmd_buff
    memset(cmd_buff, 0, sizeof(cmd_buff_t));

    // Remove any leading spaces
    while (*start && isspace(*start))
    {
        start++;
    }

    // Get the length of the command and make sure it does not exceed SH_CMD_MAX length
    if (strlen(start) > SH_CMD_MAX)
    {
        return ERR_CMD_OR_ARGS_TOO_BIG;
    }

    // Create a copy of the command using strdup
    cmd_copy = strdup(start);
    if (cmd_copy == NULL)
    {
        return ERR_MEMORY;
    }

    // Initialize cmd_buff and set argc (argument count) to 0
    cmd_buff->_cmd_buffer = cmd_copy;
    cmd_buff->argc = 0;

    // Parse arguments
    arg_token = cmd_copy;

    // Find and process each argument
    while (*arg_token)
    {
        // Skip leading spaces
        while (*arg_token && isspace(*arg_token))
        {
            arg_token++;
        }

        // Break if no more arguments
        if (!*arg_token)
        {
            break;
        }

        // Check argument count
        if (cmd_buff->argc >= CMD_ARGV_MAX)
        {
            free(cmd_copy);
            return ERR_TOO_MANY_COMMANDS;
        }

        // Store start of argument
        char *arg_start = arg_token;

        // Process argument, handling spaces in quotes and not storing actual quotes
        while (*arg_token)
        {
            if (*arg_token == '"' || *arg_token == '\'')
            {
                // Remove quote by shifting
                memmove(arg_token, arg_token + 1, strlen(arg_token));
                in_quotes = !in_quotes;
                continue;
            }

            // End of argument if character is a space and not in quotes
            if (isspace(*arg_token) && !in_quotes)
            {
                break;
            }

            arg_token++;
        }

        // Null-terminate the argument
        if (*arg_token)
        {
            *arg_token++ = '\0';
        }

        // Store argument if it is not empty and has at least one character
        if (strlen(arg_start) > 0)
        {
            cmd_buff->argv[cmd_buff->argc++] = arg_start;
        }
    }

    return OK;
}

int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    char *start = cmd_line;
    char *cmd_token;
    int cmd_index = 0;

    // Remove leading spaces
    while (*start && isspace(*start))
    {
        start++;
    }

    // Reset command list number to 0
    clist->num = 0;

    // Split commands by pipe character '|'
    cmd_token = strtok(start, "|");
    while (cmd_token != NULL && cmd_index < CMD_MAX)
    {
        // Trim leading and trailing spaces from command
        while (*cmd_token && isspace(*cmd_token))
        {
            cmd_token++;
        }
        char *end = cmd_token + strlen(cmd_token) - 1;
        while (end > cmd_token && isspace(*end))
        {
            *end = '\0';
            end--;
        }

        // Build command buffer for this command
        int result = build_cmd_buff(cmd_token, &clist->commands[cmd_index]);
        if (result != OK)
        {
            return result;
        }

        // Move to next command
        cmd_index++;
        clist->num = cmd_index;

        // Get next command using pipe character "|" as delimiter again
        cmd_token = strtok(NULL, "|");
    }

    return OK;
}

Built_In_Cmds match_command(const char *input)
{
    // If command is "cd"
    if (strcmp(input, "cd") == 0)
    {
        return BI_CMD_CD;
    }
    // If command is "exit"
    else if (strcmp(input, "exit") == 0)
    {
        return BI_CMD_EXIT;
    }
    return BI_NOT_BI;
}

Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd)
{
    // For now, supports built-in commands: "exit", "cd"
    if (strcmp(cmd->argv[0], "exit") == 0)
    {
        return OK_EXIT;
    }
    else if (strcmp(cmd->argv[0], "cd") == 0)
    {
        if (cmd->argc > 1)
        {
            if (chdir(cmd->argv[1]) != 0)
            {
                perror("cd failed");
            }
        }
    }
    return BI_EXECUTED;
}

int clear_cmd_buff(cmd_buff_t *cmd_buff)
{
    // Clear cmd_buff arguments
    for (int i = 0; i < cmd_buff->argc; i++)
    {
        cmd_buff->argv[i] = NULL;
    }
    // Set argument count to 0
    cmd_buff->argc = 0;
    return OK;
}

int free_cmd_buff(cmd_buff_t *cmd_buff)
{
    free(cmd_buff->_cmd_buffer);
    return OK;
}

int free_cmd_list(command_list_t *cmd_lst)
{
    // go through each command in cmd_list->commands
    for (int i = 0; i < cmd_lst->num; i++)
    {
        // reset argument count
        cmd_lst->commands[i].argc = 0;
        // clear list of arguments + command
        for (int j = 0; j < CMD_ARGV_MAX; j++)
        {
            cmd_lst->commands[i].argv[j] = NULL;
        }
        // clear cmd_buffer
        if (cmd_lst->commands[i]._cmd_buffer != NULL)
        {
            free_cmd_buff(&cmd_lst->commands[i]);
            clear_cmd_buff(&cmd_lst->commands[i]);
            cmd_lst->commands[i]._cmd_buffer = NULL;
        }
    }
    // reset cmd_list number of commands
    cmd_lst->num = 0;
    return OK;
}

int exec_cmd(cmd_buff_t *cmd)
{
    // execute external command
    pid_t pid;
    if ((pid = fork()) < 0)
    {
        // error with creating child process
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        // if fork was sucessful
        if (execvp(cmd->argv[0], &cmd->argv[0]) == -1)
        {
            // if cannot execute external command, return error code
            perror("execvp");
            // terminate child process with ERR_EXEC_CMD = -6 error code
            exit(ERR_EXEC_CMD);
        }
    }
    else {
        wait(NULL);
    }

    return OK;
}

int execute_pipeline(command_list_t *clist)
{
    // Initialize pipe file descriptors for every pair of commands
    int pipe_fds[2 * (clist->num - 1)];
    pid_t pids[clist->num];

    // Create all pipes needed for pairs of commands
    for (int i = 0; i < clist->num - 1; i++)
    {
        if (pipe(pipe_fds + i * 2) == -1)
        {
            perror("pipe");
            return ERR_EXEC_CMD;
        }
    }

    // Execute each command in the pipeline
    for (int i = 0; i < clist->num; i++)
    {
        // Create a fork for each child process
        if ((pids[i] = fork()) < 0)
        {
            perror("fork");
            return ERR_EXEC_CMD;
        }
        else if (pids[i] == 0)
        {
            // If not the first command running for child process, set up stdin from previous pipe
            if (i > 0)
            {
                if (dup2(pipe_fds[(i - 1) * 2], STDIN_FILENO) == -1)
                {
                    perror("dup2 stdin");
                    exit(ERR_EXEC_CMD);
                }
            }

            // If not the last command, then set up stdout to next pipe 
            if (i < clist->num - 1)
            {
                if (dup2(pipe_fds[i * 2 + 1], STDOUT_FILENO) == -1)
                {
                    perror("dup2 stdout");
                    exit(ERR_EXEC_CMD);
                }
            }

            // Close all the pipe file descriptors in child process
            for (int j = 0; j < 2 * (clist->num - 1); j++)
            {
                close(pipe_fds[j]);
            }

            // Execute the command
            if (execvp(clist->commands[i].argv[0], clist->commands[i].argv) == -1)
            {
                perror("execvp");
                exit(ERR_EXEC_CMD);
            }
        }
        else {
            if (i > 0) {
                close(pipe_fds[(i-1) * 2]);
            }
            if (i < clist->num - 1) {
                close(pipe_fds[i * 2 + 1]);
            }
        }
    }

    // parent process closes all the pipe file descriptors
    for (int i = 0; i < 2 * (clist->num - 1); i++)
    {
        if (fcntl(pipe_fds[i], F_GETFD) != -1) {
            close(pipe_fds[i]);
        }
    }

    // Wait for all the child processes to complete
    for (int i = 0; i < clist->num; i++)
    {
        waitpid(pids[i], NULL, 0);
    }

    return OK;
}