// Connection Function Declarations
// Christopher Matian 10/28/2019

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>


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
