#include "format.h"
#include "labelTable.h"
#include <string.h>
#include "main.h"
#include "firstPass.h"
#include "instructions.h"
#include "parsers.h"    






/* Function to check if a word is an instruction
// Returns the index of the instruction in the instruction_names array if it is an instruction
// Returns 6 if it is not an instruction
// The function iterates through the instruction_names array to find a match
// The function does not check for NULL pointers, which should be handled by the caller
// The function does not check if the word is NULL, which should be handled by the caller
*/
int getInstructionType(char *word) {
    char *trimed;
    int i;
    if (!word) {
        return -1; 
    }
    trimed = malloc(MAX_NAME_LEN);
    if (!trimed){
        perror("malloc");
        return -1;
    }
    strcpy(trimed, word);
    word = trim_whitespace(trimed);
    for (i = 0; i < 5; i++) {
        if (strcmp(word, instruction_names[i]) == 0) {
            free(trimed);
            return i; /*word is an instruction*/
        }
    }
    free(trimed);
    return -1; /*word is not an instruction*/
}

/* Function to handle instructions
// The function takes the type of instruction, the line, a label (if any), and the line number
// It processes the instruction based on its type and updates the data counter (DC  or instruction counter (IC) accordingly
// The function returns the type of instruction if successful, or -1 if there is an error
// The function uses the instruction_names array to get the name of the instruction
// The function does not check for NULL pointers, which should be handled by the caller
// The function does not check if the line is NULL, which should be handled by the caller
*/
int instructionHandler(int type, char *line, Label *label, int line_num, int pass) {
    char *token;
    int i, num, num2;
    size_t len;
    char name[MAX_LABEL_NAME];
    char inst_name[MAX_INSTRUCTION_NAME_LENGTH];

    strcpy(inst_name, instruction_names[type]);
    
    if (line == NULL) {
        fprintf(stderr, "Error: line %d instruction %s is missing data or opernd\n", line_num, inst_name);
        return -1; /*error*/
    }
    
    line = trim_whitespace(line);

    /*handle data*/
    if (type == 0) {

        if (label) {
            label->type = ".data"; /*set label type to data*/
            label->location = DC; /*set label location to data counter*/
        }
        (token = strtok(line, ","));
        while (token != NULL) {
            num = 0; /*initialize num*/
            num = atoi(trim_whitespace(token));
            if (num == 0 && strcmp(token, "0") != 0) {
                fprintf(stderr, "Error: line %d data %s is not a number\n", line_num, token);
                return -1; /*error*/
            }
            if (num < -511 || num > 511) {
                fprintf(stderr, "Error: line %d data %s is out of range\n", line_num, token);
                return -1; /*error*/
            }
            memory_d[DC] = toQuad(toBinary(num)); /*store the data in the data_coded array*/
            DC++;
            token = strtok(NULL, ",");
            }
        return type;
    }


    /*handle string*/ 
    if (type == 1) {
        /*check if the string is valid*/
        len = strlen(line);
        if (len < 2 || line[0] != '"' || line[len-1] != '"') {
            fprintf(stderr, "Error: line %d string %s is not a valid string\n", line_num, line);
            return -1;
        }
        /*if there is label before the string*/
        if (label) {
            label->type = ".string"; /*set label type to string*/
            label->location = DC; /*set label location to data counter*/
        }

        /*store the string in the data_coded array*/

        for (i = 1; i < (strlen(line) - 1); i++) {
            memory_d[DC] = toQuad(toBinary(line[i])); /*convert char to binary and store it*/
            DC++;
        }
        memory_d[DC] = toQuad(toBinary('\0')); /*null terminate the string*/
        DC++; /*increment data counter for null terminator*/
        return type;
    } 

    /*handle mat*/
    if (type == 2) {
        num = 0; /*initialize num*/
        num2 = 0; /*initialize num2*/
        i = 0; /*initialize i*/
        /*check if the matrix is valid and extract the numbers of the suze of the matrix*/
        if (sscanf(line, "[%d][%d]", &num, &num2) != 2) {
            return -1; /*error*/
        }
        if (num < 1 || num2 < 1) {
            fprintf(stderr, "Error: line %d mat %s is not size\n", line_num, line);
            return -1; /*error*/
        }

        /*if there is label before the mat*/
        if (label) {
        label->type = ".mat"; /*set label type to mat*/
        label->location = DC; /*set label location to data counter*/
        }

        num2 = num * num2; /*calculate the size of the matrix*/

        line = jump_next_word(line);
        token = strtok(line, ",");

        while (token  != NULL) {
            token = trim_whitespace(token);
            num = atoi(token);
            if (num == 0 && strcmp(token, "0") != 0) {
                fprintf(stderr, "Error: line %d mat %s is not a number\n", line_num, &line[i]);
                return -1; /*error*/
            }
            if (num < -511 || num > 511) {
                fprintf(stderr, "Error: line %d mat %s is out of range\n", line_num, &line[i]);
                return -1; /*error*/
            }
            memory_d[DC] = toQuad(toBinary(num)); /*store the data in the data_coded array*/
            DC++;
            num2--;
            token = strtok(NULL, ",");
        }
        while (num2 > 0) {
            memory_d[DC] = toQuad(toBinary(0)); /*store the data in the data_coded array*/
            DC++;
            num2--;
        }
        return type; 
    } 
    /*handle entry*/
    if (type == 3) {            
        if(pass == 1){
            /*if there is no label after entry*/
            if (sscanf(line, "%s", name) != 1) {
                fprintf(stderr, "Error: line %d entry is missing operand\n", line_num);
                return -1; /*error*/
            };
            /*if label is not defined*/
            label = checkLabelExist(name);
            if(!label) {
                fprintf(stderr, "Error: entry used on label %s at %d, label doesnt exist in the file\n", name, line_num);
                return -1; /*error*/
            } 
            /*if label is defined*/
            else {
                return type; 
            }
        }
        if (pass == 2) {
             /*if there is no label after entry*/
            if (sscanf(line, "%s", name) != 1) {
            fprintf(stderr, "Error: line %d extern is missing operand\n", line_num);
            return -1; /*error*/
            }
            /*if label is defined in the file*/
            label = checkLabelExist(name);
            if(!label) {
                fprintf(stderr, "Error: entry used on label %s at %d, label alredy exist in the file\n", name, line_num);
                return -1; /*error*/
            }
            /*if label is not defined in the file*/
            else {
                label->type = ".entry"; /*set label type to extern*/
                return type; /*return the type of the extern*/  
            }
        }
    }
    /*handle extern*/
    if (type == 4) {
         /*if there is no label after entry*/
        if (sscanf(line, "%s", name) != 1) {
            fprintf(stderr, "Error: line %d extern is missing operand\n", line_num);
            return -1; /*error*/
        }
        /*if label is defined in the file*/
        label = checkLabelExist(name);
        if(!label) {
            fprintf(stderr, "Error: extern used on label %s at %d, label alredy exist in the file\n", name, line_num);
            return -1; /*error*/
        }
        /*if label is not defined in the file*/
        else {
            label->type = ".extern"; /*set label type to extern*/
            return type; /*return the type of the extern*/  
        }
    }
    else {
        fprintf(stderr, "Error: unknown type %d in line %s\n", type, line);
        return -1; /*error*/
    }
}