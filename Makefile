CFLAGS=-std=c99 -pedantic -c
SRC=src
BIN=tli
OBJ=obj
BINDIR=bin
PREFIX=/usr/local

install: all
	mkdir -p $(PREFIX)/bin
	cp $(BINDIR)/$(BIN) $(PREFIX)/bin

all: $(BINDIR) main.o argParse.o parseType.o lexer.o
	$(CC) $(OBJ)/main.o $(OBJ)/argParse.o $(OBJ)/parseType.o -o $(BINDIR)/$(BIN)

main.o: $(OBJ)
	$(CC) $(CFLAGS) $(SRC)/main.c -o $(OBJ)/main.o

argParse.o: $(OBJ)
	$(CC) $(CFLAGS) $(SRC)/argParse.c -o $(OBJ)/argParse.o

parseType.o: $(OBJ)
	$(CC) $(CFLAGS) $(SRC)/parseType.c -o $(OBJ)/parseType.o

lexer.o: $(OBJ)
	$(CC) $(CFLAGS) $(SRC)/lexer.c -o $(OBJ)/lexer.o

$(OBJ):
	mkdir -p obj

$(BINDIR):
	mkdir -p bin

clean:
	rm -f $(BINDIR)/$(BIN) $(OBJ)/*.o

uninstall:
	rm -f $(PREFIX)/bin/$(BIN)
