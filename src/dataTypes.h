#ifndef DATATYPES_H
#define DATATYPES_H
#include <stdint.h>

enum dataType { DataT, DataByte, DataInteger, DataDecimal, DataArray, DataString, DataCons, DataList, DataStructure, DataFunction, DataSymbol };

struct TLObject {
  enum dataType dt;
  void *data;
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
struct TLString **getTLString(struct TLObject *obj);
struct TLCons *makeTLCons(struct TLObject *car, struct TLObject *cdr);
struct TLObject **TLCar(struct TLCons **cons, int *fail);
struct TLObject **TLCdr(struct TLCons **cons, int *fail);
struct TLCons **getTLCons(struct TLObject *obj);
struct TLStruct *makeTLStruct(struct structField *members, int size);
struct TLObject **getField(struct TLStruct **structure, const char *symbol, int *fail);
void cleanTLCons(struct TLCons **cons);
void cleanTLStruct(struct TLStruct **structure);
void cleanTLObject(struct TLObject **obj);

#endif
