#ifndef MACRO_TABLE_H
#define MACRO_TABLE_H

#define MACRO_HASH_SIZE 31

typedef struct Macro {
    char *name;
    char *body;
    struct Macro *next;
} Macro;


char* getMacroBody (char *name);

Macro *checkMacroExist(char *word);

int checkMacroName(char *word);

Macro *insertMacroNameToTbl(const char *word);

void insertLineToMacro(Macro *macro, char* line);

void initialMacroTable ();

void printMacroTable() ;
#endif /* MACRO_TABLE_H */