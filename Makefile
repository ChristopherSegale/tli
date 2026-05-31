CFLAGS=-std=c89 -pedantic -c
BIN=tli

all: main
	$(CC) main.o -o $(BIN)

main:
	$(CC) $(CFLAGS) main.c

clean:
	rm -f $(BIN) *.o
