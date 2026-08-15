#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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
  case dot:
    puts(".");
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

void runLexerTests(char *t[], int size) {
  for(int i = 0; i < size; i++) {
    printf("Running test %d:\n", i + 1);
    struct ast *a = read(t[i]);
    if(a) {
      printAST(a);
      free(a);
    }
  }
}

int main(int argc, char *argv[]) {
  //char *test = "\"hello\" ( \" World!\"";
  char *tests[] = {
    ";;;Hello World Program\n(((defun hello ()\n  #|\n  This is a comment.\n  (format t \"Hello World!~%\")\n  |#\n  (funcall #'write \"Hello \" 1 \"st World!\"))\n`',finished", //Too many left-parantheses
    ";;;Hello World Program\n(defun hello ()\n  #|\n  This is a comment.\n  (format t \"Hello World!~%\")\n  |#\n  (funcall #'write \"Hello \" 1 \"st World!\"))))\n`',finished", //Too many right-parantheses
    ";;;Hello World Program\n(defun hello ()\n  #|\n  This is a comment.\n  (format t \"Hello World!~%\")\n  |#\n  (funcall #'write \"Hello \" 1 \"st World!\"))\n`', finished",  //Space after comma
    ";;;Hello World Program\n(defun hello ()\n  #|\n  This is a comment.\n  (format t \"Hello World!~%\")\n  |#\n  (funcall #'write \"Hello \" 1 \"st World!\"))\n#('a 'b 'c",    //Too many left-parantheses with sharp
    "'Here 'are 'some 'symbols . 'Error 'here",                                                                                                                                   //Dot outside list
    ").)",                                                                                                                                                                        //Dot in unbalanced list
    ";;;Hello World Program\n(defun hello ()\n  #|\n  This is a comment.\n  (format t \"Hello World!~%\")\n  |#\n  (funcall #'write \"Hello \" 1 \"st World!\"))\n`',finished",   //Success
    ";;;Hello World Program\n(defun hello ()\n  #|\n  This is a comment.\n  (format t \"Hello World!~%\")\n  |#\n  (funcall #'write \"Hello \" 1 \"st World!\"))\n#('a 'b 'c)",   //Success with sharp balanced
    "'(a . -.17.5)"                                                                                                                                                               //Success with dotted pair
  };
  runLexerTests(tests, sizeof(tests) / sizeof(tests[0]));
  return 0;
}
