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
    hints.ai_family = AF_INET; // Use IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP
    // Evaluate whether our address is set or not
    if(strcmp(sock_arg->address, "NULL") == 0) {
        hints.ai_flags = AI_PASSIVE;
        arg = NULL;
    } else {
        arg = sock_arg->address;
    }

    // Set up addr struct for later - we really only need to pass in the port. We'll use the server's native IP.
    if ((status = getaddrinfo(arg, sock_arg->port, &hints, &res)) != 0) {
        fprintf(stderr, "Server: Exception - get_address_info error: %s\n", gai_strerror(status));
        exit(1);
    }
    return res;
}

int socket_setup(struct addrinfo * p, int type) {
    int sockfd = 0, sbind, sconn;
    // Loop through the address structures in p until we find a usable address.
    for(; p != NULL; p = p->ai_next) {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1) {
            close(sockfd);
            fprintf(stderr, "Server: Exception - Socket creation error... trying a different address.\n");
            continue;
        }

        /**
         * This conditional acts like a router to determine what we're doing with socket_setup.
         * The server only needs to perform binding on initialization. When we need the server to
         * actually connect to the client (for data transfer), we route it using type == 1 and try to test for a connection.
         */
        if (type == 0) {
            sbind = bind(sockfd, p->ai_addr, p->ai_addrlen);
            if (sbind == -1) {
                close(sockfd);
                fprintf(stderr, "Server: Exception - Socket binding error - trying a different address.\n");
                continue;
            }
        } else {
            sconn = connect(sockfd, p->ai_addr, p->ai_addrlen);
            if(sconn == -1) {
                close(sockfd);
                fprintf(stderr, "Server: Exception - Socket connection error - trying a different address.\n");
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
        fprintf(stderr, "Server: Exception - listen_socket error.\n");
        exit(1);
    }
}

void accept_connection(struct data_info * data_struct, int * sock_fd, int * new_fd) {
    char s[INET6_ADDRSTRLEN];
    struct sockaddr_storage their_addr;
    socklen_t sin_size = sizeof their_addr;

    // Accept incoming connection
    *new_fd = accept(*sock_fd, (struct sockaddr *)&their_addr, &sin_size);

    // Convert the incoming address into a string for later.
    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
    printf("Server: Received a connection from the following address - %s.\n", s);

    // Store the address into the sock_info structure.
    data_struct->address = malloc(100 * sizeof(s));
    strcpy(data_struct->address, s);
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
    sock_arg->address = "NULL";
    sock_arg->port = argv[1];
}




