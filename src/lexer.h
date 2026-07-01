#ifndef LEXER_H
#define LEXER_H

enum lexChars { lp, rp, sharp, quote, doubleQuote, number, symbol };

struct lexeme {
  enum lexChars lexType;
  char *data;
};

struct lexeme *read(char *expression); 

#endif
