// Validation Function Declarations
// Christopher Matian 10/23/2019

// Library Imports
#include "string.h"
#include "stdio.h"
#include "ctype.h"

#ifndef validation
#define validation

#define ERROR_KEY_LENGTH 5
#define MAX_HANDLE_SIZE 10

int isValid(char *);

int printErrors();

void resetErrors();

#endif
