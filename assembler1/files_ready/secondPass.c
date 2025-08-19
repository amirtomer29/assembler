#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "labelTable.h"
#include "parsers.h"
#include "instructions.h"
#include "opCode.h"
#include "format.h"
#include "secondPass.h"

/**
 secondPass
 Performs the second pass of the assembler.
 - Opens source file and creates output files (.ob, .ent, .ext).
 - Parses labels, instructions, and opcodes.
 - Handles .entry and .extern directives.
 - Calls handlers for instructions/opcodes in pass 2.
 - Creates final output files if no errors occur.

 @param filename - source file to process
 @return   - 1 on success, 0 on failure
*/
int secondPass(char *filename) {
    int error = 0;
    int line_num = 0;
    int extFound = 0;
    int entFound = 0;
    int instr_index;
    int opcode_index;
    char *ch_in_line = NULL;
    char line[MAX_LINE_LEN + 2]; /* +2 for \0 and \n */
    char *line_p;
    char *word;
    char *obj_file;
    char *ent_file;
    char *ext_file;
    Label *label = NULL;
    FILE *src, *obj, *ext, *ent;

    /* Build filenames for output files */
    obj_file = buildFileName(filename, ".ob");
    ent_file = buildFileName(filename, ".ent");
    ext_file = buildFileName(filename, ".ext");
    
    /* Open input and output files */
    src = fopen(filename, "r");
    obj = fopen(obj_file, "w");
    ext = fopen(ext_file, "w");
    ent = fopen(ent_file, "w");

    /* Check for file errors */
    if (!src||!obj||!ext||!ent) {
        perror("cant open file");
        free(obj_file);
        free(ent_file);
        free(ext_file);
        return 0;
    } else {
        IC = 0;
        /* Main loop: read each line */
        while (fgets(line, MAX_LINE_LEN+2, src) != NULL){
            line_num++;
            line_p = line;
            word = getWord(line); /* Get first word */

            if (!word) {
                continue; /* Empty line */
            }
             
            /* Check if line starts with a label */
            label = checkLabelExist(strtok(word, ":"));
            if (label) {
                /* Skip label and get next word */
                ch_in_line = strchr(line, ':');
                line_p = ch_in_line + 1;
                word = getWord(line_p);
                if (!word) {
                    continue;
                }
            }

            /* Check if word is an instruction */
            instr_index = getInstructionType(word);
            if (instr_index == 0 || instr_index == 1 || instr_index == 2 ) {
                if (word) {
                    free(word);
                }
                continue;   
            }
            if (instr_index == 3){
                line_p = jump_next_word(line_p);
                /* Handle .entry instruction */
                if(instructionHandler(instr_index, line_p, NULL, line_num, 2) == -1) {
                    error = 1;
                }
                entFound = 1;
                if (word) {
                    free(word);
                }
                continue;
            }
            if(instr_index == 4){
                /* Found extern directive */
                extFound = 1;
                if (word) {
                    free(word);
                }
                continue;
            }

            /* Check if word is an opcode */
            opcode_index = getOpCodeIndex(word);
            if (opcode_index != -1) {
                line_p = jump_next_word(line_p);
                /* Handle opcode in pass 2 */
                if (opCodeHandler(line_p, line_num, opcode_index , label, 2) == -1) {
                    error = 1;
                }
                if (word) {
                    free(word);
                }
                continue;
            }
        }

        /* If any error occurred, delete output files */
        if (error) {
            fprintf(stderr, "\nError occured while reading the file in the SECPASS%s\n", filename);
            remove(ext_file);
            remove(ent_file);
            remove(obj_file); 
            fclose(src);
            free(obj_file);
            free(ent_file);
            free(ext_file);
            return 0;
        }

        /* Create ext/ent/obj files */
        if (!createExtEntFile(ent, ext)){
            fprintf(stderr,"nError: create Ext or Ent File FAILED");
            error = 1;
        } else {
            fclose(ext);
            fclose(ent);
            ext = fopen(ext_file, "r");
            ent = fopen(ent_file, "r");
            if(fgetc(ent)==EOF){
                entFound = 0;
            }
            if(fgetc(ext)==EOF){
                extFound = 0;
            }
        }
        if (!createObjFile(obj)){
            fprintf(stderr,"nError: createObjFile FAILED");
            error = 1;
        } 
        /* If failed, cleanup */
        if (error) {
            fprintf(stderr, "\nError occured while creating the files in the SECOND - PASS%s\n", filename);
            remove(ext_file);
            remove(ent_file);
            remove(obj_file); 
            fclose(src);
            free(obj_file);
            free(ent_file);
            free(ext_file);
            return 0;
        }
    }

    /* Close created files */
    fclose(src);
    fclose(obj);
    fclose(ext);
    fclose(ent);
    if (!extFound) {
            remove(ext_file);
    }
    if (!entFound) {
            remove(ent_file);
    }
    return 1;
}

