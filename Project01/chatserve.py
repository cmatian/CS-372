# Programmer Name:         Christopher Matian
# Program Name:            TCP Simple chat client and server
# Program Description:     This is a simple chat program that allows messaging between a client and a server.
# Course Name:             CS-372 400 Fall Quarter
# Last Modified:           10/30/2019 - 9:10pm

# Source Citation:
# Flushing input: https://stackoverflow.com/a/38993222 --
# Used the linked function so that I can evaluate which import to use depending on the OS
#
# Kurose and Ross Networking Textbook --
# Used the Python starter code for the server from section 2.7. It barely resembles the original and has
# been modified heavily.
#

# Imports
from socket import *
import sys
import signal
import os

# Function:         signal_handler
#
# Description:      The function is mainly implemented for testing purposes on Windows
#                   and to simulate ctrl+c signal back to the client.
#
# Pre-condition:    The only precondition is that the function is initialized within the top of the
#                   second nested loop that handles the messaging between the client and server
#
# Post-condition:   The function will print a short message and exit using the exit method from sys
#                   On windows, it will terminate the current message loop, but it will not
#                   terminate the entire program. It will continue waiting for another connection.
#
def signal_handler(sig, frame):
    print('^C')
    sys.exit(0)

# Function:         flush_input
#
# Description:      The function will flush the io buffer and allow subsequent loops to have a
#                   a fresh buffer to work with. Alleviates a buffer bug when the user types
#                   messages on UNIX servers even though it's not the server's turn to send a message.
#
# Pre-condition:    None
#
# Post-condition:   The io buffer will be flushed depending on the operating system.
#
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

# Function:         setupServer
#
# Description:      The function will establish the initial server set up.
#
# Pre-condition:    The function assumes that the port number has been set by the user in the command line
#                   when the program was executed.
#
# Post-condition:   The function will establish the socket as an IPv4 TCP socket, it will bind it to the
#                   port number argument, and it will set the server to listen for 1 connection. It will
#                   return the socket for use out of the function.
#
def setupServer(port):
    serverSocket = socket(AF_INET, SOCK_STREAM)
    serverSocket.bind(('', port))
    serverSocket.listen(1)
    return serverSocket

# Function:         main
#
# Description:      main will handle the bulk of program work. It holds the two while loops that continuously
#                   set up the server to listen for a new connection, and handle messages between the
#                   client and server, respectively.
#
# Pre-condition:    Expects that the port number has been sent as an argument through the command line.
#
# Post-condition:   Will execute the server and listen for incoming connections and allow
#                   communication between the client and the server.
#
def main():
    if len(sys.argv) < 2:
        print("Error: You need to specify a PORT argument...")
        print("Usage: ./chatserver [PORT]")
        exit(1)

    # Initialize Server Details
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
            # This sigint will send a blank message back to client which will terminate the client gracefully.
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
            # This handles errors for when the client exits but the server is prompted to send a message back.
            # When the server sends a message it will catch some error, print it, and then break out of the loop
            # to listen for a new connection.
            except error as e:
                print(e)
                break
            # Keyboard interruption backup
            except KeyboardInterrupt:
                print("Keyboard Interrupt - ^C")
                connectionSocket.close()
                sys.exit(0)
            # All other errors
            except:
                print("Undetermined error - establishing a new connection...")
                break

        # Close the socket when we break out of the message loop
        connectionSocket.close()

# Execute main program - need python 3 otherwise it won't work due to syntax difference from 2
main()





