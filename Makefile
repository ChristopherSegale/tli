CFLAGS=-std=c99 -pedantic -c
SRC=src
BIN=tli
OBJ=obj
OBJS=$(OBJ)/main.o $(OBJ)/error.o $(OBJ)/argParse.o $(OBJ)/parseType.o $(OBJ)/lexer.o
BINDIR=bin
PREFIX=/usr/local

install: all
	mkdir -p $(PREFIX)/bin
	cp $(BINDIR)/$(BIN) $(PREFIX)/bin

all: $(BINDIR) main.o error.o argParse.o parseType.o lexer.o
	$(CC) $(OBJS) -o $(BINDIR)/$(BIN)

main.o: $(OBJ)
	$(CC) $(CFLAGS) $(SRC)/main.c -o $(OBJ)/main.o

error.o: $(OBJ)
	$(CC) $(CFLAGS) $(SRC)/error.c -o $(OBJ)/error.o

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
