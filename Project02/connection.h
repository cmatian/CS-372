/**
 * Programmer Name:         Christopher Matian
 * Program Name:            Simple FTP Server and Client
 * Program Description:     This is a simple FTP Server and Client program that allows a user (the client) to connect
 *                          to the server and request either a list of the server's current directory, or download a
 *                          file from that directory.
 * Course Name:             CS-372 400 Fall Quarter
 * Created On:              11/21/2019 - 5:59pm
 * Last Modified:
 */

// Library Dependencies
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>


/**
 * Server Struct
 * We will likely be creating 2 structs (one to hold the FTP control connection and the other to
 * hold the FTP data connection.
 */
#ifndef server // Define Once
typedef struct server {
    char * name; // Address Name
    char * port; // Address Port
} server;
#endif // server

#ifndef CONNECTION_H
#define CONNECTION_H

#endif // CONNECTION_H
