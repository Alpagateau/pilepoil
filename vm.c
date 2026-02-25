#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
      }else{vm->pc+=4;}
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
      }else{vm->pc+=4;}
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
      }else{vm->pc+=4;}
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
      }else{vm->pc+=4;}
      break;
    case CALL:
      stack_push(&vm->stack, vm->pc+1);
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
    case LNK:
      {
        char str[64] = {};
        vm->pc++;
        int idx = 0;
        while(vm->ROM[vm->pc] != 0)
        {
          str[idx++] = vm->ROM[vm->pc++];
        }
        int found = 0;
        for(int i = 0; i < STACK_FT_SIZE; i++)
        {
          if(strcmp(vm->func_table[i].name, str) == 0)
          {
            vm->func_table[i].idx = vm->func_table_num++;
            found = 1;
            break;
          }
        }
        if(found == 0)
        {
          printf("[ERROR] %s function wasn't linked\n", 
                 str);
          exit(1);
        }
      }
      break;
    case VMCALL:
      {
        vm->pc++;
        char a = vm->ROM[vm->pc++]; 
        char b = vm->ROM[vm->pc++];
        char c = vm->ROM[vm->pc++];
        char d = vm->ROM[vm->pc];
        int v = pack4chars(a, b, c, d);
        vm->func_table[v].func(&vm->stack);
      } 
      break;
    case HALT:
      vm->pc--;
      break;
  }
  vm->flags.negative = 
    (vm->stack.buffer[vm->stack.size - 1] < 0);
  vm->flags.zero = 
    (vm->stack.buffer[vm->stack.size - 1] == 0);
  vm->pc++;
  return 0;
}

const char* opcode_name(enum OP o)
{ 
switch (o) {
  case NOP:
    return "NOP";
  case PUSH:
    return "PUSH";
  case POP:
    return "POP";
  case SWP:
    return "SWP";
  case ROLL:
    return "ROLL";
  case RWD:
    return "RWD";
  case DUP:
    return "DUP";
  case ADD:
    return "ADD";
  case SUB:
    return "SUB";
  case MUL:
    return "MUL";
  case SHR:
    return "SHR";
  case SHL:
    return "SHL";
  case PPC:
    return "PPC";
  case JMP:
    return "JMP";
  case JPS:
    return "JPS";
  case JPZ:
    return "JPZ";
  case JNZ:
    return "JNZ";
  case JPN:
    return "JPN,";
  case JPP:
    return "JPP";
  case CALL:
    return "CALL";
  case VMCALL:
    return "VMCALL";
  case LNK:
    return "LNK";
  case DEBUG:
    return "DEBUG";
  case HALT:
    return "HALT";
  default:
    return "Do not see this";
  }
}
