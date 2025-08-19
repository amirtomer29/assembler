#include "format.h"
#include "labelTable.h"
#include <string.h>
#include "main.h"
#include "firstPass.h"
#include "instructions.h"
#include "parsers.h"    

/**
 * Function: getInstructionType
 * Checks if a given word is a valid instruction directive (.data, .string, .mat, etc.).
 * 
 * @param word - the word to be checked (e.g., ".data")
 * @return index (0-4) if the word is an instruction, -1 otherwise.
 * 
 * Notes:
 * - Caller must ensure that `word` is not NULL.
 * - Uses instruction_names[] to compare.
 */
int getInstructionType(char *word) {
    char *trimed;
    int i;

    /*initialing instruction_names*/
    char *instruction_names[] = {
        ".data",
        ".string",
        ".mat",
        ".entry",
        ".extern",
        "DEFAULT"
    };

    if (!word) {
        return -1; 
    }
    trimed = malloc(MAX_NAME_LEN);
    if (!trimed){
        perror("malloc");
        return -1;
    }
    strcpy(trimed, word);
    word = trim_whitespace(trimed); /** Remove leading/trailing spaces. */

    for (i = 0; i < 5; i++) {
        if (strcmp(word, instruction_names[i]) == 0) {
            free(trimed);
            return i; /** Word is a valid instruction type. */
        }
    }
    free(trimed);
    return -1; /** Not a recognized instruction. */
}

/**
 * Function: instructionHandler
 * Parses and handles instruction lines such as .data, .string, .mat, .entry, .extern.
 * 
 * @param type     - index of the instruction (0 = .data, 1 = .string, etc.)
 * @param line     - the remainder of the instruction line (after the keyword)
 * @param label    - optional label associated with the instruction
 * @param line_num - the current source line number (for error reporting)
 * @param pass     - which assembler pass is running (1 or 2)
 * 
 * @return the instruction type on success, -1 on failure
 * 
 * Behavior:
 * - Updates memory_d[] and the label's location/type.
 * - Verifies correct formatting and values.
 * - Assumes global DC and memory_d are initialized.
 */
