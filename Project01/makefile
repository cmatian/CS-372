CC=gcc
CFLAGS=-std=gnu11

make: client.o connection.o validation.o
	$(CC) $(CFLAGS) -o client client.c connection.c validation.c

clean:
	-rm -f *.o *.exe