from socket import *
import sys

def flush_input():
    try:
        import msvcrt
        while msvcrt.kbhit():
            msvcrt.getch()
    except ImportError:
        import sys, termios    #for linux/unix
        termios.tcflush(sys.stdin, termios.TCIOFLUSH)

if len(sys.argv) < 2:
    print("You need to specify a PORT argument...")
    exit(1)

serverHost = gethostname()
serverPort = int(sys.argv[1])
serverSocket = socket(AF_INET, SOCK_STREAM)
serverHandle = "HAL9000"

serverSocket.bind(('', serverPort))
serverSocket.listen(1)

print(f'The server at HOST "{serverHost}" is ready to receive on PORT "{serverPort}"...')
nullterm = '\0'

while True:
    # accept incoming socket connection once
    connectionSocket, addr = serverSocket.accept()
    # Close serverSocket - we don't need it anymore unless we're loop through again to listen for another connection
    serverSocket.close()
    while True:

        inMessage = connectionSocket.recv(511).decode()
        if inMessage == "":
            break

        print(inMessage, end="")

        flush_input()
        getInput = input(f"{serverHandle}>>")

        # Combine input message with serverHandle and null terminator
        outMessage = f"{serverHandle} >>{getInput}{nullterm}"

        # Send message back
        connectionSocket.send(outMessage.encode())

        # Set flag to exit if either message contains "./quit" command
        if "./quit" in inMessage or "./quit" in outMessage:
            print("Quit command received - terminating socket and connection.")
            break

    connectionSocket.close()



