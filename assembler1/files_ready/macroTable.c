#include "preAssembler.h"
#include "labelTable.h"
#include "instructions.h"
#include "main.h"
#include "opCode.h"
#include "macroTable.h"

/*
 * macroTable.c
 * -----------------
 * This file implements the macro table, which allows for the storage, retrieval,
 * and management of macros defined during the pre-assembly stage.
 * Each macro is stored in a hash table using a linked list to handle collisions.
 */

/**
 * Initialize the macro table.
 * Sets all hash buckets to NULL. Must be called before using the table.
 */
void initialMacroTable () {
    int i;
    for (i = 0; i < MACRO_HASH_SIZE; i++) {
        macro_table[i] = NULL;
    }
}

/**
 * Compute the hash value of a macro name.
 * @param str Macro name
 * @return Index in the range [0, MACRO_HASH_SIZE - 1]
 */
unsigned int hashMacro(const char* str) {
    unsigned int hash = 0;
    while (*str) {
        hash = (hash * MACRO_HASH_SIZE + *str++) % MACRO_HASH_SIZE;
    }
    return hash;
}

/**
 * Insert a new macro by name.
 * Allocates memory for the macro and adds it to the table.
 * @param name Macro name
 * @return Pointer to the created Macro, or NULL on failure
 */
Macro *insertMacroNameToTbl(const char *name) {
    int hashVal;
    Macro* ancestor;
    Macro* new_macro = malloc(sizeof(Macro));
    if (!new_macro) {
        perror("malloc");
        return NULL;
    }
    new_macro->name = malloc(strlen(name) + 1);
    strcpy(new_macro->name, name);
    new_macro->body = calloc(1, sizeof(char));
    new_macro->next = NULL;

    hashVal = hashMacro(name);

    if (macro_table[hashVal] == NULL) {
        macro_table[hashVal] = new_macro;
    } else {
        ancestor = macro_table[hashVal];
        while (ancestor->next != NULL) {
            ancestor = ancestor->next;
        }
        ancestor->next = new_macro;
    }
    return new_macro;
}

/**
 * Append a line of text to a macro body.
 * @param macro Target macro
 * @param line  Text to append
 */
void insertLineToMacro(Macro *macro, char* line) {
    char *body = macro->body;
    int size = strlen(body) + strlen(line) + 2;
    char *temp = realloc(body, size);
    if (!temp) {
        free(body);
        macro->body = calloc(1, sizeof(char));
        perror("realloc faild");
    } else {
        macro->body = temp;
        strcat(macro->body, line);
        macro->body[size-2] = '\n';
        macro->body[size-1] = '\0';
    }
}

/**
 * Get the body string of a macro by name.
 * @param name Macro name
 * @return Body string, or NULL if not found
 */
char* getMacroBody(char *name) {
    Macro *macro = checkMacroExist(name);
    return (macro) ? macro->body : NULL;
}

/**
 * Check if a macro exists in the table.
 * @param word Macro name
 * @return Pointer to Macro if found, NULL otherwise
 */
Macro *checkMacroExist(char *word) {
    unsigned int hash_val;
    Macro *macro;

    hash_val = hashMacro(word);
    macro = macro_table[hash_val];

    while (macro != NULL && strcmp(macro->name, word) != 0) {
        macro = macro->next;
    }
    return macro;
}

/**
 * Validate that a macro name is not already used.
 * Checks opcodes, labels, macros, instructions, and registers.
 * @param word Macro name
 * @param line_num Line number for error reporting
 * @return 1 if valid, 0 otherwise
 */
int checkMacroName(char *word, int line_num) {
    if (getOpCodeIndex(word) != -1) {
        fprintf(stderr, "Error: line %d macro %s name already used as opcode name\n", line_num, word);
        return 0;
    }
    if (checkLabelExist(word)) {
        fprintf(stderr, "Error: line %d macro %s name already used as label name\n", line_num, word);
        return 0;
    }
    if (checkMacroExist(word)) {
        fprintf(stderr, "Error: line %d macro %s name already used as macro name\n", line_num, word);
        return 0;
    }
    if (getInstructionType(word) != -1) {
        fprintf(stderr, "Error: line %d macro %s name already used as instruction name\n", line_num, word);
        return 0;
    }
    if (checkIfRegister(word)) {
        fprintf(stderr, "Error: line %d macro %s name already used as register name\n", line_num, word);
        return 0;
    }
    return 1;
}

/**
 * Print all macros in the table.
 * Prints name and body of each macro to stdout.
 */
void printMacroTable() {
    int i;
    Macro *macro;
    for (i = 0; i < MACRO_HASH_SIZE; i++) {
        macro = macro_table[i];
        while (macro != NULL) {
            macro = macro->next;
        }
    }
}
