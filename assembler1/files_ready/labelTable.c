#include "labelTable.h"
#include "instructions.h"
#include "macroTable.h"
#include "opCode.h"
#include <ctype.h>
#include "parsers.h"
#include "preAssembler.h"

/**
 * Initialize the label table.
 * Sets all buckets of the hash table to NULL.
 */
void initialLabelTable () {
    int i;
    for (i = 0; i < LABEL_HASH_SIZE; i++) {
        label_table[i] = NULL; /* Reset each bucket */
    }
}

/**
 * Compute the hash value of a label name.
 * @param str Label name
 * @return Index in range [0, LABEL_HASH_SIZE - 1]
 */
unsigned int hashLabel (const char* str) {
    unsigned int hash = 0;
    while (*str) {
        /* Multiply current hash by table size and add ASCII value, then modulo */
        hash = (hash * LABEL_HASH_SIZE + *str++) % LABEL_HASH_SIZE;
    }
    return hash; /* Final hash index */
}

/**
 * Insert a new label into the table.
 * Performs validation against reserved names.
 * @param name Label name
 * @param line_num Source line number for error reporting
 * @return 1 on success, 0 on failure
 */
int insertLabelToTbl (char* name, int line_num) {
    unsigned int hashVal;
    Label *ancestor;
    Label *new_label = malloc(sizeof(Label));
    if (!new_label) {
        perror("malloc"); /* Allocation failed */
        return 0;
    }


    /*Validate label name*/

    /*length must be <= MAX_LABEL_NAME */
    if (!name || name[0] == '\0') {
        fprintf(stderr,"error: label '%s' is over MAX_LABEL_NAME (%d) characters\n",name, MAX_LABEL_NAME);
        return 0;
    }
    /*first character must be alphabetic*/
    if (!isalpha((unsigned char)name[0])) {
        fprintf(stderr, "error: label %s doesn't start with an alphabetic character\n", name);
        return 0;
    }
    /* MAX_LABEL_NAME == 30 */
    if (strlen(name) > MAX_LABEL_NAME) {
        fprintf(stderr, "error: label %s is over `MAX_LABEL_NAME` characters\n", name);
        return 0;
    }
    /* Check for name conflicts */
    if (checkLabelExist(name)){
        fprintf(stderr, "error: label %s already exist\n", name);
        return 0;
    }
    if (checkMacroExist(name)){
        fprintf(stderr, "error: label %s use as macro\n", name);
        return 0;
    }
    if (checkIfRegister(name)){
        fprintf(stderr, "error: label %s already use as register\n", name);
        return 0;
    }
    if (getInstructionType(name) <= INSTR_EXTERN && getInstructionType(name) >= INSTR_DATA){
        fprintf(stderr, "error: label %s already use as instruction word\n", name);
        return 0;
    }
    if (getOpCodeIndex(name) >= OP_MOV){
        fprintf(stderr, "error: label %s already use as opcode word\n", name);
        return 0;
    }

    /* Allocate memory for label name */
    new_label->name = malloc(strlen(name) + 1);
    if (!new_label->name) {
        perror("malloc");
        free(new_label);
        return 0;
    }

    /* Allocate memory for label type string */
    new_label->type = malloc(MAX_LABEL_TYPE_NAME);
    if (!new_label->type) {
        perror("malloc");
        free(new_label);
        return 0;
    }

    /* Set defaults */
    strcpy(new_label->type, "DEFAULT");
    new_label->location = 0;
    new_label->extern_locatins_num = 0;

    strcpy(new_label->name, name);
    new_label->next = NULL;

    /* Compute hash index */
    hashVal = hashLabel(name);

    /* Insert into hash table */
    if (label_table[hashVal] == NULL) {
        label_table[hashVal] = new_label;
    } else {
        ancestor = label_table[hashVal];
        while (ancestor->next != NULL) {
            /* Check again for duplicates */
            if (strcmp(ancestor->name, name) == 0) {
                fprintf(stderr, "Error: label %s at %d already exists\n", name, line_num);
                free(new_label->name);
                free(new_label);
                return 0; 
            }
            ancestor = ancestor->next;
        }
        ancestor->next = new_label; /* Append to end */
    }
    return 1; /* Success */
}

/**
 * Check if a label exists in the table.
 * @param word Label name
 * @return Pointer to Label if found, NULL otherwise
 */
Label* checkLabelExist(char *word){
    unsigned int hash_val;
    Label *label;

    if (word == NULL) {
        fprintf(stderr, "Error: checkLabelExist called with NULL word\n");
        return NULL;
    }

    word = trim_whitespace(word); /* Remove extra spaces */
    if (strlen(word) == 0) {
        fprintf(stderr, "Error: checkLabelExist called with empty word\n");
        return NULL;
    }

    hash_val = hashLabel(word);
    label = label_table[hash_val];

    /* Traverse linked list at bucket */
    while (label != NULL) {
        if (strcmp(label->name, word) == 0) {  
            return label; /* Found match */
        }
        label = label->next;
    }
    return label; /* Not found */
}

/**
 * Print all labels in the table.
 * Shows name, type, location, and extern references if applicable.
 */
void printLabelTable() {
    int i;
    int extern_apears_left;
    int type;
    Label *current_label;

    for (i = 0; i < LABEL_HASH_SIZE; i++) {
        current_label = label_table[i];
        while (current_label) {
            /* Print base info */
            printf("  Label: %s type: %s , type_i: %d , locatoin: %d\n",
                   current_label->name,
                   current_label->type,
                   getInstructionType(current_label->type),
                   current_label->location);

            type = getInstructionType(current_label->type);

            /* If extern, list all appearances */
            if (type == 4) {
                extern_apears_left = current_label->extern_locatins_num;
                printf("%d %d\n", extern_apears_left, current_label->extern_locatins_num);

                for (; extern_apears_left > 0; extern_apears_left--) {
                    printf("EXT: %d", current_label->extern_locatoins[extern_apears_left - 1]);
                }
            }
            current_label = current_label->next;
        }
    }
}

/**
 * Update label locations after IC is finalized.
 * Data labels get IC offset, extern/entry labels get base offset.
 * @param size_ic Final instruction counter size
 */
void updateDataLabelsLocations(int size_ic) {
    Label *current_label;
    int i;
    int type;

    /* Traverse all buckets */
    for (i = 0; i < LABEL_HASH_SIZE; i++) {
        current_label = label_table[i];
        while (current_label) {
            type = getInstructionType(current_label->type);

            /* Data labels: add IC and base */
            if (type >= INSTR_DATA && type < INSTR_ENTRY) {
                current_label->location += REAL_MEMORY_LOAD + size_ic;
            }

            /* Unknown labels: add base only */
            if (type == -1){
                current_label->location += REAL_MEMORY_LOAD;
            }

            current_label = current_label->next;
        }
    }
}
