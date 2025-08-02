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

void insertLabelToTbl (const char* name) {
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
        while (ancestor->next !=NULL) {ancestor = ancestor->next;}
        ancestor->next = new_label;
    }
    
}

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