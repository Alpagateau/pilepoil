#include "stack_asm.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "vm.h"

#define debug_print(...) if(verbose) printf(__VA_ARGS__)

bool verbose = false;

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
    debug_print("[LEXED] ID %s\n", t.string);
    return t;
  }
  t = lex_int(l);
  if(t.type == IMMEDIATE)
  {
    debug_print("[LEXED] IM %d\n", t.value);
    return t;
  }
  if(peek(l) == '@')
  {
    consume(l);
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
    while(isalnum(peek(l)) || peek(l) == '_')
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
    debug_print("[Error] Expecting %s but got %s : (%s)\n",
                token_type_name(tt),
                token_type_name(p->last_token.type), 
                p->last_token.string);
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
  else if(strcmp(p->last_token.string, "mul") == 0)
  {
    out = MUL;
  }
  else if(strcmp(p->last_token.string, "shr") == 0)
  {
    out = SHR;
  }
  else if(strcmp(p->last_token.string, "shl") == 0)
  {
    out = SHL;
  }
  else if(strcmp(p->last_token.string, "ppc") == 0)
  {
    out = PPC;
  }
  else if(strcmp(p->last_token.string, "jmp") == 0)
  {
    out = JMP;
  }
  else if(strcmp(p->last_token.string, "jps") == 0)
  {
    out = JPS;
  }
  else if(strcmp(p->last_token.string, "jpz") == 0)
  {
    out = JPZ;
  }
  else if(strcmp(p->last_token.string, "jnz") == 0)
  {
    out = JNZ;
  }
  else if(strcmp(p->last_token.string, "jpn") == 0)
  {
    out = JPN;
  }
  else if(strcmp(p->last_token.string, "jpp") == 0)
  {
    out = JPP;
  }
  else if(strcmp(p->last_token.string, "call") == 0)
  {
    out = CALL;
  }
  else if(strcmp(p->last_token.string, "debug") == 0)
  {
    out = DEBUG;
  }
  else if(strcmp(p->last_token.string, "halt") == 0)
  {
    out = HALT;
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

struct marker parse_marker(struct parser* p)
{
  struct marker m;  
  expect(p, ATSIGN);
  advance(p);
  expect(p, IDENTIFIER); 
  strcpy(m.name, p->last_token.string);
  advance(p);
  return m; 
}

int assemble(struct parser* p, char* out, int max_len)
{ 

  printf("Compiling:\n");
  struct marker markers[127] = {};
  struct marker linkables[127] = {};
  int marker_num = 0;
  int linkable_num = 0;

  int head = 0;
  unsigned int argumented = 
    1 << PUSH | 
    1 << ROLL | 
    1 << RWD;

  unsigned int addressable = 
    1 << JPZ  | 
    1 << JNZ  | 
    1 << JPN  | 
    1 << JMP  |
    1 << CALL;

  debug_print("Argumented : %X\n", argumented);
  while(head < max_len && p->last_token.type != ERROR)
  {
    if(accept(p, ATSIGN))
    {
      markers[marker_num] = parse_marker(p);
      markers[marker_num].position = head-1;
      printf("\tMarker \"%s\" at %d\n", 
             markers[marker_num].name, 
             markers[marker_num].position);
      marker_num++;
      continue;
    }else{
      unsigned char opcode = parse_opcode(p);
      out[head++] = opcode;
      if( ((1 << opcode) & argumented) != 0)
      {
        debug_print("Opcode : %X | %X\n", opcode, opcode & argumented);
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
      else if(((1 << opcode) & addressable) != 0)
      {
        printf("\topcode : \t%s => %s\n", opcode_name(opcode), p->last_token.string);
        linkables[linkable_num] = (struct marker){.position = head}; 
        strcpy(linkables[linkable_num].name, p->last_token.string);
        linkable_num++;
        out[head++] = 0xFF;
        out[head++] = 0xFF;
        out[head++] = 0xFF;
        out[head++] = 0xFF;
      }
    }
  }

  printf("Linking :\n");
  int old_head = head;

  for(int i = 0; i < linkable_num; i++)
  {
    int linked_pos = -1;
    for(int m = 0; m < marker_num; m++)
    {
      if(strcmp(markers[m].name, linkables[i].name) == 0) 
      {
        linked_pos = markers[m].position;
        break;
      }
    }
    printf("\t%s => %d\n", linkables[i].name, linked_pos);
    if(linked_pos < 0) 
    {
      fflush(stdout);
      exit(1);
    }
    head = linkables[i].position; 
    int v = linked_pos;
    char a = (v & 0xFF000000) >> 24;
    char b = (v & 0x00FF0000) >> 16;
    char c = (v & 0x0000FF00) >>  8;
    char d = (v & 0x000000FF) >>  0;

    out[head++] = a;
    out[head++] = b;
    out[head++] = c;
    out[head++] = d;
    
  }
  head = old_head;
  return head;
}

const char* token_type_name(enum token_type t)
{
  switch(t)
  {
    case IDENTIFIER:
      return "Identifier";
      break;
    case IMMEDIATE:
      return "Immediate";
      break;
    case ATSIGN:
      return "\'@\'";
      break;
    case ERROR:
      return "Error";
      break;
    default:
      return "You shouldn\'t see this";
  }
}

int main(int argc, char** argv)
{
  if(argc < 2)
  {
    printf(
      "Please provide a stack assembly"
      "file to assemble....\n");
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
