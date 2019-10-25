// validation.c Function Definitions
// Christopher Matian 10/23/2019

// Project Header Files
#include "validation.h"

/*
 * Was originally a key-value pair struct.
 * Just storing strings for now until I decide to optimize it further.
*/
typedef struct {
    char * error;
} error;

error errors[4] = {
        {"Blank handles are not allowed."},
        {"The first character cannot be a number."},
        {"spaces are not allowed."},
        {"special characters are not allowed."}
};

int isValid(char * string) {
    int errorFlag = 0;
    // 0 = No Error
    // 1 = Error
    int errorKeyLength = 4;
    int errorKey[] = {
        0, 0, 0, 0
    };
    memset(errorKey, 0, errorKeyLength*sizeof(int));

    // Is the input blank? - error key 0
    printf("%lu\n", strlen(string));

    if(strlen(string) - 1 == 0) {
        errorKey[0] = 1;
        errorFlag = 1;
    }

    // Is the first character a number? - error key 1
    if(isdigit(string[0])) {
        errorKey[1] = 1;
        errorFlag = 1;
    }

   for(int i = 0; i < strlen(string) - 1; i++) {
       // Is the character a space? - error key 2
       if(isspace(string[i])) {
           errorKey[2] = 1;
           errorFlag = 1;
           continue;
       }

       // Is the character not alpha? - error key 3
       if(isalpha(string[i]) == 0) {
           errorKey[3] = 1;
           errorFlag = 1;
           continue;
       }
   }

   // Check if any error keys were set
   if(errorFlag == 1) {
       return printErrors(errorKey, errorKeyLength);
   }

   // Valid
   return 1;
}

int printErrors(int * errorKey, int length) {
    printf("Warning: There were one or more errors with your input:\n");
    for(int i = 0; i < length; i++) {
        if(errorKey[i] == 1) {
            printf("\t- %s\n", errors[i].error);
        }
    }
    return 0;
}
