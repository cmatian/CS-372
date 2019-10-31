CC=GCC
CFLAGS=-std=gnu11

make: client.o connection.o validation.o
	$(CC) -o client client.o connection.o validation.o $(CFLAGS)

clean:
	-rm -f *.o *.exe