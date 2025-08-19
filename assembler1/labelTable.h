#ifndef LABEL_TABLE_H
#define LABEL_TABLE_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_LABEL_TYPE_NAME 10
#define MAX_EXTERN_APEARS 30

/**
 * Struct representing a label entry in the label table.
 * Each label has:
 * - name: string of the label
 * - type: string describing its type (data, string, extern, entry, etc.)
 * - location: memory address or position in file
 * - extern_locatoins: array of locations where extern labels appear
 * - extern_locatins_num: number of extern appearances
 * - next: pointer to the next label in the same bucket
 */
typedef struct Label {
    char *name;
    char *type;
    int location; /* location of the label in the file */
    int extern_locatoins[MAX_EXTERN_APEARS];
    int extern_locatins_num;
    struct Label *next;
} Label;

/**
 * Initialize the label table.
 * Sets all hash buckets to NULL.
 */
void initialLabelTable ();

/**
 * Compute the hash value of a label name.
 * @param str Label name
 * @return Hash index in the table
 */
unsigned int hashLabel (const char* str);

/**
 * Insert a new label into the table.
 * Validates that the name does not conflict with reserved words.
 * @param name Label name
 * @param line_num Source line number (for error reporting)
 * @return 1 on success, 0 on failure
 */
int insertLabelToTbl (char* name, int line_num);

/**
 * Check if a label with the given name exists.
 * @param word Label name
 * @return Pointer to Label if found, NULL otherwise
 */
Label* checkLabelExist(char *word);

/**
 * Print all labels currently stored in the table.
 * Prints name, type, location, and extern appearances.
 */
void printLabelTable();

/**
 * Trim leading and trailing whitespace from a string.
 * @param str Input string
 * @return Pointer to the trimmed string
 */
char* trim_whitespace(char *str);

/**
 * Update the locations of data labels after instruction count (IC) is finalized.
 * Adds base offset and IC size where needed.
 * @param size_ic Final instruction counter size
 */
void updateDataLabelsLocations(int size_ic);

#endif /* LABEL_TABLE_H */
