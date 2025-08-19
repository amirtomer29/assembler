#ifndef MACRO_TABLE_H
#define MACRO_TABLE_H

/**
 * Represents a macro entry stored in the macro table.
 * Each macro has a name, a body string, and a pointer to the next macro in the bucket list.
 */
typedef struct Macro {
    char *name;          /**< Macro name */
    char *body;          /**< Macro body text */
    struct Macro *next;  /**< Next macro in the linked list (for collisions) */
} Macro;

/**
 * Retrieve the body string of a macro by its name.
 * @param name Macro name
 * @return Pointer to body string, or NULL if not found
 */
char* getMacroBody(char *name);

/**
 * Check if a macro with the given name exists.
 * @param word Macro name
 * @return Pointer to Macro if found, NULL otherwise
 */
Macro *checkMacroExist(char *word);

/**
 * Validate that a macro name is not already in use.
 * @param word Macro name
 * @param line_num Source line number (for error reporting)
 * @return 1 if valid, 0 otherwise
 */
int checkMacroName(char *word, int line_num);

/**
 * Insert a new macro entry into the table.
 * @param word Macro name (must be unique)
 * @return Pointer to the created Macro, or NULL on allocation failure
 */
Macro *insertMacroNameToTbl(const char *word);

/**
 * Append a line of text to the body of a macro.
 * @param macro Target macro
 * @param line  Text line to append
 */
void insertLineToMacro(Macro *macro, char* line);

/**
 * Initialize the macro table.
 * Sets all hash buckets to NULL before any insertion.
 */
void initialMacroTable();

/**
 * Print all macros currently in the table.
 * Prints names and bodies to stdout.
 */
void printMacroTable();

#endif /* MACRO_TABLE_H */
