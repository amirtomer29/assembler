#ifndef MACRO_TABLE_H
#define MACRO_TABLE_H







typedef struct Macro {
    char *name;
    char *body;
    struct Macro *next;
} Macro;


char* getMacroBody (char *name);

Macro *checkMacroExist(char *word);

int checkMacroName(char *word, int line_num);

Macro *insertMacroNameToTbl(const char *word);

void insertLineToMacro(Macro *macro, char* line);

void initialMacroTable ();

void printMacroTable() ;


#endif /* MACRO_TABLE_H */