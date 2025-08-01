#define MACRO_HASH_SIZE 31

typedef struct Macro {
    char *name;
    char *body;
    struct Macro *next;
} Macro;

char* getMacroBody (char *name);

Macro *macro_table[MACRO_HASH_SIZE];

Macro *checkMacroExist(char *word);

int checkMacroName(char *word);

void insertMacroNameToTbl(char word);