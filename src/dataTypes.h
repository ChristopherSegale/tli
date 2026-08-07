#ifndef DATATYPES_H
#define DATATYPES_H

enum dataType { t, nil, byte, integer, decimal, array, string, structure, cons, list, function, symbol };

struct TLObject {
  enum dataType;
  void *value;
};

struct TLObject *makeObject(enum dataType);
int initValue(struct TLObject **obj, enum dataType);
int assignInteger(struct TLObject **obj, int value);
int assignDecimal(struct TLObject **obj, double value);
int assignArray(struct TLObject **obj, struct TLArray *value);
int assignStructure(struct TLObject **obj, struct TLStruct *value);
int assignCons(struct TLObject **obj, struct TLCons *value);
int assignList(struct TLObject **obj, struct TLList *value);
int assignFunction(struct TLObject **obj, struct TLFunc *value);
int assignSymbol(struct TLObject **obj, char *value);
void cleanTLObject(struct TLObject **obj);

#endif
