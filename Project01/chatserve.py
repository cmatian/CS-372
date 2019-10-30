# Simple chatserver
# Christopher Matian 10/23/2019


# Source Citation
# Flushing input: https://stackoverflow.com/a/38993222
# Used the linked function so that I can evaluate which import to use depending on the OS


from socket import *
import sys
import signal
import os

def signal_handler(sig, frame):
    print('You pressed Ctrl+C!')
    sys.exit(0)

def flush_input():
    try:
        # Windows
        import msvcrt
        while msvcrt.kbhit():
            msvcrt.getch()
    except ImportError:
        # Linux and Unix
        import sys, termios
        termios.tcflush(sys.stdin, termios.TCIOFLUSH)

def setupServer(port):
    serverSocket = socket(AF_INET, SOCK_STREAM)
    serverSocket.bind(('', port))
    serverSocket.listen(1)
    return serverSocket

def main():
    if len(sys.argv) < 2:
        print("Error: You need to specify a PORT argument...")
        print("Usage: ./chatserver [PORT]")
        exit(1)

    # Initial Server Details
    serverHost = gethostname()
    serverPort = int(sys.argv[1])
    serverHandle = "HAL9000"

    # Establish the server socket
    serverSocket = setupServer(serverPort)

    while True:
        print(f'The server on HOST "{serverHost}" is ready to receive on PORT "{serverPort}"...')
        # accept incoming socket connection once
        connectionSocket, addr = serverSocket.accept()
        # Clear the terminal
        os.system('clear')
        print("== Chatterbox ==")
        while True:
            # Windows workaround for killing connection during messaging
            signal.signal(signal.SIGINT, signal_handler)
            # Send and Receive execution
            try:
                inMessage = connectionSocket.recv(511).decode()
                # If the client terminates with ctrl+c during their prompt, the server will catch and reset
                if inMessage == "":
                    print("Client was aborted via CTRL+C during their prompt - listening for a new connection.\n")
                    break

                # If the client sent the quit command, the server will terminate the connection and reset
                if "./quit" in inMessage:
                    print("Quit command received - terminating socket and listening for a new connection.\n")
                    # Send message back
                    connectionSocket.send("./quit".encode())
                    break

                print(inMessage, end="")

                flush_input()
                getInput = input(f"{serverHandle} >> ")

                # Combine input message with serverHandle and null terminator
                outMessage = f"{serverHandle} >> {getInput}\0"

                # Send message back
                connectionSocket.send(outMessage.encode())

                if "./quit" in outMessage:
                    print("Quit command received - server is terminating the current connection...\n")
                    break

            # Exception Handling
            except error as e:
                print(e)
                break
            except KeyboardInterrupt:
                print("Keyboard Interrupt - ^C")
                connectionSocket.close()
                sys.exit(0)
            except:
                print("Undetermined error")
                break

        connectionSocket.close()

# Execute main program - need python 3 otherwise it won't work due to syntax difference
main()





