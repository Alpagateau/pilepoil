#include <stdio.h>
#include <stdlib.h>
#include "vm.h"
#include "stack.h"

int main(int argc, char** argv)
{
  int buffer[256] = {};
  int buffer2[256] = {}; 
  int buffer3[256] = {};

  struct stack_vm vm = stack_vm_empty_vm();
  struct stack_vm vm2 = stack_vm_empty_vm();
  vm.stack = new_stack(buffer2, 256);
  vm2.stack = new_stack(buffer3, 256);
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
  // This prgm computes the first 16 fibonacci numbers
  char prg[] = {
    PUSH, 0x00, 0x00, 0x00, 0x00,
    PUSH, 0x00, 0x00, 0x00, 0x01,
    PUSH, 0x00, 0x00, 0x00, 0x04,
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
    HALT
  };

  printf("Program length : %ld\n", sizeof(prg));
  stack_vm_load_prg(&vm, prg, sizeof(prg));

  while(vm.ROM[vm.pc] != HALT)
  {
    stack_vm_step(&vm);
    if(vm.pc > sizeof(prg))
      break;
  } 
  if(argc > 1)
  {
    FILE* source = fopen(argv[1], "rb");
    char data[256] = {};
    fread(data, 1, 255, source);
    fclose(source);

    printf("=== Assembled version test ===\n");

    stack_vm_load_prg(&vm2, data, sizeof(data));
    while(vm2.ROM[vm2.pc] != HALT)
    {
      stack_vm_step(&vm2);
      if(vm2.pc > sizeof(prg))
        break;
    }
  }
  
  return 0;
}
