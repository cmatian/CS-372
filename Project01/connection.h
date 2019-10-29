// Connection Function Declarations
// Christopher Matian 10/28/2019

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

int tether(struct server *, int *);

void set_addr_and_port(struct server *, char **);

void * get_in_addr(struct sockaddr *);



#endif
