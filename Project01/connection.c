// Connection Function Definitions
// Christopher Matian 10/23/2019

// Linked Header File
#include "connection.h"

int tether(struct server * server, int * sockfd) {
    int rv;
    struct addrinfo hints, *servinfo, *p;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    // Validate that the address can be reached
    // servinfo is now a linked list pointing to some list of available addresses
    if((rv = getaddrinfo(server->name, server->port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        // Return 1 means we hit an issue creating the address linked list
        return 1;
    }

    // Make the connection between the sockets
    for(p = servinfo; p != NULL; p = p->ai_next) {

        if((*sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("Socket Error");
            continue;
        }

        if(connect(*sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(*sockfd);
            perror("Connect Error");
            continue;
        }
        break;
    }

    if(p == NULL) {
        fprintf(stderr, "There was an issue connecting to the server...is the server running?\n");
        // Return 2 means we were unable to establish a connection between the sockets
        return 2;
    }

    // Binding our client to the address
    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
    freeaddrinfo(servinfo);

    printf("Successfully connected to %s on port %s\n", s, server->port);
    // Return 0 means we successfully connected to the server
    return 0;
}

void set_addr_and_port(struct server * server, char ** argv) {
    server->name = argv[1]; // Set Address
    server->port = argv[2]; // Set Port
}

void * get_in_addr(struct sockaddr * sa) {
    if(sa->sa_family == AF_INET) {
        return &((struct sockaddr_in *)sa)->sin_addr;
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}