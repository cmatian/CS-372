from socket import *

serverPort = 12001
serverSocket = socket(AF_INET, SOCK_STREAM)

serverSocket.bind(('127.0.0.1', serverPort))

serverSocket.listen(1)

print('The Server is ready to receive')
nt = '\0'
flag = 1

connectionSocket, addr = serverSocket.accept()
while addr and flag == 1:
    inMessage = connectionSocket.recv(2048).decode()

    print(f"{inMessage}")
    outMessage = f"Server Received: {inMessage}\0"

    # Send message back
    connectionSocket.send(outMessage.encode())

    # Set flag to exit if message is "./quit"
    if inMessage[:6] == "./quit":
        print("Quit command received - terminating socket and connection.")
        flag = 0

connectionSocket.close()



