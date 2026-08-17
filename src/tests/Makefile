CFLAGS=-std=c99 -pedantic -c
SRC=../
OBJ=../../obj

all: ast string dt

ast: ast.o
	$(CC) ast.o $(OBJ)/util.o $(OBJ)/error.o $(OBJ)/lexer.o -o ast

ast.o:
	$(CC) $(CFLAGS) ast.c -o ast.o

string: string.o testUtil.o
	$(CC) string.o testUtil.o $(OBJ)/util.o $(OBJ)/error.o $(OBJ)/TLString.o -o string

string.o:
	$(CC) $(CFLAGS) string.c -o string.o

dt: dt.o testUtil.o
	$(CC) dt.o testUtil.o $(OBJ)/util.o $(OBJ)/error.o $(OBJ)/dataTypes.o -o dt

dt.o:
	$(CC) $(CFLAGS) dt.c -o dt.o

testUtil.o:
	$(CC) $(CFLAGS) testUtil.c -o testUtil.o

clean:
	$(RM) *.o ast dt
