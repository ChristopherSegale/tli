#ifndef LEXER_H
#define LEXER_H

enum lexChars { lp, rp, sharp, quote, backQuote, comma, doubleQuote, substring, number, symbol, end };

struct lexeme {
  enum lexChars lexType;
  char *data;
};

struct lexeme *read(char *expression); 

#endif
