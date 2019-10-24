from socket import *

serverPort = 12000
serverSocket = socket(AF_INET, SOCK_STREAM)

serverSocket.bind(('', serverPort))

serverSocket.listen(1)

print('The Server is ready to receive')

while True:
    connectionSocket, addr = serverSocket.accept()
    message = connectionSocket.recv(2048).decode()

    upperCaseMessage = message.upper()

    print(upperCaseMessage)

    connectionSocket.send(upperCaseMessage.encode())

    connectionSocket.close()

