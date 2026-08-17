#include <stdio.h>
#include <stdlib.h>
#include "testUtil.h"
#include "../error.h"
#include "../dataTypes.h"

int byteTest(uint8_t a, uint8_t b) {
  return a == b;
}

int intTest(int a, int b) {
  return a == b;
}

int decTest(double a, double b) {
  return a == b;
}

void checkFail(int fail) {
  if(fail) {
    setError("Retrieving data failed.", NULL, 0);
    printError();
    exit(1);
  }
}

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

  return 0;
}
