#include "macroTable.h"
#include "labelTable.h"
#include "preAssembler.h"


void initialMacroTable () {
    int i;
    for (i=0; i<MACRO_HASH_SIZE; i++){
        macro_table[i] = NULL;
    }
}

/*hash function*/
unsigned int hashMacro(const char* str) {
    unsigned int hash = 0;
    while (*str) {
        hash = (hash * MACRO_HASH_SIZE + *str++) % MACRO_HASH_SIZE;
    }
    return hash;
}

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

char* getMacroBody(char *name) {
    Macro *macro = checkMacroExist(name);
    return (macro) ? macro->body : NULL;
}

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

int checkMacroName(char *word){
    int i;
    for (i = 0; i<16; i++){
        if (strcmp(word, opcode_names[i]) == 0 ) {return 0;}
    }
    if (checkLabelExist(word)) {return 0;}
    if (checkMacroExist(word)) {return 0;}
    return 1;
}


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


