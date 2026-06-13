#ifndef PARSETYPE_H
#define PARSETYPE_H

#include "dataType.h"

enum parseError { not_decimal, multiple_dot };

enum dataType parseType(char *token);

#endif
