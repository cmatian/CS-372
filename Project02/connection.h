/**
 * Programmer Name:         Christopher Matian
 * Program Name:            Simple FTP Server and Client
 * Program Description:     This is a simple FTP Server and Client program that allows a user (the client) to connect
 *                          to the server and request either a list of the server's current directory, or download a
 *                          file from that directory.
 * Course Name:             CS-372 400 Fall Quarter
 * Created On:              11/21/2019 - 5:59pm
 * Last Modified:           11/29/2019 - 2:27pm
 */

#define BACKLOG 5

#include "librarium.h"

/**
 * Socket Information Struct
 * We will likely be creating 2 structs (one to hold the FTP control connection and the other to
 * hold the FTP data connection.
 */

#ifndef CONNECTION_H
#define CONNECTION_H
void * get_in_addr(struct sockaddr *);
struct addrinfo * get_address_info(struct sock_info *);
int socket_setup(struct addrinfo *, int);
void listen_socket(int);
void accept_connection(struct data_info *, int *, int *);
void set_port(struct sock_info *, char **);

#endif // CONNECTION_H
