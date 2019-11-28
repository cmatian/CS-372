/**
 * Programmer Name:         Christopher Matian
 * Program Name:            Simple FTP Server and Client
 * Program Description:     This is a simple FTP Server and Client program that allows a user (the client) to connect
 *                          to the server and request either a list of the server's current directory, or download a
 *                          file from that directory.
 * Course Name:             CS-372 400 Fall Quarter
 * Created On:              11/27/2019 - 12:09pm
 * Last Modified:
 */

#ifndef LIBRARIUM_H
#define LIBRARIUM_H
/**
 * Data Information Struct
 * Holds the information that the client passes to us for initialization on data connection socket.
 */
#ifndef data_info_struct
#define data_info_struct
typedef struct data_info {
    char * command;
    char * file_name;
    char * address;
    char * port;
} data_info;
#endif
/**
 * Socket Information Struct
 * We will likely be creating 2 structs (one to hold the FTP control connection and the other to
 * hold the FTP data connection.
 */
#ifndef sock_info_struct
#define sock_info_struct // Define Once
typedef struct sock_info {
    char * address; // Address Name
    char * port; // Address Port
} sock_info;
#endif

// Library Dependencies and Resources
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
#include <dirent.h>
#include <time.h>

#endif // LIBRARIUM_H
