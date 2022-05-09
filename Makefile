CC = gcc
CFLAGS = -Wall -Werror
DEPS = init.h encrypt.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

debug: debug.o encrypt.o init.o
	$(CC) -o debug debug.o encrypt.o init.o $(CFLAGS)
