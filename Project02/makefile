CC = gcc
CFLAGS = -std=gnu11
DEPS = connection.h
OBJ = ftclient.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

ftclient: $(OBJ)
	gcc $(CFLAGS) -o $@ $^

clean:
	-rm -f *.o *.exe *.out ftclient