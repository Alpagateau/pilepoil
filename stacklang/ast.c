#include <stdio.h>
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

void print_ast(int depth, struct AST* a)
{
  for(int i = 0; i < depth; i++)
    printf("  ");
  switch(a->type)
  {
    case INT_LIT:
      print_int_lit(a->next_node);
      break;
    case OP:
      print_op(depth, a->next_node);
      break;
  }
}
void print_int_lit(struct int_lit* il)
{
  printf(">%d\n", il->val);
}
void print_op(int depth, struct op* o)
{
  printf("\'%c\'\n", o->o);
  print_ast(depth+1, o->le);
  print_ast(depth+1, o->re);
}

void compute_ast(FILE* f, struct AST* a)
{
  if(a == NULL)
    return;
  switch(a->type)
  {
    case INT_LIT:
      compute_int_lit(f, a->next_node);
    case OP:
      compute_op(f, a->next_node);
  }
}

void compute_op(FILE* f, struct op* o)
{
  compute_ast(f, o->le);
  compute_ast(f, o->re);
  switch(o->o)
  {
    case '+':
      fprintf(f, "add");
      break;
    case '-':
      fprintf(f, "sub");
      break;
    case '*':
      fprintf(f, "mul");
  }
  fprintf(f, "\n");
}

void compute_int_lit(FILE* f, struct int_lit* il)
{
  fprintf(f, "push %d", il->val);
}
