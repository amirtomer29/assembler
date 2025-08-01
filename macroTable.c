#include "macroTable.c"
#include "preAssembler.h"


void initialMacroTable () {
    int i = 0;
    for (i; i<MACRO_HASH_SIZE; i++){
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

void insertMacroNameToTbl (const char* name, const char *body) {
    Macro *ancestor;
    Macro *new_macro = malloc(sizeof(MAX_LINE_LEN));
    if (!new_macro) {
        perror("malloc");
    }
    int hashVal = hashMacro(name);
    if (macro_table[hashVal] == NULL) {
        macro_table[hashVal] = new_macro;
    } else {
        ancestor = macro_table[hashVal];
        while (ancestor->next !=NULL)
            {
                ancestor = ancestor->next;
            }
            ancestor->next = new_macro;
            new_macro->name = name;
            new_macro->body = body;
    }
}

void insertLineToMacro(Macro *macro, char* line){
    char *body = macro->body;
    int size = strlen(body)+strlen(line) + 2;
    char *temp = realloc(body, size);
    if (!temp){
        free(str);
        perror("realloc faild");
    } else {
        body = strcat(temp, strcat(line, '\n'));
    }
}


char* getMacroBody (char *name){
    if (checkMacroExcist(name)) {
        return (checkMacroExcist(name))->body;
    }
    return NULL;
}

Macro *checkMacroExist(char *word){
    int hash_val = hashMacro(word);
    Macro *macro = macro_table[hash_val];
    while (strcmp((macro->name), word != 0))
    {
        macro = macro->next;
    } 
    if (!macro) {return NULL;}
    else {return macro;}
}

int checkMacroName(char *word){
    int i = 0;
    for (i; i<17; i++){
        if (strcmp(word, opcode[i])==0) {return 0;}
    }
    if (checkLabelExcist(word)) {return 0;}
    if (checkMacroExcist(word)) {return 0;}
    return 1;
}

void insertMacroNameToTbl(char word) {
    int hash_val = hashMacro(word);
    Macro *new;
    Macro *macro = macro_table[hash_val];
    while (macro->next != NULL)
    {
        macro = macro->next;
    } 
    new->name = word;
    macro->next = new;
}


