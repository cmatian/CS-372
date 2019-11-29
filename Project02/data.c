/**
 * Programmer Name:         Christopher Matian
 * Program Name:            Simple FTP Server and Client
 * Program Description:     This is a simple FTP Server and Client program that allows a user (the client) to connect
 *                          to the server and request either a list of the server's current directory, or download a
 *                          file from that directory.
 * Course Name:             CS-372 400 Fall Quarter
 * Created On:              11/26/2019 - 7:06pm
 * Last Modified:           11/29/2019 - 3:45pm
 */

#include "data.h"
#include "connection.h"

/**
 * Function:            get_payload_length
 *
 * Description:         Initial handshake between server and client over control port. Gets the payload length first.
 *
 * Pre-condition:       Assumes that the socket has been created and the two end hosts are connected to each other.
 *
 * Post-condition:      Returns the size of the payload length.
 *
 */
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


/**
 * Function:            setup_data_info
 *
 * Description:         Initializes the data info struct using the incoming payload data from the client.
 *
 * Pre-condition:       Assumes that the socket has been created, connected, and that the payload length is set.
 *
 * Post-condition:      Initializes the data_info struct with the required command, data port, and/or filename details.
 *
 */
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


/**
 * Function:            create_2d_array
 *
 * Description:         Creates a two dimensional array of strings. Refer to sources on ftserver.c for source link.
 *
 * Pre-condition:       N/A
 *
 * Post-condition:      Returns a pointer to a 2d array that was initialized.
 *
 */
char **create_2d_array(int length) {
    char **array = malloc(length * sizeof(char *)); // Array of variable items
    for (int i = 0; i < length; i++) {
        array[i] = malloc(100 * sizeof(char)); // Set a char array inside of size 100
        memset(array[i], '\0', (100 * sizeof(char)));
    }
    return array;
}

/**
 * Function:            free_2d_array
 *
 * Description:         Frees malloc'd memory in the 2d array we created.
 *
 * Pre-condition:       Assumes a 2d array was created and filled with some data.
 *
 * Post-condition:      Clears the array so we don't trigger memory leaks (stack overflows) on subsequent server loops.
 *
 */
void free_2d_array(char **storage, int length) {
    int i = 0;
    while (i < length) {
        free(storage[i]);
        i++;
    }
    free(storage);
}


/**
 * Function:            read_directory
 *
 * Description:         Opens the current working directory and stores valid files into the 2d array.
 *                      Refer to ftserver.c page for source on this function's structure.
 *
 * Pre-condition:       Expects a 2d array to have been initialized.
 *
 * Post-condition:      Fills the 2d array with data from the current working directory and returns the length of it.
 *                      Uses a loop counter to track how many times we fill that array with valid data.
 */
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

/**
 * Function:            get_directory
 *
 * Description:         Chains various functions together to produce a directory and send it to the client over
 *                      the data connection.
 *
 * Pre-condition:       Assumes that the server is connected to the client over the control port and that payload
 *                      data has been initialized.
 *
 * Post-condition:      Sends the contents of our storage array (the directory names) to our client. Frees the storage
 *                      array of content.
 *
 */
void get_directory(int *main_fd, struct sock_info *sock_arg, struct data_info *data_arg) {
    char **storage = create_2d_array(500); // create a 2d array
    int length = read_directory(storage); // Simultaneously fill the array and log the running length
//    for(int i = 0; i < length; i++) {
//        printf("GET FILE %d: %s\n", i, storage[i]);
//    }

    // Send the contents of the directory.
    send_directory(main_fd, sock_arg, data_arg, storage, length);

    // Clear the array
    free_2d_array(storage, length);
}


/**
 * Function:            file_exists
 *
 * Description:         Looks at the current directory and checks that the file exists in the requested location.
 *
 * Pre-condition:       Assumes that the data_info payload has a file_name specified.
 *
 * Post-condition:      Returns a bool value for whether the file exists and can be read by the server for transfer.
 *
 */
int file_exists(struct data_info *data_arg) {
    if (access(data_arg->file_name, F_OK | R_OK) != -1) {
        printf("Server: File { %s } exists and is readable.\n", data_arg->file_name);
        return 1;
    }
    printf("Server: File { %s } does not exist or cannot be read.\n", data_arg->file_name);
    return 0;
}


