/**
 * Programmer Name:         Christopher Matian
 * Program Name:            TCP Simple chat client and server
 * Program Description:     This is a simple chat program that allows messaging between a client and a server.
 * Course Name:             CS-372 400 Fall Quarter
 * Last Modified:           10/30/2019 - 9:10pm
 */

/* Source Citation:
 *  - Beej's TCP Client Src: https://beej.us/guide/bgnet/html/multi/clientserver.html#simpleclient
 *      - I used his starter code to make the connection between the client and server.
 */

// Project Header Files
#include "validation.h"
#include "connection.h"


// MACRO Definitions
#define HANDLE_BUFFER_SIZE 1024
#define SEND_BUFFER_SIZE 501
#define RECV_BUFFER_SIZE 501

/**
 * Function:            main
 *
 * Description:         The main client function which executes the bulk of the function. It calls the corresponding
 *                      functions to create the user handle, establish the connection with the server, and
 *                      send/receive messages in a loop.
 *
 * Pre-condition:       No preconditions.
 *
 * Post-condition:      Returns 0 upon program completion and closes the socket (if it's open).
 *
 */
int main(int argc, char * argv[]) {

    /* Validate Argument Count
     * If there is less than 3 args we will exit the program immediately
     */
    int argumentValidation = isArgValid(argc);
    if(argumentValidation == 0) {
        fprintf(stderr, "Incorrect number of arguments");
        exit(1);
    }

    // User Handle Buffer
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

        printf(">>");
        // We use a big buffer here and just validate the size inside of the isHandleValid function.
        fgets(handleBuffer, HANDLE_BUFFER_SIZE, stdin);

    } while(isHandleValid(handleBuffer) == 0);
    // Copy handle to smaller char array
    char handleName[MAX_HANDLE_SIZE];
    int handleLength = strlen(handleBuffer);
    strcpy(handleName, handleBuffer);
    // Replace trailing newline with null terminator instead
    handleName[handleLength - 1] = '\0';
    printf("Your online handle has been set to: %s\n", handleName);

    /* Server Address and Port Initialization
     * This will just initialize the server struct address name and port information strings
     * */
    struct server server;
    set_addr_and_port(&server, argv); // Set address and port

    /* socket is used for send and recv statements.
     * connectionResult is a simple validation for successful connections.
     */
    int socket;
    int connectionResult;

    /* tether function will do the following:
     * 1. Create the address linked list,
     * 2. Create the client socket
     * 3. Connect the client socket to the server socket
     * 4. Bind the two sockets to each other.
     */
    if((connectionResult = tether(&server, &socket)) != 0) {
        return connectionResult;
    };

    // Create a function for messaging between servers in different header/source files
    // do-while loop?
    while(1) {
        char newmsg[SEND_BUFFER_SIZE] = "";
        char outmsg[SEND_BUFFER_SIZE + MAX_HANDLE_SIZE];
        char * outHandlePtr = handleName;
        char * newmsgPtr = newmsg;
        char * outBufferPtr = outmsg;
        char inBuffer[RECV_BUFFER_SIZE] = "";

        printf("%s >> ", handleName);
        fgets(newmsg, SEND_BUFFER_SIZE, stdin);

        // Generate outmessage combining handle with newmsg
        // This code will basically concatenate: handle name + >> + newmsg
        outBufferPtr = stpcpy(outBufferPtr, outHandlePtr);
        outBufferPtr = stpcpy(outBufferPtr, " >> ");
        outBufferPtr = stpcpy(outBufferPtr, newmsgPtr);

        // Send message to the server
        send(socket, outmsg, strlen(outmsg), 0);

        // Read from Server
        // This should usually wait for a response from the server (socket will block here until it gets a response)

        int result = read(socket, inBuffer, sizeof(inBuffer));
        /* This indicates that there was an error.
         * The logic here is that the server is always sending back at least ~12 bytes of data (blank
         * input still has the name preprended to it). When the server terminates via CTRL+C, we can catch it
         * because the result will have a value of 0. We can exit gracefully from here.
         */
        if(result == 0) {
            printf("The server seems to have been terminated (^C) - gracefully closing the client.");
            break;
        }

        if (result < 0) {
            printf("There was an issue with the server - gracefully exiting the client.");
            break;
        }

        // Print the message from the server
        printf("%s\n", inBuffer);

        // Gracefully exit if the server tells us to quit
        if(strstr(inBuffer, "./quit") || strstr(outmsg, "./quit")) {
            printf("Quit command received from server - terminating the client.");
            break;
        }
    }
    close(socket);
    return 0;
}