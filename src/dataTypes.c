#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "util.h"
#include "dataTypes.h"
#include "error.h"

struct TLObject *makeObject(enum dataType d, void *value) {
  struct TLObject *obj = malloc(sizeof(struct TLObject));
  if(!obj)
    return NULL;
  obj->dt = d;
  switch (d) {
  case DataByte:
    (*obj).data = malloc(sizeof(uint8_t));
    checkNullInit((*obj).data);
    if(!value) {
      *(uint8_t *)((*obj).data) = 0;
    }
    else
      *(uint8_t *)((*obj).data) = *(uint8_t *)value;
    break;
  case DataInteger:
    (*obj).data = malloc(sizeof(int));
    checkNullInit((*obj).data);
    if(!value)
      *(uint8_t *)((*obj).data) = 0;
    else
      *(uint8_t *)((*obj).data) = *(int *)value;
    break;
  case DataDecimal:
    (*obj).data = malloc(sizeof(double));
    checkNullInit((*obj).data);
    if(!value)
      *(double *)((*obj).data) = *(double *)value;
    break;
  case DataSymbol:
    if(!value) {
      setError("Symbol value must be given for for the symbol data type.", NULL, 0);
      free(obj);
      return NULL;
    }
    else {
      (*obj).data = malloc(sizeof(char) * (strlen(value) + 1));
      checkNullInit((*obj).data);
      strcpy((char *)((*obj).data), value);
    }
    break;
  default:
    obj->data = NULL;
    break;
  }
  return obj;
}

void cleanTLObject(struct TLObject **obj) {
  if(*obj) {
    if(!((**obj).data))
      free((**obj).data);
    free(*obj);
  }
}
