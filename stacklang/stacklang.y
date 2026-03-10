%{
#include <stdio.h>
#include "ast.h"

int yylex(void);
void yyerror(struct AST**, const char *);
%}

%union{
  struct AST* a;
  int i;
  char c;
}

%token <i> INT
%token <c> OPS
%token <c> OPP
%type <a> val atom expr prog sum product
%left OPP OPS
%start prog
%define parse.error verbose
%parse-param {struct AST** a}

%%

prog : expr        {*a = $1;}
     | expr '\n'   {*a = $1;}
     ;

expr: sum          {$$ = $1;}
    ;

sum : product OPS product  {$$ = ast(OP, op($2, $1, $3));}
    | product
    ;

product : atom OPP product {$$ = ast(OP, op($2, $1, $3));}
        | atom
        ;

atom: '(' expr ')'         {$$ = $2;}
    | val
    ;

val: INT {$$ = ast(INT_LIT, int_lit($1));} 
   ;

%%

void yyerror(struct AST** a, const char*s)
{ 
  fprintf(stderr, "%s\n", s);
}
