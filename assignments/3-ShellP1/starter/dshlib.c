#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist)
{   
    clist->num = 0;
    char *cmd_line_copy = strdup(cmd_line);
    char *saveptr1;
    char *saveptr2;
    char* str = strtok_r(cmd_line_copy, "|", &saveptr1);

    while (str != NULL) {
        // trim leading and trailing spaces
        int l = 0;
        int r = strlen(str) - 1;

        // Trim leading spaces
        while (isspace((unsigned char)str[l])) l++;

        // Trim trailing spaces
        while (r >= 0 && isspace((unsigned char)str[r])) r--;

        // Create trimmed string
        for (int index = l; index <= r; index++) {
            str[index - l] = str[index];
        }

        str[r - l + 1] = '\0';

        // parse through command
        command_t input_command =  { .exe = "", .args = "" };

        char *cmd = strtok_r(str, " ", &saveptr2);
        if (cmd != NULL) {
            if (strlen(cmd) + 1 <= EXE_MAX) {
                strcpy(input_command.exe, cmd);
            }
            else {
                free(cmd_line_copy);
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }
        }

        char *arg = strtok_r(NULL, " ", &saveptr2);
        while (arg != NULL) {
            if (strlen(arg) + strlen(input_command.args) <= ARG_MAX) {
                strcat(input_command.args, arg); 
            } else {
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }
            arg = strtok_r(NULL, " ", &saveptr2);
        }

        if (clist->num < CMD_MAX) {
            clist->commands[clist->num++] = input_command;
        } else {
            free(cmd_line_copy);
            return ERR_TOO_MANY_COMMANDS;
        }

        str = strtok_r(NULL, "|", &saveptr1);
    }

    free(cmd_line_copy);
    return OK;
}