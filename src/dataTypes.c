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
    obj->data = malloc(sizeof(uint8_t));
    checkNullInit(obj->data);
    if(!value) {
      *(uint8_t *)(obj->data) = 0;
    }
    else
      *(uint8_t *)(obj->data) = *(uint8_t *)value;
    break;
  case DataInteger:
    obj->data = malloc(sizeof(int));
    checkNullInit(obj->data);
    if(!value)
      *(int *)(obj->data) = 0;
    else
      *(int *)(obj->data) = *(int *)value;
    break;
  case DataDecimal:
    obj->data = malloc(sizeof(double));
    checkNullInit(obj->data);
    if(!value)
      *(double *)(obj->data) = 0.0;
    else
      *(double *)(obj->data) = *(double *)value;
    break;
  case DataString:
    if(value) {
      obj->data = value;
    }
    else {
      obj->data = makeTLString(NULL, 0);
    }
    break;
  case DataCons:
    obj->data = value;
    break;
  case DataStructure:
    obj->data = value;
    break;
  case DataSymbol:
    if(!value) {
      setError("Symbol value must be given for for the symbol data type.", NULL, 0);
      free(obj);
      return NULL;
    }
    else {
      obj->data = malloc(sizeof(char) * (strlen((char *)value) + 1));
      checkNullInit(obj->data);
      strcpy((char *)(obj->data), (char *)value);
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

struct TLString **getTLString(struct TLObject *obj) {
  if(obj && obj->dt == DataString && obj->data)
    return (struct TLString **)(&(obj->data));
  else
    return NULL;
}

struct TLCons *makeTLCons(struct TLObject *car, struct TLObject *cdr) {
  struct TLCons *val = malloc(sizeof(struct TLCons));
  checkNullInit(val);
  val->car = malloc(sizeof(struct TLObject *));
  checkNullInit(val->car);
  val->cdr = malloc(sizeof(struct TLObject *));
  checkNullInit(val->cdr);
  *(val->car) = car;
  *(val->cdr) = cdr;
  return val;
}

struct TLObject **TLCar(struct TLCons **cons, int *fail) {
  if(cons) {
    if(*cons) {
      if((**cons).car)
	return (**cons).car;
      else
	return NULL;
    }
  }
  setError("Null pointer given to 'TLCar' function.", NULL, 0);
  *fail = 1;
  return NULL;
}

struct TLObject **TLCdr(struct TLCons **cons, int *fail) {
  if(cons) {
    if(*cons) {
      if((**cons).cdr)
	return (**cons).cdr;
      else
	return NULL;
    }
  }
  setError("Null pointer given to 'TLCdr' function.", NULL, 0);
  *fail = 1;
  return NULL;
}

struct TLCons **getTLCons(struct TLObject *obj) {
  if(obj && obj->dt == DataCons && obj->data)
    return (struct TLCons **)(&(obj->data));
  else
    return NULL;
}

struct TLStruct *makeTLStruct(struct structField *members, int size) {
  struct TLStruct *val = malloc(sizeof(struct TLStruct));
  checkNullInit(val);
  val->members = members;
  val->size = size;
  return val;
}

struct TLObject **getField(struct TLStruct **structure, const char *symbol, int *fail) {
  if(structure && *structure) {
    struct structField *fields = (**structure).members;
    if(fields) {
      for(int i = 0; i < (**structure).size; i++) {
	if(strcmp((*(fields + i)).sname, symbol) == 0)
	  return &((*(fields + i)).value);
      }
      setError("Given field is not in given structure.", NULL, 0);
    }
    else {
      setError("The members field of given structure points to null.", NULL, 0);
    }
  }
  else {
    setError("Null pointer given to 'getField' function.", NULL, 0);
  }
  *fail = 1;
  return NULL;
  
}

struct TLStruct **getTLStruct(struct TLObject *obj) {
  if(obj) {
    return (struct TLStruct **)(&(obj->data));
  }
  else
    return NULL;
}

void cleanTLCons(struct TLCons **cons) {
  if(cons && *cons) {
    if((**cons).car) {
      cleanTLObject((**cons).car);
    }
    if((**cons).cdr) {
      cleanTLObject((**cons).cdr);
    }
    free(*cons);
  }
}

void cleanTLStruct(struct TLStruct **structure) {
  if(structure && *structure) {
    if((**structure).members) {
      for(int i = 0; i < (**structure).size; i++) {
	if(((**structure).members + i)->value)
	  cleanTLObject(&(((**structure).members + i)->value));
      }
      free((**structure).members);
    }
    free(*structure);
  }
}

void cleanTLObject(struct TLObject **obj) {
  if(obj && *obj) {
    if((**obj).data) {
      switch((**obj).dt) {
      case DataString:
	cleanTLString(getTLString(*obj));
	break;
      case DataCons:
	cleanTLCons(getTLCons(*obj));
	break;
      case DataStructure:
	cleanTLStruct(getTLStruct(*obj));
	break;
      default:
	free((**obj).data);
	break;
      }
    }
    free(*obj);
  }
}
