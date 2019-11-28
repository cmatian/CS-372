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
     * control_socket: Holds the address and port information of the server. The address is going to be NULL, while the port
     *                 will be initialized at runtime using the argument from argv.
     *
     * data_socket:    Holds the address and port information from the client. The address will be translated when the connection is
     *                 first made (from the client via ntop). The tether function will set the address when the client connects.
     */
    struct sock_info control_socket, data_socket;
    set_port(&control_socket, argv); // Set up address -> "NULL" and port -> argv[1]

    // Set up the address and the socket
    struct addrinfo * p = get_address_info(&control_socket);
    int sockfd = socket_setup(p, 0);

    // Validate p and sockfd
    if (p == NULL || sockfd < 0)  {
        fprintf(stderr, "Failed to bind the server.\n");
        exit(1);
    }
    // Set up the listener on the server.
    listen_socket(sockfd);

    int ctr = 10; // delete later
    printf("Server was successfully initialized on port %s - listening for a new connection.\n", control_socket.port);
    while(ctr > 0) {
        struct data_info payload;
        // 1. Run the server and wait for new connections.
        int newfd;
        accept_connection(&payload, &sockfd, &newfd);
        // Validate the connection
        if (newfd == -1) {
            close(newfd);
            fprintf(stderr, "Exception - there was an issue with the connection.\n");
            ctr--;
            continue;
        }

        // 2. Connection was found - get the payload length first
        int payload_length = get_payload_length(&newfd);

        // 3. Initialize a 2d array structure and grab the actual payload data.
        //    - The payload data here consists of the command, possibly the filename, and lastly the data port number.
        setup_data_info(&newfd, payload_length, &payload);

        // 4. Initialize data_socket to hold the data from our payload struct - this is used later for a data connection
        //    - The payload here is to just reinforce what command came through and grab the filename.
        prepare_data_socket(&data_socket, &payload);

        // 5. Pass our data socket and payload structure to a function to route based on commands. Also pass the original
        // socket through for communication to set up the connection inside.
        data_command_router(&newfd, &data_socket, &payload);

        close(newfd); // Close socket for new one.
        free_data(&payload); // clean up
        ctr--; // Delete later - just getting rid of annoying endless loop inspection errors
    }

    return 0;
}