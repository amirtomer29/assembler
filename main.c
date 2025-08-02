#include "main.h"
#include "preAssembler.h"
#include "macroTable.h"
#include "labelTable.h"

/*initialing instruction_names*/
const char *instruction_names[] = {
    ".data",
    ".string",
    ".mat",
    ".entry",
    ".extern"
};

/*initialing opcode_names*/
const char *opcode_names[] = {
    "mov", "cmp", "add", "sub", "not", "clr",
    "lea", "inc", "dec", "jmp", "bne", "red",
    "prn", "jsr", "rts", "stop"
};

/*initialing hash tables*/
Macro *macro_table[MACRO_HASH_SIZE];
Label *label_table[LABEL_HASH_SIZE];


int main(int argc, char *argv[]){
    int i;
    
    initialMacroTable();
    initialLabelTable();
    

    /*loading the files and calling the pre assembler*/
    for(i=1 ;i<argc;i++){
        preAssembler(argv[i]);
    };
    
    return 0;
}