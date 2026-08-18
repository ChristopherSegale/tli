#include <stdio.h>
#include <stdlib.h>
#include "testUtil.h"
#include "../util.h"
#include "../error.h"
#include "../TLString.h"

int main(int argc, char *argv[]) {
  int tn = 1, fail = 0;
  uint8_t s1[] = {'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'};
  uint8_t s2[] = {'t', 'e', 's', 't', 'i', 'n', 'g', ' ', 'c', 'o', 'd', 'e'};
  uint8_t s3[] = {'c', 'o', 'd', 'e'};
  printTest(tn++, TLStringcmp(s1, s1, sizeof(s1) / sizeof(s1[0])));

  struct TLString t1 = makeTLString(NULL, 5);
  printTest(tn++, t1.string == NULL);
  printTest(tn++, t1.size == 0);
  pushTLChar(&t1, 'h');
  printTest(tn++, getTLChar(t1, 0) == 'h');
  pushTLChar(&t1, 'w');
  printTest(tn++, getTLChar(t1, 1) == 'w');
  free(t1.string);

  struct TLString t2 = makeTLString(s1, sizeof(s1) / sizeof(s1[0]));
  printTest(tn++, TLStringcmp(t2.string, s1, sizeof(s1) / sizeof(s1[0])));
  printTest(tn++, t2.size == sizeof(s1) / sizeof(s1[0]));
  free(t2.string);

  struct TLString t3 = makeTLString(s2, sizeof(s2) / sizeof(s2[0]));
  struct TLString t4 = TLSubstring(t3, 8, 11, &fail);
  free(t3.string);
  if(fail)
    printError();
  else {
    printTest(tn++, TLStringcmp(t4.string, s3, sizeof(s3) / sizeof(s3[0])));
    printTest(tn++, t4.size == sizeof(s3) / sizeof(s3[0]));
    free(t4.string);
  }
  return 0;
}
