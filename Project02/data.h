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

#include "connection.h"

#ifndef data_info_struct
#define data_info_struct
typedef struct data_info {
    char * command;
    char * port;
    char * file_name;
} data_info;
#endif

#ifndef DATA_H
#define DATA_H

int get_payload_length(int *);

#endif // DATA_H