/**
 * Function:            get_file
 *
 * Description:         Checks for file existence and then confirms with the client. Sends the file payload.
 *
 * Pre-condition:       Expects that the server has received a connection from the client and that the file the client
 *                      is requesting exists.
 *
 * Post-condition:      Sends the file to the client over the data connection.
 *
 */
void get_file(int *main_fd, struct sock_info *sock_arg, struct data_info *data_arg) {
    int exists = file_exists(data_arg);
    if (exists <= 0) {
        write(*main_fd, "__error__", strlen("__error__"));
        return;
    }
    write(*main_fd, "__found__", strlen("__found__"));
    // File exists so initiate the connection and file transfer
    send_file(main_fd, sock_arg, data_arg);
}


/**
 * Function:            send_file
 *
 * Description:         Creates the data connection with the client and sends the file to them chunk by chunk.
 *                      Refer to the ftserver.c file for the source. This code block uses some code
 *                      from a stack overflow answer for sending file data. Source is cited.
 *
 * Pre-condition:       Expects the control connection to be initialized and that the file exists/is accessible.
 *
 * Post-condition:      Reads chunks of the file into the data socket and sends it through to the client.
 *
 */
void send_file(int *main_fd, struct sock_info *sock_arg, struct data_info *data_arg) {
    printf("Server: Preparing to send { %s } to the client...\n", data_arg->file_name);
    /**
     * Sleep for one second. During this pause client will send a "ready" message over the main_fd arg
     * indicating that it is ready and listening for connections.
     */
    sleep(1);
    char in_buffer[100] = "";
    memset(in_buffer, '\0', sizeof(in_buffer));
    /**
     * Generate the connection. The client has to be LISTENING first before we can proceed. Otherwise
     * the connection will not be established properly. Read from the main connection port.
     */
    while (1) {
        read(*main_fd, in_buffer, sizeof(in_buffer));
        if (strncmp(in_buffer, "ready", strlen("ready")) == 0) {
            printf("Server: Client is ready - creating data connection.\n");
            break;
        }
    }

    /**
     * Connect to the client using a similar process to when the main_fd socket was produced.
     * The only difference is that we're using connect() and not bind(). Additionally, we don't need
     * the listen() function - the client is the listener.
     */
    struct addrinfo *p = get_address_info(sock_arg);
    int sockfd = socket_setup(p, 1); // Second param = try to connect to client on data port (don't bind).

    if (p == NULL || sockfd < 0) {
        close(sockfd);
        fprintf(stderr, "Server: Failed to connect back to the client at %s on  data port %s.\n", data_arg->address,
                data_arg->port);
        exit(1);
    }
    printf("Server: Successfully connected back to the client at %s on data port %s.\n", data_arg->address,
           data_arg->port);

    int open_file = open(data_arg->file_name, O_RDONLY);
    while(1) {
        char write_buffer[1000] = "";
        int bytes_read = read(open_file, write_buffer, sizeof(write_buffer));
        if(bytes_read == 0) {
            break; // done reading
        }

        if(bytes_read < 0) {
            close(sockfd);
            fprintf(stderr, "Server: Fatal error during read operation.");
            return;
        }

        void * write_ptr = write_buffer;
        while(bytes_read > 0) {
            int bytes_written = write(sockfd, write_ptr, bytes_read);
            if(bytes_written <= 0) {
                close(sockfd);
                fprintf(stderr, "Server: Fatal error during write operation.");
                return;
            }
            bytes_read -= bytes_written;
            write_ptr += bytes_written;
        }
        memset(write_buffer, '\0', sizeof(write_buffer)); // refresh our buffer
    }
    // Send a complete message
    write(sockfd, "__complete__", strlen("__complete__"));
    printf("Server: File transfer complete.\n");
    close(sockfd); // Close out the data socket
}


/**
 * Function:            send_directory
 *
 * Description:         Sends the directory stored in the 2d array through to the client.
 *
 * Pre-condition:       Assumes that we have the storage initialized with directory data and that the
 *                      client is listening for a connection from the server.
 *
 * Post-condition:      Sends the directory to the client over the data connection.
 *
 */
