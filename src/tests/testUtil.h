#ifndef TESTUTIL_H
#define TESTUTIL_H
#include <stdint.h>

void printTest(int testNumber, int test);
int TLStringcmp(uint8_t *a, uint8_t *b, int count);
int byteTest(uint8_t a, uint8_t b);
int intTest(int a, int b);
int decTest(double a, double b);

#endif
