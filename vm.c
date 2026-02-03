#include <stdio.h>
#include "vm.h"
#include "stack.h"

int pack4chars(char c1, char c2, char c3, char c4) {
    return ((int)(((unsigned char)c1) << 24)
            |  (int)(((unsigned char)c2) << 16)
            |  (int)(((unsigned char)c3) << 8)
            |  (int)((unsigned char)c4));
}


struct stack_vm stack_vm_empty_vm()
{
  return (struct stack_vm){};
}


int stack_vm_load_prg(struct stack_vm* vm, char* prg, int size)
{
  for(int i = 0; i < size; i++)
  {
    vm->ROM[i] = prg[i];
  }
  return 0;
}

int stack_vm_step(struct stack_vm* vm)
{
  switch(vm->ROM[vm->pc])
  {
    case NOP:
      break;
    case PUSH:{
      vm->pc++;
      char a = vm->ROM[vm->pc++];
      char b = vm->ROM[vm->pc++];
      char c = vm->ROM[vm->pc++];
      char d = vm->ROM[vm->pc];
      int v = pack4chars(a, b, c, d);
      stack_push(&vm->stack, v);
      }
      break;
    case POP:
      //See where to return this if needed
      stack_pop(&vm->stack);
      break;
    case SWP: 
      stack_swap(&vm->stack);
      break;
    case ROLL: {
      vm->pc++;
      char a = vm->ROM[vm->pc++];
      char b = vm->ROM[vm->pc++];
      char c = vm->ROM[vm->pc++];
      char d = vm->ROM[vm->pc];
      int v = pack4chars(a, b, c, d);
      stack_roll(&vm->stack, v);
      } 
      break;
    case RWD: {
      vm->pc++;
      char a = vm->ROM[vm->pc++];
      char b = vm->ROM[vm->pc++];
      char c = vm->ROM[vm->pc++];
      char d = vm->ROM[vm->pc];
      int v = pack4chars(a, b, c, d);
      stack_rewind(&vm->stack, v);
      } 
      break;
    case DUP:
      stack_dup(&vm->stack);
      break;
    case DEBUG:
      print_stack(&vm->stack);
      break;
    case ADD:
      {
        int a = stack_pop(&vm->stack);
        int b = stack_pop(&vm->stack);
        stack_push(&vm->stack, a + b);
      }
      break;
    case SUB:
      {
        int a = stack_pop(&vm->stack);
        int b = stack_pop(&vm->stack);
        stack_push(&vm->stack, b - a);
      }
      break;
    case MUL:
      {
        int a = stack_pop(&vm->stack);
        int b = stack_pop(&vm->stack);
        stack_push(&vm->stack, a * b);
      }
      break;
    case SHR: 
      {
        int a = stack_pop(&vm->stack);
        stack_push(&vm->stack, a >> 1);
      }
      break;
    case SHL:
      {
        int a = stack_pop(&vm->stack);
        stack_push(&vm->stack, a << 1);
      }
      break;
    case PPC: 
      stack_push(&vm->stack, vm->pc+1);
      break;
    case JMP:
      {
        vm->pc++;
        char a = vm->ROM[vm->pc++]; 
        char b = vm->ROM[vm->pc++];
        char c = vm->ROM[vm->pc++];
        char d = vm->ROM[vm->pc];
        int v = pack4chars(a, b, c, d);
        vm->pc = v;
      } 
      break;
    case JPS:
      vm->pc = stack_pop(&vm->stack);
      break;
    case JPZ:
      if(vm->flags.zero == 1)
      {
        vm->pc++;
        char a = vm->ROM[vm->pc++]; 
        char b = vm->ROM[vm->pc++];
        char c = vm->ROM[vm->pc++];
        char d = vm->ROM[vm->pc];
        int v = pack4chars(a, b, c, d);
        vm->pc = v;
      }
      break;
    case JNZ:
      if(vm->flags.zero == 0)
      {
        vm->pc++;
        char a = vm->ROM[vm->pc++]; 
        char b = vm->ROM[vm->pc++];
        char c = vm->ROM[vm->pc++];
        char d = vm->ROM[vm->pc];
        int v = pack4chars(a, b, c, d);
        vm->pc = v;
      }
      break;
    case JPN:
      if(vm->flags.negative == 1)
      {
        vm->pc++;
        char a = vm->ROM[vm->pc++]; 
        char b = vm->ROM[vm->pc++];
        char c = vm->ROM[vm->pc++];
        char d = vm->ROM[vm->pc];
        int v = pack4chars(a, b, c, d);
        vm->pc = v;
      }
      break;
    case JPP:
      if(vm->flags.negative == 0)
      {
        vm->pc++;
        char a = vm->ROM[vm->pc++]; 
        char b = vm->ROM[vm->pc++];
        char c = vm->ROM[vm->pc++];
        char d = vm->ROM[vm->pc];
        int v = pack4chars(a, b, c, d);
        vm->pc = v;
      }
      break;
  }
  vm->flags.negative = 
    (vm->stack.buffer[vm->stack.size - 1] < 0);
  vm->flags.zero = 
    (vm->stack.buffer[vm->stack.size - 1] == 0);
  vm->pc++;
  return 0;
}

