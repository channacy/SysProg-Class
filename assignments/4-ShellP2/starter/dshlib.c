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

    // TODO IMPLEMENT MAIN LOOP
    while(1){
        printf("%s", SH_PROMPT);
        cmd_buff = (char *)malloc(SH_CMD_MAX * sizeof(char));
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL)
        {
            printf("\n");
            break;
        }

        // remove the trailing \n from cmd_buff
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        // if user entered no commands/arguments
        if (cmd_buff[0] == '\0') {
            printf(CMD_WARN_NO_CMD);
            // return WARN_NO_CMDS;
        }

        // TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff

        // if user enters command and/or arguments to parse
        else {
            rc = build_cmd_buff(cmd_buff, &cmd);
            if (rc == -3) {
                printf("error: limited to %i length size for exe and %i for arg\n", EXE_MAX, ARG_MAX);
            } else if (rc == -2) {
                printf("error: limited to %i commands\n", SH_CMD_MAX);
            }
            else {
                // TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
                // the cd command should chdir to the provided directory; if no directory is provided, do nothing
                if (strcmp(cmd.argv[0], EXIT_CMD) == 0) {
                    return OK;
                } else if (strcmp(cmd.argv[0], "cd") == 0) {
                    if (cmd.argc > 1) {
                        if (chdir(cmd.argv[1]) != 0) {
                            perror("cd failed"); 
                        }
                }
                } else {
                    // TODO IMPLEMENT if not built-in command, fork/exec as an external command
                    // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"
                    pid_t pid;
                    // Fork a child process
                    if ((pid = fork()) < 0) {
                        perror("fork");
                        return -1;
                    } else if (pid == 0) { // Child process
                        // Execute the command using execvp
                        if (execvp(cmd.argv[0], &cmd.argv[0]) == -1) {
                            perror("execvp");
                            exit(EXIT_FAILURE);
                        }
                    }
                    else {
                        wait(NULL);
                    }
                }
            }
            free(cmd_buff);
        }
    }

    return OK;
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    char *start = cmd_line;
    char *cmd_copy;
    char *token;
    int in_quotes = 0;
    
    // remove leading spaces
    while (*start && isspace(*start)) {
        start++;
    }
    
    cmd_copy = strdup(start);
    if (strlen(cmd_copy) > SH_CMD_MAX) {
        return ERR_CMD_OR_ARGS_TOO_BIG;
    }
    
    // assign cmd_buffer
    cmd_buff->_cmd_buffer = cmd_copy;
    cmd_buff->argc = 0;
    
    token = cmd_copy;
    
    while (*token) {
        // skip any spaces between arguments
        while (*token && isspace(*token)) {
            token++;
        }
        
        // if token is null, exit out of loop
        if (!*token) {
            break;
        }
        
        // if current number of arguments is greater than CMD_MAX
        if (cmd_buff->argc >= CMD_MAX) {
            free(cmd_copy);
            return ERR_TOO_MANY_COMMANDS;
        }
        
        cmd_buff->argv[cmd_buff->argc] = token;
        
        // Process the argument and include space between quotes
        while (*token) {
            if (*token == '"') {
                in_quotes = !in_quotes;
                memmove(token, token + 1, strlen(token)); // remove the quote
            } else if (isspace(*token) && !in_quotes) {
                break;
            }
            token++;
        }
        
        if (*token) {
            *token++ = '\0';
        }
        cmd_buff->argc++;
    }
    return OK;
}