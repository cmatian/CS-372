from socket import *

serverName = 'localhost'
serverPort = 12021

clientSocket = socket(AF_INET, SOCK_STREAM)
clientSocket.connect((serverName, serverPort))

clientSocket.send("Hello".encode())

# modMessage = clientSocket.recv(2048)

# print(f"From Server: {modMessage.decode()}")

clientSocket.close()