#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "lexer.h"
#include "error.h"

/* symbols have a 50 character limit */
#define BUFFERSIZE 51

char buffer[BUFFERSIZE];
int stringIndex = 0;
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
void addBranch(struct ast **tree, struct lexeme leaf);
int parse(struct ast **tree, int lchar, int pchar, enum state *st, int *pc, int *igq, int *isPipe);

struct ast *read(char *expression) {
  int size = strlen(expression);
  struct ast *tree = NULL;
  int i = 0, pairCount = 0, igq = 0, isPipe = 0;
  enum state st = reading;
  stringIndex = 0;
  for(int c = *(expression + i), p = *(expression + i + 1); i < size; i++, c = *(expression + i)) {
    int fail = parse(&tree, c, p, &st, &pairCount, &igq, &isPipe);
    if(fail) {
      cleanAST(tree);
      printError();
      return NULL;
    }
    if(i < (size - 2))
      p = *(expression + i + 2);
  }
  if(st == collect) {
    buffer[stringIndex] = '\0';
    if(isNumber(buffer))
      addBranch(&tree, makeNumber(buffer));
    else
      addBranch(&tree, makeSymbol(buffer));
  }
  else if(st == hashCollect) {
    buffer[stringIndex] = '\0';
    addBranch(&tree, makeSharp(buffer));
  }
  return tree;
}

int parse(struct ast **tree, int lchar, int pchar, enum state *st, int *pc, int *igq, int *isPipe) {
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
      addBranch(tree, makeDoubleQuote());
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
    else if(lchar == '\'')
      addBranch(tree, makeQuote());
    else if(isspace(lchar)) {
    }
    else {
      buffer[stringIndex] = lchar;
      stringIndex++;
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
      if(stringIndex < (BUFFERSIZE - 1)) {
	buffer[stringIndex] = lchar;
	stringIndex++;
      }
      else {
	setError("Symbols cannot exceed 50 characters in length.", NULL, 0);
	return 1;
      }
      if(lchar == ')' || lchar == EOF) {
	if(stringIndex < (BUFFERSIZE - 1)) {
	  buffer[stringIndex] = '\0';
	  if(isNumber(buffer))
	    addBranch(tree, makeNumber(buffer));
	  else
	    addBranch(tree, makeSymbol(buffer));
	  stringIndex = 0;
	  *st = reading;
	}
	else {
	  memoryError();
	  return 1;
	}
      }
    }
    else {
      if(stringIndex < BUFFERSIZE)
	buffer[stringIndex] = '\0';
      else {
	setError("Symbols cannot exceed 50 characters in length.", NULL, 0);
	return 1;
      }
      if(isNumber(buffer))
	addBranch(tree, makeNumber(buffer));
      else 
	addBranch(tree, makeSymbol(buffer));
      stringIndex = 0;
      *st = reading;
    }
    break;
  case hashCollect:
    if(!isspace(lchar)) {
      if(stringIndex < (BUFFERSIZE - 1)) {
	buffer[stringIndex] = lchar;
	stringIndex++;
	if(lchar == '(') {
	  if(stringIndex < BUFFERSIZE) {
	    buffer[stringIndex] = '\0';
	    addBranch(tree, makeSharp(buffer));
	    stringIndex = 0;
	    *(pc)++;
	    *st = reading;
	  }
	  else {
	    setError("Symbols after # cannot exceed 50 characters in length.", NULL, 0);
	    return 1;
	  }
	}
      }
      else {
	setError("Symbols after # cannot exceed 50 characters in length.", NULL, 0);
	return 1;
      }
    }
    else {
      if(stringIndex < BUFFERSIZE)
	buffer[stringIndex] = '\0';
      else {
	setError("Symbols after # cannot exceed 50 characters in length.", NULL, 0);
	return 1;
      }
      addBranch(tree, makeSharp(buffer));
      stringIndex = 0;
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
  return 0;
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
      return 0;
  }
  return 1;
}

struct ast *makeAST(struct lexeme l) {
  struct ast *val = malloc(sizeof(struct ast));
  if(!val) {
    memoryError();
  }
  val->leaf = l;
  val->rest = NULL;
  return val;
}

void cleanAST(struct ast *tree) {
  struct ast *next;
  for(struct ast *current = tree; current; current = next) {
    next = current->rest;
    cleanLexeme(current->leaf);
    free(current);
  }
}

void addBranch(struct ast **tree, struct lexeme leaf) {
  if(!(*tree))
    *tree = makeAST(leaf);
  else {
    struct ast *current = *tree;
    while(current->rest) {
      current = current->rest;
    }
    current->rest = makeAST(leaf);
  }
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
  val.data = malloc(sizeof(char) * (size + 1));
  if(!(val.data)) {
    memoryError();
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
  val.data = malloc(sizeof(char));
  if(!(val.data)) {
    memoryError();
    return val;
  }
  *(val.data) = token;
  return val;
}

struct lexeme makeNumber(char *token) {
  struct lexeme val = makeLexeme(number);
  int size = strlen(token);
  val.data = malloc(sizeof(char) * (size + 1));
  if (!(val.data)) {
    memoryError();
    return val;
  }
  strcpy(val.data, token);
  return val;
}

struct lexeme makeSymbol(char *token) {
  struct lexeme val = makeLexeme(symbol);
  int size = strlen(token);
  val.data = malloc(sizeof(char) * (size + 1));
  if (!(val.data)) {
    memoryError();
    return val;
  }
  strcpy(val.data, token);
  return val;
}
