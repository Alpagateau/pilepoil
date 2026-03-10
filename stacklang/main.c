#include "ast.h"
#include "y.tab.h"
#include <stdio.h>

extern FILE* yyin;

int main(int argc, char** argv) {
  struct AST *test_ast =
      ast(OP, 
          op('-', 
             ast(OP, 
                 op('+',
                    ast(INT_LIT, int_lit(3)),
                    ast(INT_LIT, int_lit(4))
                 )
                ),
             ast(INT_LIT, int_lit(10))
             )
          );
  printf("Eval : %d\n", evaluate(test_ast));
  print_ast(0, test_ast);

  printf("===========\n");
  if(argc == 1)
  {
    yyin = stdin;
  }
  else
  {
    yyin = fopen(argv[1], "r");
    if(yyin == NULL)
      return 1;
  }
  struct AST* new;
  yyparse(&new);
  print_ast(0, new);

  printf("== COMPILED ==\n");
  compute_ast(stdout, new);

  //free_ast(new);

  fclose(yyin);
  return 0;
}
