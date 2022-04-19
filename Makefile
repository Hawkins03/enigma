CC = gcc
CFLAGS = -Wall -Werror
DEPS = init.h
OBJ = init.o

init: init.c init.h
	$(CC) -o init.o init.c -c $(CFLAGS)

debug: init.c init.h init_debug.c
	$(CC) -o init.o init.c -c $(CFLAGS)
	$(CC) -o init init.o init_debug.c $(CFLAGS)
