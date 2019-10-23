from socket import *

serverName = '127.0.0.1'
serverPort = 12000

clientSocket = socket(AF_INET, SOCK_STREAM)
clientSocket.connect((serverName, serverPort))

message = input('Input a message:')

clientSocket.send(message.encode())

modMessage = clientSocket.recv(2048)

print(f"From Server: {modMessage.decode()}")

clientSocket.close()
