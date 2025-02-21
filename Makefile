CC = gcc
CFLAGS = -Wall -Werror
DEPS = init.h encrypt.h term.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

encrypt: term.o encrypt.o init.o
	$(CC) -o encrypt term.o encrypt.o init.o $(CFLAGS)

tui: tui.o encrypt.o init.o
	$(CC) -o tui tui.o -lncurses encrypt.o init.o $(CFLAGS)
debug: debug.o encrypt.o init.o
	$(CC) -o debug debug.o encrypt.o init.o $(CFLAGS)

.phony: clean
clean:
	rm *.o encrypt debug tui .message.txt .plaintext.txt .ciphertext.txt
