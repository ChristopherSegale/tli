#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "util.h"
#include "dataTypes.h"
#include "error.h"

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

int assignByte(struct TLObject **obj, uint8_t value) {
  if(!(**obj).data)
    return 0;
  *(uint8_t *)((**obj).data) = value;
  return 1;
}


int assignInteger(struct TLObject **obj, int value) {
  if(!(**obj).data)
    return 0;
  *(int *)((**obj).data) = value;
  return 1;
}

int assignDecimal(struct TLObject **obj, double value) {
  if(!(**obj).data)
    return 0;
  *(double *)((**obj).data) = value;
  return 1;
}

int assignSymbol(struct TLObject **obj, const char *value) {
  if((**obj).data) {
    setError("TLObject data is not NULL.", NULL, 0);
    return 0;
  }
  (**obj).data = malloc(sizeof(char) * (strlen(value) + 1));
  strcpy((char *)((**obj).data), value);
  return 1;
}
