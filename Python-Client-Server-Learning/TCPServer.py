from socket import *
import sys

if(len(sys.argv) < 2):
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
connectionSocket, addr = serverSocket.accept()
serverSocket.close()
while flag == 1:
    inMessage = connectionSocket.recv(2048).decode()

    print(f"{inMessage}")
    outMessage = f"Server Received: {inMessage}{nullterm}"

    # Send message back
    connectionSocket.send(outMessage.encode())

    # Set flag to exit if message is "./quit"
    if "./quit" in inMessage:
        print("Quit command received - terminating socket and connection.")
        flag = 0
connectionSocket.close()



