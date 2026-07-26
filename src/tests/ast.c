#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../lexer.h"
#include "../error.h"

void printLexeme(struct lexeme l) {
  switch (l.lexType) {
  case lp:
    puts("(");
    break;
  case rp:
    puts(")");
    break;
  case sharp:
    printf("#%s\n", l.data);
    break;
  case quote:
    puts("'");
    break;
  case backQuote:
    puts("`");
    break;
  case comma:
    puts(",");
    break;
  case doubleQuote:
    putchar('"');
    break;
  case substring:
    putchar(*(l.data));
    break;
  case number:
    printf("Number: %s\n", l.data);
    break;
  case symbol:
    printf("Symbol: %s\n", l.data);
    break;
  default:
    puts("Unknown lexeme");
    break;
  }
}

void printAST(struct ast *tree) {
  struct ast *next;
  for(struct ast *current = tree; current; current = next) {
    next = current->rest;
    printLexeme(current->leaf);
    if((current->leaf).lexType == doubleQuote)
      putchar('\n');
  }
}

int main(int argc, char *argv[]) {
  //char *test = "\"hello\" ( \" World!\"";
  char *test = ";;;Hello World Program\n(defun hello ()\n  (funcall #'write \"Hello \" 1 \"st World!\"))\n'finished ";
  struct ast *a = read(test);
  printf("Running the following expression through the AST printer:\n%s\n", test);
  printAST(a);
  return 0;
}
