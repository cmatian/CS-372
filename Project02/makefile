CC = gcc
CFLAGS = -std=gnu11
DEPS = connection.h validation.h
OBJ = ftserver.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

ftserver: $(OBJ)
	gcc $(CFLAGS) -o $@ $^

test: ftserver
	./ftserver localhost 12000 -l 12001

clean:
	-rm -f *.o *.exe *.out ftserver