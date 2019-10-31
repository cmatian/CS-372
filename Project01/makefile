CC = gcc
CFLAGS = -std=gnu11
DEPS = connection.h validation.h
OBJ = chatclient.o connection.o validation.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

chatclient: $(OBJ)
	gcc $(CFLAGS) -o $@ $^

clean:
	-rm -f *.o *.exe *.out chatclient