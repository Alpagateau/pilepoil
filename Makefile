CFLAGS:=-Wall -Wextra -g

all: test sasm test.stack

test.stack: test.sasm
	./sasm ./test.sasm
	mv ./test.sasm.stack ./test.stack

test: main.c vm.o stack.o
	gcc -std=c99 main.c *.o -o test $(CFLAGS)

sasm: stack_asm.c
	gcc -std=c99 stack_asm.c -o sasm $(CFLAGS)

vm.o: vm.h vm.c
	gcc vm.c -c -o vm.o $(CFLAGS)

stack.o: stack.h stack.c
	gcc stack.c -c -o stack.o	$(CFLAGS)