void send_directory(int *main_fd, struct sock_info *sock_arg, struct data_info *data_arg, char **storage, int length) {
    printf("Server: Preparing to send directory to the client...\n");
    /**
     * Sleep for one second. During this pause client will send a "ready" message over the main_fd arg
     * indicating that it is ready and listening for connections.
     */
    sleep(1);
    char in_buffer[100] = "";
    memset(in_buffer, '\0', sizeof(in_buffer));
    /**
     * Generate the connection. The client has to be LISTENING first before we can proceed. Otherwise
     * the connection will not be established properly. Read from the main connection port.
     */
    while (1) {
        read(*main_fd, in_buffer, sizeof(in_buffer));
        if (strncmp(in_buffer, "ready", strlen("ready")) == 0) {
            printf("Server: Client is ready - creating data connection.\n");
            break;
        }
    }

    /**
     * Connect to the client using a similar process to when the main_fd socket was produced.
     * The only difference is that we're using connect() and not bind(). Additionally, we don't need
     * the listen() function - the client is the listener.
     */
    struct addrinfo *p = get_address_info(sock_arg);
    int sockfd = socket_setup(p, 1); // Second param = try to connect to client on data port (don't bind).

    if (p == NULL || sockfd < 0) {
        close(sockfd);
        fprintf(stderr, "Server: Failed to connect back to the client at %s on  data port %s.\n", data_arg->address,
                data_arg->port);
        exit(1);
    }
    printf("Server: Successfully connected back to the client at %s on data port %s.\n", data_arg->address,
           data_arg->port);

    /**
     * Perform the file transfer and send the directory. I am throttling the loop with a brief pause (150ms)
     * because of newline issues on the client. The transfer happens so fast that the client fails to append newlines
     * and causes the directory to appear on one line. 150ms gives enough time for the client read loop to complete and
     * be ready for the next server write.
     */
    for (int i = 0; i < length; i++) {
        usleep(150000); // artificially slow down the transfer rate (150ms) - this will stop data from flooding the client buffer
        int size = strlen(storage[i]);
        write(sockfd, storage[i], size); // write
    }
    // Send a complete message
    write(sockfd, "__complete__", strlen("__complete__"));
    printf("Server: Directory transfer complete.\n");

    close(sockfd); // Close out the data socket
}


/**
 * Function:            data_command_router
 *
 * Description:         Routes commands to different functions. Returns from unrecognized commands.
 *
 * Pre-condition:       Assumes the client has sent us a payload of data containing our information.
 *
 * Post-condition:      Initiates either get_directory, get_file, or returns (invalid command).
 *
 */
void data_command_router(int *main_fd, struct sock_info *sock_arg, struct data_info *data_arg) {
    if (strncmp(data_arg->command, "-l", strlen("-l")) == 0) {
        // Command is "-l" (Get Directory)
        printf("Server: Client requested the current directory listing.\n");
        get_directory(main_fd, sock_arg, data_arg);
    } else if (strncmp(data_arg->command, "-g", strlen("-g")) == 0) {
        // Command is "-g" (Get File)
        printf("Server: Client requested a file: { %s }.\n", data_arg->file_name);
        get_file(main_fd, sock_arg, data_arg);
    } else {
        printf("Server: Client command %s not recognized.\n", data_arg->command);
        return;
    }
}


/**
 * Function:            prepare_data_socket
 *
 * Description:         Initializes a socket with the client address and data port.
 *
 * Pre-condition:       Assumes that the client has sent us a payload, and that the server has initialized the data.
 *
 * Post-condition:      Sets up the sock_info struct using data from the data_arg payload.
 *
 */
void prepare_data_socket(struct sock_info *sock_arg, struct data_info *data_arg) {
    sock_arg->address = data_arg->address;
    sock_arg->port = data_arg->port;
}


/**
 * Function:            free_data
 *
 * Description:         Frees the data_info structure memory. (initialized by malloc)
 *
 * Pre-condition:       Assumes that the dat_info struct contains malloc'd memory.
 *
 * Post-condition:      Clears the char arrays. Skips the file_name segment if it's not initialized.
 *
 */
void free_data(struct data_info *data_arg) {
    if (strcmp(data_arg->command, "-g") == 0) {
        free(data_arg->file_name);
    }
    free(data_arg->address);
    free(data_arg->port);
    free(data_arg->command);
}


