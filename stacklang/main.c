#include <stdio.h>
#include "ast.h"

int main()
{
  struct AST* test_ast = ast(OP,
                             op(
                              '-',
                              ast(OP, op(
                                '+',
                                ast(INT_LIT, int_lit(3)),
                                ast(INT_LIT, int_lit(2))
                              ),
                              ast(INT_LIT, int_lit(5))
                              )
                             )
                             );
  printf("Eval : %d\n", evaluate(test_ast));
  return 0;
}
