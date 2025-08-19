#flags
CC = gcc
CFLAGS = -g -Wall -ansi -pedantic


assembler: main.o preAssembler.o macroTable.o labelTable.o firstPass.o secondPass.o instructions.o opCode.o parsers.o format.o
	$(CC) $(CFLAGS) main.o preAssembler.o macroTable.o labelTable.o firstPass.o secondPass.o instructions.o opCode.o parsers.o format.o -o assembler
main.o: main.c preAssembler.h macroTable.h labelTable.h
	$(CC) $(CFLAGS) -c main.c -o main.o
macroTable.o: macroTable.c macroTable.h  instructions.h
	$(CC) $(CFLAGS) -c macroTable.c -o macroTable.o
labelTable.o: labelTable.c labelTable.h instructions.h
	$(CC) $(CFLAGS) -c labelTable.c -o labelTable.o
preAssembler.o: preAssembler.c preAssembler.h macroTable.h labelTable.h
	$(CC) $(CFLAGS) -c preAssembler.c -o preAssembler.o
firstPass.o: firstPass.c firstPass.h instructions.h
	$(CC) $(CFLAGS) -c firstPass.c -o firstPass.o
secondPass.o: secondPass.c secondPass.h
	$(CC) $(CFLAGS) -c secondPass.c -o secondPass.o
instructions.o: instructions.c instructions.h
	$(CC) $(CFLAGS) -c instructions.c -o instructions.o
opCode.o: opCode.c opCode.h instructions.h
	$(CC) $(CFLAGS) -c opCode.c -o opCode.o	
parsers.o: parsers.c parsers.h
	$(CC) $(CFLAGS) -c parsers.c -o parsers.o
format.o: format.c	format.h
	$(CC) $(CFLAGS) -c format.c -o format.o

clean:
	rm -f *.o assembler