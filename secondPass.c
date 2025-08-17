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



int secondPass(char *filename) {
    int error = 0;
    int line_num = 0;
    int extFound = 0;
    int entFound = 0;
    int instr_index;
    int opcode_index;
    char *ch_in_line = NULL;
    char line[MAX_LINE_LEN + 2]; /*+2 for \0 and \n*/
    char *line_p;
    char *word;
    char *obj_file;
    char *ent_file;
    char *ext_file;
    Label *label = NULL;
    FILE *src, *obj, *ext, *ent;

    obj_file = buildFileName(filename, ".ob");
    ent_file = buildFileName(filename, ".ent");
    ext_file = buildFileName(filename, ".ext");
    
    /*loading the files*/
    src = fopen(filename, "r");
    obj = fopen(obj_file, "w");
    ext = fopen(ext_file, "w");
    ent = fopen(ent_file, "w");

    /*check error while loading the files*/
    if (!src||!obj||!ext||!ent) {
        perror("cant open file");
        free(obj_file);
        free(ent_file);
        free(ext_file);
        return 0;
    } else {
        IC = 0;
        while (fgets(line, MAX_LINE_LEN+2, src) != NULL){
            if (error) {printf("ERR");}
            printf("\n\tnew line: \t%s\n", line); /*print the line to the console*/
            line_num++;
            line_p = line; /*set the line pointer to the start of the line*/
            word = getWord(line); /*getWord func is from preAssembler.c*/
            /*if getWord failed*/
            if (!word) {
                continue;
            }
             
            /*if line is start with label*/
            label = checkLabelExist(strtok(word, ":"));
            if (label) {
                /*skip to the next word*/
                ch_in_line = strchr(line, ':'); /*find the colon in the line*/
                line_p = ch_in_line + 1; /*move the line pointer to the next word after the colon*/
                word = getWord(line_p); /*get the next word after the colon*/
                /*if getWord failed*/
                if (!word) {
                    continue;
                }
            }
            /*if line has instruction word*/
            instr_index = getInstructionType(word);
            if (instr_index == 0 || instr_index == 1 || instr_index == 2 ) {
                if (word) {
                    free(word);
                }
                continue;   
            }
            if (instr_index == 3){
                line_p = jump_next_word(line_p);
                if(instructionHandler(instr_index, line_p, NULL, line_num, 2) == -1) {
                    error = 1; /*error in instruction handling*/
                }
                entFound = 1;
                if (word) {
                    free(word);
                }
                continue;

            }
            if(instr_index == 4){
                extFound = 1;
                if (word) {
                    free(word);
                }
                continue;
            }
            /*if line have opcode word*/
            opcode_index = checkIfOpCode(word);
            if (opcode_index != -1) {
                /*skip to the next word*/
                line_p = jump_next_word(line_p);
                if (opCodeHandler(line_p, line_num, opcode_index , label, 2) == -1) {
                    error = 1; /*error in opcode handling*/ 
                }
                if (word) {
                    free(word);
                }
                continue;
            }
        }

        /*if error occured while reading the file*/
        if (error) {
            fprintf(stderr, "\nError occured while reading the file in the SECPASS%s\n", filename);
            /*delete the output files*/
            remove(ext_file);
            remove(ent_file);
            remove(obj_file); 
            fclose(src);
            free(obj_file);
            free(ent_file);
            free(ext_file);
            return 0;
        }


        if (!createExtEntFile(ent, ext)){
            fprintf(stderr,"ERROR: createExtEntFile FAILED");
            error = 1;
        };
        if (!createObjFile(obj)){
            fprintf(stderr,"ERROR: createObjFile FAILED");
            error = 1;
        };

        /*if error occured while reading the file*/
        if (error) {
            fprintf(stderr, "\nError occured while creating the files in the SECPASS%s\n", filename);
            /*delete the output files*/
            remove(ext_file);
            remove(ent_file);
            remove(obj_file); 
            fclose(src);
            free(obj_file);
            free(ent_file);
            free(ext_file);
            return 0;
        }

        fclose(ext);
        fclose(ent);
        fclose(obj);



        ext = fopen(ext_file, "r");
        ent = fopen(ent_file, "r");
        obj =  fopen(obj_file, "r");
        fflush(ent);
        fflush(ext);
        rewind(ext);
        rewind(ent);
        printf("\nEXTFILE:\n"); /*print the line to the console*/
        while (fgets(line, MAX_LINE_LEN+2, ext) != NULL){
            printf("%s", line); /*print the line to the console*/
        }
        printf("\nENTFILE:\n"); /*print the line to the console*/
        while (fgets(line, MAX_LINE_LEN+2, ent) != NULL){
            printf("%s", line); /*print the line to the console*/
        }
        printf("\nOBFILE:\n"); /*print the line to the console*/
        while (fgets(line, MAX_LINE_LEN+2, obj) != NULL){
            printf("%s", line); /*print the line to the console*/
        }
        /*finishing creting files*/
        
    }

    fclose(src);
    fclose(obj);
    fclose(ext);
    fclose(ent);
    if (!extFound) {
            fprintf(stderr, "Warning: no extern found in file %s\n", filename);
            remove(ext_file);; /*delete the ext file*/
    }
    if (!entFound) {
            fprintf(stderr, "Warning: no entry found in file %s\n", filename);
            remove(ent_file); /*delete the ent file*/
    }
    return 1;
}

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

    for (i = 0; i < LABEL_HASH_SIZE; i++) {
        current_label = label_table[i];
        while (current_label) {
            type = getInstructionType(current_label->type);
           
            if (type == 3) {
                formatted_address = toQuadAddressFormat(toQuad(toBinary(current_label->location)));
                sprintf(line, "%s\t%s\n", current_label->name, formatted_address);
                fputs(line, ent);
            } 
            if (type == 4) {
                extern_apears_left = current_label->extern_locatins_num;
                printf("%d %d\n", extern_apears_left, current_label->extern_locatins_num);
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

    /* Write the IC and DC as the first line of the .ob file */
    sprintf(line, "%s\t%s\n", toQuad(toBinary(DC)), toQuad(toBinary(IC)));
    fputs(line, obj);
    printf("OB: %s", line);

    for (i=0; i<IC; i++){
        sprintf(line, "%s\t%s\n", toQuadAddressFormat(toQuad(toBinary(i+REAL_MEMORY_LOAD))), memory_c[i]);
        fputs(line, obj);
        printf("OB: %s", line);
    }
    for (i=0; i<DC; i++){
        sprintf(line, "%s\t%s\n", toQuadAddressFormat(toQuad(toBinary(i+IC+REAL_MEMORY_LOAD))), memory_d[i]);
        fputs(line, obj);
        printf("OB: %s", line);
    }
    printf("OB:done");
    free(line);
    return 1;
}