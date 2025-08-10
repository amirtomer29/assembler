#include "firstPass.h"

int firstPass(char *filename) {
    int error = 0;
    int line_num = 0;
    int labelFound = 0;
    int extFound = 0;
    int entFound = 0;
    int instr_index;
    char *index_in_line = NULL;
    char *line;
    char *word;
    char name[MAX_LABEL_NAME];
    Label *label = NULL;

    IC = 100; /* Initialize instruction counter */
    DC = 0; /* Initialize data counter */

    /*loading the files*/
    FILE *src = fopen(filename, "r");
    FILE *obj = fopen("obj.ob", "w");
    FILE *ext = fopen("ext.ext", "w");
    FILE *ent = fopen("ent.ent", "w");


    /*check error while loading the files*/
    if (!src||!obj||!ext||!ent) {
        perror("cant open file");
    } else {
        /*main loop - while the file not over*/
        while (fgets(line, MAX_LINE_LEN+2, src) != NULL) /*+2 for \0 and \n*/ {
            line_num++;

            word = getWord(line); /*getWord func is from preAssembler.c*/
            /*if getWord failed*/
            if (!word) {
                free(word);
                continue;
            }
            /*if line start with instruction word*/
            instr_index = checkIfInstruction(word);
            if (instr_index != -1) {
                    index_in_line = (line, '.'); /*find the point in the line*/
                    line = index_in_line + 1; /*move the line pointer to the next word after the point*/
                if(instructionHandler(instr_index, line, NULL) == -1) {
                    error = 1; /*error in instruction handling*/
                }
                if (instr_index == 3) {
                    entFound = 1; /*entry found*/
                }
                if (instr_index == 4) {
                    extFound = 1; /*extern found*/
                }
                free(word);
                continue;
            }
            /*if line start with opcode word*/

            
            /*if line is start with label*/
            label = checkLabelExist(word);
            /* if the first word is label - move to the next word*/
            if(label) {
                instr_index = checkIfInstruction(sscanf(line, "%*s %s", word));
                if (instr_index != -1) {

                    /*two parts in case label contain "."*/
                    index_in_line = (line, ':'); /*find the colon in the line*/
                    line = index_in_line + 1; /*move the line pointer to the next word after the colon*/
                    index_in_line = (line, '.'); /*find the point in the line*/
                    line = index_in_line + 1; /*move the line pointer to the next word after the point*/
                    
                    if(instructionHandler(instr_index, line, label) == -1) {
                        error = 1; /*error in instruction handling*/
                    }
                    if (instr_index == 3) {
                        entFound = 1; /*entry found*/
                    }
                    if (instr_index == 4) {
                        extFound = 1; /*extern found*/
                    }
                    free(word);
                    continue;
                }
            }
        }
    }
     
    
        
    /*if error occured while reading the file*/
    if (error) {
        fprintf(stderr, "\nError occured while reading the file %s\n", filename);
        remove("obj.ob", "ext.ext", "ent.ent"); /*delete the output files*/
        return 0;
    }
}


/* Function to check if a word is an instruction
// Returns the index of the instruction in the instruction_names array if it is an instruction
// Returns 6 if it is not an instruction
// The function iterates through the instruction_names array to find a match
// The function does not check for NULL pointers, which should be handled by the caller
// The function does not check if the word is NULL, which should be handled by the caller
*/
int checkIfInstruction(char *word) {
    int i;
    for (i = 0; i < 5; i++) {
        if (strcmp(word, instruction_names[i]) == 0) {
            return i; /*word is an instruction*/
        }
    }
    return -1; /*word is not an instruction*/
}

