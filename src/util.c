#include <string.h>
#include <ctype.h>
#include "util.h"

int isNumber(char *string) {
  int useDecimal = 0;
  for(int i = 0; i < strlen(string); i++) {
    char c = *(string + i);
    if (c == '.') {
      if(useDecimal)
	return 0;
      else
	useDecimal = 1;
    }
    else if(!isdigit(c)) {
      if(i == 0 && c == '-') {
      }
      else
	return 0;
    }
  }
  return 1;
}
