#ifndef SECOND_PASS_H
#define SECOND_PASS_H

/*
 * secondPass.h
 * Declarations for the assembler's second pass and related output writers.
 */

/*
 * LINE_BUFFER_SIZE
 * Computed buffer size for lines written to output files:
 *   address (4 chars) + tab (1) + name (MAX_LABEL_NAME) + newline (1) + null terminator (1)
 */
#define LINE_BUFFER_SIZE (4 + 8 + MAX_LABEL_NAME + 2) /* address + tab + name + \n + null */

/**
 * Run the assembler's second pass on the given source file.
 * @param filename - path to the input assembly file
 * @return 1 on success, 0 on failure
 */
int secondPass(char *filename);

/**
 * Write .ent and .ext files based on the label table.
 * @param ent - FILE* for the .ent output
 * @param ext - FILE* for the .ext output
 * @return 1 on success, 0 on failure
 */
int createExtEntFile(FILE *ent, FILE *ext);

/**
 * Write the .ob object file from the assembled memory images.
 * @param obj - FILE* for the .ob output
 * @return 1 on success, 0 on failure
 */
int createObjFile(FILE *obj);

#endif /*SECOND_PASS_H*/
