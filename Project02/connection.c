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

// Header File(s)
#include "connection.h"

struct addrinfo get_address_info(char * address, char * port) {
    struct addrinfo hints;
    struct addrinfo * res;
    int status;
    char * arg;

    // Set up Hints
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP
    // Evaluate whether our address is set or not
    if(strcmp(address, "NULL") == 0) {
        hints.ai_flags = AI_PASSIVE;
        arg = NULL;
    } else {
        arg = address;
    }
    // Set up addr struct for later - we really only need to pass in the port. We'll use the server's native IP.
    if ((status = getaddrinfo(arg, port, &hints, &res)) != 0) {
        fprintf(stderr, "Exception - get_address_info error: %s\n", gai_strerror(status));
        exit(1);
    }
    return * res;
}

int create_socket(struct addrinfo * p) {
    int sockfd;
    if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol) == -1)) {
        fprintf(stderr, "Exception - create_socket error");
        exit(1);
    }
    return sockfd;
}

void bind_socket(int sockfd, struct addrinfo * p) {
    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
        close(sockfd);
        fprintf(stderr, "Exception - bind_socket error.");
        exit(1);
    }
};

void listen_socket(int sockfd) {
    // Listen for connections - throw error's if any arise
    if(listen(sockfd, BACKLOG) == -1) {
        fprintf(stderr, "Exception - listen_socket error.");
        exit(1);
    }
};

void tether(int sockfd, struct addrinfo * p) {
    int result;
    if((result = connect(sockfd, p->ai_addr, p->ai_addrlen)) == -1) {
        fprintf(stderr, "Exception - tether error.");
        exit(1);
    }
}




