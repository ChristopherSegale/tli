#ifndef DATATYPES_H
#define DATATYPES_H

enum dataType { t, nil, integer, decimal, array, structure, cons, list, function, symbol };

struct TLObject {
  enum dataType;
  void *value;
};

struct TLObject *makeObject(enum dataType);
void assignValue(enum dataType, struct TLObject **obj);
void cleanTLObject(struct TLObject **obj);

#endif
