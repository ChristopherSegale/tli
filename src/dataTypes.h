#ifndef DATATYPES_H
#define DATATYPES_H
#include <stdint.h>

enum dataType { DataT, DataByte, DataInteger, DataDecimal, DataArray, DataString, DataStructure, DataCons, DataList, DataFunction, DataSymbol };

struct TLObject {
  enum dataType dt;
  void *data;
};

struct TLCons {
  struct TLObject *car;
  struct TLObject *cdr;
};

struct TLObject *makeObject(enum dataType d, void *value);
int changeByte(struct TLObject **obj, uint8_t value);
int changeInteger(struct TLObject **obj, int value);
int changeDecimal(struct TLObject **obj, double value);
uint8_t getByte(struct TLObject *obj, int *fail);
int getInteger(struct TLObject *obj, int *fail);
double getDecimal(struct TLObject *obj, int *fail);
struct TLString **getTLString(struct TLObject *obj);
struct TLCons *createTLCons(struct TLObject *car, struct TLObject *cdr);
struct TLObject **TLCar(struct TLCons **cons, int *fail);
struct TLObject **TLCdr(struct TLCons **cons, int *fail);
struct TLCons **getTLCons(struct TLObject *obj);
void cleanTLCons(struct TLCons **cons);
int assignArray(struct TLObject **obj, struct TLArray *value);
int assignString(struct TLObject **obj, char *string);
int assignStructure(struct TLObject **obj, struct TLStruct *value);
int assignCons(struct TLObject **obj, struct TLCons *value);
int assignList(struct TLObject **obj, struct TLList *value);
int assignFunction(struct TLObject **obj, struct TLFunc *value);
void cleanTLObject(struct TLObject **obj);

#endif
