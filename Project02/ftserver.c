/**
 * Programmer Name:         Christopher Matian
 * Program Name:            Simple FTP Server and Client
 * Program Description:     This is a simple FTP Server and Client program that allows a user (the client) to connect
 *                          to the server and request either a list of the server's current directory, or download a
 *                          file from that directory.
 * Course Name:             CS-372 400 Fall Quarter
 * Created On:              11/21/2019 - 5:53pm
 * Last Modified:
 */

/* Source Citation:
 *  - Beej's TCP Client and Server Src: https://beej.us/guide/bgnet/html/multi/clientserver.html#simpleclient
 *      - I used a large portion of his starter code and modified it for my program.
 *      - I used portions of his server.c code and abstracted parts of his listen, accept, bind, etc sample functions.
 *
 *  - Christopher Matian's Project 1:
 *      - I some code from my assignment one project. Most of it is the already modular functions used in the
 *        to establish a connection between client and server.
 *      - I took code from there and broke it down further into more modular chunks based on Beej's specifications.
 *
 *  - Makefile Sample: https://www.gribblelab.org/CBootCamp/12_Compiling_linking_Makefile_header_files.html
 *      - Modified his example for my makefile.
 */

// Header File(s)
#include "validation.h"
#include "connection.h"
#include "data.h"


int main(int argc, char * argv[]) {
    // Validate argument count
    validate_arguments(argc); // Expected 2 args in the form ./ftserver <control_port_number>

    /**
     * socket_1: Holds the address and port information of the server. The address is going to be NULL, while the port will be
     *           initialized at runtime using the argument from argv.
     * socket_2: Holds the address and port information from the client. The address will be translated when the connection is
     *           first made (from the client via intop). The tether function will set the address when the client connects.
     *
     * I need to set the socket addresses and port numbers manually rather than using the helper function from Project 01.
     */
    struct sock_info socket_1, socket_2;
    set_port(&socket_1, argv); // Set up address -> "NULL" and port -> argv[1]

    // Set up the address and the socket
    struct addrinfo * p = get_address_info(&socket_1);
    int sockfd = socket_setup(p, 0);

    // Validate p and sockfd
    if (p == NULL || sockfd < 0)  {
        fprintf(stderr, "Failed to bind the server.\n");
        exit(1);
    }
    // Set up the listener on the server.
    listen_socket(sockfd);

    int ctr = 500; // delete later

    printf("Server was successfully initialized on port %s - listening for a new connection.\n", socket_1.port);
    while(ctr > 0) {
        // 1. Run the server and wait for new connections.
        int new_fd;
        accept_connection(&socket_2, &sockfd, &new_fd);
        // Validate the connection
        if (new_fd == -1) {
            fprintf(stderr, "Exception - there was an issue with the connection.");
            continue;
        }

        // 2. Connection was found - get the payload length first
        int payload_length = get_payload_length(&new_fd);

        // 3. Initialize a 2d array structure and grab the actual payload data.
        char data_info[payload_length][100];


//        // Refresh in_buffer every time we read on it
//        memset(in_buffer, '\0', sizeof(in_buffer));
//        read(new_fd, in_buffer, sizeof(in_buffer));
//        printf("%s\n", in_buffer);
//        if(strstr(in_buffer, "initialize")) {
//            printf("Hello\n");
//            write(new_fd, "ready", strlen("ready"));
//
//            memset(in_buffer, '\0', sizeof(in_buffer));
//            read(new_fd, in_buffer, sizeof(in_buffer)); // Read in the length of the payload data
//
//            init_len = atoi(in_buffer); // Set the length
//            char init[init_len][100]; // initialize the 2d array to the init_len
//
//            for(int i = 0; i < init_len; i++) {
//                write(new_fd, "ready", strlen("ready")); // Tell the client we are ready.
//                read(new_fd, init[i], sizeof(init[i])); // Read the first item in.
//                printf("init %d: %s\n", i, init[i]);
//            }
//
//            write(new_fd, "ready", strlen("ready"));
//
//            memset(in_buffer, '\0', sizeof(in_buffer));
//            read(new_fd, in_buffer, sizeof(in_buffer));
//
//            if(strstr(in_buffer, "complete") == 0) {
//                close(new_fd); // Close socket for new one.
//                continue;
//            }
//        }

        close(new_fd); // Close socket for new one.
        ctr--; // Delete later - just getting rid of annoying endless loop inspection errors
    }

    return 0;
}