/**
 createExtEntFile
 Writes entry (.ent) and extern (.ext) labels to output files.

 ent - pointer to entry file
 ext - pointer to extern file
 return - 1 on success, 0 on failure
*/
int createExtEntFile(FILE *ent, FILE *ext) {
    Label *current_label;
    int i, type;
    char *formatted_address;
    char *line;
    int extern_apears_left;
    
    line = malloc(LINE_BUFFER_SIZE);
    if (!line) {
        perror("malloc");
        return 0;
    }

    /* Iterate over label table */
    for (i = 0; i < LABEL_HASH_SIZE; i++) {
        current_label = label_table[i];
        while (current_label) {
            type = getInstructionType(current_label->type);
           
            if (type == INSTR_ENTRY) {
                /* Handle .entry label */
                formatted_address = toQuadAddressFormat(toQuad(toBinary(current_label->location)));
                sprintf(line, "%s\t%s\n", current_label->name, formatted_address);
                fputs(line, ent);
            } 
            if (type == INSTR_EXTERN) {
                /* Handle .extern label */
                extern_apears_left = current_label->extern_locatins_num;
                if (extern_apears_left==0){
                    fprintf(stderr, "WARNING: label %s type .extern never used!\n",current_label->name);
                }
                for (;extern_apears_left > 0; extern_apears_left--){
                    formatted_address = toQuadAddressFormat(toQuad(toBinary(REAL_MEMORY_LOAD +current_label->extern_locatoins[extern_apears_left-1])));
                    sprintf(line, "%s\t%s\n", current_label->name, formatted_address);
                    fputs(line, ext);
                }
            }
            current_label = current_label->next;
        }
    }
    free(formatted_address);
    free(line);
    return 1;
}

/**
 createObjFile
 Writes the object code to the .ob file:
 - First line contains IC and DC.
 - Then writes memory image of instructions and data.

 obj - pointer to object file
 return - 1 on success, 0 on failure
*/
int createObjFile(FILE *obj) {
    char *line;
    int i;
    
    if (!obj) {
        fprintf(stderr, "Invalid file pointer passed to createObFile.\n");
        return 0;
    }

    line = malloc(LINE_BUFFER_SIZE);
    if (!line) {
        perror("malloc");
        return 0;
    }

    /* Write IC and DC as the first line */
    sprintf(line, "%s\t%s\n", toQuad(toBinary(IC)), toQuad(toBinary(DC)));
    fputs(line, obj);

    /* Write instruction memory */
    for (i=0; i<IC; i++){
        sprintf(line, "%s\t%s\n", toQuadAddressFormat(toQuad(toBinary(i+REAL_MEMORY_LOAD))), memory_c[i]);
        fputs(line, obj);
    }

    /* Write data memory */
    for (i=0; i<DC; i++){
        sprintf(line, "%s\t%s\n", toQuadAddressFormat(toQuad(toBinary(i+IC+REAL_MEMORY_LOAD))), memory_d[i]);
        fputs(line, obj);
    }
    free(line);
    return 1;
}
