#include "stack_asm.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct lexer new_lexer(FILE* input)
{
  fseek(input, 0, SEEK_SET);
  return (struct lexer){.input = input};
}

int peek(struct lexer* l)
{
  char c = fgetc(l->input);
  ungetc(c, l->input);
  return c;
}

int consume(struct lexer* l)
{
  char c = fgetc(l->input);
  return c;
}

void skipSpaces(struct lexer* l)
{
  while(isspace(peek(l)))
  {
    consume(l);
  }
}

struct token next_token(struct lexer* l)
{
  skipSpaces(l);
  struct token t = {};
  t.type = ERROR;
  t = lex_ident(l);
  if(t.type == IDENTIFIER) return t;
  t = lex_int(l);
  if(t.type == IMMEDIATE) return t;
  if(peek(l) == '@')
  {
    return (struct token){
      .type = ATSIGN
    };
  }
  return t;
}

struct token lex_ident(struct lexer* l)
{
  if(isalpha(peek(l)))
  {
    char ident[STR_LEN] = {};
    int s = 0;
    ident[s++] = consume(l);
    while(isalnum(peek(l)))
    {   
      ident[s++] = (char)consume(l);
    }
    struct token t = {
      .type = IDENTIFIER
    };
    strcpy(t.string, ident);
    return t;
  }
  else
  {
    return (struct token){.type = ERROR};
  }
}

struct token lex_int(struct lexer* l)
{
  if(peek(l) == '-' || peek(l) == '+' || isdigit(peek(l)))
  {
    char ident[STR_LEN] = {};
    int s = 0;
    ident[s++] = consume(l);
    while(isdigit(peek(l)))
    {   
      ident[s++] = (char)consume(l);
    }
    struct token t = {
      .type = IMMEDIATE,
      .value = atoi(ident)
    };
    return t;
  }
  else
  {
    return (struct token){.type = ERROR};
  }
}
