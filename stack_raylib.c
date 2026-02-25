#include <stdio.h>
#include <raylib.h>
#include <string.h>

#include "stack.h"
#include "vm.h"

void window_should_close(struct stack* s)
{
  int v = (WindowShouldClose() ? 1 : 0);
  stack_push(s, v);
}

void begin_draw(struct stack* s)
{
  BeginDrawing();
}

void end_draw(struct stack* s)
{
  EndDrawing();
}

void fill_white(struct stack* s)
{
  ClearBackground(RAYWHITE);
}

int main(int argc, char** argv)
{

  if(argc < 2) 
  {
    printf("Please provide a script to run\n");
    return 0;
  }

  int buffer[256] = {};
  struct stack_vm vm = {};
  vm.stack = new_stack(buffer, 256);

  vm.func_table[0].func = window_should_close;
  strcpy(vm.func_table[0].name, "window_should_close");

  vm.func_table[1].func = begin_draw;
  strcpy(vm.func_table[1].name, "begind");

  vm.func_table[2].func = end_draw;
  strcpy(vm.func_table[2].name, "endd");

  vm.func_table[3].func = fill_white;
  strcpy(vm.func_table[3].name, "white");

  char prg[255] = {};
  FILE* source = fopen(argv[1], "rb");
  fread(prg, 1, 255, source);
  fclose(source);
  stack_vm_load_prg(&vm, prg, 255);

  InitWindow(400, 400, "Stack Raylib");

  while(vm.ROM[vm.pc] != HALT && vm.pc < 255)
  {
    stack_vm_step(&vm);
  }

  return 0;
}
