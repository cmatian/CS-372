/**
 * Programmer Name:         Christopher Matian
 * Program Name:            Simple FTP Server and Client
 * Program Description:     This is a simple FTP Server and Client program that allows a user (the client) to connect
 *                          to the server and request either a list of the server's current directory, or download a
 *                          file from that directory.
 * Course Name:             CS-372 400 Fall Quarter
 * Created On:              11/24/2019 - 9:53pm
 * Last Modified:           11/29/2019 - 2:56pm
 */

#include "validation.h"
#include "connection.h"

/**
 * Function:            validate_arguments
 *
 * Description:         Validates the argument count received from the commandline.
 *
 * Pre-condition:       Program has been executed and there are arguments in the command line.
 *
 * Post-condition:      The function proceeds normally if there are more than 2 arguments. Exits otherwise.
 *
 */
void validate_arguments(int argc) {
    if(argc < 2) {
        fprintf(stderr, "Exception - Invalid argument count.\nCorrect usage: \"./ftserver <port>\"");
        exit(1);
    }
}