#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "util.h"
#include "lexer.h"
#include "error.h"
#include "lexHelper.h"

struct ast *read(const char *expression) {
  int size = strlen(expression);
  struct ast *tree = NULL;
  int i = 0, pairCount = 0, igq = 0, isPipe = 0;
  enum state st = reading;
  int stringIndex = 0;
  char buffer[BUFFERSIZE];
  for(int c = *(expression + i), p = *(expression + i + 1); i < size; i++, c = *(expression + i)) {
    int fail = parse(&tree, buffer, c, p, &st, &stringIndex, &pairCount, &igq, &isPipe);
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
  if(pairCount != 0) {
    if(pairCount > 0)
      setError("Unbalanced parantheses: Not enough right-side parantheses.", NULL, 0);
    else
      setError("Unbalanced parantheses: Too many right-side parantheses.", NULL, 0);
    printError();
    cleanAST(tree);
    return NULL;
  }
  else
    return tree;
}

int parse(struct ast **tree, char *buffer, int lchar, int pchar, enum state *st, int *stringIndex, int *pc, int *igq, int *isPipe) {
  switch (*st) {
  case reading:
    if(lchar == '(') {
      addBranch(tree, makeLP());
      (*pc)++;
    }
    else if(lchar == ')') {
      addBranch(tree, makeRP());
      (*pc)--;
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
    else if(lchar == '.') {
      if(pchar == ' ') {
	if(*pc > 0)
	  addBranch(tree, makeDot());
	else {
	  setError("The '.' symbol must be placed in a list", NULL, 0);
	  return 1;
	}
      }
      else {
	*(buffer + *stringIndex) = lchar;
	(*stringIndex)++;
	*st = collect;
      }
    }
    else if(isspace(lchar)) {
    }
    else {
      *(buffer + *stringIndex) = lchar;
      (*stringIndex)++;
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
      if(lchar == ')') {
	if(*stringIndex < (BUFFERSIZE - 1)) {
	  *(buffer + *stringIndex) = '\0';
	  if(isNumber(buffer))
	    addBranch(tree, makeNumber(buffer));
	  else
	    addBranch(tree, makeSymbol(buffer));
	  *stringIndex = 0;
	  *st = reading;
	  addBranch(tree, makeRP());
	  (*pc)--;
	}
	else {
	  memoryError();
	  return 1;
	}
      }
      else if(*stringIndex < (BUFFERSIZE - 1)) {
	*(buffer + *stringIndex) = lchar;
	(*stringIndex)++;
      }
      else {
	setError("Symbols cannot exceed 50 characters in length.", NULL, 0);
	return 1;
      }
    }
    else {
      if(*stringIndex < BUFFERSIZE)
	*(buffer + *stringIndex) = '\0';
      else {
	setError("Symbols cannot exceed 50 characters in length.", NULL, 0);
	return 1;
      }
      if(isNumber(buffer))
	addBranch(tree, makeNumber(buffer));
      else 
	addBranch(tree, makeSymbol(buffer));
      *stringIndex = 0;
      *st = reading;
    }
    break;
  case hashCollect:
    if(!isspace(lchar)) {
      if(*stringIndex < (BUFFERSIZE - 1)) {
	*(buffer + *stringIndex) = lchar;
	(*stringIndex)++;
	if(lchar == '(') {
	  if(*stringIndex < BUFFERSIZE) {
	    *(buffer + *stringIndex) = '\0';
	    addBranch(tree, makeSharp(buffer));
	    *stringIndex = 0;
	    (*pc)++;
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
      if(*stringIndex < BUFFERSIZE)
	*(buffer + *stringIndex) = '\0';
      else {
	setError("Symbols after # cannot exceed 50 characters in length.", NULL, 0);
	return 1;
      }
      addBranch(tree, makeSharp(buffer));
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
  return 0;
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
  for(struct ast *current = tree, *next; current; current = next) {
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
  val.data = NULL;
  return val;
}

void cleanLexeme(struct lexeme l) {
  if((l.lexType == sharp || l.lexType == substring || l.lexType == number || l.lexType == symbol) && l.data)
    free(l.data);
}

struct lexeme makeLP() {
  return makeLexeme(lp);
}

struct lexeme makeRP() {
  return makeLexeme(rp);
}

struct lexeme makeSharp(const char *token) {
  struct lexeme val = makeLexeme(sharp);
  int size = strlen(token);
  val.data = malloc(sizeof(char) * (size + 1));
  checkNullInit(val.data);
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

struct lexeme makeDot() {
  return makeLexeme(dot);
}

struct lexeme makeDoubleQuote() {
  return makeLexeme(doubleQuote);
}

struct lexeme makeSubstring(char token) {
  struct lexeme val = makeLexeme(substring);
  val.data = malloc(sizeof(char));
  checkNullInit(val.data);
  *(val.data) = token;
  return val;
}

struct lexeme makeNumber(const char *token) {
  struct lexeme val = makeLexeme(number);
  int size = strlen(token);
  val.data = malloc(sizeof(char) * (size + 1));
  checkNullInit(val.data);
  strcpy(val.data, token);
  return val;
}

struct lexeme makeSymbol(const char *token) {
  struct lexeme val = makeLexeme(symbol);
  int size = strlen(token);
  val.data = malloc(sizeof(char) * (size + 1));
  checkNullInit(val.data);
  strcpy(val.data, token);
  return val;
}
