/**
 * Programmer Name:         Christopher Matian
 * Program Name:            Simple FTP Server and Client
 * Program Description:     This is a simple FTP Server and Client program that allows a user (the client) to connect
 *                          to the server and request either a list of the server's current directory, or download a
 *                          file from that directory.
 * Course Name:             CS-372 400 Fall Quarter
 * Created On:              11/26/2019 - 7:06pm
 * Last Modified:
 */

#include "data.h"

// Handles the initial handshake between the server and client to establish the properties we need for the data connection
int get_payload_length(int * sock_fd) {
    int size;
    char in_buffer[5] = "";
    char * next = "next";

    // First result is the data we need to initialize our array.
    memset(in_buffer, '\0', sizeof(in_buffer));
    read(*sock_fd, in_buffer, sizeof(in_buffer));

    size = atoi(in_buffer);

    write(*sock_fd, next, strlen(next));

    return size;
}

// Chained after get_payload_length - Logic is 100% functional and works provided that the args are passed in order.
void setup_data_info(int * sock_fd, int payload_length, struct data_info * data_arg) {
    char * next = "next";
    for(int i = 0; i < payload_length; i++) {
        // Initiate each loop with a fresh in_buffer and a read command.
        char in_buffer[100] = "";
        read(*sock_fd, in_buffer, sizeof(in_buffer));

        // 1. First loop is always the command
        if(strcmp(in_buffer, "-g") == 0 || strcmp(in_buffer, "-l") == 0) {
            data_arg->command = malloc(100 * sizeof(char *));
            strcpy(data_arg->command, in_buffer);
            write(*sock_fd, next, strlen(next));
            continue;
        }

        // 2. Is it the port? We know it's the port if it's the last loop.
        if(i == payload_length - 1) {
            data_arg->port = malloc(100 * sizeof(char *));
            strcpy(data_arg->port, in_buffer);
            write(*sock_fd, next, strlen(next));
            break;
        }
        // 3. If not we know the loop is going to run another time, so we log the filename.
        data_arg->file_name = malloc(100 * sizeof(char *));
        strcpy(data_arg->file_name, in_buffer);
        write(*sock_fd, next, strlen(next));
    }
}

void data_command_router(struct sock_info * sock_arg, struct data_info * data_arg) {

}

void prepare_data_socket(struct sock_info * sock_arg, struct data_info * data_arg) {
    strcpy(sock_arg->address, data_arg->address);
    strcpy(sock_arg->port, data_arg->port);
}

void free_data(struct data_info * data_arg) {
    if(strcmp(data_arg->command, "-g") == 0) {
        free(data_arg->file_name);
    }
    free(data_arg->address);
    free(data_arg->port);
    free(data_arg->command);
}


