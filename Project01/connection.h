/**
 * Programmer Name:         Christopher Matian
 * Program Name:            TCP Simple chat client and server
 * Program Description:     This is a simple chat program that allows messaging between a client and a server.
 * Course Name:             CS-372 400 Fall Quarter
 * Last Modified:           10/30/2019 - 9:10pm
 */

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

#ifndef server

typedef struct server {
    char * name; // Address Name
    char * port; // Address Port
} server;

#endif

#ifndef connection_h
#define connection_h

#define SEND_BUFFER_SIZE 501
#define RECV_BUFFER_SIZE 501

int send_message(int *, int, char *);

int tether(struct server *, int *);

void set_addr_and_port(struct server *, char **);

void * get_in_addr(struct sockaddr *);



#endif
