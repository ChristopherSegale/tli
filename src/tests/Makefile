CFLAGS=-std=c99 -pedantic -c
SRC=../
OBJ=../../obj

all: ast dt

ast: ast.o
	$(CC) ast.o $(OBJ)/util.o $(OBJ)/error.o $(OBJ)/lexer.o -o ast

ast.o:
	$(CC) $(CFLAGS) ast.c -o ast.o

dt: dt.o
	$(CC) dt.o $(OBJ)/util.o $(OBJ)/error.o $(OBJ)/dataTypes.o -o dt

dt.o:
	$(CC) $(CFLAGS) dt.c -o dt.o

clean:
	$(RM) *.o ast dt
