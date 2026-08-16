#include <stdio.h>
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

void printTest(int testNumber, int test) {
  char *result;
  if(test)
    result = "PASSED";
  else
    result = "FAILED";
  printf("Test %d: %s\n", testNumber, result);
}

int main(int argc, char *argv[]) {
  int tn = 1;
  struct TLObject *obj = makeObject(DataByte, NULL);
  printTest(tn++, byteTest(*(uint8_t *)((*obj).data), 0));

  changeByte(&obj, 'a');
  printTest(tn++, byteTest(*(uint8_t *)((*obj).data), 'a'));
  cleanTLObject(&obj);

  uint8_t a = 'b';
  obj = makeObject(DataByte, &a);
  printTest(tn++, byteTest(*(uint8_t *)((*obj).data), 'b'));
  cleanTLObject(&obj);

  obj = makeObject(DataInteger, NULL);
  printTest(tn++, intTest(*(int *)((*obj).data), 0));

  changeInteger(&obj, 5);
  printTest(tn++, intTest(*(int *)((*obj).data), 5));
  cleanTLObject(&obj);

  int b = 65;
  obj = makeObject(DataInteger, &b);
  printTest(tn++, intTest(*(int *)((*obj).data), 65));
  cleanTLObject(&obj);

  obj = makeObject(DataDecimal, NULL);
  printTest(tn++, decTest(*(double *)((*obj).data), 0.0));

  changeDecimal(&obj, 4.0);
  printTest(tn++, decTest(*(double *)((*obj).data), 4.0));
  cleanTLObject(&obj);

  double c = 7.5;
  obj = makeObject(DataDecimal, &c);
  printTest(tn++, decTest(*(double *)((*obj).data), 7.5));
  cleanTLObject(&obj);

  return 0;
}
