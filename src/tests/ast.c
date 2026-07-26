#include <stdio.h>
#include "../lexer.h"

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
    printf("%s\"\n", l.data);
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
  }
}

int main(int argc, char *argv[]) {
  //struct ast *a = read("(defun ()\n  (write \"Hello World!\"))");
  struct ast *a = read("()");
  printAST(a);
  return 0;
}
