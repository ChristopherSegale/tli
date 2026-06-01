CFLAGS=-std=c89 -pedantic -c
SRC=src
BIN=tli

all: main.o
	$(CC) $(SRC)/main.o $(SRC)/argParse.o -o $(BIN)

main.o: argParse.o
	$(CC) $(CFLAGS) $(SRC)/main.c -o $(SRC)/main.o

argParse.o:
	$(CC) $(CFLAGS) $(SRC)/argParse.o -o $(SRC)/argParse.o

clean:
	rm -f $(BIN) $(SRC)/*.o
