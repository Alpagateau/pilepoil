CFLAGS:=-Wall -Wextra -g

all: test sasm test.stack syr.stack test_func.stack

test.stack: test.sasm
	./sasm ./test.sasm
	mv ./test.sasm.stack ./test.stack

test_func.stack: test_func.sasm
	./sasm ./test_func.sasm
	mv ./test_func.sasm.stack ./test_func.stack

syr.stack: syr.sasm
	./sasm ./syr.sasm
	mv ./syr.sasm.stack ./syr.stack

test: main.c vm.o stack.o
	gcc -std=c99 main.c *.o -o test $(CFLAGS)

sasm: stack_asm.c
	gcc -std=c99 stack_asm.c vm.c stack.c -o sasm $(CFLAGS)

vm.o: vm.h vm.c
	gcc vm.c -c -o vm.o $(CFLAGS)

stack.o: stack.h stack.c
	gcc stack.c -c -o stack.o	$(CFLAGS)
