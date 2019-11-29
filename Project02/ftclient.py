# Requires Python3
# Argument order is not validated - you must take care to enter the arguments in the correct order.

from socket import *
import sys
import os

# Function:         flush_input
#
# Description:      The function will flush the io buffer and allow subsequent loops to have a
#                   a fresh buffer to work with. Alleviates a buffer bug when the user types
#                   messages on UNIX servers even though it's not the server's turn to send a message.
#
# Pre-condition:    None
#
# Post-condition:   The io buffer will be flushed depending on the operating system.
def flush_input():
    try:
        # Windows
        import msvcrt
        while msvcrt.kbhit():
            msvcrt.getch()
    except ImportError:
        # Linux and Unix
        import sys, termios
        termios.tcflush(sys.stdin, termios.TCIOFLUSH)

def return_help():
    print(f"ftclient information:\n"
          f"\t- Usage: \"python3 ftclient.py [flip address] [server port] [command] [file name (only if command is -g)] [data port]\".\n"
          f"\t- The ftserver must be up and listening for new connections. It, or this file, must reside on two different directories.\n"
          f"\t- The arguments you pass in must be in the correct order (refer above for ordering) - no exceptions.\n"
          f"\t\t- The [flip address] argument is the name of the flip server the ftserver.c file is residing on.\n"
          f"\t\t- The [server port] is the port number the ftserver is listening for.\n"
          f"\t\t- The [command] is of the form '-g or '-l'.\n"
          f"\t\t- If the [command] is specified as '-g', you need to supply the [file name] with the name of the file you want to retrieve.\n"
          f"\t\t- If the [command] is specified as '-l' you can skip the [file name] and instead specify [data port].\n"
          f"\t\t- The [data port] is the port the server will be using to send data back to the client.\n\n")
    sys.exit(0)


def validate_arguments():
    length = len(sys.argv)

    if sys.argv[1] == "help":
        return_help()
        sys.exit(0)

    if length < 5 or length > 6:
        print(f"Invalid arguments - Usage: \"python3 ftclient.py [flip address] [server port] "
              f"[command] [file name (only if command is -g)] [data port]\"\n"
              f"For more information type \"python3 ftclient.py help\"")
        sys.exit(1)

    if sys.argv[3] == "-g" and length < 6:
        print("-g command was specified but not enough arguments supplied on the command line.")
        sys.exit(1)

    if sys.argv[3] == "-g" or sys.argv[3] == "-l":
        return
    else:
        print(f'Command {sys.argv[3]} not recognized.')
        sys.exit(1)


def create_socket():
    client_socket = socket(AF_INET, SOCK_STREAM)  # connect using TCP over IPv4
    return client_socket


def create_server_socket():
    server_host = gethostname()
    server_port = int(sys.argv[len(sys.argv) - 1])  # Data Port = last item in the argv array
    server_socket = socket(AF_INET, SOCK_STREAM)
    server_socket.bind(('', server_port))
    server_socket.listen(1)
    print(f'Incoming file transfer - client is now listening from {server_host} on port {server_port}.')
    return server_socket


def accept_from_server(server_socket, client_socket):
    client_socket.send("ready".encode()) # Server wakes up from sleep and reads this ready message
    data_socket, address = server_socket.accept()
    return data_socket


def connect_to_server(client_socket):
    # Key: [1] = server name, [2] = server port
    server_name = sys.argv[1]
    server_port = int(sys.argv[2])
    client_socket.connect((server_name, server_port))


# Initial payload is the bits of data we want to send to the server.
# We want the server to know the command, the file to look for (if the command is -g), and what the data port is.
# We send this data along the first time the connection is made.
def create_initial_payload():
    fn_payload = []
    # Begin at the command arg argv[3]
    for i in range(3, len(sys.argv)):
        fn_payload.append(sys.argv[i])
    return fn_payload


def send_initial_payload(payload, client_socket):
    client_socket.send(str(len(payload)).encode())  # Send the size of the payload.
    client_socket.recv(2048).decode()
    for i in range(len(payload)):
        client_socket.send((payload[i] + '\0').encode())
        client_socket.recv(2048).decode()  # Wait for a server response before continuing the loop


def get_directory(data_socket):
    print("\n=== Directory Contents ===\n")
    dir_payload = data_socket.recv(400).decode()
    while "__complete__" not in dir_payload:
        print(f'\t- {dir_payload}')
        dir_payload = data_socket.recv(400).decode()
    data_socket.recv(400).decode()
    print("\nGet Directory Complete.\n")


def get_file(data_socket, file):
    fd = file
    with open(fd, "w", encoding="utf-8") as f:
        print(f'\nGetting file {fd} from server.\n')
        file_payload = data_socket.recv(2048).decode()
        while "__complete__" not in file_payload:
            f.write(file_payload)
            file_payload = data_socket.recv(2048).decode()
        data_socket.recv(400).decode() # Receive a final message from server indicating completion
        print("\nFile transfer complete.\n")

def validate_g_command():
    if sys.argv[3] == "-l":
        return # Don't care about -l

    file = sys.argv[4]
    if os.path.isfile(file):
        while True:
            flush_input()
            print(f'The {file} already exists. Do you want to overwrite it? Y/N')
            confirm = input("Confirm: ")
            if confirm.lower() == "y" :
                break
            elif confirm.lower() == "n":
                print("Client shutting down process...")
                sys.exit(0)
            else:
                continue

def command_route(command, data_socket, file):
    if command == "-g":
        get_file(data_socket, file)
    else:
        get_directory(data_socket)


def main():
    # Validate the number of args coming through.
    validate_arguments()

    # Perform validation here based on what the command is and whether it is valid.
    #   - Is the command -l or -g...continue normally if -l.
    #   - If -g does the file already exist locally?
    #       - If so ask if they want to overwrite - otherwise exit. The server won't see a connection so we don't have
    #         to worry about sending an error msg over the pipeline.
    validate_g_command()

    # Initial client socket
    client_socket = create_socket()
    # Initial payload
    payload = create_initial_payload()

    # Connect to the server
    connect_to_server(client_socket)
    # Send the initial payload data to the server
    send_initial_payload(payload, client_socket)

    # If the payload is a get file request we need to wait briefly for an all clear signal
    # The server will tell us whether the file exists and is readable. If not, we need to close the
    # client socket and exit.
    if payload[0] == "-g":
        all_clear = client_socket.recv(100).decode()
        if "__error__" in all_clear:
            print("There was an error retrieving the file. Client shutting down.")
            client_socket.close()
            sys.exit(0)

    # Prep a socket for connection.
    server_socket = create_server_socket()
    data_socket = accept_from_server(server_socket, client_socket)

    # Route to get_file or get_directory based on commands
    command_route(payload[0], data_socket, payload[1])

    # Purge all open sockets
    data_socket.close()
    client_socket.close()

main()
