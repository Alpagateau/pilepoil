
stc: main.c lex.yy.o y.tab.o ast.o
	gcc main.c lex.yy.o y.tab.o ast.o -o stc -Wall -Wextra -I../ -g

ast.o: ast.c
	gcc ast.c -c -g

lex.yy.o: lex.yy.c
	gcc lex.yy.c -c -g

y.tab.o: y.tab.c
	gcc y.tab.c -c -g

y.tab.c: stacklang.y
	yacc -d stacklang.y -Wcounterexamples

lex.yy.c: stacklang.l y.tab.c
	lex stacklang.l
