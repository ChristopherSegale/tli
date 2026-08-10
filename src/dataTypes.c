#include <stdlib.h>
#include "dataTypes.h"

struct TLObject *makeObject(enum dataType d) {
  struct TLObject *obj = malloc(sizeof(struct TLObject));
  if(!obj)
    return NULL;
  obj->dt = d;
  return obj;
}
