#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

/* symbols have a 50 character limit */
#define BUFFERSIZE 51

enum state { comment, sharpComment, qt, collect, hashCollect, reading };

int isNumber(char *string) {
  int useDecimal = 0;
  for(int i = 0; i < strlen(string); i++) {
    char c = *(string + i);
    if (c == '.') {
      if(useDecimal)
	return 1;
      else
	useDecimal = 1;
    }
    else if(!isdigit(c))
      return 1;
  }
  return 0;
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

struct lexeme makeSharp(char *token, int *fail) {
  struct lexeme val = makeLexeme(sharp);
  int size = strlen(token);
  val.data = malloc((sizeof(char) * size) + 1);
  if(!(val.data)) {
    *fail = 1;
    return val;
  }
  strcpy(val.data, token);
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

int parse(struct lexeme *tree, char lchar, char pchar, char *string, int *treeIndex, int *stringIndex, enum state *st, int *pc) {
  int noAlloc = 0;
  int igq = 0;
  int isPipe = 0;
  switch (*st) {
  case reading:
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
      if(pchar == '|')
	*st = sharpComment;
      else {
	*st = hashCollect;
      }
    }
    else if(lchar == '`') {
      *(tree + *treeIndex) = makeBackQuote();
      *(treeIndex)++;
    }
    else if(lchar == ',') {
      if(isspace(pchar))
	return 1;
      *(tree + *treeIndex) = makeComma();
      *(treeIndex)++;
    }
    else if(isspace(lchar)) {
    }
    else {
      *(string + *stringIndex) = lchar;
      *(stringIndex)++;
      *st = collect;
    }
    break;
  case qt:
    if(igq) {
      *(tree + *treeIndex) = makeSubstring(lchar);
      *(treeIndex)++;
      igq = 0;
    }
    else {
      if(lchar != '\\') {
	if(lchar == '"') {
	  *(tree + *treeIndex) = makeDoubleQuote();
	  *(treeIndex)++;
	  *st = reading;
	}
	else {
	  *(tree + *treeIndex) = makeSubstring(lchar);
	  *(treeIndex)++;
	}
      }
      else
	igq = 1;
    }
    break;
  case collect:
    if(!isspace(lchar)) {
      if(*stringIndex < (BUFFERSIZE - 1)) {
	*(string + *stringIndex) = lchar;
	*(stringIndex)++;
      }
      else
	return 1;
    }
    else {
      if(*stringIndex < BUFFERSIZE)
	*(string + *stringIndex) = '\0';
      else
	return 1;
      if(isNumber(string))
	*(tree + *treeIndex) = makeNumber(string, &noAlloc);
      else 
	*(tree + *treeIndex) = makeSymbol(string, &noAlloc);
      if(noAlloc)
	return 1;
      *stringIndex = 0;
      *(treeIndex)++;
      *st = reading;
    }
    break;
  case hashCollect:
    if(!isspace(lchar)) {
      if(*stringIndex < (BUFFERSIZE - 1)) {
	*(string + *stringIndex) = lchar;
	*(stringIndex)++;
	if(lchar == '(' && *stringIndex < BUFFERSIZE) {
	  *(string + *stringIndex) = '\0';
	  *(tree + *treeIndex) = makeSharp(string, &noAlloc);
	  if(noAlloc)
	    return 1;
	  *stringIndex = 0;
	  *(treeIndex)++;
	  *(pc)++;
	  *st = reading;
	}
	else
	  return 1;
      }
      else
	return 1;
    }
    else {
      if(*stringIndex < BUFFERSIZE)
	*(string + *stringIndex) = '\0';
      else
	return 1;
      *(tree + *treeIndex) = makeSharp(string, &noAlloc);
      if(noAlloc)
	return 1;
      *stringIndex = 0;
      *(treeIndex)++;
      *st = reading;
    }
    break;
  case comment:
    if(lchar == '\n')
      *st = reading;
    break;
  case sharpComment:
    if(lchar == '|')
      isPipe = 1;
    else if (lchar == '#') {
      if(isPipe)
	*st = reading;
      isPipe = 0;
    }
    else {
    }
    break;
  }
}

struct lexeme *read(char *expression) {
  int size = strlen(expression);
  struct lexeme *ast = malloc(sizeof(struct lexeme) * size);
  if (!ast)
    return NULL;
  int i = 0, pairCount = 0, j = 0, k = 0; //j index for ast and k index for token
  char buffer[BUFFERSIZE];
  enum state st = reading;
  for(char c = *(expression + i), p = *(expression + i + 1); i < size; i++, c = *(expression + i)) {
    int fail = parse(ast, c, p, buffer, &j, &k, &st, &pairCount);
    if(fail) {
      free(ast);
      return NULL;
    }
    if(i < (size - 1))
      p = *(expression + i + 1);
  }
  *(ast + j) = makeEnd();
  return ast;
}
