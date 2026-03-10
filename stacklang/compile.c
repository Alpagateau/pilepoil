#include <stdlib.h>
#include <string.h>
#include "compile.h"

#define BASE_ALLOC 256

int compile_ast(struct AST* a, char* out)
{
  char* prg = malloc(BASE_ALLOC);
  memset(prg, 0, BASE_ALLOC);
  int size = 0;
  int capacity = BASE_ALLOC;

}
