#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parseType.h"
#include "dataTypes.h"

int checkT(char *token) {
  if (strcmp(token, "T") == 0)
    return 1;
  else
    return 0;
}

int checkNil(char *token) {
  if (strcmp(token, "NIL") == 0)
    return 1;
  else
    return 0;
}

int checkInteger(char *token) {
  int i = 0, length = strlen(token);
  for(char c = *(token + i); i < length; i++, c = *(token + i)) {
    if (!isdigit(c))
      return 0;
  }
  return 1;
}

int checkDecimal(char *token, enum parseError *error) {
  int i = 0, length = strlen(token), isDot = 0;
  for (char c = *(token + i); i < length; i++, c = *(token + i)) {
    if (!isdigit(c) || c != '.') {
      *error = not_decimal;
      return 0;
    }
    if (c == '.') {
      if (!isDot)
	isDot = 1;
      else {
	*error = multiple_dot;
	return 0;
      }
    }
  }
  return 1;
}

int checkArray(char *token) {
  return (*token == '#') &&
    (*(token + 1) == '(') &&
    (*(token + (strlen(token) - 1)) == ')');
}

int checkStructure(char *token) {
  return (*token == '#') &&
    (*(token + 1) == 'S') &&
    (*(token + 2) == '(') &&
    (*(token + (strlen(token) - 1)) == ')');
}
