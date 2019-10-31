/**
 * Programmer Name:         Christopher Matian
 * Program Name:            TCP Simple chat client and server
 * Program Description:     This is a simple chat program that allows messaging between a client and a server.
 * Course Name:             CS-372 400 Fall Quarter
 * Last Modified:           10/30/2019 - 9:10pm
 */

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
