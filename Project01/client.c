// Project 1 TCP Client and Server
// Christopher Matian 10/23/2019

/* Source Citation:
 *  - Beej's TCP Client Src: https://beej.us/guide/bgnet/html/multi/clientserver.html#simpleclient
 */

// Project Header Files
#include "validation.h"
#include "connection.h"

// Project Library Imports


// MACRO Definitions
#define HANDLE_BUFFER_SIZE 1024
#define SEND_BUFFER_SIZE 501
#define RECV_BUFFER_SIZE 501

int main(int argc, char * argv[]) {

    /* Validate Argument Count
     * If there is less than 3 args we will exit the program immediately
     */
    int argumentValidation = isArgValid(argc);
    if(argumentValidation == 0) {
        fprintf(stderr, "Incorrect number of arguments");
        exit(1);
    }

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

        printf(">>");
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
    int flag = 1;
    while(flag == 1) {
        char newmsg[SEND_BUFFER_SIZE] = "";
        char outmsg[SEND_BUFFER_SIZE + MAX_HANDLE_SIZE];
        char * outHandlePtr = handleName;
        char * newmsgPtr = newmsg;
        char * outBufferPtr = outmsg;
        char inBuffer[RECV_BUFFER_SIZE] = "";

        bzero(newmsg, SEND_BUFFER_SIZE);
        bzero(outmsg, SEND_BUFFER_SIZE + MAX_HANDLE_SIZE);

        printf("%s >>", handleName);
        fgets(newmsg, SEND_BUFFER_SIZE, stdin);

        // Generate outmessage combining handle with newmsg
        // The guy who designed C string concatenation should be tried for crimes against humanity
        outBufferPtr = stpcpy(outBufferPtr, outHandlePtr);
        outBufferPtr = stpcpy(outBufferPtr, ">> ");
        outBufferPtr = stpcpy(outBufferPtr, newmsgPtr);

        // Send to Server
        send(socket, outmsg, strlen(outmsg), 0);

        // Read from Server
        // This should usually wait for a response from the server (socket will block here until it gets a response)
        // Might need to implement a while loop here
        read(socket, inBuffer, sizeof(inBuffer));

        // Print Result
        printf("%s\n", inBuffer);

        // Gracefully Exit
        if(strstr(inBuffer, "./quit")) {
            flag = 0;
        }
    }
    printf("Quit command received from server - terminating");
    close(socket);
    return 0;
}