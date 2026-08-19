#include <stdio.h>
#include <stdlib.h>
#include "testUtil.h"
#include "../error.h"
#include "../TLString.h"
#include "../dataTypes.h"

int main(int argc, char *argv[]) {
  int tn = 1;
  int fail = 0;
  struct TLObject *obj = makeObject(DataByte, NULL);
  printTest(tn++, byteTest(getByte(obj, &fail), 0));

  changeByte(&obj, 'a');
  printTest(tn++, byteTest(getByte(obj, &fail), 'a'));
  cleanTLObject(&obj);

  uint8_t a = 'b';
  obj = makeObject(DataByte, &a);
  printTest(tn++, byteTest(getByte(obj, &fail), 'b'));
  cleanTLObject(&obj);

  obj = makeObject(DataInteger, NULL);
  printTest(tn++, intTest(getInteger(obj, &fail), 0));

  changeInteger(&obj, 5);
  printTest(tn++, intTest(getInteger(obj, &fail), 5));
  cleanTLObject(&obj);

  int b = 65;
  obj = makeObject(DataInteger, &b);
  printTest(tn++, intTest(getInteger(obj, &fail), 65));
  cleanTLObject(&obj);

  obj = makeObject(DataDecimal, NULL);
  printTest(tn++, decTest(getDecimal(obj, &fail), 0.0));

  changeDecimal(&obj, 4.0);
  printTest(tn++, decTest(getDecimal(obj, &fail), 4.0));
  cleanTLObject(&obj);

  double c = 7.5;
  obj = makeObject(DataDecimal, &c);
  printTest(tn++, decTest(getDecimal(obj, &fail), 7.5));
  cleanTLObject(&obj);

  obj = makeObject(DataString, NULL);
  struct TLString *d = (struct TLString *)obj->data;
  printTest(tn++, !d->string);
  printTest(tn++, intTest(d->size, 0));
  cleanTLObject(&obj);

  uint8_t e[] = {'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'};
  struct TLString f = makeTLString(e, sizeof(e) / sizeof(e[0]));
  obj = makeObject(DataString, &f);
  d = (struct TLString *)obj->data;
  printTest(tn++, TLStringcmp(d->string, e, sizeof(e) / sizeof(e[0])));
  printTest(tn++, intTest(d->size, sizeof(e) / sizeof(e[0])));
  cleanTLString(d);
  cleanTLObject(&obj);

  return 0;
}
