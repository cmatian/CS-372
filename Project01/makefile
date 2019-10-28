CC=GCC
CFLAGS=-std=c11 -E

make: client.o connection.o validation.o
	$(CC) -o client *.c

clean:
	-rm -f *.o *.exe