/**
 * Programmer Name:         Christopher Matian
 * Program Name:            Simple FTP Server and Client
 * Program Description:     This is a simple FTP Server and Client program that allows a user (the client) to connect
 *                          to the server and request either a list of the server's current directory, or download a
 *                          file from that directory.
 * Course Name:             CS-372 400 Fall Quarter
 * Created On:              11/21/2019 - 5:53pm
 * Last Modified:
 */

/* Source Citation:
 *  - Beej's TCP Client and Server Src: https://beej.us/guide/bgnet/html/multi/clientserver.html#simpleclient
 *      - I used a large portion of his starter code and modified it for my program.
 *      - I mostly used the server portion of his code for the ftserver
 *
 *  - Christopher Matian's Assignment 1:
 *      - I some code from my assignment one project. Most of it is the already modular functions used in the
 *        to establish a connection between client and server.
 *
 *  - Makefile Sample: https://www.gribblelab.org/CBootCamp/12_Compiling_linking_Makefile_header_files.html
 *      - Modified his example for my makefile.
 */

// File Dependencies
#include <stdio.h>

//
int main() {
    printf("Hello, World!\n");
    return 0;
}