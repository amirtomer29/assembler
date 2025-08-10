#include "labelTable.h"
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
void insertLabelToTbl (const char* name, int line_num) {
    unsigned int hashVal;
    Label *ancestor;
    Label *new_label = malloc(sizeof(Label));
    if (!new_label) {
        perror("malloc");
        return;
    }
    new_label->name = malloc(strlen(name) + 1);
    if (!new_label->name) {
        perror("malloc");
        free(new_label);
        return;
    }

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
                return; // Duplicate label
            }
            ancestor = ancestor->next;
        }
        ancestor->next = new_label;
    }
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
    hash_val = hashLabel(word);
    label = label_table[hash_val];

    while (label && strcmp((label->name), word)!= 0)
    {
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
    Label *label;
    for (i = 0; i < LABEL_HASH_SIZE; i++) {
        label = label_table[i];
        if (label) {
            while (label) {
                printf("  Label: %s\n", label->name);
                label = label->next;
            }
        }
    }
}