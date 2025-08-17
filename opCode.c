#include "main.h"
#include "format.h"
#include "parsers.h"
#include "firstPass.h"
#include "labelTable.h"
#include "instructions.h"
#include "preAssembler.h"
#include "opCode.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int opCodeHandler(char *line, int line_num, int opcode_index , Label *label, int pass) {
    char *token;
    char *operand_s = NULL;
    char *operand_d = NULL;
    int ope_s_type, ope_d_type;
    char op_name[MAX_OP_CODE_NAME_LENGTH];
    int value_for_binary;

    strcpy(op_name, opcode_names[opcode_index]); /*get the opcode name from the opcode_names array*/
    
    ope_s_type = 0;
    ope_d_type = 0;

    /*if type is rst , stop - no operands*/
    if (opcode_index == 14 || opcode_index == 15) {
        if (line) {
            fprintf(stderr, "Error: line %d opcode %s dont need operands\n", line_num, op_name);
            return -1; /*error*/
        }
        strcpy(label->type, ".code");
        memory_c[IC] = toQuad(toBinary(opcode_index<<6));
        printf("IC: %d\t VAL: %s\n",IC, memory_c[IC]);
        IC++;
        return 1;
    }

    if ((opcode_index != 14 || opcode_index != 15) && line) {
        operand_s = strtok(line, ",");
        operand_d = strtok(NULL, ",");

        /*if there is more than 2 operands*/
        if ((token = strtok(NULL, ",")) != NULL) {
        fprintf(stderr, "Error: line %d opcode %s has too many operands\n", line_num, op_name);
        return -1; /*error*/
        }
    } 

    /*if type is mov , cmp , add , sub - destanition: 1, 2, 3 source: 0, 1, 2, 3 *cmp has also 0 in destanition* 
    *lea only 1 , 2 at source */
    if (opcode_index == 0 || opcode_index == 1 || opcode_index == 2 || opcode_index == 3) {
        if (!operand_s || !operand_d) {
            fprintf(stderr, "Error: line %d opcode %s is need 2 operands\n", line_num, op_name);
            return -1; /*error*/
        }
        ope_s_type = getOperandType(operand_s, line_num);
        ope_d_type = getOperandType(operand_d, line_num);
        if (ope_s_type == -1 || ope_d_type == -1){
            return -1; /*error*/
        }
        if (ope_d_type == 0 && opcode_index != 1) {
            fprintf(stderr, "Error: line %d opcode %s has invalid destanition operand\n", line_num, op_name);
            return -1; /*error*/
        }
        if ((ope_s_type == 1 || ope_s_type == 2) && opcode_index == 4) {
            fprintf(stderr, "Error: line %d opcode %s has invalid source operand\n", line_num, op_name);
            return -1; /*error*/
        }
    }

    /*if type is clr , not , inc, dec, jmp, bne, jsr, red ,prn - destanition: 1, 2, 3 source: null*/
    if (opcode_index == 5 || opcode_index == 6 || opcode_index == 7 || opcode_index == 8
        || opcode_index == 9 || opcode_index == 10 || opcode_index == 11 
        || opcode_index == 12 || opcode_index == 13 ) {
        
        
        if (!operand_s || operand_d) {
            fprintf(stderr, "Error: line %d opcode %s is need 1 operand\n", line_num, op_name);
            return -1; /*error*/
        }

        operand_d = operand_s; /*move the source operand to destanition operand*/
        ope_s_type = -1; /*set source operand type to empty value for the binaery procces*/
        ope_d_type = getOperandType(operand_d, line_num);
        if (ope_d_type == 0 && opcode_index != 13) {
            fprintf(stderr, "Error: line %d opcode %s has invalid destanition operand\n", line_num, op_name);
            return -1; /*error*/
        }
    }

    if (pass==1){
        /*proccesing the opCode line to IC*/
        if (label) {
            label->type = "code"; /*set label type to opcode*/
            label->location = IC; /*set label location to instruction counter*/
        }
        value_for_binary = opcode_index << 6; /*shift the opcode index to the left by 6 bits*/
        if (ope_s_type == -1 && ope_d_type == -1) {
            value_for_binary = value_for_binary + 0; /*no operands*/
        } else if (ope_s_type == -1) {
            value_for_binary = value_for_binary + (ope_d_type << 2); /*only destanition operand*/
        } else if (ope_d_type == -1) {
            value_for_binary = value_for_binary + (ope_s_type << 4); /*only source operand*/
        } else {
            value_for_binary = value_for_binary + (ope_s_type << 4) + (ope_d_type << 2); 
        }
        memory_c[IC] = toQuad(toBinary(value_for_binary)); /*store the opcode in the instructions_coded array*/
        printf("IC: %d\t VAL: %s\n",IC, memory_c[IC]);
        IC++;
        /*proccesing the operands to IC*/
        if (ope_s_type == 3 && ope_d_type == 3) {
            printf("%d",(getRegisterNum(operand_d)));
            memory_c[IC] = toQuad(toBinary((getRegisterNum(operand_s) << 6) + (getRegisterNum(operand_d) << 2))); /*store the first register index in the instructions_coded array*/
            memory_c[IC][4] ='a';
            printf("IC: %d\t VAL: %s\n",IC, memory_c[IC]);
            IC++;
        }
        else {
            if (ope_s_type != -1){
                if (handleOperand(operand_s, ope_s_type, 1, line_num, 0) == -1) {
                    return -1;/*error*/
                }
            }
            if (ope_d_type != -1) {
                if (handleOperand(operand_d, ope_d_type, 1, line_num, 1) == -1){
                    return -1;/*error*/
                } 
            }
        }
        return 1; /*succses*/
    }
    if (pass == 2) {
        IC++;
        if (ope_s_type == 3 && ope_d_type == 3) {
            IC++;
            return 1;
        }
        if (ope_s_type != -1){
            if (handleOperand(operand_s, ope_s_type, 2, line_num ,0) == -1) {
                return -1;/*error*/
            }
        }
        if (ope_d_type != -1) {
            if (handleOperand(operand_d, ope_d_type, 2, line_num,1) == -1){
                return -1;/*error*/
            } 
        }
    }
    return 1; /*succses*/
}


