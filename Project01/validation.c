/**
 * Programmer Name:         Christopher Matian
 * Program Name:            TCP Simple chat client and server
 * Program Description:     This is a simple chat program that allows messaging between a client and a server.
 * Course Name:             CS-372 400 Fall Quarter
 * Last Modified:           10/30/2019 - 9:10pm
 */

// Linked Header File
#include "validation.h"

// Error structure that holds a flag and a description of the error
error errors[ERROR_KEY_LENGTH] = {
        {0, "Blank handle names are not allowed."}, // 0
        {0, "The handle name can only be 10 characters long."}, // 1
        {0, "The first character cannot be a number."}, // 2
        {0, "Spaces are not allowed."}, // 3
        {0, "Special characters are not allowed."} // 4
};

/**
 * Function:            isArgValid
 *
 * Description:         This function simply validates the argument count. Must be 3 in order to pass.
 *
 * Pre-condition:       Assumes that the user has input arguments at the command line level.
 *
 * Post-condition:      Returns 0 or 1 depending on whether the correct number of arguments were passed.
 *
 */
int isArgValid(int argc) {
    if(argc < 3) {
        return 0;
    }
    return 1;
}

/**
 * Function:            isHandleValid
 *
 * Description:         This function handles validation of the user handle input. If the handle
 *                      is too long, has a number at the beginning, or various others, the function will
 *                      print errors and return a fail value (0). If it's successful, it will return a value of 1.
 *
 * Pre-condition:       Assumes that the user has input a handle for validation.
 *
 * Post-condition:      Will spit out either an error code along with a log of which errors were hit, or it will
 *                      return 1 (pass value).
 *
 */
int isHandleValid(char * string) {

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
    if(strlen(string) > MAX_HANDLE_SIZE + 1) {
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

/**
 * Function:            printErrors
 *
 * Description:         The function will loop through the error structure and check the flag. If its
 *                      value is 1 then the loop will print out it's linked error text.
 *
 * Pre-condition:       Assumes that the error structure has been created.
 *
 * Post-condition:      Will output the errors that have been flagged (1) to the terminal.
 *                      Provides a more informative error dump to the user when they input
*                       multiple, unique mistakes.
 *
 */
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

/**
 * Function:            resetErrors
 *
 * Description:         Resets the error flags located in the error structure.
 *
 * Pre-condition:       Expects that the error structure has been created.
 *
 * Post-condition:      Will reset each error key back to 0 for the next loop.
 *
 */
void resetErrors() {
    for(int i = 0; i < ERROR_KEY_LENGTH; i++) {
        errors[i].flag = 0;
    }
}
