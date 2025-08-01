#include "labelTable.h"
#include "preAssembler.h"


void initialLabelTable () {
    int i = 0;
    for (i, i<LABEL_HASH_SIZE; i++){
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
    Label *ancestor;
    Label *new_label = malloc(sizeof(MAX_LABEL_LEN));
    if (!new_label) {
        perror("malloc");
    }
    int hashVal = hashLabel(name);
    if (label_table[hashVal] == NULL) {
        label_table[hashVal] = new_label;
    } else {
        ancestor = label_table[hashVal];
        while (ancestor->next !=NULL)
            {
                ancestor = ancestor->next;
            }
            ancestor->next = new_label;
            new_label->name = name;
    }
}

Label checkLabelExcist(char *word){
    int hash_val = hashLabel(word);
    Label *label = label_table[hash_val];
    while (strcmp((label->name), word != 0))
    {
        label = label->next;
    } 
    if (!label) {return NULL;}
    else {return label;}
}