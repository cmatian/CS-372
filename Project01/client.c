// Project 1 TCP Client and Server
// Christopher Matian 10/23/2019

/* Source Citation:
 *  - Beej's TCP Client Src: https://beej.us/guide/bgnet/html/multi/clientserver.html#simpleclient
 */

// Project Header Files
#include "validation.h"
#include "connection.h"

// Project Library Imports
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

// MACRO Definitions
#define HANDLE_BUFFER_SIZE 1024

int main(int argc, char * argv[]) {
    // User Handle Buffer (No More than 16 characters
    char handleBuffer[HANDLE_BUFFER_SIZE];

    /* Capture User Input for user handle
     * Rules:
     *      - No spaces
     *      - Cannot be blank
     *      - No special characters
     *      - First character cannot be a number
     */
    printf("Hello there, please create a name for your online handle.\n");
    do {

        printf(">> ");
        fgets(handleBuffer, HANDLE_BUFFER_SIZE, stdin);

    } while(isValid(handleBuffer) == 0);

    printf("Your online handle has been set to: %s\n\n", handleBuffer);

    /* Server Connection
     */
    char * serverName = "127.0.0.1";
    char * portNum = "12001";

    int sockfd, numbytes;
    struct addrinfo hints, *servinfo, *p;
    int rv;

    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    // Set up the address
    if((rv = getaddrinfo(serverName, portNum, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // Make the connection between the sockets
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if(connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("clients: connect");
            continue;
        }
        break;
    }

    if(p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    // Binding our client to the address
    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
    freeaddrinfo(servinfo);

    printf("client: connecting to %s\n", s);
    int flag = 1;
    while(flag == 1) {
        char buffer[501] = "";
        char newmsg[501] = "";
        printf("Send a message to the server: ");
        fgets(newmsg, 501, stdin);

        // Send to Server
        send(sockfd, newmsg, strlen(newmsg), 0);

        // Read from Server
        read(sockfd, buffer, sizeof(buffer));

        // Print Result
        printf("%s\n", buffer);

        // Gracefully Exit
        if(strstr(buffer, "./quit")) {
            flag = 0;
        }
    }
    printf("Quit command received from server - terminating");
    close(sockfd);
    return 0;
}