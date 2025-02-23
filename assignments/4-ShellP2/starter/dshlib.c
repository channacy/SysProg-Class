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
    int rc = 0;
    cmd_buff_t cmd;

    while (1)
    {
        // Begin by printing shell prompt "dsh2>"
        printf("%s", SH_PROMPT);
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

        if (cmd_buff[0] == '\0')
        {
            printf(CMD_WARN_NO_CMD);
        }
        else
        {
            // Parse through cmd_buff and load into cmd
            rc = build_cmd_buff(cmd_buff, &cmd);
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
                int result = match_command(cmd.argv[0]);
                if (result != 3) {
                    // execute built in command
                    int exec_result = exec_built_in_cmd(&cmd);
                    if (exec_result == -7) {
                        break;
                    }
                }
                else if (result == 3)
                {
                    // external command
                    pid_t pid;
                    if ((pid = fork()) < 0)
                    {
                        perror("fork");
                        return -1;
                    }
                    else if (pid == 0)
                    {
                        if (execvp(cmd.argv[0], &cmd.argv[0]) == -1)
                        {
                            perror("execvp");
                            exit(EXIT_FAILURE);
                        }
                    }
                    else
                    {
                        wait(NULL);
                    }
                }
            }
            free(cmd_buff);
            free_cmd_buff(&cmd);
            clear_cmd_buff(&cmd);
        }
    }

    return OK;
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff)
{
    char *start = cmd_line;
    char *cmd_copy;
    char *token;
    int in_quotes = 0;

    // Remove leading spaces
    while (*start && isspace(*start))
    {
        start++;
    }

    cmd_copy = strdup(start);
    if (strlen(cmd_copy) > SH_CMD_MAX)
    {
        return ERR_CMD_OR_ARGS_TOO_BIG;
    }

    // Assign cmd_copy to cmd_buffer and set argument count to 0
    cmd_buff->_cmd_buffer = cmd_copy;
    cmd_buff->argc = 0;

    token = cmd_copy;

    while (*token)
    {
        // Skip any spaces between arguments
        while (*token && isspace(*token))
        {
            token++;
        }
        // If token is null, exit out of loop
        if (!*token)
        {
            break;
        }
        // If current number of arguments is greater than CMD_MAX
        if (cmd_buff->argc >= CMD_MAX)
        {
            free(cmd_copy);
            return ERR_TOO_MANY_COMMANDS;
        }

        cmd_buff->argv[cmd_buff->argc] = token;
        // Process the argument and include space between quotes
        while (*token)
        {
            if (*token == '"')
            {
                in_quotes = !in_quotes;
                memmove(token, token + 1, strlen(token)); // Remove the quote
            }
            else if (isspace(*token) && !in_quotes)
            {
                break;
            }
            token++;
        }

        if (*token)
        {
            *token++ = '\0';
        }
        cmd_buff->argc++;
    }

    return OK;
}

Built_In_Cmds match_command(const char *input)
{
    if (strcmp(input, "cd") == 0)
    {
        return BI_CMD_CD;
    }
    else if (strcmp(input, "exit") == 0)
    {
        return BI_CMD_EXIT;
    }
    return BI_NOT_BI;
}

Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd) {
    if (strcmp(cmd->argv[0], "exit") == 0) {
        return OK_EXIT;
    } else if (strcmp(cmd->argv[0], "cd") == 0) {
        if (cmd->argc > 1) {
            if (chdir(cmd->argv[1]) != 0)
            {
                perror("cd failed");
            }
        }
    }
    return BI_EXECUTED;
}

int clear_cmd_buff(cmd_buff_t *cmd_buff) {
    // Clear cmd_buff arguments
    for (int i = 0; i < cmd_buff->argc; i++) {
        cmd_buff->argv[i] = NULL;
    }
    // Set argument count to 0
    cmd_buff->argc = 0;
    return OK;
}

int free_cmd_buff(cmd_buff_t *cmd_buff) {
    free(cmd_buff->_cmd_buffer);
    return OK;
}