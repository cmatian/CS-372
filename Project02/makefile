CC = gcc
CFLAGS = -std=gnu11
DEPS = librarium.h connection.h validation.h data.h
OBJ = ftserver.o connection.o validation.o data.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

ftserver: $(OBJ)
	gcc $(CFLAGS) -o $@ $^

test: ftserver
	./ftserver 12021

clean:
	-rm -f *.o *.exe *.out ftserver