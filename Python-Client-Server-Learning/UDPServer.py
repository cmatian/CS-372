from socket import *

serverPort = 12000

serverSocket = socket(AF_INET, SOCK_DGRAM)

serverSocket.bind(('', serverPort))

flag = True

print("The server is ready to receive")

while flag:
    message, clientAddress = serverSocket.recvfrom(2048)

    message = message.decode()

    if message:
        print(f'Received from {gethostname()} the following message: {message}')
    modifiedMessage = message.upper()
    serverSocket.sendto(modifiedMessage.encode(), clientAddress)

    if message == 'Exit' or message == 'Shutdown':
        print(f"{message} message received. Shutting down")
        exitFlag = False
        serverSocket.shutdown(0)
