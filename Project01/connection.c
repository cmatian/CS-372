/**
 * Programmer Name:         Christopher Matian
 * Program Name:            TCP Simple chat client and server
 * Program Description:     This is a simple chat program that allows messaging between a client and a server.
 * Course Name:             CS-372 400 Fall Quarter
 * Last Modified:           10/31/2019 - 5:28pm
 */

// Linked Header File
#include "connection.h"

/**
 * Function:            send_message
 *
 * Description:         This function was brought in from the chatclient main fn for cluttering. This will
 *                      query the user for an input and send a message along to the server.
 *
 * Pre-condition:       Assumes that a socket variable has been created, that the handle size is initialized,
 *                      and that the username was created.
 *
 * Post-condition:      Sends a message to the server and returns either 0 or 1. The value is used
 *                      to validate whether a ./quit command was sent to the server.
 *
 */
int send_message(int * socket, int maxHandleSize, char * handleName) {
    char newmsg[SEND_BUFFER_SIZE] = "";
    char outmsg[SEND_BUFFER_SIZE + maxHandleSize];
    char * outHandlePtr = handleName;
    char * newmsgPtr = newmsg;
    char * outBufferPtr = outmsg;

    // Query the user for the message they want to send
    printf("%s >> ", handleName);
    fgets(newmsg, SEND_BUFFER_SIZE, stdin);

    // Generate outmessage combining handle with newmsg
    // This code will basically concatenate: handle name + >> + newmsg
    outBufferPtr = stpcpy(outBufferPtr, outHandlePtr);
    outBufferPtr = stpcpy(outBufferPtr, " >> ");
    outBufferPtr = stpcpy(outBufferPtr, newmsgPtr);

    send(*socket, outmsg, strlen(outmsg), 0);

    if(strstr(outmsg, "./quit")) {
        return 1;
    }
    return 0;
};

/**
 * Function:            tether
 *
 * Description:         The function will handle the heavy-duty lifting and establish the connection between
 *                      the client and the server. It will first produce a socket, connect it, and finally
 *                      bind it to the server.
 *
 * Pre-condition:       Assumes that a socket variable has been declared, and that the server struct has been initialized
 *                      with the address and port numbers from argv.
 *
 * Post-condition:      The socket will connect to the server and return 1 of 3 return codes. 1 means the address
 *                      had an issue being created. 2 means the connection couldn't be established (usually the
 *                      server info is wrong or the server is not running yet. 0 means no errors and the
 *                      and pass the if-conditional which expects this result.
 *
 */
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
/**
 * Function:            set_addr_and_port
 *
 * Description:         The function will assign the server structure's name and port
 *
 * Pre-condition:       Assumes that the address and port were set on the command line argument, and that the
 *                      server structure was initialized at run time.
 *
 * Post-condition:      The server name and server port will be set by the function for use throughout the program.
 *
 */
void set_addr_and_port(struct server * server, char ** argv) {
    server->name = argv[1]; // Set Address
    server->port = argv[2]; // Set Port
}

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