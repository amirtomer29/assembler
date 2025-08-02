#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

#define MAFILENAME_MAX 100

/*function declaration*/
int preAssembler(char *filename);

int skipSpaces (char *line);

char* getWord(char *line);

#endif /* PRE_ASSEMBLER_H */



