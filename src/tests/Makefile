CFLAGS=-std=c99 -pedantic -c
SRC=../
OBJ=../../obj

all: ast

ast: ast.o
	$(CC) ast.o $(OBJ)/error.o $(OBJ)/lexer.o -o ast

ast.o:
	$(CC) $(CFLAGS) ast.c -o ast.o

clean:
	$(RM) *.o ast
