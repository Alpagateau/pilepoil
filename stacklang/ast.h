#ifndef AST_H
#define AST_H

#include <stdio.h>

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

//
void print_ast(int depth, struct AST*);
void print_int_lit(struct int_lit* il);
void print_op(int depth, struct op* o);

//
void compute_ast(FILE*, struct AST*);
void compute_op(FILE*, struct op*);
void compute_int_lit(FILE*, struct int_lit*);
#endif
