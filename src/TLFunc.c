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
