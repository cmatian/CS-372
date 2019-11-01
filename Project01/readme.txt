Christopher Matian - 10/31/2019

======= Instructions for running the chat client program ========

0) Take the program files and extract them into a directory of your choice on flip.

1) Make sure that you have two different flip clients running. E.g., flip1 on one terminal, and flip2 on a different terminal.

2) On any one of the terminals, compile the chatclient code by calling "make" (without parens) in the terminal. Type ls and you
   should see *.o files and a chatclient executable.

3) On one of the flip terminals, initiate the server like so: python3 chatserve.py [Port Number]. You can change the port number
   if the number is already in use.

4) After the server starts it will print out a string indicating that it's listening for a connection, and which server
   and port number it's bound to. Make note of both values because you'll use them in step 5.

5) On the other flip terminal, initiate the client like so: ./chatclient [Flip Server] [Port Number]. The Flip server value will
   be the host name that the server is running on. So if the server is on flip2.engr.oregonstate.edu then that is the input you'll
   use. The port number should match the port number the server was initiated with.

6) On the chatclient you'll be prompted for a user name. Only alpha-numeric characters are allowed up to 10 characters. The first
   character cannot be a number.

7) When you enter your name you'll automatically connect to the server. On the server terminal you will see the screen clear
   indicating that it accepted a connection and it's listening for a message in the chatterbox.

8) The chatclient must send the first message so go ahead and do so. The message can be up to 500 characters long. If you exceed
   500 characters the fgets buffer will loop again and send another message without prompting you for one.

9) Once you send a message the client will place the line onto a blank newline indicating its waiting for a response.

10) Switch to the server terminal window and you should see the message from the client prepended with their handle. The
    server handlename is hardcoded to HAL9000. The server is now prompted to return a message and you can either hit enter to
    send a blank message or type in whatever you want. The server will then switch back to listening for a message from the client.

11) You can keep alternating between the two hosts sending messages or exit. To send an exit command type in "./quit" on whichever
    host has been prompted for sending an input. Regardless of which host sends the quit signal, the server will continue to
    listen for a new client connection until it receives a SIGINT (Ctrl + C). On unix the server will exit normally with SIGINT. On
    windows servers, you need to break using ctrl+b or fn+ctrl+b. You don't need to worry about Windows though since flip is unix
    based.

12) There is exception handling for various situations. In the event the client is prompted for input but prematurely terminates (Ctrl+C), the
    server will catch this and reset to listen for a new connection. Likewise, if the server is prompted and hits ctrl+c, the interrupt
    will reset the server, and the client will catch the response, realize it's blank (0 bytes) and terminate the connection.

13) When the server is prompted and you hit ctrl+c it will reset (i.e. listen for a new connection). This is expected behavior
    and a result of trying to bridge the gap between Windows and Unix handling of SIGINT. When you reset during the server prompt
    you can hit ctrl+c again to terminate the server entirely.