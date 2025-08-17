#include "preAssembler.h"
#include "main.h"
#include "labelTable.h"
#include "macroTable.h"
#include "instructions.h"
#include "opCode.h"
#include <ctype.h>
#include "firstPass.h"
#include "parsers.h"

int firstPass(char *filename) {
    int error = 0;
    int line_num = 0;
    int instr_index;
    int opcode_index;
    char line[MAX_LINE_LEN + 2]; /*+2 for \0 and \n*/
    char* line_p;
    char* first_word= NULL;
    char* pnt = NULL;
    char* instruction_or_opcode = NULL;
    char* end_line = NULL;
    Label *label = NULL;
    FILE *src;

    /*loading the files*/
    src = fopen(filename, "r");
    

    /*check error while loading the files*/
    if (!src) {
        perror("cant open file");
        return 0;
    } else {
        /*main loop - while the file not over*/
        while (fgets(line, MAX_LINE_LEN+2, src) != NULL) /*+2 for \0 and \n*/ {

            label=NULL;
            printf("\tnew line: \t%s\n", line); /*print the line to the console*/
            line_num++;
            line_p = line; /*set the line pointer to the start of the line*/
            line_p = trim_whitespace(line_p);
            end_line = line_p + strlen(line_p);
            if (line_p == end_line) {
                continue;
            }  

            first_word = line_p;
            /*check if line is start with label*/
            pnt = strchr(first_word, ':');
            if (pnt != NULL) {
                line_p = pnt + 1;/*Advance the line pointer past the label*/
                if (line_p >= end_line) {
                    fprintf(stderr, "ERROR: empty label at line %d", line_num);
                    error = 1;
                    continue;
                }  
                line_p = trim_whitespace(line_p);
                *pnt = '\0'; /* Null-terminate the label name*/
                label = checkLabelExist(first_word);
                *pnt = ':';
            }

            /*if was label -we get next word , else still first word*/
            instruction_or_opcode = line_p;
            pnt = trim_char(line_p);
            if (pnt < end_line) {
                *pnt = '\0'; /* Null-terminate the word*/
            } 
            line_p = pnt+1;


            /*if line has instruction word*/
            instr_index = getInstructionType(instruction_or_opcode);
            if (instr_index != -1) { 
                line_p = trim_whitespace(line_p);
                if(instructionHandler(instr_index, line_p>=end_line? NULL : line_p, label, line_num, 1) == -1) {
                    error = 1; /*error in instruction handling*/
                }
                continue;
            }

            /*if line have opcode word*/
            opcode_index = checkIfOpCode(instruction_or_opcode);
            if (opcode_index != -1) {
                line_p = trim_whitespace(line_p);
                if (opCodeHandler(line_p>=end_line? NULL : line_p, line_num, opcode_index , label, 1) == -1) {
                    error = 1; /*error in opcode handling*/ 
                }
                continue;
            }
        } 
        
        /*if error occured while reading the file*/
        if (error) {
            fprintf(stderr, "\nError occured in firstPass in the file %s\n", filename);
            return 0;
        } else {
            /*update locations of the data labels*/
            updateDataLabelsLocations(IC);
            return 1; /*return success*/
        }
    }
}







