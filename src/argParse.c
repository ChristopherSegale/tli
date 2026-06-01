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
  char *a;
  struct argParse *r = initArgs();
  for(int i = 1; i < argc; i++) {
    a = *(argv + i);
    int k = 0;
    for(char j = *a; j; k++, j = *(a + k)) {
      if(k <= 0 && j != '-') {
	if(i < 2) {
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
    }
  }
  return r;
}
