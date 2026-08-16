#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "TLString.h"
#include "util.h"
#include "error.h"

struct TLString makeTLString(uint8_t *string, int length) {
  struct TLString val;
  if(!string) {
    val.string = NULL;
    val.size = 0;
  }
  else {
    val.string = malloc(sizeof(uint8_t) * length);
    checkNullInit(val.string);
    memcpy(val.string, string, length);
    val.size = length;
  }
  return val;
}

int pushTLChar(struct TLString *s, uint8_t c) {
  if(s) {
    if(!s->string) {
      s->string = malloc(sizeof(uint8_t));
      checkNullInit(s->string);
      *(s->string) = c;
      s->size = 1;
    }
    else {
      (s->size)++;
      s->string = realloc(s->string, sizeof(uint8_t) * s->size);
      checkNullInit(s->string);
      *(s->string + (s->size - 1)) = c;
    }
  }
  else {
    setError("NULL pointer given to pushTLChar function.", NULL, 0);
    return 0;
  }
}
