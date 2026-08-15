#ifndef LEXER_H
#define LEXER_H

enum lexChars { lp, rp, sharp, quote, backQuote, comma, dot, doubleQuote, substring, number, symbol };

struct lexeme {
  enum lexChars lexType;
  char *data;
};

struct ast {
  struct lexeme leaf;
  struct ast *rest;
};

struct ast *makeAST(struct lexeme l);
void cleanAST(struct ast *tree);
void cleanLexeme(struct lexeme l);
struct ast *read(const char *expression); 

#endif
