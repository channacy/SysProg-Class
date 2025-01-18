#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);
//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here

int setup_buff(char *buff, char *user_str, int len){
    if (!buff || len <= 0) {
        return -2;  // return -2 for any other error (add error condition)
    }
    //TODO: #4:  Implement the setup buff as per the directions
    // return 0; //for now just so the code compiles. 
    int length = 0;
    int space_flag = 0;
    while (*user_str != '\0') {
        if (length > len) {
            return -1; // length of user string is longer than available buffer size
        }
        if (*user_str == ' ' || *user_str == '\t') {
            if (!space_flag) {
                *buff = ' ';
                space_flag = 1;
                length++;
                buff++;
            }
        } 
        else {
            *buff = *user_str;
            space_flag = 0;
            length++;
            buff++;
        }
        user_str++;
    }
    
    for (int i = length + 1; i <= len; i++) {
        *buff = '.';
        buff++;
    }

    return length; //return length of user input string if there are no errors
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len) {
    // Arguments: (a)a pointer to the buffer, (b) the length of the buffer, and (c)the length of the user supplied string
    //YOU MUST IMPLEMENT
    int word_count = 0;
    int word_start = 0;
    for (int i = 0; i < str_len; i++) {
        if (!word_start)
        {
            if (!(isspace(*buff))){
                word_count++;
                word_start = 1;
            }
        }
        else {
            if (isspace(*buff)){
                word_start = 0;
            }
        }
        buff++;
    }
    
    return word_count;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    // The first condition checks to see if there is 1 or 0 command line arguments and the second condition accesses the first argument's value 
    // to see if it is equal to the character '-.' It is safe because if arv[1] did not exist and did not equal "-", both conditions would be false.
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //  The if conditional checks to see if there were at most two given command-line arguments.
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = malloc(BUFFER_SZ * sizeof(char));
    // If malloc fails
    if (buff == NULL) {
        return 99; 
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }
    printf("Length of user string %d\n", user_str_len);

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;
        case 'r':
            break;
        case 'w':
            break;
        case 'x':
            break;
        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        default:
            usage(argv[0]);
            exit(1);
    }
    print_buff(buff,BUFFER_SZ);

    //TODO:  #6 Dont forget to free your buffer before exiting
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE