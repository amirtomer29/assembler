#include "labelTable.h"
#include "instructions.h"
#include "macroTable.h"
#include "opCode.h"
#include <ctype.h>
#include "parsers.h"
#include "preAssembler.h"


void initialLabelTable () {
    int i;
    for (i = 0; i<LABEL_HASH_SIZE; i++){
        label_table[i] = NULL;
    }
}

/*hash function*/
unsigned int hashLabel (const char* str) {
    unsigned int hash = 0;
    while (*str) {
        hash = (hash * LABEL_HASH_SIZE + *str++) % LABEL_HASH_SIZE;
    }
    return hash;
}

/* Function to insert a label into the label table 
// The label is inserted at the end of the linked list at the hash index
// The label's name is dynamically allocated, and the label structure is created
// The function check for duplicate labels
// The function does not return any value, it simply inserts the label into the table
*/
int insertLabelToTbl (char* name, int line_num) {
    unsigned int hashVal;
    Label *ancestor;
    Label *new_label = malloc(sizeof(Label));
    if (!new_label) {
        perror("malloc");
        return 0;
    }
    if (checkLabelExist(name)){
        fprintf(stderr, "ERROR: label %s already exist\n", name);
        return 0;
    }
    if (checkMacroExist(name)){
        fprintf(stderr, "ERROR: label %s use as macro\n", name);
        return 0;
    }
    if (checkIfRegister(name)){
        fprintf(stderr, "ERROR: label %s already use as register\n", name);
        return 0;
    }
    if (getInstructionType(name)>=0){
        fprintf(stderr, "ERROR: label %s already use as instruction word\n", name);
        return 0;
    }
    if (checkIfOpCode(name)>=0){
        fprintf(stderr, "ERROR: label %s already use as opcode word\n", name);
        return 0;
    }

    new_label->name = malloc(strlen(name) + 1);
    if (!new_label->name) {
        perror("malloc");
        free(new_label);
        return 0;
    }

    new_label->type = malloc(MAX_LABEL_TYPE_NAME);
    if (!new_label->type) {
        perror("malloc");
        free(new_label);
        return 0;
    }
    strcpy(new_label->type, "type");


    new_label->location = 0;
    
    new_label->extern_locatins_num = 0;


    strcpy(new_label->name, name);
    new_label->next = NULL;
    
    hashVal = hashLabel(name);
    if (label_table[hashVal] == NULL) {
        label_table[hashVal] = new_label;
    } else {
        ancestor = label_table[hashVal];
        while (ancestor->next !=NULL) {
            if (strcmp(ancestor->name, name) == 0) {
                fprintf(stderr, "Error: label %s at %d already exists\n", name, line_num);
                free(new_label->name);
                free(new_label);
                return 0; 
            }
            ancestor = ancestor->next;
        }
        ancestor->next = new_label;
    }
    return 1;
}

/* Function to check if a label exists in the label table
// The function returns a pointer to the label if it exists, or NULL if it does not
// The function uses the hash function to find the index in the label table
// It then traverses the linked list at that index to find the label
// The function does not check for NULL pointers, which should be handled by the caller
*/
Label* checkLabelExist(char *word){
    unsigned int hash_val;
    Label *label;
    
    if (word == NULL) {
        fprintf(stderr, "Error: checkLabelExist called with NULL word\n");
        return NULL; /*error*/
    }

    word = trim_whitespace(word); /*trim whitespace from the word*/
    if (strlen(word) == 0) {
        fprintf(stderr, "Error: checkLabelExist called with empty word\n");
        return NULL; /*error*/
    }
    hash_val = hashLabel(word);
    label = label_table[hash_val];

    
    while (label != NULL)
    {
        if (strcmp((label->name), word) == 0) {  
            return label; /*found the label*/
        }
        label = label->next;
    } 
    return label;
}

/* Function to print the label table
// The function prints the labels in the table, including their names and types
// It iterates through each index of the label table and prints the labels in the linked list
// The function does not return any value, it simply prints the labels to stdout
*/
void printLabelTable() {
    int i;
    int extern_apears_left;
    int type;
    Label *current_label;
    
    for (i = 0; i < LABEL_HASH_SIZE; i++) {
        current_label = label_table[i];
        while (current_label) {
            printf("  Label: %s type: %s , type_i: %d , locatoin: %d\n", current_label->name, current_label->type, getInstructionType(current_label->type) ,current_label->location);
            type = getInstructionType(current_label->type);
            if (type == 4) {
                extern_apears_left = current_label->extern_locatins_num;
                printf("%d %d\n", extern_apears_left, current_label->extern_locatins_num);
                for (;extern_apears_left > 0; extern_apears_left--){
                    printf("EXT: %d", current_label->extern_locatoins[extern_apears_left-1]);
                }
            }
            current_label = current_label->next;
        }  
    }
}

void updateDataLabelsLocations(int size_ic) {
    Label *current_label;
    int i;
    int type;
    /*Loop through each linked list of the hash table*/ 
    for (i = 0; i < LABEL_HASH_SIZE; i++) {
        current_label = label_table[i];
        /*Traverse the linked list in the current bucket*/
        while (current_label) {
            /*Determine the type of the label*/
            type = getInstructionType(current_label->type);
            /*If the label is a data type (e.g., types 0, 1, or 2)*/
            if (type >= 0 && type < 4) {
                /*Update the location by adding the size of the instruction code*/
                current_label->location += REAL_MEMORY_LOAD+size_ic;
            }
            if (type == -1){
                current_label->location += REAL_MEMORY_LOAD;
            }
            /*Move to the next label in the linked list*/
            current_label = current_label->next;
        }
    }
}

