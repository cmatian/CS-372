CC = gcc
CFLAGS = -std=gnu11
DEPS = connection.h validation.h
OBJ = ftserver.o connection.o validation.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

ftserver: $(OBJ)
	gcc $(CFLAGS) -o $@ $^

test: ftserver
	./ftserver 12021

clean:
	-rm -f *.o *.exe *.out ftserver