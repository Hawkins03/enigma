CC = gcc
CFLAGS = -Wall -Werror -I.
DEPS = init.h
OBJ = init.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

init_debug: init.o init_debug.o
	$(CC) -o init_debug init.o init_debug.o $(CFLAGS)

encrypt: encrypt.o init.o
	$(CC) -o encrypt encrypt.o init.o $(CFLAGS)
