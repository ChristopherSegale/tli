#include <stdio.h>
#include <stdlib.h>
#include "testUtil.h"
#include "../util.h"
#include "../error.h"
#include "../TLString.h"

int main(int argc, char *argv[]) {
  uint8_t test1[] = {'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'};
  uint8_t test2[] = {'t', 'e', 's', 't', 'i', 'n', 'g', ' ', 't', 'e', 's', 't', 'i', 'n', 'g'};
  printf("%d\n%d\n", sizeof(test1) / sizeof(test1[0]), sizeof(test2) / sizeof(test2[0]));
  return 0;
}
