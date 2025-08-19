#ifndef FIRST_PASS_H
#define FIRST_PASS_H

/**
 * firstPass.h
 * Header file for the first pass of the assembler.
 * Provides declaration for the main function that parses the source file
 * during the first pass, handling labels, instructions, and opcodes.
 */

/**
 * Runs the first pass of the assembler.
 * filename - path to the input assembly file
 * Returns 1 on success, 0 on failure
 */
int firstPass(char *filename);

#endif /* FIRST_PASS_H */
