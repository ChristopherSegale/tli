#include <stdlib.h>
#include "argParse.h"

struct argParse* initArgs() {
  struct argParse *r = malloc(sizeof(struct argParse));
  r->compFlag = 0;
  r->loadFlag = 0;
  r->interFlag = 0;
  r->helpFlag = 0;
  return r;
}

struct argParse* parseArgs(enum failure *f, int argc, char **argv) {
  int i = 2;
  char *a;
  struct argParse *r = initArgs();
  while(i < argc) {
    a = *(argv + i);
    char j = *a;
    int k = 0;
    while(j) {
      if(k <= 0 && j != '-') {
	if(i <= 2) {
	  *f = noDash;
	  free(r);
	  return NULL;
	}
	break;
      }
      else {
	switch (j) {
	case 'c':
	  r->compFlag = 1;
	  break;
	case 'l':
	  r->loadFlag = 1;
	  break;
	case 'i':
	  r->interFlag = 1;
	  break;
	case 'h':
	  r->helpFlag = 1;
	  break;
	}
      }
      k++;
      j = *(a + k);
    }
    i++;
  }
  return r;
}
