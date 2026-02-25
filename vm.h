#ifndef STACK_VM_H
#define STACK_VM_H

#include "stack.h"
#define STACK_ROM_SIZE 256
#define STACK_FT_SIZE 64

enum OP
{
  NOP = 0,
  //STACK MANIPULATION
  PUSH,
  POP,
  SWP,
  ROLL,
  RWD,
  DUP,
  //MATH OPERATION
  ADD,
  SUB,
  MUL,
  SHR,
  SHL,
  //VM CALLS
  PPC,
  JMP,
  JPS,
  JPZ,
  JNZ,
  JPN, 
  JPP,
  CALL,
  VMCALL,
  LNK,
  //DEBUG
  DEBUG,
  HALT
};

struct named_function
{
  char name[64];
  int idx;
  void (*func)(struct stack* s);
};

struct stack_vm
{
  struct stack stack;
  char ROM[STACK_ROM_SIZE];
  struct named_function func_table[STACK_FT_SIZE];
  int func_table_num;
  int pc;
  struct
  {
    unsigned char zero:1;
    unsigned char negative:1;
  }flags;
};

//Create an empty VM. The stack still needs to be initialized 
struct stack_vm stack_vm_empty_vm();

//Load a program into the vm's rom
int stack_vm_load_prg(struct stack_vm* vm, char* prg, int size);

//Take 1 step into the execution
int stack_vm_step(struct stack_vm*);

const char* opcode_name(enum OP);

#endif
