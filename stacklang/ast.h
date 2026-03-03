#ifndef AST_H
#define AST_H

enum node_type
{
  INT_LIT,
  OP
};

struct AST;
struct int_lit;
struct op;

//Constructors
struct AST* ast(enum node_type t, void* n);
struct int_lit* int_lit(int v);
struct op* op(char c, struct AST* l, struct AST* r);

//Utils
int evaluate(struct AST* a);
int eval_op(struct op* o);

//Free
void free_ast(struct AST* a);
void free_int_lit(struct int_lit* il);
void free_op(struct op* o);

#endif