int instructionHandler(int type, char *line, Label *label) {
    char *token;
    int i, num, num2;
    char name[MAX_LABEL_NAME];

    /*handle data*/
    if (type == 0) {

        if (label) {
            label->type = "data"; /*set label type to data*/
            label->location = DC; /*set label location to data counter*/
        }

        while (token = strtok(line, ",") != NULL) {
            num = 0; /*initialize num*/
            num = atoi(token);
            if (num == 0 && strcmp(token, "0") != 0) {
                fprintf(stderr, "Error: line %d data %s is not a number\n", line_num, token);
                return -1; /*error*/
            }
            if (num < -511 || num > 511) {
                fprintf(stderr, "Error: line %d data %s is out of range\n", line_num, token);
                return -1; /*error*/
            }
            data_coded[DC] = toQuad(toBinary(num)); /*store the data in the data_coded array*/
            DC++;
            }
        }
    return 0; /*return the data counter*/
    

    /*handle string*/ 
    if (type == 1) {
        /*check if the string is valid*/
        if (line[0] != '"' || line[strlen(line) - 1] != '"') {
            fprintf(stderr, "Error: line %d string %s is not a valid string\n", line_num, line);
            return -1; 
        }

        /*if there is label before the string*/
        if (label) {
            label->type = "srting"; /*set label type to string*/
            label->location = DC; /*set label location to data counter*/
        }

        /*store the string in the data_coded array*/
        for (i = 1; i < strlen(line) - 1; i++) {
            data_coded[DC] = toQuad(toBinary(line[i])); /*convert char to binary and store it*/
            DC++;
        }
        data_coded[DC] = toQuad(toBinary('\0')); /*null terminate the string*/
        DC++; /*increment data counter for null terminator*/
        return type;
     
    } 
    /*handle mat*/
    if (type == 2) {
        num = 0; /*initialize num*/
        num2 = 0; /*initialize num2*/
        i = 0; /*initialize i*/
        /*check if the matrix is valid and extract the numbers of the suze of the matrix*/
        if (sscanf(line, "[%d][%d]", &num, &num2) != 1) {
            fprintf(stderr, "Error: line %d mat %s is not a valid matrix\n", line_num, line);
            return -1; /*error*/
        }
        if (num < 1 || num2 < 1) {
            fprintf(stderr, "Error: line %d mat %s is not size\n", line_num, line);
            return -1; /*error*/
        }

        /*if there is label before the mat*/
        if (label) {
        label->type = "mat"; /*set label type to mat*/
        label->location = DC; /*set label location to data counter*/
        }

        num2 = num * num2; /*calculate the size of the matrix*/
        while (token = strtok(line, ",") != NULL) {
            num = atoi(&line[i]);
            if (num == 0 && strcmp(&line[i], "0") != 0) {
                fprintf(stderr, "Error: line %d mat %s is not a number\n", line_num, &line[i]);
                return -1; /*error*/
            }
            if (num < -511 || num > 511) {
                fprintf(stderr, "Error: line %d mat %s is out of range\n", line_num, &line[i]);
                return -1; /*error*/
            }
            data_coded[DC] = toQuad(toBinary(num)); /*store the data in the data_coded array*/
            DC++;
            num2--;
        }
        while (num2 != 0) {
            data_coded[DC] = toQuad(toBinary(0)); /*store the data in the data_coded array*/
            DC++;
            num2--;
        }
        return type; 
    } 
    /*handle entry*/
    if (type == 3) {
        /*if there is no label after entry*/
        if (sscanf(line, "%s", name) != 1) {
            fprintf(stderr, "Error: line %d entry is missing operand\n", line_num, name);
            return -1; /*error*/
        };
        /*if label is not defined*/
        if(!checkLabelExist(name)) {
            fprintf(stderr, "Error: entry used on label %s at %d, label doesnt exist in the file\n", name, line_num);
            return -1; /*error*/
        } 
        /*if label is defined*/
        else {
            /*in the future enter data to file*/
            return type; 
        }
    }
    /*handle extern*/
    if (type == 4) {
        /*if there is no label after entry*/
        if (sscanf(line, "%s", name) != 1) {
            fprintf(stderr, "Error: line %d extern is missing operand\n", line_num, name);
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
            label->type = "extern";
            label->value = "0"; /*extern labels have no value*/
            return type; /*return the type of the extern*/  
        }
    }
    
    else {
        fprintf(stderr, "Error: unknown type %d in line %s\n", type, line);
        return -1; /*error*/
    }
}



/**/
char* toBinary(int value) {
    int i;
    char *binary = malloc(10);
    if (!binary) {
        perror("malloc");
        return NULL;
    }
    if (value < 0){ 
        value = value*-1;
        for (i = 9; i >= 1; i--) {
            binary[i] = (value % 2) ? '0' : '1';
            value /= 2;
        }
        value[0]=1;
    } else {
        for (i = 9; i >= 0; i--) {
        binary[i] = (value % 2) ? '1' : '0';
        value /= 2;
        }
    }
    return binary;
}

/**/
char* toQuad(char *value) {
    int i;
    char *quad = malloc(5);
    if (!quad) {
        perror("malloc");
        return NULL;
    }
    for (i = 9; i >= 0; i-2) {
        if(value[i] == '0' && value[i-1] == '0') {
            quad[i/2+1] = 'a';
            continue;
        }
        if(value[i] == '0' && value[i-1] == '1') {
            quad[i/2+1] = 'b';
            continue;
        }
        if(value[i] == '1' && value[i-1] == '0') {
            quad[i/2+1] = 'c';
            continue;
        }
        if(value[i] == '1' && value[i-1] == '1') {
            quad[i/2+1] = 'd';
            continue;
        }
    }
    return quad;
}

