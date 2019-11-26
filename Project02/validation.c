/**
 * Programmer Name:         Christopher Matian
 * Program Name:            Simple FTP Server and Client
 * Program Description:     This is a simple FTP Server and Client program that allows a user (the client) to connect
 *                          to the server and request either a list of the server's current directory, or download a
 *                          file from that directory.
 * Course Name:             CS-372 400 Fall Quarter
 * Created On:              11/24/2019 - 9:53pm
 * Last Modified:
 */

#include "validation.h"
#include "connection.h"

void validate_arguments(int argc) {
    if(argc < 2) {
        fprintf(stderr, "Exception - Invalid argument count.\nCorrect usage: \"./ftserver <port>\"");
        exit(1);
    }
}