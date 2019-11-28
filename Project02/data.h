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

#include "librarium.h"

#ifndef DATA_H
#define DATA_H

int get_payload_length(int *);

void setup_data_info(int *, int, struct data_info *);

void prepare_data_socket(struct sock_info *, struct data_info *);

void data_command_router(struct sock_info *, struct data_info *);

void free_data(struct data_info *);

#endif // DATA_H
