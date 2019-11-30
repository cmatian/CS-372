Christopher Matian - 11/29/2019

======= Instructions for running the Simple FTP program ========

1)	Take the program files and extract them into a directory of your choice on flip.

2)	Place some text files into the directory with your ftserver.c file.

3)	Take the 'ftclient.py' file and copy it into a different directory from your server. I suggest just creating a
	subdirectory and copying it into there.

4)	In the server's directory compile it by calling 'make' or 'make ftserver'.

5)	Initialize the server using './ftserver [control-port-#]'. Use a control port # in the range of 10,000 to 65,000.
	Keep trying different port numbers if the one you tried is in use.

6) 	Before you initialize python client change your current flip directory. If the server is on flip1 you should be
	starting the client on flip2 or flip3.

7)	In the client directory, start the 'ftclient' by calling 'python3 ftclient.py [server flip] [server control port #]
	[command] [optional filename] [data port]'

8)	If the [command] argument is "-g" you need to pass a file text name. Otherwise, you can skip this and just pass the
	the data port number.

9)	The only available [command]s are "-g" (get file) and "-l" (list directory).

10)	The get file command has been tested and can transfer files > 10mb (tested using a 13mb text file full of the
	King James bible).

11)	The list directory command is throttled when it prints out the directory to the client. I did this because I was
	experiencing newline issues during the print out (I'm on windows). I didn't test for this on the flip server
	but the throttling doesn't have any adverse effect on flip.

12)	I don't recommend interrupting the server or client during a transfer. I haven't built any exception handling
	for these situations, so you'll likely trigger adverse behavior and need to restart both the client and server
	from scratch.

13) The server is limited to 10 loops. After 10 different connections it'll terminate and you need to re-establish it.
	This is just something I did to get rid of endless-loop warnings from my IDE. I decided to carry it over just as a
	hard-stop against runaway connections that might fail to terminate and cycle endlessly.
