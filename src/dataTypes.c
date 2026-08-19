#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "util.h"
#include "dataTypes.h"
#include "TLString.h"
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
      *(double *)((*obj).data) = 0.0;
    else
      *(double *)((*obj).data) = *(double *)value;
    break;
  case DataString:
    (*obj).data = malloc(sizeof(struct TLString));
    checkNullInit((*obj).data);
    if(value) {
      *(struct TLString *)((*obj).data) = *(struct TLString *)value;
    }
    else {
      *(struct TLString *)((*obj).data) = makeTLString(NULL, 0);
    }
    break;
  case DataSymbol:
    if(!value) {
      setError("Symbol value must be given for for the symbol data type.", NULL, 0);
      free(obj);
      return NULL;
    }
    else {
      (*obj).data = malloc(sizeof(char) * (strlen((char *)value) + 1));
      checkNullInit((*obj).data);
      strcpy((char *)((*obj).data), (char *)value);
    }
    break;
  default:
    obj->data = NULL;
    break;
  }
  return obj;
}

int changeByte(struct TLObject **obj, uint8_t value) {
  if(*obj) {
    if((**obj).data) {
      if((**obj).dt == DataByte)
	*(uint8_t *)((**obj).data) = value;
      else {
	setError("TLObject isn't of the byte type.", NULL, 0);
	return 0;
      }
    }
    else {
      setError("The data field on TLObject is NULL.", NULL, 0);
      return 0;
    }
  }
  else {
    setError("Null TLObject pointer given to changeByte function.", NULL, 0);
    return 0;
  }
  return 1;
}

int changeInteger(struct TLObject **obj, int value) {
  if(*obj) {
    if((**obj).data) {
      if((**obj).dt == DataInteger)
	*(int *)((**obj).data) = value;
      else {
	setError("TLObject isn't of the integer type.", NULL, 0);
	return 0;
      }
    }
    else {
      setError("The data field on TLObject is NULL.", NULL, 0);
      return 0;
    }
  }
  else {
    setError("Null TLObject pointer given to changeInteger function.", NULL, 0);
    return 0;
  }
  return 1;
}

int changeDecimal(struct TLObject **obj, double value) {
  if(*obj) {
    if((**obj).data) {
      if((**obj).dt == DataDecimal)
	*(double *)((**obj).data) = value;
      else {
	setError("TLObject isn't of the decimal type.", NULL, 0);
	return 0;
      }
    }
    else {
      setError("The data field on TLObject is NULL.", NULL, 0);
      return 0;
    }
  }
  else {
    setError("Null TLObject pointer given to changeDecimal function.", NULL, 0);
    return 0;
  }
  return 1;
}

uint8_t getByte(struct TLObject *obj, int *fail) {
  if(obj) {
    if(obj->dt == DataByte && obj->data)
      return *(uint8_t *)(obj->data);
    else {
      *fail = 1;
      return 0;
    }
  }
  else {
    *fail = 1;
    return 0;
  }
}

int getInteger(struct TLObject *obj, int *fail) {
  if(obj) {
    if(obj->dt == DataInteger && obj->data)
      return *(int *)(obj->data);
    else {
      *fail = 1;
      return 0;
    }
  }
  else {
    *fail = 1;
    return 0;
  }
}

double getDecimal(struct TLObject *obj, int *fail) {
  if(obj) {
    if(obj->dt == DataDecimal && obj->data)
      return *(double *)(obj->data);
    else {
      *fail = 1;
      return 0;
    }
  }
  else {
    *fail = 1;
    return 0;
  }
}

struct TLString *getTLString(struct TLObject *obj) {
  if(obj && obj->dt == DataString && obj->data)
    return (struct TLString *)(obj->data);
  else
    return NULL;
}

void cleanTLObject(struct TLObject **obj) {
  if(*obj) {
    if(!((**obj).data))
      free((**obj).data);
    free(*obj);
  }
}
