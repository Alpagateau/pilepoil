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
%token <c> OPE
%type <a> val atom expr prog
%left OPE
%start prog
%parse-param {struct AST** a}

%%

prog : expr        {*a = $1;}
     | expr '\n'   {*a = $1;}
     ;

expr: atom OPE expr {$$ = ast(OP, op($2, $1, $3)};
    | atom
    ;

atom: '(' expr ')' {$$ = $2;}
    | val
    ;

val: INT {$$ = ast(INT_LIT, int_lit($1));} 
   ;

%%

void yyerror(struct AST** a, const char*s)
{ 
  fprintf(stderr, "%s\n", s);
}
