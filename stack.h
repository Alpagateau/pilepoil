#ifndef STACK_H
#define STACK_H

struct stack
{
  int size;
  int capacity;
  int *buffer;
};

#define STACK_ERROR_STACK_FULL -1
#define STACK_ERROR_STACK_EMPTY -2

//Creates a new stack from a given buffer and size
//The buffer needs to be continuous.
struct stack new_stack(int* buffer, int size);

//Pushes an int on the stack
int stack_push(struct stack* st, int v);

//Removes a value off the stack and returns it
int stack_pop(struct stack* st);

//Duplicate the first element up the stack
int stack_dup(struct stack* st);

//Swaps the first and second element atop the stack
int stack_swap(struct stack *st);

//Rotates the n first values down 
//(n, n1, n2, n3) -> (n3, n, n1, n2)
int stack_roll(struct stack *st, int n);

//Rotates the n first values up 
//(n, n1, n2, n3) -> (n1, n2, n3, n)
int stack_rewind(struct stack *st, int n);

//Calls the function atop the stack
int stack_call(struct stack *st);

//Prints the stack in the console
void print_stack(struct stack* st);
#endif
