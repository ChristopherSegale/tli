#include <stdlib.h>
#include <stdint.h>
#include "dataTypes.h"
#include "error.h"

void checkNullInit(void *p) {
  if(!p) {
    memoryError();
  }
}

struct TLObject *makeObject(enum dataType d) {
  struct TLObject *obj = malloc(sizeof(struct TLObject));
  if(!obj)
    return NULL;
  obj->dt = d;
  return obj;
}

int initValue(struct TLObject **obj, enum dataType d) {
  if(!(*obj)) {
    setError("Given null TLObject to initialize.", NULL, 0);
    return 0;
  }
  switch (d) {
  case DataByte:
    (**obj).data = malloc(sizeof(uint8_t));
    checkNullInit((**obj).data);
    *(uint8_t *)((**obj).data) = 0;
    break;
  case DataInteger:
    (**obj).data = malloc(sizeof(int));
    checkNullInit((**obj).data);
    *(int *)((**obj).data) = 0;
    break;
  case DataDecimal:
    (**obj).data = malloc(sizeof(double));
    checkNullInit((**obj).data);
    *(double *)((**obj).data) = 0.0;
    break;
  default:
    (**obj).data = NULL;
    break;
  }
  return 1;
}
