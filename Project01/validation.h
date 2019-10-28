// Validation Function Declarations
// Christopher Matian 10/23/2019

// Library Imports
#include "string.h"
#include "stdio.h"
#include "ctype.h"

#ifndef error
/*
 * int flag = set the flag to 1 if there is an error of this type
 * char * error = string describing the error
*/
typedef struct error {
    int flag;
    char * error;
} error;
#endif

#ifndef validation_h
#define validation_h

#define ERROR_KEY_LENGTH 5
#define MAX_HANDLE_SIZE 10

int isArgValid(int);

int isHandleValid(char *);

int printErrors();

void resetErrors();

#endif
