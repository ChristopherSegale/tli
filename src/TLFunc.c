#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "dataTypes.h"
#include "error.h"
#include "util.h"

struct TLFunc *makeTLFunc(struct chunk *code, struct namespace *ns) {
  struct TLFunc *val = malloc(sizeof(struct TLFunc));
  checkNullInit(val);
  val->code = code;
  val->ns = ns;
  return val;
}

struct chunk *makeChunk() {
  struct chunk *val = malloc(sizeof(struct chunk));
  checkNullInit(val);
  if(initChunk(&val))
    return val;
  else {
    free(val);
    return NULL;
  }
}

int initChunk(struct chunk **code) {
  if(code && *code) {
    (**code).size = 0;
    (**code).capacity = 0;
    (**code).instructions = NULL;
    return 1;
  }
  else {
    setError("Initialization of chunk failed.", NULL, 0);
    return 0;
  }
}
