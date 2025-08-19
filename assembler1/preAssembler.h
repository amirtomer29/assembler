#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

#define MAFILENAME_MAX 100

/**
 * Run the pre-assembler phase on a given source file.
 * Expands macros, processes labels, and generates a .am file.
 * @param filename Input source file name
 * @return Name of the generated .am file, or NULL on error
 */
char* preAssembler(char *filename);

/**
 * Check if a word is a register (r0–r7).
 * @param word Input string
 * @return 1 if the word is a register, 0 otherwise
 */
int checkIfRegister(char *word);

#endif /* PRE_ASSEMBLER_H */
