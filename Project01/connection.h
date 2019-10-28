// Connection Function Declarations
// Christopher Matian 10/28/2019

#include <arpa/inet.h>

#ifndef connection
#define connection

void set_addr_and_port(char *, char *);

void * get_in_addr(struct sockaddr *);

#endif
