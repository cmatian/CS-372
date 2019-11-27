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

/**
 * Function:            get_in_addr
 *
 * Description:         The function will evaluate which IP version to use for the IP address
 *
 * Pre-condition:       Assumes that the socket has been created and a link from the list
 *                      has been passed in for evaluation.
 *
 * Post-condition:      Returns a reference of an IPv4 or IPv6 address depending on the family type
 *
 */
void * get_in_addr(struct sockaddr * sa) {
    if(sa->sa_family == AF_INET) {
        return &((struct sockaddr_in *)sa)->sin_addr;
    }
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

struct addrinfo * get_address_info(struct sock_info * sock_arg) {
    struct addrinfo hints;
    struct addrinfo * res;
    int status;
    char * arg;

    // Set up Hints
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // Use IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_protocol = 0;
    // Evaluate whether our address is set or not
    if(strcmp(sock_arg->name, "NULL") == 0) {
        arg = NULL;
    } else {
        arg = sock_arg->name;
    }

    // Set up addr struct for later - we really only need to pass in the port. We'll use the server's native IP.
    if ((status = getaddrinfo(arg, sock_arg->port, &hints, &res)) != 0) {
        fprintf(stderr, "Exception - get_address_info error: %s\n", gai_strerror(status));
        exit(1);
    }
    return res;
}

int socket_setup(struct addrinfo * p, int type) {
    char s[INET6_ADDRSTRLEN];
    int status;
    int sockfd = 0;
    int yes = 1;

    // Loop through the address structures in p until we find a usable address.
    for(; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol) == -1)) {
            close(sockfd);
            fprintf(stderr, "Exception - Socket creation error... trying a different address.\n");
            continue;
        }

        if ((status = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)) {
            close(sockfd);
            fprintf(stderr, "Exception - %s... trying a different address.\n", gai_strerror(status));
            continue;
        }

        /**
         * This conditional acts like a router to determine what we're doing with socket_setup.
         * The server only needs to perform binding on initialization. When we need the server to
         * actually connect to the client (for data transfer), we route it using type == 1 and try to test for a connection.
         */
        if (type == 0) {
            if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
                close(sockfd);
                fprintf(stderr, "Exception - Socket binding error... trying a different address.\n");
                continue;
            }
        } else {
            if(connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
                close(sockfd);
                fprintf(stderr, "Exception - Socket connection error... trying a different address.\n");
                continue;
            }
        }
        break;
    }
    return sockfd;
}

void listen_socket(int sockfd) {
    // Listen for connections - throw errors if any arise.
    if(listen(sockfd, BACKLOG) == -1) {
        close(sockfd);
        fprintf(stderr, "Exception - listen_socket error.\n");
        exit(1);
    }
}

void socket_address_flip(struct sock_info * sock_arg, char * s) {
    if(strcmp(sock_arg->name, "NULL") == 0) {
        sock_arg->name = s;
        return;
    }
    sock_arg->name = "NULL";
}

// Might not actually need this...
void tether(int sockfd, struct addrinfo * p, struct sock_info * sock_arg) {
    char s[INET6_ADDRSTRLEN];
    int result;
    if((result = connect(sockfd, p->ai_addr, p->ai_addrlen)) == -1) {
        fprintf(stderr, "Exception - tether (connection) error.\n");
        exit(1);
    }
    // Converts the address into a legible string
    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);

    // Store the address into our server struct (usually socket_2)
    sock_arg->name = s;
}

/**
 * Function:            set_addr_and_port
 *
 * Description:         The function will assign the sock_info structure's name and port
 *
 * Pre-condition:       Assumes that the port was set on the command line argument, and that the
 *                      server structure was initialized at run time. The sock_info address will be
 *                      set when the connection is made between client and sre
 *
 * Post-condition:      The sock_info name and sock_info port will be set by the function for use throughout the program.
 *
 */
void set_addr_and_port(struct sock_info * sock_arg, char * addr, char * port) {
    sock_arg->name = addr; // Set Address
    sock_arg->port = port; // Set Port
}

/**
 * Function:            set_port
 *
 * Description:         The function will assign the sock_info structure's name as NULL and port from the argv[]
 *
 * Pre-condition:       Assumes that the port was set on the command line argument, and that the
 *                      sock_info structure was initialized at run time.
 *
 * Post-condition:      The sock_info name will be NULL  and sock_info port will be set by the function for use throughout the program.
 *
 */
void set_port(struct sock_info * sock_arg, char ** argv) {
    sock_arg->name = "NULL";
    sock_arg->port = argv[1];
}




