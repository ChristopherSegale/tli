CFLAGS=-std=c89 -pedantic -c
SRC=src
BIN=tli

all: main
	$(CC) $(SRC)/main.o -o $(BIN)

main:
	$(CC) $(CFLAGS) $(SRC)/main.c -o $(SRC)/main.o

clean:
	rm -f $(BIN) $(SRC)/*.o
