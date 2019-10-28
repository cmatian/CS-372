// Connection Function Definitions
// Christopher Matian 10/23/2019

// Linked Header File
#include "connection.h"

void set_addr_and_port(char * addr, char * port) {

}

void * get_in_addr(struct sockaddr * sa) {
    if(sa->sa_family == AF_INET) {
        return &((struct sockaddr_in *)sa)->sin_addr;
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}