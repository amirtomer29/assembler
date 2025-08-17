#include <stdio.h>
#include "preAssembler.h"
#include "firstPass.h"
#include "secondPass.h"
#include "labelTable.h"
#include "macroTable.h"
#include "main.h"

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
    "mov", "cmp", "add", "sub", "lea", "clr","not",
     "inc", "dec", "jmp", "bne", "jsr", "red",
    "prn", "rts", "stop"
};

/*initialing hash tables*/
Macro *macro_table[MACRO_HASH_SIZE] = {0};
Label *label_table[LABEL_HASH_SIZE] = {0};

char *memory_d[256];    /*array of instructions coded in the file, used in firstPass.c*/
char *memory_c[256];

int IC = 0; /* Initialize instruction counter */
int DC = 0; /* Initialize data counter */


int main(int argc, char *argv[]){
    int i;
    FILE *file_after_pre_p = NULL; /*file after pre-assembling*/
    char *file_after_pre_assembling = NULL;
    char line[MAX_LINE_LEN + 2]; /*+2 for \0 and \n*/
    
    /*initialize the memory arrays with NULL*/
    i = 0;
    while (i<256) {
        memory_c[i] = NULL; 
        i++;
    }
    i=0;
    while (i<256) {
        memory_d[i] = NULL; 
        i++;
    }



    initialMacroTable();
    initialLabelTable();
    

    /*loading the files and calling the pre assembler*/
    for(i=1 ;i<argc;i++){
        file_after_pre_assembling = NULL;
        file_after_pre_assembling = preAssembler(argv[i]);
       
        
        if (!file_after_pre_assembling) {
            fprintf(stderr, "Error: pre-assembling failed for file %s\n", argv[i]);
            free(file_after_pre_assembling);
            continue; /*skip to the next file if pre-assembling failed*/
        } else{
            printf("File %s is after pre-assembler successfully.\n", file_after_pre_assembling);
            file_after_pre_p = fopen(file_after_pre_assembling, "r"); /*open the pre-assembled file*/
            while (fgets(line, MAX_LINE_LEN+2, file_after_pre_p) != NULL) /*+2 for \0 and \n*/ {
                printf("%s", line); /*print the line to the console*/
            }
            fclose(file_after_pre_p); /*close the pre-assembled file*/
        }
        /*call first pass on the pre-assembled file*/
        if (firstPass(file_after_pre_assembling) == 1) {
            fprintf(stderr, "File %s is after FIRSTPASS successfully.\n", file_after_pre_assembling);
            printf("\n\nIC\n\n");
            for (i = 0; i <= IC; i++) {
            printf("%d : %s\t",i, memory_c[i]); /*free the memory allocated for the instructions*/     
            }
            printf("\n\nDC\n\n");
            for (i = 0; i <=  DC; i++) {
            printf("%d : %s\t",i, memory_d[i]); /*free the memory allocated for the instructions*/     
            }
            secondPass(file_after_pre_assembling);
            for (i = 0; i <= IC; i++) {
            printf("%d : %s\t",i, memory_c[i]); /*free the memory allocated for the instructions*/     
            }
            printf("\n\nDC\n\n");
            for (i = 0; i <=  DC; i++) {
            printf("%d : %s\t",i, memory_d[i]); /*free the memory allocated for the instructions*/     
            }
        }
        else {
            fprintf(stderr, "File %s is ERROR FIRSTPASS.\n", file_after_pre_assembling);
        }


    }
    return 0;
}