int getOperandType(char *operand, int line_num) {
    char r1[3], r2[3];
    r1[2] = '\0'; /*initialize registers for matrix access*/
    r2[2] = '\0';

    if (operand == NULL) {
        fprintf(stderr, "Error: line %d operand is NULL\n", line_num);
        return -1; /*error*/
    }


    /*if operand is a matrix access*/
    if (sscanf(operand, "%*[^[][%2s][%2s]", r1, r2) == 2) {
        if (checkIfRegister(r1) && checkIfRegister(r2)){
            return 2; /*matrix access*/
        }
        else {
            fprintf(stderr, "Error: line %d operand %s is not a valid matrix access\n", line_num, operand);
            return -1; /*error*/
        }
    }
    /*if operand is a direct value*/
    if (operand[0] == '#') {
        return 0; 
    } 
    /*if operand is a direct register value*/
    if (checkIfRegister(operand)) {
        return 3; 
    }
    /*if operand is a direct label value*/
    if (checkLabelExist(operand) != NULL) {
        return 1; 
    }
    else {
        fprintf(stderr, "Error: line %d operand %s is not a valid operand\n", line_num, operand);
        return -1; /*error*/
    }
}


int handleOperand(char *operand, int type, int pass, int line_num, int s_d) {
    Label *label;
    char r1[3], r2[3]; /*registers for matrix access*/
    char label_name[MAX_LABEL_NAME];
    int i;
    int value;

    printf("%d\n", IC);
    r1[2] = '\0'; /*initialize registers for matrix access*/
    r2[2] = '\0';   

    if (operand == NULL) {
        return -1; /*error*/
    }
    /*if operand is a matrix access*/
    if (type == 2) {
        if (pass == 1){
            IC++;/*skipping address of label name of the matrix*/
            sscanf(operand, "%*[^[][%2s][%2s]", r1, r2); /*get the registers from the operand*/
            memory_c[IC] = toQuad(toBinary((atoi(&r1[1]) << 6) + (atoi(&r2[1]) << 2))); /*store the first register index in the instructions_coded array*/
            memory_c[IC][4] ='a';
            printf("IC: %d\t VAL: %s\n",IC, memory_c[IC]);
            IC++;
            return 1; /*success*/
        }
        if (pass == 2){
            sscanf(operand, "%[^[]", label_name);
            label = checkLabelExist(label_name);
            if (!label) {
                fprintf(stderr, "Error: line %d label %s not found\n", line_num, label_name);
                return -1; /*error*/
            }
            if (getInstructionType(label->type)==4){
                memory_c[IC] =  toQuad(toBinary(1));
                printf("IC: %d\t VAL: %s\n",IC, memory_c[IC]);
                label->extern_locatoins[label->extern_locatins_num] = IC;
                label->extern_locatins_num++;
            }
            if (getInstructionType(label->type)==2){
                memory_c[IC] =  toQuadAddressFormat(toQuad(toBinary(label->location)));
                memory_c[IC][4] = 'c';
                printf("IC: %d\t VAL: %s lovcL %d \n",IC, memory_c[IC], label->location);
            }
            IC+=2;
            return 1;
        }
    }
    /*if operand is a direct value*/
    if (type == 0) {
        if(pass==1) {
            value = atoi(operand+1); /*convert the string to integer without #*/
            memory_c[IC] = toBinary(value); /*store the value in the instructions_coded array*/
            for (i=0; i<10;i++){
                (memory_c[IC])[i-1] = (memory_c[IC])[i];
            }
            for (i=0; i<10;i++){
                (memory_c[IC])[i-1] = (memory_c[IC])[i];
            }
            (memory_c[IC])[8] = '0';
            (memory_c[IC])[9] = '0';
            memory_c[IC] = toQuad(memory_c[IC]);
            printf("IC: %d\t VAL: %s\n",IC, memory_c[IC]);
            IC++;
            return 1; /*success*/
        }
        if(pass==2){
            IC++;
            return 1; /*success*/
        }
    } 
    /*if operand is a direct register value*/
    if (type == 3) { 
        if (pass==1){
            i = s_d ? 2 : 6;
            memory_c[IC] = toQuad(toBinary(atoi(&operand[1]) << i)); /*store the register index in the instructions_coded array*/
            printf("IC: %d\t VAL: %s\n",IC, memory_c[IC]);
            IC++;
            return 1; /*success*/
        }
        if (pass==2){
                printf("%d\n", IC);
            IC++;
                printf("%d\n", IC);
            return 1; /*success*/
        }
    } 
    /*if operand is a direct label value*/
    if (type == 1) {
        if (pass==1){
            label = checkLabelExist(operand);
            if (!label) {
                fprintf(stderr, "Error: line %d label %s not found\n", line_num, label_name);
                return -1; /*error*/
            }
            if (getInstructionType(label->type)==4){
                memory_c[IC] =  toQuad(toBinary(1));
                printf("IC: %d\t VAL: %s\n",IC, memory_c[IC]);
                label->extern_locatoins[label->extern_locatins_num] = IC;
                label->extern_locatins_num++;
            }
            else {
                printf("IC: %d\t VAL: %s\n",IC, memory_c[IC]);
                memory_c[IC] =  toQuad(toBinary(label->location + 2));
            }
            IC++;
            return 1; /*success*/  
        }
        if (pass==2){
            label = checkLabelExist(operand);
            if (getInstructionType(label->type)<3){
                memory_c[IC] =  toQuadAddressFormat(toQuad(toBinary(label->location)));
                memory_c[IC][4] = 'c';
                printf("IC: %d\t VAL: %s\n",IC, memory_c[IC]);
            }
            IC++;
            return 1; /*success*/ 
        }
    }
    return -1; /*error*/ 
}

int checkIfOpCode(char *word) {
    int i;
    for (i = 0; i < 16; i++) {
        if (strcmp(word, opcode_names[i]) == 0) {
            return i; /*return the opcode index*/
        }
    }
    return -1; /*return error code*/
}