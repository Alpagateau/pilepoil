CFLAGS:=-Wall -Wextra -g

test: main.c vm.o stack.o
	gcc -std=c99 main.c *.o -o test $(CFLAGS)

vm.o: vm.h vm.c
	gcc vm.c -c -o vm.o $(CFLAGS)

stack.o: stack.h stack.c
	gcc stack.c -c -o stack.o	$(CFLAGS)
