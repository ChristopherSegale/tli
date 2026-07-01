#include <stdlib.h>
#include "lexer.h"

struct lexeme *read(char *expression) {
  struct lexeme *ast = malloc(sizeof(struct lexeme) * 512);
  if (!ast)
    return NULL;
  return ast;
}
