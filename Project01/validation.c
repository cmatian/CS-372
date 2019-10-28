// validation Function Definitions
// Christopher Matian 10/23/2019

// Linked Header File
#include "validation.h"

/*
 * int flag = set the flag to 1 if there is an error of this type
 * char * error = string describing the error
*/
typedef struct {
    int flag;
    char * error;
} error;

// Set errors isValid for quick reset on each loop
error errors[ERROR_KEY_LENGTH] = {
        {0, "Blank handle names are not allowed."}, // 0
        {0, "The handle name can only be 10 characters long."}, // 1
        {0, "The first character cannot be a number."}, // 2
        {0, "Spaces are not allowed."}, // 3
        {0, "Special characters are not allowed."} // 4
};

int isValid(char * string) {

    int errorFlag = 0;

    // Blank handle names - exit immediately
    // EK = 0
    if(strcmp(string, "\n") == 0) {
        printf("%s", errors[0].error);
        errors[0].flag = 1;
        return printErrors();
    }

    // Handle names exceeding 10 characters
    // EK = 1
    if(strlen(string) > MAX_HANDLE_SIZE) {
        errors[1].flag = 1;
        errorFlag = 1;
    }

    // Is the first character a number?
    // EK = 2
    if(isdigit(string[0])) {
        errors[2].flag = 1;
        errorFlag = 1;
    }

    for(int i = 0; i < strlen(string) - 1; i++) {
       // Is the character a space?
       // EK = 3
       if(isspace(string[i])) {
           errors[3].flag = 1;
           errorFlag = 1;
           continue;
       }

       // Is the character not alpha?
       // EK = 4
       if(isalpha(string[i]) == 0) {
           errors[4].flag = 1;
           errorFlag = 1;
           continue;
       }
    }

    // Check error flag. 1 = error
    if(errorFlag == 1) {
        return printErrors();
    }

    // Valid Handle
    return 1;
}

int printErrors() {
    printf("Warning: There were one or more errors with your input:\n");
    for(int i = 0; i < ERROR_KEY_LENGTH; i++) {
        if(errors[i].flag == 1) {
            printf("\t- %s\n", errors[i].error);
        }
    }

    // Reset errors before exiting
    resetErrors();

    return 0;
}

void resetErrors() {
    for(int i = 0; i < ERROR_KEY_LENGTH; i++) {
        errors[i].flag = 0;
    }
}
