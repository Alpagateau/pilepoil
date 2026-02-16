#include "stack_asm.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "vm.h"

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
  if(t.type == IDENTIFIER)
  {
    printf("[LEXED] ID %s\n", t.string);
    return t;
  }
  t = lex_int(l);
  if(t.type == IMMEDIATE)
  {
    printf("[LEXED] IM %d\n", t.value);
    return t;
  }
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

void advance(struct parser* p)
{
  p->last_token = next_token(p->lexer);
}

struct parser new_parser(struct lexer* l)
{
  struct parser p = {
    .lexer = l
  };
  advance(&p);
  return p;
}

bool accept(struct parser* p, enum token_type tt)
{
  return (p->last_token.type == tt);
}

void expect(struct parser* p, enum token_type tt)
{
  if(!accept(p,tt))
  {
    printf("[Error] Expecting %d but got %d(%s)\n", tt, p->last_token.type, p->last_token.string);
    exit(1);
  }
}

char parse_opcode(struct parser *p)
{
  char out = NOP;
  expect(p, IDENTIFIER);
  if(strcmp(p->last_token.string, "push") == 0)
  {
    out = PUSH;
  }
  else if(strcmp(p->last_token.string, "pop") == 0)
  {
    out = POP;
  }
  else if(strcmp(p->last_token.string, "swp") == 0)
  {
    out = SWP;
  }
  else if(strcmp(p->last_token.string, "roll") == 0)
  {
    out = ROLL;
  }
  else if(strcmp(p->last_token.string, "rwd") == 0)
  {
    out = RWD;
  }
  else if(strcmp(p->last_token.string, "dup") == 0)
  {
    out = DUP;
  }
  else if(strcmp(p->last_token.string, "add") == 0)
  {
    out = ADD;
  }
  else if(strcmp(p->last_token.string, "sub") == 0)
  {
    out = SUB;
  }
  else if(strcmp(p->last_token.string, "debug") == 0)
  {
    out = DEBUG;
  }
  else if(strcmp(p->last_token.string, "jnz") == 0)
  {
    out = JNZ;
  }

  advance(p);
  return out;
}

int parse_integer(struct parser* p)
{
  expect(p, IMMEDIATE);
  int i = p->last_token.value;
  advance(p);
  return i;
}

int assemble(struct parser* p, char* out, int max_len)
{ 
  int head = 0;
  unsigned int argumented = 1 << PUSH | 
    1 << ROLL | 
    1 << RWD  | 
    1 << JPZ  | 
    1 << JNZ  | 
    1 << JPN  | 
    1 << JMP;
  printf("Argumented : %X\n", argumented);
  while(head < max_len && p->last_token.type != ERROR)
  {
    unsigned char opcode = parse_opcode(p);
    out[head++] = opcode;
    if( ((1 << opcode) & argumented) != 0)
    {
      printf("Opcode : %X | %X\n", opcode, opcode & argumented);
      int v = parse_integer(p);
      char a = (v & 0xFF000000) >> 24;
      char b = (v & 0x00FF0000) >> 16;
      char c = (v & 0x0000FF00) >>  8;
      char d = (v & 0x000000FF) >>  0;

      out[head++] = a;
      out[head++] = b;
      out[head++] = c;
      out[head++] = d;
    }
  }
  return head;
}

int main(int argc, char** argv)
{
  if(argc < 2)
  {
    printf("Please provide a stack assembly file to assemble....\n");
    return 0;
  }
  char test[255] = {};
  struct lexer l = new_lexer(fopen(argv[1], "r"));
  struct parser p = new_parser(&l);
  assemble(&p, test, 255);
 
  char title[127] = {};
  strcpy(title, argv[1]);
  strcat(title, ".stack");
  fclose(l.input);
  FILE* out = fopen(title, "wb");
  fwrite(test, 1, 255, out);
  fclose(out);
  return 0;
}
