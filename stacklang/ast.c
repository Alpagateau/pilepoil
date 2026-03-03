#include <stdlib.h>
#include "ast.h"

struct AST
{
  enum node_type type;
  void* next_node;
};

struct int_lit
{
  int val;
};

struct op
{
  char o;
  struct AST* le;
  struct AST* re;
};


struct AST* ast(enum node_type t, void* n)
{
  struct AST* a = (struct AST*)malloc(sizeof(*a));
  *a = (struct AST){
      .type = t,
      .next_node = n
    };
  return a;
}

struct int_lit* int_lit(int v)
{
  struct int_lit* i = (struct int_lit*)malloc(sizeof(*i));
  i->val = v;
  return i;
}

struct op* op(char c, struct AST* l, struct AST* r)
{
  struct op* i = (struct op*)malloc(sizeof(*i));
  i->o = c;
  i->le = l;
  i->re = r;
  return i;
}

int evaluate(struct AST* a)
{
  switch (a->type) {
    case INT_LIT:
      return ((struct int_lit*)a->next_node)->val;
      break;
    case OP:
      return eval_op(
        (struct op*)a->next_node
      );
      break;
  }
}

int eval_op(struct op* o)
{
  int l = evaluate(o->le);
  int r = evaluate(o->re);
  switch(o->o)
  {
    case '+':
      return l + r;
      break;
    case '-':
      return l - r;
      break;
    case '*':
      return l * r;
      break;
    case '/':
      return l / r;
      break;
  }
  return 0;
}

void free_ast(struct AST* a)
{
  switch(a->type)
  {
    case INT_LIT:
      free_int_lit(a->next_node);
      break;
    case OP:
      free_int_lit(a->next_node);
      break;
  }
  free(a);
}

void free_int_lit(struct int_lit* il)
{
  free(il);
}

void free_op(struct op* o)
{
  free_ast(o->le);
  free_ast(o->re);
  free(o);
}
