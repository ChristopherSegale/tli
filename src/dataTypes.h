#ifndef DATATYPES_H
#define DATATYPES_H
#include <stdint.h>

enum dataType { DataT, DataNil, DataByte, DataInteger, DataDecimal, DataArray, DataString, DataCons, DataList, DataStructure, DataFunction, DataSymbol };

struct TLObject {
  enum dataType dt;
  void *data;
};

struct TLArray {
  int size;
  int capacity;
  struct TLObject **members;
};

struct TLCons {
  struct TLObject **car;
  struct TLObject **cdr;
};

struct structField {
  char *sname;
  struct TLObject *value;
};

struct TLStruct {
  struct structField *members;
  int size;
};

struct TLObject *makeObject(enum dataType d, void *value);
int changeByte(struct TLObject **obj, uint8_t value);
int changeInteger(struct TLObject **obj, int value);
int changeDecimal(struct TLObject **obj, double value);
uint8_t getByte(struct TLObject *obj, int *fail);
int getInteger(struct TLObject *obj, int *fail);
double getDecimal(struct TLObject *obj, int *fail);
struct TLArray *makeTLArray(struct TLObject **members, int size, int capacity);
struct TLObject **getArrayElement(struct TLArray **array, int index);
int pushArrayElement(struct TLArray **array, struct TLObject *obj);
struct TLArray **getTLArray(struct TLObject *obj);
struct TLString **getTLString(struct TLObject *obj);
struct TLCons *makeTLCons(struct TLObject *car, struct TLObject *cdr);
struct TLObject **TLCar(struct TLCons **cons, int *fail);
struct TLObject **TLCdr(struct TLCons **cons, int *fail);
struct TLCons **getTLCons(struct TLObject *obj);
struct TLStruct *makeTLStruct(struct structField *members, int size);
struct TLObject **getField(struct TLStruct **structure, const char *symbol, int *fail);
struct TLStruct **getTLStruct(struct TLObject *obj);
void cleanTLArray(struct TLArray **array);
void cleanTLCons(struct TLCons **cons);
void cleanTLStruct(struct TLStruct **structure);
void cleanTLStruct(struct TLStruct **structure);
void cleanTLObject(struct TLObject **obj);

#endif
