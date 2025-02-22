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

        // preprocess cmd_line
        preprocess_cmd(cmd_buff);

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
            } else if (rc == 0) {
                printf("PARSED COMMANDLINE - TOTAL COMMANDS %i\n", 1);
                printf("<%i>", 1);
                printf("%s", cmd.argv[0]);
                if (cmd.argc > 1) {
                    printf("[");
                    for (int i = 1; i < cmd.argc; i++) {
                        printf("%s", cmd.argv[i]);
                    }
                    printf("]");
                } 
                printf("\n");
            }
        }

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
            printf("cd entered");
        }

        // TODO IMPLEMENT if not built-in command, fork/exec as an external command
        // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"

    }

    return OK;
}

void trim_extra_spaces(char *str) {
    char *end;

    while (isspace((unsigned char)*str)) str++;

    if (*str == 0)
    return;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    *(end + 1) = 0;
}

void preprocess_cmd(char *cmd_line) {
    char temp[SH_CMD_MAX];
    int i = 0;
    int j = 0;
    int in_quotes = 0;

    trim_extra_spaces(cmd_line);

    while (cmd_line[i] != '\0' && j < SH_CMD_MAX - 1) {
        if (cmd_line[i] == '"') {
            in_quotes = !in_quotes;
            i++;
            continue;
        }

        if (!in_quotes && isspace((unsigned char)cmd_line[i])) {
            if (j > 0 && !isspace((unsigned char)temp[j-1])) {
                temp[j++] = ' ';
            }
        } else {
            temp[j++] = cmd_line[i];
        }

        i++;
    }

    temp[j] = '\0';
    strcpy(cmd_line, temp); 
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {

    char *token;

    cmd_buff->_cmd_buffer = strdup(cmd_line);
    cmd_buff->argc = 0; // set to 0 for now

    token = strtok(cmd_line, " "); // get the command

    if (strlen(token) > EXE_MAX) {
        return ERR_CMD_OR_ARGS_TOO_BIG;
    }

    while (token != NULL) {
        if (cmd_buff->argc < CMD_ARGV_MAX) {
            cmd_buff->argv[cmd_buff->argc] = token;
            cmd_buff->argc++;
        } else {
            return ERR_TOO_MANY_COMMANDS;
        }

        // get the next token
        token = strtok(NULL, " ");
    }

    return OK;

}
