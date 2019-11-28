# Requires Python3
# Argument order is not validated - you must take care to enter the arguments in the correct order.

from socket import *
import sys


# import os

def return_help():
    if len(sys.argv) < 5 and sys.argv[3] == "help":
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
    if length < 5 or length > 6:
        print(f"Invalid arguments - Usage: \"python3 ftclient.py [flip address] [server port] "
              f"[command] [file name (only if command is -g)] [data port]\"\n"
              f"For more information type \"python3 ftclient.py help\"")
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
    client_socket.send("ready".encode())
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
    # Begin at the command arg argv[4]
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


def main():
    # Initial client socket
    client_socket = create_socket()
    # Initial payload
    payload = create_initial_payload()

    # Connect to the server
    connect_to_server(client_socket)
    # Send the initial payload data to the server
    send_initial_payload(payload, client_socket)


    # Prep a socket for connection.
    # server_socket = create_server_socket()
    # data_socket = accept_from_server(server_socket, client_socket)

    # Get the directory
    # get_directory(data_socket)

    # Get a file - client should do some client side checking to ensure they're ok with overwriting if it exists.

    # Purge all open sockets
    # data_socket.close()
    client_socket.close()

main()
