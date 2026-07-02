#include <stdlib.h>
#include <string.h>
#include "lexer.h"

struct lexeme *read(char *expression) {
  struct lexeme *ast = malloc(sizeof(struct lexeme) * 512);
  if (!ast)
    return NULL;
  return ast;
}

struct lexeme makeLexeme(enum lexChars token) {
  struct lexeme val;
  val.lexType = token;
  return val;
}

struct lexeme makeLP() {
  return makeLexeme(lp);
}

struct lexeme makeRP() {
  return makeLexeme(rp);
}

struct lexeme makeSharp(char specialChar) {
  struct lexeme val = makeLexeme(sharp);
  *(val.data) = specialChar;
  return val;
}

struct lexeme makeQuote() {
  return makeLexeme(quote);
}

struct lexeme makeBackQuote() {
  return makeLexeme(backQuote);
}

struct lexeme makeComma() {
  return makeLexeme(comma);
}

struct lexeme makeDoubleQuote() {
  return makeLexeme(doubleQuote);
}

struct lexeme makeNumber(char *token, int *fail) {
  struct lexeme val = makeLexeme(number);
  int size = strlen(token);
  val.data = malloc((sizeof(char) * size) + 1);
  if (!(val.data)) {
    *fail = 1;
    return val;
  }
  strcpy(val.data, token);
  return val;
}

struct lexeme makeSymbol(char *token, int *fail) {
  struct lexeme val = makeLexeme(symbol);
  int size = strlen(token);
  val.data = malloc((sizeof(char) * size) + 1);
  if (!(val.data)) {
    *fail = 1;
    return val;
  }
  strcpy(val.data, token);
  return val;
}
