#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);
//prototypes for functions to handle required functionality
int  count_words(char *, int);
//add additional prototypes here
void reverse_string(char *, int);
void word_print(char *, int);

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
    printf("Buffer: [");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    printf("]");
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int str_len) {
    // Arguments: (a)a pointer to the buffer, (b) the length of the buffer, and (c)the length of the user supplied string
    //YOU MUST IMPLEMENT
    int word_count = 0;
    bool word_start = 0;
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
void reverse_string(char *buff, int str_len) {
    char* start = buff;
    char* end = buff + str_len - 1;

    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;

        start++;
        end--;
    }
}

void  word_print(char *buff, int str_len) {
    int last_char_idx = str_len - 1;  //index of last char - strlen(str)-1;
    int wc = 0; //counts word
    int wlen = 0;       //length of current word
    bool word_start = 0;  //am I at the start of a new word
    for (int i = 0; i < str_len; i++)
    {
        if (!word_start)
        {
            if (isspace(*buff)) {
                printf(" (%d)\n", wlen);
                word_start = 0;
                wlen = 0;
            }
            else {
                wc++;
                word_start = 1;
                wlen++;
                printf("%d. ", wc);
                printf("%c", *buff); 
            }
        }
        else if (i == last_char_idx) {
            wlen++;
            printf("%c", *buff); 
            printf(" (%d)\n", wlen);
            word_start = 0;
            wlen = 0;
        }
        else {
            if (isspace(*buff)) {
                printf(" (%d)\n", wlen);
                word_start = 0;
                wlen = 0;
            }
            else {
                printf("%c", *buff); 
                wlen++;
            }
        }
        buff++;
    }
}

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

    switch (opt){
        case 'c':
            rc = count_words(buff, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;
        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        case 'r':
            reverse_string(buff, user_str_len);
            // printf("Reversed string: %s\n", buff);
            break;
        case 'w':
            printf("Word Print\n----------\n");
            word_print(buff, user_str_len);
            break;
        case 'x':
            break;
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
//          It is good software engineering practice to pass in the pointer to memory and 
//          how big the self contained part of memory is. This can assist in help avoid issues such
//          as memory leaks in which memory is not freed when it is not needed any longer. Being able to 
//          access the data in the buffer allows for efficient adding and modification of data without 
//          having to create copies as well as being able to reuse the same buffer.