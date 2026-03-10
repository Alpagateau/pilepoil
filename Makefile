CFLAGS:=-Wall -Wextra -g

all: test sasm test.stack syr.stack test_func.stack stack_raylib raylib.stack

test.stack: test.sasm
	./sasm ./test.sasm

test_func.stack: test_func.sasm
	./sasm ./test_func.sasm

syr.stack: syr.sasm
	./sasm ./syr.sasm

raylib.stack: raylib.sasm
	./sasm ./raylib.sasm

test: main.c vm.o stack.o ast.o
	gcc -std=c99 main.c *.o -o test $(CFLAGS)

sasm: stack_asm.c ast.o vm.o stack.o 
	gcc -std=c99 stack_asm.c vm.o stack.o ast.o -o sasm $(CFLAGS)

vm.o: vm.h vm.c
	gcc vm.c -c -o vm.o $(CFLAGS)

stack.o: stack.h stack.c
	gcc stack.c -c -o stack.o	$(CFLAGS)

ast.o:	stacklang/ast.c 
	gcc stacklang/ast.c -c -o ast.o

stack_raylib: stack_raylib.c
	gcc stack_raylib.c *.o -o stack_raylib -I~/LIBS/raylib/include -L~/LIBS/raylib/linux -lraylib

