#include <stdlib.h>
#include "dataTypes.h"

struct TLObject *makeObject(enum dataType d, int *fail) {
  struct TLObject *obj = malloc(sizeof(struct TLObject));
  if(!obj) {
    *fail = 1;
    return NULL;
  }
  obj->dt = d;
  return obj;
}
