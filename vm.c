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
  }
  vm->pc++;
  return 0;
}

