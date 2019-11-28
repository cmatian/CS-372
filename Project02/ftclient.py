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
              f"\t\t- The [data port] is the port the server will be using to send data back to the client.\n")
        sys.exit(0)


def validate_arguments():
    length = len(sys.argv)
    if length < 5 or length > 6:
        print(f"Invalid arguments - Usage: \"python3 ftclient.py [flip address] [server port] "
              f"[command] [file name (only if command is -g)] [data port]\"\n"
              f"For more information type \"python3 ftclient.py help\"")
        sys.exit(1)


def create_socket():
    clientSocket = socket(AF_INET, SOCK_STREAM) # connect using TCP over IPv4
    return clientSocket

def create_server_socket():
    serverHost = gethostname()
    serverPort = int(sys.argv[len(sys.argv) - 1]) # Data Port = last item in the argv array
    serverSocket = socket(AF_INET, SOCK_STREAM)
    serverSocket.bind(('', serverPort))
    serverSocket.listen(1)
    print(f'Incoming file transfer - client is now listening from {serverHost} on port {serverPort}.')
    return serverSocket

def accept_from_server(serverSocket, clientSocket):
    clientSocket.send("ready".encode())
    data_socket, address = serverSocket.accept()
    return data_socket


def connect_to_server(clientSocket):
    # Key: [2] = server name, [3] = server port
    serverName = sys.argv[1]
    serverPort = int(sys.argv[2])
    clientSocket.connect((serverName, serverPort))

# Initial payload is the bits of data we want to send to the server.
# We want the server to know the command, the file to look for (if the command is -g), and what the data port is.
# We send this data along the first time the connection is made.
def create_initial_payload():
    fn_payload = []
    # Begin at the command arg argv[4]
    for i in range(3, len(sys.argv)):
        fn_payload.append(sys.argv[i])
    return fn_payload

def send_initial_payload(payload, clientSocket):
    clientSocket.send(str(len(payload)).encode()) # Send the size of the payload.
    clientSocket.recv(2048).decode()
    for i in range(len(payload)):
        clientSocket.send((payload[i] + '\0').encode())
        clientSocket.recv(2048).decode() # Wait for a server response before continuing the loop

def main():
    clientSocket = create_socket() # Initial client socket
    payload = create_initial_payload() # Initial payload
    connect_to_server(clientSocket) # Connect to the server
    send_initial_payload(payload, clientSocket) # Send the initial payload data to the server (command, [filename], port).

    # Prep a socket for connection.
    serverSocket = create_server_socket()
    dataSocket = accept_from_server(serverSocket, clientSocket)

    print("Directory Contents")
    dir_payload = dataSocket.recv(200).decode()
    while dir_payload != "complete":
        print(f'{dir_payload}')
        dir_payload = dataSocket.recv(200).decode()
    print("Waiting for server confirmation...", end="")
    dataSocket.recv(150).decode()
    print("Get Directory Complete...")
    dataSocket.close()
    clientSocket.close()

main()

