#include <stdio.h>
#include <stdlib.h>
#include "vm.h"
#include "stack.h"

int main(int argc, char** argv)
{
  int buffer[256] = {};
  int buffer2[256] = {};

  struct stack_vm vm = stack_vm_empty_vm();
  vm.stack = new_stack(buffer2, 256);
  struct stack st = new_stack(buffer, 256);
  if(argc > 1)
  {
    for(int i = 1; i < argc; i++)
    {
      stack_push(&st, atoi(argv[i])); 
      stack_push(&vm.stack, atoi(argv[i]));
    }
  }

  printf("=== MANUAL TEST ===\n");
  print_stack(&st);

  stack_push(&st, 24);
  print_stack(&st);

  stack_dup(&st);
  stack_push(&st, 0);
  stack_swap(&st);
  print_stack(&st);

  stack_roll(&st, 3);
  print_stack(&st);

  stack_rewind(&st, 3);
  print_stack(&st);


  stack_pop(&st);
  print_stack(&st);
 
  printf("=== VM TEST === \n");
  // This prgm computes the first 0x24 fibonacci numbers
  char prg[] = {
    PUSH, 0x00, 0x00, 0x00, 0x00,
    PUSH, 0x00, 0x00, 0x00, 0x01,
    PUSH, 0x00, 0x00, 0x00, 0x24,
    DEBUG,
    PUSH, 0x00, 0x00, 0x00, 0x01,
    SUB,
    RWD,  0x00, 0x00, 0x00, 0x03,
    DUP,
    ROLL, 0x00, 0x00, 0x00, 0x03, 
    ADD,
    ROLL, 0x00, 0x00, 0x00, 0x03,
    DEBUG,
    JNZ,  0x00, 0x00, 0x00, 0x0F,
    NOP,
    NOP
  };

  stack_vm_load_prg(&vm, prg, sizeof(prg));

  while(vm.ROM[vm.pc] != NOP)
  {
    stack_vm_step(&vm);
  }
  
  return 0;
}
