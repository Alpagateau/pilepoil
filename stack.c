#include "stack.h"
#include <stdio.h>

struct stack new_stack(int* buffer, int size)
{
  return (struct stack)
    {
      .buffer = buffer,
      .capacity = size,
      .size = 0
    };
}

int stack_push(struct stack *st, int v)
{
  if(st->size >= st->capacity) return STACK_ERROR_STACK_FULL;
  st->buffer[st->size++] = v;
  return 0;
}

int stack_pop(struct stack *st)
{
  return st->buffer[(st->size--) - 1];
}

int stack_dup(struct stack* st)
{
  return stack_push(st, st->buffer[st->size-1]);
}

int stack_swap(struct stack* st)
{
  if(st->size < 2) return STACK_ERROR_STACK_EMPTY;
  int a = st->buffer[st->size-1];
  st->buffer[st->size-1] = st->buffer[st->size-2];
  st->buffer[st->size-2] = a;
  return 0;
}

int stack_roll(struct stack *st, int n)
{
  if(st->size < n) return STACK_ERROR_STACK_EMPTY;
  int a = st->buffer[st->size - n];
  for(int i = n-1; i > 0; i--)
  {
    st->buffer[st->size-(i+1)] = st->buffer[st->size - i];
  }
  st->buffer[st->size-1] = a;
  return 0;
}

int stack_rewind(struct stack *st, int n)
{
  if(st->size < n) return STACK_ERROR_STACK_EMPTY;
  int a = st->buffer[st->size - 1];
  for(int i = 1; i < n; i++)
  {
    st->buffer[st->size-i] = st->buffer[st->size-(i+1)];
  }
  st->buffer[st->size-n] = a;
  return 0;
}

int stack_call(struct stack *st)
{
  if(st->buffer[st->size-1] >= 0) return -3;
  int f = stack_pop(st);
  return f;
}

void print_stack(struct stack *st)
{
  for(int i = 0; i < st->size; i++)
  {
    printf("%d, ", st->buffer[i]);
  }
  printf("<%d>\n", st->capacity - st->size);
}
