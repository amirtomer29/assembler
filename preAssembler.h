#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

#define MAFILENAME_MAX 100

/*function declaration*/
char* preAssembler(char *filename);

int checkIfRegister(char *word);

#endif /* PRE_ASSEMBLER_H */



