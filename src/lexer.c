#include <stdlib.h>
#include <string.h>
#include "lexer.h"

/* characters have a 50 character limit */
#define BUFFERSIZE 51

enum state { comment, qt, bq, collect, read };

struct lexeme *read(char *expression) {
  int size = strlen(expression);
  struct lexeme *ast = malloc(sizeof(struct lexeme) * size);
  if (!ast)
    return NULL;
  int i = 0, pairCount = 0, j = 0, k = 0; //j index for ast and k index for token
  char buffer[BUFFERSIZE];
  enum state st = read;
  for(char c = *(expression + i), p = *(expression + i + 1); i < size; i++, c = *(expression + i)) {
    int fail = parse(ast, c, p, buffer, &j, &k, &s, &pairCount);
    if(fail) {
      free(ast);
      return NULL;
    }
    if(c == '#' || c == '\\')
      i++;
    if(i <= size)
      p = *(expression + i + 1);
  }
  *(ast + j + 1) = makeEnd();
  return ast;
}

int parse(struct lexeme *tree, char lchar, char pchar, char *string, int *treeIndex, int *stringIndex, enum state *st, int *pc) {
  int noAlloc = 0;
  int igq = 0;
  switch (*st) {
  case read:
    if(lchar == '(') {
      *(tree + *treeIndex) = makeLP();
      *(treeIndex)++;
      *(pc)++;
    }
    else if(lchar == ')') {
      *(tree + *treeIndex) = makeRP();
      *(treeIndex)++;
      *(pc)--;
    }
    else if(lchar == '"') {
      *st = qt;
    }
    else if(lchar == ';') {
      *st = comment;
    }
    else if(lchar == '#') {
      *(tree + *treeIndex) = makeSharp();
      *(treeIndex)++;
    }
    else if(lchar == '`') {
      *(tree + *treeIndex) = makeBackQuote();
      *(treeIndex)++;
      *st = bq;
    }
    else if(lchar == ',') {
      if(pchar == ' ')
	return 1;
      *(tree + *treeIndex) = makeComma();
      *(treeIndex)++;
    }
    else if(lchar == ' ') {
    }
    else {
      *(string + *stringIndex) = lchar;
      *(stringIndex)++;
      *st = collect;
    }
    break;
  case qt:
    if(lchar == '"') {
      if(igq) {
	*(tree + *treeIndex) = makeSubstring('"');
	igq = 0;
      }
      else {
	if(pchar != ' ')
	  return 1;
	*st = read;
        *(tree + *treeIndex) = makeDoubleQuote();
      }
    }
    else {
      if(lchar == '\\') {
	if(igq) {
	  *(tree + *treeIndex) = makeSubstring('\\');
	  igq = 0;
	}
	else
	  igq = 1;
      }
      else
	*(tree + *treeIndex) = makeSubstring(lchar);
    break;
  }
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

struct lexeme makeDoubleQuote(char *token, int *fail) {
  return makeLexeme(doubleQuote);
}

struct lexeme makeSubstring(char token) {
  struct lexeme val = makeLexeme(substring);
  *(val.data) = token;
  return val;
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

struct lexeme makeEnd() {
  return makeLexeme(end);
}
