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
#include "connection.h"

// Handles the initial handshake between the server and client to establish the properties we need for the data connection
int get_payload_length(int *sock_fd) {
    int size;
    char in_buffer[5] = "";
    char *next = "next";

    // First result is the data we need to initialize our array.
    memset(in_buffer, '\0', sizeof(in_buffer));
    read(*sock_fd, in_buffer, sizeof(in_buffer));

    size = atoi(in_buffer);

    write(*sock_fd, next, strlen(next));

    return size;
}

// Chained after get_payload_length - Logic is 100% functional and works provided that the args are passed in order.
void setup_data_info(int *sock_fd, int payload_length, struct data_info *data_arg) {
    char *next = "next";
    for (int i = 0; i < payload_length; i++) {
        // Initiate each loop with a fresh in_buffer and a read command.
        char in_buffer[100] = "";
        read(*sock_fd, in_buffer, sizeof(in_buffer));

        // 1. First loop is always the command
        if (strcmp(in_buffer, "-g") == 0 || strcmp(in_buffer, "-l") == 0) {
            data_arg->command = malloc(100 * sizeof(char *));
            strcpy(data_arg->command, in_buffer);
            write(*sock_fd, next, strlen(next));
            continue;
        }

        // 2. Is it the port? We know it's the port if it's the last loop.
        if (i == payload_length - 1) {
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

char **create_2d_array(int length) {
    char **array = malloc(length * sizeof(char *)); // Array of variable items
    for (int i = 0; i < length; i++) {
        array[i] = malloc(100 * sizeof(char)); // Set a char array inside of size 100
        memset(array[i], '\0', (100 * sizeof(char)));
    }
    return array;
}

void free_2d_array(char **storage) {
    int i = 0;
    while (storage[i] != NULL) {
        free(storage[i]);
        i++;
    }
    free(storage);
}


int read_directory(char **storage) {
    int total = 0;
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG) {
                strcpy(storage[total], dir->d_name); // can't assign, need to copy into the memory location
                total += 1; // Keep a running total of valid directory items
            }
        }
    }
    closedir(d);
    return total;
}

void get_directory(int *main_fd, struct sock_info *sock_arg, struct data_info *data_arg) {
    char **storage = create_2d_array(500); // create a 2d array
    int length = read_directory(storage); // Simultaneously fill the array and log the running length
//    for(int i = 0; i < length; i++) {
//        printf("GET FILE %d: %s\n", i, storage[i]);
//    }

    // Send the contents of the directory.
    send_directory(main_fd, sock_arg, data_arg, storage, length);

    free_2d_array(storage);
}

void send_directory(int *main_fd, struct sock_info *sock_arg, struct data_info *data_arg, char **storage, int length) {
    sleep(1); // Sleep for a moment so that the client can catch up (Python is slower).
    char in_buffer[100] = "";
    /**
     * Generate the connection. The client has to be LISTENING first before we can proceed. Otherwise
     * the connection will not be established properly. Read from the main connection port.
     */
    while (1) {
        read(*main_fd, in_buffer, sizeof(in_buffer));
        if (strncmp(in_buffer, "ready", strlen("ready")) == 0) {
            printf("Ready for directory data transfer...\n");
            break;
        }
    }

    struct addrinfo *p = get_address_info(sock_arg);
    int sockfd = socket_setup(p, 1); // Second param = try to connect to client on data port (don't bind).

    if (p == NULL || sockfd < 0) {
        close(sockfd);
        fprintf(stderr, "Failed to connect back to the client at %s on  data port %s.\n", data_arg->address,
                data_arg->port);
        exit(1);
    }
    printf("Successfully connected back to the client at %s on data port %s.\n", data_arg->address, data_arg->port);

    /**
     * Perform the file transfer and send the directory files over
     */
    for (int i = 0; i < length; i++) {
        sleep(1);
        char out_buffer[500] = "";
        strcpy(out_buffer, storage[i]);
        out_buffer[strlen(out_buffer)] = '\0';
        printf("%lu\n", strlen(out_buffer));
        write(sockfd, out_buffer, sizeof(out_buffer));
    }
    // Send a complete message
    write(sockfd, "complete", strlen("complete"));

    close(sockfd); // Close out the TCP data socket
    // Prolly crashing here...
//    free_2d_array(storage); // Kill the 2d array

}

void data_command_router(int *main_fd, struct sock_info *sock_arg, struct data_info *data_arg) {
    if (strncmp(data_arg->command, "-l", strlen("-l")) == 0) {
        // Command is to "-l" directory
        get_directory(main_fd, sock_arg, data_arg);
    } else {
        // Command is to "-g" file
    }
}

void prepare_data_socket(struct sock_info *sock_arg, struct data_info *data_arg) {
    strcpy(sock_arg->address, data_arg->address);
    strcpy(sock_arg->port, data_arg->port);
}

void free_data(struct data_info *data_arg) {
    if (strcmp(data_arg->command, "-g") == 0) {
        free(data_arg->file_name);
    }
    free(data_arg->address);
    free(data_arg->port);
    free(data_arg->command);
}


