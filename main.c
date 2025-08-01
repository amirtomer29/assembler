#include "preAssembler.h"
#include "macroTable.h"
#include "labelTable.h"



/*initialing hash tables*/
Macro *macro_table[MACRO_HASH_SIZE];
Label *label_table[LABEL_HASH_SIZE];


int main(int argc, char *argv[]){
    int i=1;
    
    initialMacroTable();
    initialLabelTable();
    

    /*loading the files and calling the pre assembler*/
    for(i;i<argc;i++){
        preAssembler(argv[i]);
    };  
}