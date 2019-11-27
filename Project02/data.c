/**
 * Programmer Name:         Christopher Matian
 * Program Name:            Simple FTP Server and Client
 * Program Description:     This is a simple FTP Server and Client program that allows a user (the client) to connect
 *                          to the server and request either a list of the server's current directory, or download a
 *                          file from that directory.
 * Course Name:             CS-372 400 Fall Quarter
 * Created On:              11/26/2019 - 7:06pm
 * Last Modified:
 */

#include "data.h"

// Handles the initial handshake between the server and client to establish the properties we need for the data connection
int get_payload_length(int * sock_fd) {
    int size;
    char in_buffer[5] = "";
    char * next = "next";

    // First result is the data we need to initialize our array.
    memset(in_buffer, '\0', sizeof(in_buffer));
    read(*sock_fd, in_buffer, sizeof(in_buffer));

    size = atoi(in_buffer);

    write(*sock_fd, next, strlen(next));

    return size;
}


