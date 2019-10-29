from socket import *
import sys

if len(sys.argv) < 2:
    print("You need to specify a PORT argument...")
    exit(1)

serverHost = gethostname()
serverPort = int(sys.argv[1])
serverSocket = socket(AF_INET, SOCK_STREAM)

serverSocket.bind(('', serverPort))

serverSocket.listen(1)

print(f'The server at HOST "{serverHost}" is ready to receive on PORT "{serverPort}"...')
nullterm = '\0'
flag = 1

# accept incoming socket connection once
connectionSocket, addr = serverSocket.accept()
# Close serverSocket - we don't need it anymore unless we're loop through again to listen for another connection
# Might flesh this out more to continue listening for new socket connections
serverSocket.close()
while flag == 1:
    inMessage = connectionSocket.recv(2048).decode()

    print(f"{inMessage}")

    # Query server for input
    getInput = input("Send a response to the client: ")

    outMessage = f"Server Received: {getInput}{nullterm}"

    # Send message back
    connectionSocket.send(outMessage.encode())

    # Set flag to exit if either message contains "./quit" command
    if "./quit" in inMessage or "./quit" in outMessage:
        print("Quit command received - terminating socket and connection.")
        flag = 0
connectionSocket.close()



