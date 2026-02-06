#ifndef STACK_ASM_H
#define STACK_ASM_H

#include <stdio.h>
#define STR_LEN 256

enum token_type 
{
  IDENTIFIER,
  IMMEDIATE,
  ATSIGN,
  ERROR
};

struct token
{
  enum token_type type;
  union{
    char string[STR_LEN];
    int value;
  };
};

struct lexer
{
  FILE* input;
};

struct parser
{
  struct lexer* lexer;
};

//Lexer
struct lexer new_lexer(FILE* input);
struct token next_token(struct lexer* l);
int peek(struct lexer* l);
int consume(struct lexer* l);
//Tokens
void skipSpaces(struct lexer* l);
struct token lex_ident(struct lexer* l);
struct token lex_int(struct lexer* l);

//Parser
struct parser new_parser(struct lexer* l);

#endif
