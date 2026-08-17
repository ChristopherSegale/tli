#include <stdio.h>
#include <stdint.h>
#include "testUtil.h"

void printTest(int testNumber, int test) {
  char *result;
  if(test)
    result = "PASSED";
  else
    result = "FAILED";
  printf("Test %d: %s\n", testNumber, result);
}

int TLStringcmp(uint8_t *a, uint8_t *b, int count) {
  for(int i = 0; i < count; i++) {
    if(*(a + i) != *(b + i))
      return 0;
  }
  return 1;
}
