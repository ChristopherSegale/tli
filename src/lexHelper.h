#ifndef LEXHELPER_H
#define LEXHELPER_H

/* symbols have a 50 character limit */
#define BUFFERSIZE 51

enum state { comment, sharpComment, qt, collect, hashCollect, reading };

struct lexeme makeLexeme(enum lexChars token);
struct lexeme makeLP();
struct lexeme makeRP();
struct lexeme makeSharp(const char *token);
struct lexeme makeQuote();
struct lexeme makeBackQuote();
struct lexeme makeComma();
struct lexeme makeDot();
struct lexeme makeDoubleQuote();
struct lexeme makeSubstring(char token);
struct lexeme makeNumber(const char *token);
struct lexeme makeSymbol(const char *token);
void addBranch(struct ast **tree, struct lexeme leaf);
int parse(struct ast **tree, char *buffer, int lchar, int pchar, enum state *st, int *stringIndex, int *pc, int *igq, int *isPipe);

#endif
