#include "preAssembler.h"
#include "macroTable.h"
#include "labelTable.h"

/* preAssembler.c
 * This file contains the implementation of the pre-assembler functionality.
 * It processes the input assembly files, handles macros, and manages labels.
 */
int preAssembler(char *filename){
    int error = 0;
    char line[MAX_LINE_LEN+2];
    int line_num = 0;
    char *word;
    Macro *macro = NULL;
    char name[MAX_LABEL_NAME];
    char new_file_name[FILENAME_MAX];

    /*loading the files*/
    FILE *src = fopen(filename, "r");
    FILE *out = fopen("temp.txt", "w");

    /*check error while loading the files*/
    if (!src||!out) {
        perror("cant open file");
    } else {
        
        /*main loop - while the file not over*/
        while (fgets(line, MAX_LINE_LEN+2, src) != NULL) /*+2 for \0 and \n*/{
            line_num++;
            
            /*if line excceeds maximum length of line*/
            if (strchr(line, '\n')== NULL) {
                fprintf(stderr, "Error: line %d excceeds %d charcters", line_num, MAX_LINE_LEN);
                error = 1;
                continue;
            } 
            
            word = getWord(line);

            /*if getWord failed*/
            if(!word){
                free(word);
                continue;
            }

            /*if line is comment line skip line*/
            if (word[0] == ';') {
                free(word);
                continue;
            }

            /*if line is body line of macro*/
            if (macro) {

                /*if line is the end of body line of macro*/
                if (strcmp(word, "mcroend")== 0) {
                    macro = NULL;
                    /*if there is somthing after mcroend its error*/
                    if (sscanf(line, "%*s %s", word) == 1) {
                        fprintf(stderr, "Error: line %d macro %s end of line illegal\n", line_num, name);
                        error = 1;
                    }
                    free(word);
                    continue;
                }
                /*if line is body line of macro*/
                 else {
                    insertLineToMacro(macro, line);
                }
                free(word);
                continue;
            }    

            /*if word is macro*/
            if (checkMacroExist(word)) {
                fputs(getMacroBody(word), out);
                free(word);
                continue;
            } 
            
            /*if word is labal*/
            if (word[strlen(word)-1] == ':'){
                word[strlen(word)-1] = '\0'; /*remove the ':'*/
                insertLabelToTbl(word, line_num);
                fputs(line, out);
                free(word);
                continue;
            } 

            /*if line is macro definition line*/
            if ((strcmp(word, "mcro" )) == 0) {
                sscanf(line, "%*s %s", name);
                /*if line is not mcro and name only its error*/
                if (sscanf(line, "%*s %*s %s", word) == 1) {
                    fprintf(stderr, "Error: line %d macro %s end of line illegal\n", line_num, name);
                    error = 1;
                    free(word);
                    continue;
                }
                /*check if macro name is legal*/
                if (checkMacroName(name, line_num)) {
                    macro = insertMacroNameToTbl(name);
                } else {
                    error = 1;  
                }
                free(word);
                continue;
            }

            /*if line is instruction or opcode line*/
            else {
                fputs(line, out);
                free(word);
            }
        }

        /*finished running in the file*/

        /*if error occured while reading the file*/
        if (error) {
            fprintf(stderr, "\nError occured while reading the file %s\n", filename);
            remove("temp.txt"); /*delete the temp file*/
        }
        /*if no error occured while reading the file*/
        else {
            /*rename the temp file to the original file name*/
            strncpy(new_file_name, filename, FILENAME_MAX - 4);
            new_file_name[FILENAME_MAX - 4] = '\0'; 
            strcat(new_file_name, ".am");
            if (rename("temp.txt", new_file_name) != 0) {
                perror("Error renaming file");
                remove("temp.txt"); /*delete the temp file*/
            } else {
                printf("File %s pre-assembled successfully.\n", filename);
            }
        }

    /*print the label and macro tables*/
    printLabelTable();
    printMacroTable();

    /*close the files*/
    fclose(src);
    fclose(out);
    }
    return 1;
}

/* Function to skip spaces in a line
// Returns the number of spaces skipped*/
int skipSpaces (char *line) {
    int i = 0;
    while (line[i] == '\t' || line[i] == ' ') {i++;};
    return i;
}

/* Function to get a word from a line
// Returns a dynamically allocated string containing the word,
or NULL if no word is found */
char* getWord(char *line){
    int i = skipSpaces(line);
    int start = i;
    int len = 0;
    char *word = NULL;
    if (line[i] == '\0' || line[i] == '\n') {
        return NULL; /*no word found*/
    }
    /*find the end of the word*/
    while (line[i] != '\t' && line[i] != ' ' && line[i] != '\n' && line[i] != '\0'){
        i++;
    }
    len = i - start;
    word = malloc(len + 1);
    if (!word) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    strncpy(word, &line[start], len);
    word[len] = '\0';
    return word;
}


