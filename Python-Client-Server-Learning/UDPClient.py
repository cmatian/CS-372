from socket import *

# Server
serverName = '127.0.0.1'
serverPort = 12000

# Client Socket
clientSocket = socket(AF_INET, SOCK_DGRAM)

flag = True

while flag:
    message = input('Input Lowercase Sentence:')

    if message == 'Exit':
        flag = False

    clientSocket.sendto(message.encode(), (serverName, serverPort))

    modifiedMessage, serverAddress = clientSocket.recvfrom(2048)

    print(modifiedMessage.decode())
