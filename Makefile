CFLAGS=-std=c99 -pedantic -c
SRC=src
BIN=tli
OBJ=obj
OBJS=$(OBJ)/main.o $(OBJ)/util.o $(OBJ)/error.o $(OBJ)/argParse.o $(OBJ)/parseType.o $(OBJ)/lexer.o $(OBJ)/TLString.o $(OBJ)/dataTypes.o
BINDIR=bin
PREFIX=/usr/local

install: all
	mkdir -p $(PREFIX)/bin
	cp $(BINDIR)/$(BIN) $(PREFIX)/bin

all: $(BINDIR) main.o util.o error.o argParse.o parseType.o lexer.o TLString.o dataTypes.o
	$(CC) $(OBJS) -o $(BINDIR)/$(BIN)

main.o: $(OBJ)
	$(CC) $(CFLAGS) $(SRC)/main.c -o $(OBJ)/main.o

util.o: $(OBJ)
	$(CC) $(CFLAGS) $(SRC)/util.c -o $(OBJ)/util.o

error.o: $(OBJ)
	$(CC) $(CFLAGS) $(SRC)/error.c -o $(OBJ)/error.o

argParse.o: $(OBJ)
	$(CC) $(CFLAGS) $(SRC)/argParse.c -o $(OBJ)/argParse.o

parseType.o: $(OBJ)
	$(CC) $(CFLAGS) $(SRC)/parseType.c -o $(OBJ)/parseType.o

lexer.o: $(OBJ)
	$(CC) $(CFLAGS) $(SRC)/lexer.c -o $(OBJ)/lexer.o

TLString.o: $(OBJ)
	$(CC) $(CFLAGS) $(SRC)/TLString.c -o $(OBJ)/TLString.o

dataTypes.o: $(OBJ)
	$(CC) $(CFLAGS) $(SRC)/dataTypes.c -o $(OBJ)/dataTypes.o

$(OBJ):
	mkdir -p obj

$(BINDIR):
	mkdir -p bin

clean:
	rm -f $(BINDIR)/$(BIN) $(OBJ)/*.o

uninstall:
	rm -f $(PREFIX)/bin/$(BIN)
