#ifndef DATATYPES_H
#define DATATYPES_H
#include <stdint.h>

enum dataType { DataT, DataNil, DataByte, DataInteger, DataDecimal, DataArray, DataString, DataStructure, DataCons, DataList, DataFunction, DataSymbol };

struct TLObject {
  enum dataType dt;
  void *value;
};

struct TLObject *makeObject(enum dataType d);
int initValue(struct TLObject **obj, enum dataType);
int assignByte(struct TLObject **obj, uint8_t value);
int assignInteger(struct TLObject **obj, int value);
int assignDecimal(struct TLObject **obj, double value);
int assignArray(struct TLObject **obj, struct TLArray *value);
int assignString(struct TLObject **obj, char *string);
int assignStructure(struct TLObject **obj, struct TLStruct *value);
int assignCons(struct TLObject **obj, struct TLCons *value);
int assignList(struct TLObject **obj, struct TLList *value);
int assignFunction(struct TLObject **obj, struct TLFunc *value);
int assignSymbol(struct TLObject **obj, char *value);
void cleanTLObject(struct TLObject **obj);

#endif
