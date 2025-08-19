#include <stdio.h>
#include "preAssembler.h"
#include "firstPass.h"
#include "secondPass.h"
#include "labelTable.h"
#include "macroTable.h"
#include "format.h"
#include "main.h"




/*
 * Global hash tables used during assembly to store macros and labels.
 */
/*initialing hash tables*/
Macro *macro_table[MACRO_HASH_SIZE] = {0};
Label *label_table[LABEL_HASH_SIZE] = {0};

/*
 * memory_d / memory_c
 * Program images: data segment (memory_d) and code segment (memory_c).
 * Populated across the passes and written to the .ob file.
 */
char *memory_d[256];    /*array of instructions coded in the file, used in firstPass.c*/
char *memory_c[256];

/*
 * IC / DC
 * Global counters for instruction and data words.
 */
int IC = 0; /* Initialize instruction counter */
int DC = 0; /* Initialize data counter */

/**
 * main
 * Orchestrates the assembly flow over all input files:
 *  1) Initialize tables and memory.
 *  2) Run pre-assembler to expand macros (.am output).
 *  3) Run first pass (symbol resolution, partial encoding).
 *  4) Run second pass (final encoding, generate .ob/.ent/.ext).
 *
 * argc - number of command-line arguments
 * argv - array of input file names
 * return - process exit code (0 on success path here)
 */
int main(int argc, char *argv[]){
    int i, j;
    FILE *file_after_pre_p = NULL; /*file after pre-assembling*/
    char *file_after_pre_assembling = NULL;
    if(argc==1){
        fprintf(stderr,"error: no files have given to the assembler\n");
        return 0;
    }
    /* iterate over input files and process each one */
    /* loading the files and calling the pre assembler */
    for(j=1 ;j<argc;j++){
        /* initialize the memory arrays with NULL */
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

        /* initialize global tables */
        initialMacroTable();
        initialLabelTable();

        IC=0;
        DC=0;

        printf("file %s is pre-assembling\n",argv[j]);
        file_after_pre_assembling = NULL;
        file_after_pre_assembling = preAssembler(argv[j]);
       
        /* if pre-assembler failed on this file, report and continue to next */
        if (!file_after_pre_assembling) {
            fprintf(stderr, "Error: pre-assembling failed for file %s\n", argv[j]);
            free(file_after_pre_assembling);
            continue; /*skip to the next file if pre-assembling failed*/
        } else{
            /* preview the .am content (debug/verification) */
            printf("File %s is pre-assembled successfully.\n", file_after_pre_assembling);
            file_after_pre_p = fopen(file_after_pre_assembling, "r"); /*open the pre-assembled file*/
            fclose(file_after_pre_p); /*close the pre-assembled file*/
        }

        /* run first pass over the .am file */
        /* call first pass on the pre-assembled file */
        if (firstPass(file_after_pre_assembling) == 1) {
            fprintf(stderr, "File %s is after FIRST-PASS successfully.\n", file_after_pre_assembling);
        }
        else {
            /* first pass failed; report and move on */
            fprintf(stderr, "File %s got ERROR at FIRST-PASS.\n", file_after_pre_assembling);
            continue;;
        }

        /* run second pass to finalize encoding and write outputs */
        if (secondPass(file_after_pre_assembling)==1){
            fprintf(stderr, "File %s is ASSEMBELD!\n\n", file_after_pre_assembling);
        }
        else {
            /* first pass failed; report and move on */
            fprintf(stderr, "File %s got ERROR at SECOND-PASS.\n", file_after_pre_assembling);
        }
    }
    return 0;
}
/*check if IC + DC + REAL_MEMORY + new_words_LOAD no exceed memory size if yes retorn 0 else 1*/
int memory_check (int num_new_words){
    if ((IC + DC + REAL_MEMORY_LOAD + num_new_words) >= MEMORY_SIZE) {
        fprintf(stderr, "ERROR: memory is full!\n");
        return 0;
    }
    return 1;
}

/**
 * insertToMemory - Store a value in instruction (IC) or data (DC) memory.
 *
 * @inst_data: INST for instruction memory, DATA for data memory
 * @value:     The number to store
 * @format:    VALUE for normal value, ADDRESS for address format
 *
 * Returns: 1 on success, 0 on failure (invalid input or no memory left).
 */
int insertToMemory (int inst_data, int value, int format){
    if(memory_check(1)){
        if (inst_data==INST){
            if(format==VALUE){
                memory_c[IC]=toQuad(toBinary(value));
                IC++;
                return 1;
            }
            if(format==ADDRESS){
                memory_c[IC]=toQuadAddressFormat(toQuad(toBinary(value)));
                memory_c[IC][4] = 'c';
                IC++;
                return 1;
            }else {return 0;}
        }
        if (inst_data==DATA){
            if(format==VALUE){
                memory_d[DC]=toQuad(toBinary(value));
                DC++;
                return 1;
            }
            if(format==ADDRESS){
                memory_d[DC]=toQuadAddressFormat(toQuad(toBinary(value)));
                memory_d[DC][4] = 'c';
                DC++;
                return 1;
            }
            else {return 0;}
        }
        else {return 0;}
    } else {
        return 0;
    }
}