int instructionHandler(int type, char *line, Label *label, int line_num, int pass) {
    char *token;
    int i, num, num2;
    size_t len;
    char name[MAX_LABEL_NAME];
    char inst_name[MAX_INSTRUCTION_NAME_LENGTH];

    strcpy(inst_name, getInstructionName(type));

    if (line == NULL) {
        fprintf(stderr, "Error: line %d instruction %s is missing data or opernd\n", line_num, inst_name);
        return -1; /** Line is missing parameters. */
    }

    line = trim_whitespace(line); /** Clean up the line before parsing. */

    /*handle data*/
    if (type == INSTR_DATA) {
        if (label) {
            label->type = ".data"; /** Assign label type. */
            label->location = DC; /** Label's address is current DC. */
        }
        (token = strtok(line, ","));
        while (token != NULL) {
            num = 0;
            num = atoi(trim_whitespace(token));
            if (num == 0 && strcmp(token, "0") != 0) {
                fprintf(stderr, "Error: line %d data %s is not a number\n", line_num, token);
                return -1;
            }
            if (num < MIN_INTEGER_VALUE || num > MAX_INTEGER_VALUE) {
                fprintf(stderr, "Error: line %d data %s is out of range\n", line_num, token);
                return -1;
            }
            if(!insertToMemory(DATA, num, VALUE)){return -1;} /** Store value in data memory. */
            token = strtok(NULL, ",");
        }
        return type;
    }

    /*handle string*/ 
    if (type == INSTR_STRING) {
        len = strlen(line);
        if (len < 2 || line[0] != '"' || line[len-1] != '"') {
            fprintf(stderr, "Error: line %d string %s is not a valid string\n", line_num, line);
            return -1;
        }

        if (label) {
            label->type = ".string";
            label->location = DC;
        }

        for (i = 1; i < (strlen(line) - 1); i++) {
            if(!insertToMemory(DATA, line[i], VALUE)){return -1;}
        }
        if(!insertToMemory(DATA, '\0', VALUE)){return -1;}

        return type;
    }

    /*handle mat*/
    if (type == INSTR_MAT) {
        num = 0;
        num2 = 0;
        i = 0;

        /** Parse matrix dimensions from format: [rows][cols] */
        if (sscanf(line, "[%d][%d]", &num, &num2) != 2) {
            fprintf(stderr, "Error: line %d mat %s is not matrix define\n", line_num, line);
            return -1;
        }

        if (num < 1 || num2 < 1) {
            fprintf(stderr, "Error: line %d mat %s is not size of matrix\n", line_num, line);
            return -1;
        }

        if (label) {
            label->type = ".mat";
            label->location = DC;
        }

        num2 = num * num2; /** Total elements in matrix. */

        line = jump_next_word(line); /** Skip matrix size token. */
        token = strtok(line, ",");

        while (token  != NULL) {
            token = trim_whitespace(token);
            num = atoi(token);
            if (num == 0 && strcmp(token, "0") != 0) {
                fprintf(stderr, "Error: line %d mat %s is not a number\n", line_num, token);
                return -1;
            }
            if (num < MIN_INTEGER_VALUE || num > MAX_INTEGER_VALUE) {
                fprintf(stderr, "Error: line %d mat %s is out of range\n", line_num, token);
                return -1;
            }
            if(!insertToMemory(DATA, num, VALUE)){return -1;}
            num2--;
            token = strtok(NULL, ",");
        }

        /** Fill remaining matrix slots with zeroes (padding). */
        while (num2 > 0) {
            if(!insertToMemory(DATA, 0, VALUE)){return -1;}
            num2--;
        }
        if (num2<0)
        {
            fprintf(stderr, "Error: line %d mat have too much values\n", line_num);
            return -1;
        }
        
        return type;
    }

    /*handle entry*/
    if (type == INSTR_ENTRY) {            
        if (pass == 1) {
            if (sscanf(line, "%s", name) != 1) {
                fprintf(stderr, "Error: line %d entry is missing operand\n", line_num);
                return -1;
            }
            label = checkLabelExist(name);
            if (!label) {
                fprintf(stderr, "Error: entry used on label %s at %d, label doesnt exist in the file\n", name, line_num);
                return -1;
            } else {
                return type;
            }
        }

        if (pass == 2) {
            if (sscanf(line, "%s", name) != 1) {
                fprintf(stderr, "Error: line %d extern is missing operand\n", line_num);
                return -1;
            }
            label = checkLabelExist(name);
            if (!label) {
                fprintf(stderr, "Error: entry used on label %s at %d, label alredy exist in the file\n", name, line_num);
                return -1;
            } else {
                if(getInstructionType(label->type)==INSTR_DEFAULT){
                    fprintf(stderr, "Error: label %s type .entry never defined!\n",label->name);
                    return -1;
                }
                label->type = ".entry";
                return type;
            }
        }
    }

    /*handle extern*/
    if (type == INSTR_EXTERN) {
        if (sscanf(line, "%s", name) != 1) {
            fprintf(stderr, "Error: line %d extern is missing operand\n", line_num);
            return -1;
        }
        label = checkLabelExist(name);
        if (!label) {
            fprintf(stderr, "Error: extern used on label %s at %d, label alredy exist in the file\n", name, line_num);
            return -1;
        } else {
            label->type = ".extern";
            return type;
        }
    }

    else {
        fprintf(stderr, "Error: unknown type %d in line %s\n", type, line);
        return -1;
    }
}


char* getInstructionName(int instruction_index) {
    /*initialing instruction_names*/
    char *instruction_names[] = {
        ".data",
        ".string",
        ".mat",
        ".entry",
        ".extern",
        "DEFAULT"
    };
    if (instruction_index<0 || instruction_index>4) {
        return NULL;
    }
    return instruction_names[instruction_index];
}