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

uint8_t getTLChar(struct TLString s, int pos) {
  if(s.string) {
    if(pos < s.size)
      return *(s.string + pos);
    else
      setError("Index exceeds length of string", NULL, 0);
  }
  else
    setError("String is empty.", NULL, 0);
  return 0;
}

struct TLString TLSubstring(struct TLString s, int start, int end, int *fail) {
  struct TLString val;
  if(s.string) {
    if(start < end && start >= 0 && end < s.size) {
      if(end - start > 1) {
	uint8_t *buffer = malloc(sizeof(uint8_t) * (end - start));
	for(int i = 0, j = start; j <= end; i++, j++)
	  *(buffer + i) = *(s.string + j);
	val = makeTLString(buffer, (end - start) + 1);
	free(buffer);
      }
      else {
	setError("Difference between end and start needs to be greater than 1.", NULL, 0);
	*fail = 1;
      }
    }
    else {
      if(start >= end)
	setError("The start index needs to be less than the end index.", NULL, 0);
      else if(start < 0)
	setError("The start index needs to be greater than or equal to 0.", NULL, 0);
      else if(end >= s.size)
	setError("The end index needs to be less than the size of the string.", NULL, 0);
      *fail = 1;
    }
  }
  else {
    setError("String is empty.", NULL, 0);
    *fail = 1;
  }
  return val;
}
