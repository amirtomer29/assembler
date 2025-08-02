assembler: main.o preAssembler.o macroTable.o labelTable.o
	gcc -g -Wall -ansi -pedantic preAssembler.o main.o macroTable.o labelTable.o -o assembler
preAssembler.o: preAssembler.c preAssembler.h macroTable.h labelTable.h
	gcc -c -Wall -ansi -pedantic preAssembler.c -o preAssembler.o
macroTable.o: macroTable.c macroTable.h
	gcc -c -Wall -ansi -pedantic macroTable.c -o macroTable.o
labelTable.o: labelTable.c labelTable.h
	gcc -c -Wall -ansi -pedantic labelTable.c -o labelTable.o
main.o: main.c preAssembler.h macroTable.h labelTable.h
	gcc -c -Wall -ansi -pedantic main.c -o main.o

clean:
	rm -f *.o assembler