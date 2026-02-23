#ifndef STACK_ASM_H
#define STACK_ASM_H

#include <stdio.h>
#include <stdbool.h>
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
  struct token last_token;
};

struct marker
{
  char name[255];
  int position;
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
bool accept(struct parser* p, enum token_type tt);
void expect(struct parser* p, enum token_type tt);
void advance(struct parser* p);

char parse_opcode(struct parser* p);
int parse_integer(struct parser* p);
struct marker parse_marker(struct parser* p);

int assemble(struct parser* p, char* out, int max_len);

const char* token_type_name(enum token_type t);

#endif
