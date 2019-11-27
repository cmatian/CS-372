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

#define BACKLOG 5

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
 * Socket Information Struct
 * We will likely be creating 2 structs (one to hold the FTP control connection and the other to
 * hold the FTP data connection.
 */
#ifndef sock_info_struct
#define sock_info_struct // Define Once
typedef struct sock_info {
    char * name; // Address Name
    char * port; // Address Port
} sock_info;
#endif

#ifndef CONNECTION_H
#define CONNECTION_H
void * get_in_addr(struct sockaddr *);
void set_addr_and_port(struct sock_info *, char *, char *);
void set_port(struct sock_info *, char **);
struct addrinfo * get_address_info(struct sock_info *);
int socket_setup(struct addrinfo *, int);
void listen_socket(int);
void accept_connection(struct sock_info *, int *, int *);
void tether(int, struct addrinfo *, struct sock_info *);

#endif // CONNECTION_H
