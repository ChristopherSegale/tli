#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"
#include "error.h"

/* symbols have a 50 character limit */
#define BUFFERSIZE 51

enum state { comment, sharpComment, qt, collect, hashCollect, reading };

int isNumber(char *string);
struct lexeme makeLexeme(enum lexChars token);
struct lexeme makeLP();
struct lexeme makeRP();
struct lexeme makeSharp(char *token);
struct lexeme makeQuote();
struct lexeme makeBackQuote();
struct lexeme makeComma();
struct lexeme makeDoubleQuote();
struct lexeme makeSubstring(char token);
struct lexeme makeNumber(char *token);
struct lexeme makeSymbol(char *token);
struct ast *addBranch(struct ast *tree, struct lexeme leaf);
int parse(struct ast *tree, char lchar, char pchar, char *string, int *stringIndex, enum state *st, int *pc, int *igq, int *isPipe);

struct ast *read(char *expression) {
  int size = strlen(expression);
  struct ast *tree = NULL;
  int i = 0, j = 0, pairCount = 0, igq = 0, isPipe = 0;
  char buffer[BUFFERSIZE];
  enum state st = reading;
  for(char c = *(expression + i), p = *(expression + i + 1); i < size; i++, c = *(expression + i)) {
    int fail = parse(tree, c, p, buffer, &j, &st, &pairCount, &igq, &isPipe);
    if(fail) {
      cleanAST(tree);
      printError();
      return NULL;
    }
    if(i < (size - 2))
      p = *(expression + i + 2);
  }
  return tree;
}

int parse(struct ast *tree, char lchar, char pchar, char *string, int *stringIndex, enum state *st, int *pc, int *igq, int *isPipe) {
  switch (*st) {
  case reading:
    if(lchar == '(') {
      addBranch(tree, makeLP());
      *(pc)++;
    }
    else if(lchar == ')') {
      addBranch(tree, makeRP());
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
    else if(lchar == '`')
      addBranch(tree, makeBackQuote());
    else if(lchar == ',') {
      if(isspace(pchar)) {
	setError("Space after comma.", NULL, 0);
	return 1;
      }
      addBranch(tree, makeComma());
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
    if(*igq) {
      addBranch(tree, makeSubstring(lchar));
      *igq = 0;
    }
    else {
      if(lchar != '\\') {
	if(lchar == '"') {
	  addBranch(tree, makeDoubleQuote());
	  *st = reading;
	}
	else
	  addBranch(tree, makeSubstring(lchar));
      }
      else 
	*igq = 1;
    }
    break;
  case collect:
    if(!isspace(lchar)) {
      if(*stringIndex < (BUFFERSIZE - 1)) {
	*(string + *stringIndex) = lchar;
	*(stringIndex)++;
      }
      else {
	setError("Symbols cannot exceed 50 characters in length.", NULL, 0);
	return 1;
      }
    }
    else {
      if(*stringIndex < BUFFERSIZE)
	*(string + *stringIndex) = '\0';
      else {
	setError("Symbols cannot exceed 50 characters in length.", NULL, 0);
	return 1;
      }
      if(isNumber(string))
	addBranch(tree, makeNumber(string));
      else 
	addBranch(tree, makeSymbol(string));
      *stringIndex = 0;
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
	  addBranch(tree, makeSharp(string));
	  *stringIndex = 0;
	  *(pc)++;
	  *st = reading;
	}
	else {
	  setError("Symbols after # cannot exceed 50 characters in length.", NULL, 0);
	  return 1;
	}
      }
      else {
	setError("Symbols after # cannot exceed 50 characters in length.", NULL, 0);
	return 1;
      }
    }
    else {
      if(*stringIndex < BUFFERSIZE)
	*(string + *stringIndex) = '\0';
      else {
	setError("Symbols after # cannot exceed 50 characters in length.", NULL, 0);
	return 1;
      }
      addBranch(tree, makeSharp(string));
      *stringIndex = 0;
      *st = reading;
    }
    break;
  case comment:
    if(lchar == '\n')
      *st = reading;
    break;
  case sharpComment:
    if(lchar == '|')
      *isPipe = 1;
    else if (lchar == '#') {
      if(*isPipe)
	*st = reading;
      *isPipe = 0;
    }
    else {
    }
    break;
  }
}

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

struct ast *makeAST(struct lexeme l) {
  struct ast *val = malloc(sizeof(struct ast));
  if(!val) {
    setErrorType(allocation, NULL, 0);
  }
  val->leaf = l;
  return val;
}

void cleanAST(struct ast *tree) {
  for(struct ast *current = tree, *next = tree->rest; current; current = next, next = current->rest) {
    cleanLexeme(current->leaf);
    free(current);
  }
}

struct ast *addBranch(struct ast *tree, struct lexeme leaf) {
  struct ast *current = tree;
  while(current) {
    current = current->rest;
  }
  current->rest = makeAST(leaf);
}

struct lexeme makeLexeme(enum lexChars token) {
  struct lexeme val;
  val.lexType = token;
  return val;
}

void cleanLexeme(struct lexeme l) {
  if(l.lexType == sharp || l.lexType == substring || l.lexType == number || l.lexType == symbol)
    free(l.data);
}

struct lexeme makeLP() {
  return makeLexeme(lp);
}

struct lexeme makeRP() {
  return makeLexeme(rp);
}

struct lexeme makeSharp(char *token) {
  struct lexeme val = makeLexeme(sharp);
  int size = strlen(token);
  val.data = malloc((sizeof(char) * size) + 1);
  if(!(val.data)) {
    setErrorType(allocation, NULL, 0);
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

struct lexeme makeNumber(char *token) {
  struct lexeme val = makeLexeme(number);
  int size = strlen(token);
  val.data = malloc((sizeof(char) * size) + 1);
  if (!(val.data)) {
    setErrorType(allocation, NULL, 0);
    return val;
  }
  strcpy(val.data, token);
  return val;
}

struct lexeme makeSymbol(char *token) {
  struct lexeme val = makeLexeme(symbol);
  int size = strlen(token);
  val.data = malloc((sizeof(char) * size) + 1);
  if (!(val.data)) {
    setErrorType(allocation, NULL, 0);
    return val;
  }
  strcpy(val.data, token);
  return val;
}
