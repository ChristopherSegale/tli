#ifndef TLSTRING_H
#define TLSTRING_H
#include <stdint.h>

struct TLString {
  uint8_t *string;
  int size;
};

struct TLString makeTLString(uint8_t *s, int length);
int pushTLChar(struct TLString *s, uint8_t c);
uint8_t getTLChar(struct TLString s, int pos);
struct TLString TLSubstring(struct TLString s, int start, int end);

#endif
