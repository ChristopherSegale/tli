CFLAGS=-std=c89 -pedantic -c
SRC=src
BIN=tli

all: main
	$(CC) $(src)/main.o -o $(BIN)

main:
	$(CC) $(CFLAGS) $(SRC)/main.c -o $(SRC)

clean:
	rm -f $(BIN) $(SRC)/*.o
