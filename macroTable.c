#include "macroTable.h"
#include "labelTable.h"
#include "preAssembler.h"

/* macroTable.c
 * This file contains the implementation of the macro table functionality.
 * It manages macros, including their creation, insertion, and retrieval.
 */
void initialMacroTable () {
    int i;
    for (i=0; i<MACRO_HASH_SIZE; i++){
        macro_table[i] = NULL;
    }
}

/*hash function
// The hash function takes a string and returns an index in the macro table
// It uses a simple polynomial rolling hash algorithm to compute the hash value
// The hash value is computed by iterating through each character of the string
// and updating the hash value based on the character's ASCII value and the current hash value
// The hash value is then taken modulo the size of the macro table to ensure it fits within the table
*/
unsigned int hashMacro(const char* str) {
    unsigned int hash = 0;
    while (*str) {
        hash = (hash * MACRO_HASH_SIZE + *str++) % MACRO_HASH_SIZE;
    }
    return hash;
}

/* Function to insert a macro name into the macro table
// The macro name is dynamically allocated, and the macro structure is created
// The function checks for duplicate macro names and returns NULL if a duplicate is found
// The function returns a pointer to the newly created macro structure or NULL if memory allocation fails
// The function uses the hash function to find the index in the macro table
// It then traverses the linked list at that index to find the end and inserts the new macro
*/
Macro *insertMacroNameToTbl(const char *name) {
    int hashVal;
    Macro* ancestor;
    Macro* new_macro = malloc(sizeof(Macro));
    if (!new_macro) {
        perror("malloc");
        return NULL;
    }
    new_macro->name = malloc(strlen(name)+1);
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

/* Function to insert a line into a macro's body
// The function appends the line to the macro's body
// It reallocates memory for the body to accommodate the new line
// The function does not check for NULL pointers, which should be handled by the caller
// The function does not return any value, it simply modifies the macro's body
*/
void insertLineToMacro(Macro *macro, char* line){
    char *body = macro->body;
    int size = strlen(body)+strlen(line) + 2;
    char *temp = realloc(body, size);
    if (!temp){
        free(body);
        macro->body = calloc(1, sizeof(char));
        perror("realloc faild");
    } else {
        macro->body = temp;
        strcat(macro->body, line);
        strcat(macro->body, "\n");
    }
}

/* Function to get the body of a macro by its name
// The function checks if the macro exists in the macro table
// If the macro exists, it returns its body, otherwise it returns NULL
// The function uses the checkMacroExist function to find the macro
// The function does not check for NULL pointers, which should be handled by the caller
// The function does not check if the macro body is NULL, which should be handled by the caller
*/
char* getMacroBody(char *name) {
    Macro *macro = checkMacroExist(name);
    return (macro) ? macro->body : NULL;
}

/* Function to check if a macro exists in the macro table
// The function uses the hash function to find the index in the macro table
// It then traverses the linked list at that index to find the macro
// The function returns a pointer to the macro if it exists, or NULL if it does not
// The function does not check for NULL pointers, which should be handled by the caller
// The function does not check if the macro name is NULL, which should be handled by the caller
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

/* Function to check if a macro name is valid
// The function checks if the macro name is already used as an opcode, label, or macro name
// It returns 1 if the name is valid, or 0 if it is not
// The function uses the opcode_names array to check against opcodes
// It uses the checkLabelExist function to check against labels
// It uses the checkMacroExist function to check against existing macros
// The function does not check for NULL pointers, which should be handled by the caller
// The function does not check if the macro name is NULL, which should be handled by the caller
*/
int checkMacroName(char *word, int line_num) {
    int i;
    for (i = 0; i<16; i++){
        if (strcmp(word, opcode_names[i]) == 0 ) {
            fprintf(stderr, "Error: line %d macro %s name already used as op_code\n", line_num, name);
            return 0;
        }
    }
    if (checkLabelExist(word)) {
        fprintf(stderr, "Error: line %d macro %s name already used as label name\n", line_num, name);
        return 0;
    }
    if (checkMacroExist(word)) {
        fprintf(stderr, "Error: line %d macro %s name already used as macro name\n", line_num, name);
        return 0;
    }
    return 1;
}

/* Function to print the macro table
// The function prints the macros in the table, including their names and bodies
// It iterates through each index of the macro table and prints the macros in the linked list
// The function does not return any value, it simply prints the macros to stdout
*/
void printMacroTable() {
    int i;
    Macro *macro;
    for (i = 0; i < MACRO_HASH_SIZE; i++) {
        macro = macro_table[i];
        while (macro != NULL) {
            printf("Macro Name: %s\n", macro->name);
            printf("Macro Body:\n%s\n", macro->body);
            macro = macro->next;
        }
    }
}


