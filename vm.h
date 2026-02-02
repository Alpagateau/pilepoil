#ifndef STACK_VM_H
#define STACK_VM_H

#include "stack.h"
#define STACK_ROM_SIZE 256

enum OP
{
  NOP = 0,
  PUSH,
  POP,
  SWP,
  ROLL,
  RWD,
  DUP,
  DEBUG
};

struct stack_vm
{
  struct stack stack;
  char ROM[STACK_ROM_SIZE];
  int pc;
};

struct stack_vm stack_vm_empty_vm();
int stack_vm_load_prg(struct stack_vm* vm, char* prg, int size);

int stack_vm_step(struct stack_vm*);

#endif
