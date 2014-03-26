CC = gcc

main: main.o instructions.o stack.o table.o
	$(CC) -o cwici main.o instructions.o stack.o table.o

instructions: instructions.h table.h stack.h

stack: teststack.c stack.h stack.c
	$(CC) -o teststack teststack.c stack.c

table: testtable.c table.h table.c
	$(CC) -o testtable testtable.c table.c

clean:
	rm -f *.o
