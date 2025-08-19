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

/**
 * Handles opcode parsing, validation, and encoding for both passes of the assembler.
 * @param line        Current line string (may contain operands)
 * @param line_num    Current line number
 * @param opcode_index Index of the opcode in the opcode_names array
 * @param label       Pointer to a label structure (if exists)
 * @param pass        Assembly pass (1 = first pass, 2 = second pass)
 * @return 1 on success, -1 on error
 */
int opCodeHandler(char *line, int line_num, int opcode_index , Label *label, int pass) {
    char *token;
    char *operand_s = NULL;
    char *operand_d = NULL;
    int ope_s_type, ope_d_type;
    int value_for_binary;
    char op_name[MAX_OP_CODE_NAME_LENGTH];

    strcpy(op_name, getOpCodeName(opcode_index));
    ope_s_type = 0;
    ope_d_type = 0;

    /* handle instructions without operands (rts, stop) */
    if (opcode_index == OP_RTS || opcode_index == OP_STOP) {
        if (line == NULL) { 
            if (label) {
                strcpy(label->type, ".code");
            }
            if(!insertToMemory(INST, opcode_index<<6, VALUE)){return -1;}
            return 1;
        } else {
            fprintf(stderr, "Error: line %d opcode %s dont need operands\n", line_num, op_name);
            return -1; /* error */
        }
    }

    /* handle opcodes with operands */
    if ((opcode_index != OP_RTS || opcode_index != OP_STOP) && line) {
        operand_s = strtok(line, ",");
        operand_d = strtok(NULL, ",");
        operand_s = trim_whitespace(operand_s);
        operand_d = trim_whitespace(operand_d);
        
        /* if more than 2 operands exist */
        if ((token = strtok(NULL, ",")) != NULL) {
            fprintf(stderr, "Error: line %d opcode %s has too many operands\n", line_num, op_name);
            return -1; /* error */
        }
    } 

    /* validate two-operand instructions */
    if (opcode_index == OP_MOV || opcode_index == OP_CMP || opcode_index == OP_ADD || opcode_index == OP_SUB || opcode_index == OP_LEA) {
        if (!operand_s || !operand_d) {
            fprintf(stderr, "Error: line %d opcode %s is need 2 operands\n", line_num, op_name);
            return -1;
        }
        ope_s_type = getOperandType(operand_s, line_num);
        ope_d_type = getOperandType(operand_d, line_num);
        if (ope_s_type == -1 || ope_d_type == -1){
            return -1;
        }
        if (ope_d_type == OPER_IMMIDIATE && opcode_index != OP_CMP) {
            fprintf(stderr, "Error: line %d opcode %s has invalid destanition operand\n", line_num, op_name);
            return -1;
        }
        if ((ope_s_type == OPER_IMMIDIATE || ope_s_type == OPER_REGISTER) && opcode_index == OP_LEA) {
            fprintf(stderr, "Error: line %d opcode %s has invalid source operand\n", line_num, op_name);
            return -1;
        }
    }

    /* validate single-operand instructions */
    if (opcode_index == OP_CLR || opcode_index == OP_NOT || opcode_index == OP_INC || opcode_index == OP_DEC
        || opcode_index == OP_JMP || opcode_index == OP_BNE || opcode_index == OP_JSR 
        || opcode_index == OP_RED || opcode_index == OP_PRN ) {
        
        if (!operand_s || operand_d) {
            fprintf(stderr, "Error: line %d opcode %s is need 1 operand\n", line_num, op_name);
            return -1;
        }

        operand_d = operand_s; /* move the source operand to destination */
        ope_s_type = -1; /* mark source empty */
        ope_d_type = getOperandType(operand_d, line_num);
        if (ope_d_type == OPER_IMMIDIATE && opcode_index != OP_PRN) {
            fprintf(stderr, "Error: line %d opcode %s has invalid destanition operand\n", line_num, op_name);
            return -1;
        }
    }

    /* first pass encoding */
    if (pass==1){
        if (label) {
            label->type = "code";
            label->location = IC;
        }
        value_for_binary = opcode_index << 6;
        if (ope_s_type == -1 && ope_d_type == -1) {
            value_for_binary += 0;
        } else if (ope_s_type == -1) {
            value_for_binary += (ope_d_type << 2);
        } else if (ope_d_type == -1) {
            value_for_binary += (ope_s_type << 4);
        } else {
            value_for_binary += (ope_s_type << 4) + (ope_d_type << 2); 
        }
        if(!insertToMemory(INST, value_for_binary, VALUE)){return -1;}

        /* handle operands */
        if (ope_s_type == OPER_REGISTER && ope_d_type == OPER_REGISTER) {
            value_for_binary = 0;
            value_for_binary = (getRegisterNum(operand_s) << 6) + (getRegisterNum(operand_d) << 2);
            if(!insertToMemory(INST, value_for_binary, VALUE)){return -1;}
        }
        else {
            if (ope_s_type != -1){
                if (handleOperand(operand_s, ope_s_type, 1, line_num, 0) == -1) {
                    return -1;
                }
            }
            if (ope_d_type != -1) {
                if (handleOperand(operand_d, ope_d_type, 1, line_num, 1) == -1){
                    return -1;
                } 
            }
        }
        return 1;
    }

    /* second pass resolution */
    if (pass == 2) {
        IC++;
        if (ope_s_type == OPER_REGISTER && ope_d_type == OPER_REGISTER) {
            IC++;
            return 1;
        }
        if (ope_s_type != -1){
            if (handleOperand(operand_s, ope_s_type, 2, line_num ,0) == -1) {
                return -1;
            }
        }
        if (ope_d_type != -1) {
            if (handleOperand(operand_d, ope_d_type, 2, line_num,1) == -1){
                return -1;
            } 
        }
    }
    return 1;
}


/**
 * Determines the type of an operand.
 * @param operand    The operand string
 * @param line_num   Current line number (for error messages)
 * @return 0=immediate, 1=label, 2=matrix, 3=register, -1=error
 */
int getOperandType(char *operand, int line_num) {
    char r1[3], r2[3];
    r1[2] = '\0';
    r2[2] = '\0';

    if (operand == NULL) {
        fprintf(stderr, "Error: line %d operand is NULL\n", line_num);
        return -1;
    }

    /* check matrix access */
    if (sscanf(operand, "%*[^[][%2s][%2s]", r1, r2) == 2) {
        if (checkIfRegister(r1) && checkIfRegister(r2)){
            return OPER_MATRIX;
        }
        else {
            fprintf(stderr, "Error: line %d operand %s is not a valid matrix access\n", line_num, operand);
            return -1;
        }
    }
    /* check immediate value */
    if (operand[0] == '#') {
        return OPER_IMMIDIATE; 
    } 
    /* check register */
    if (checkIfRegister(operand)) {
        return OPER_REGISTER; 
    }
    /* check label */
    if (checkLabelExist(operand) != NULL) {
        return OPER_LABEL; 
    }
    else {
        fprintf(stderr, "Error: line %d operand %s is not a valid operand\n", line_num, operand);
        return -1;
    }
}

/**
 * Encodes a single operand during assembly process.
 * @param operand    Operand string
 * @param type       Operand type (0,1,2,3)
 * @param pass       Assembly pass (1 or 2)
 * @param line_num   Current line number
 * @param s_d        0=source, 1=destination
 * @return 1 on success, -1 on error
 */
int handleOperand(char *operand, int type, int pass, int line_num, int s_d) {
    Label *label;
    char r1[3], r2[3];
    char label_name[MAX_LABEL_NAME];
    int i;
    int value;

    r1[2] = '\0';
    r2[2] = '\0';   

    if (operand == NULL) {
        return -1;
    }

    /* matrix operand */
    if (type == OPER_MATRIX) {
        if (pass == 1){
            IC++; /*empty place for mat address*/
            sscanf(operand, "%*[^[][%2s][%2s]", r1, r2);
            value=0;
            value = (atoi(&r1[1]) << 6) + (atoi(&r2[1]) << 2);
            if(!insertToMemory(INST, value, VALUE)){return -1;}
            return 1;
        }
        if (pass == 2){
            sscanf(operand, "%[^[]", label_name);
            label = checkLabelExist(label_name);
            if (!label) {
                fprintf(stderr, "Error: line %d label %s not found\n", line_num, label_name);
                return -1;
            }
            if (getInstructionType(label->type)==INSTR_EXTERN){
                if(!insertToMemory(INST, 1, VALUE)){return -1;}
                label->extern_locatoins[label->extern_locatins_num] = IC;
                label->extern_locatins_num++;
            }
            if (getInstructionType(label->type)==INSTR_MAT){
                if(!insertToMemory(INST, label->location, ADDRESS)){return -1;}
            }
            IC++; /*skip ready words in memory*/
            return 1;
        }
    }

    /* immediate operand */
    if (type == OPER_IMMIDIATE) {
        if(pass==1) {
            if(!memory_check(1)){return -1;}
            value = atoi(operand+1);
            /*spaciel insert to memory*/
            if (value>255||value<-256){
                fprintf(stderr, "Error: line %d value %d out of range\n", line_num, value);
                return -1;
            }
            memory_c[IC] = toBinary(value);
            for (i=0; i<10;i++){
                (memory_c[IC])[i-1] = (memory_c[IC])[i];
            }
            for (i=0; i<10;i++){
                (memory_c[IC])[i-1] = (memory_c[IC])[i];
            }
            (memory_c[IC])[8] = '0';
            (memory_c[IC])[9] = '0';
            memory_c[IC] = toQuad(memory_c[IC]);
            IC++;
            return 1;
        }
        if(pass==2){
            IC++;
            return 1;
        }
    } 

    /* register operand */
    if (type == OPER_REGISTER) { 
        if (pass==1){
            i = s_d ? 2 : 6;
            if(!insertToMemory(INST, atoi(&operand[1]) << i, VALUE)){return -1;}
            return 1;
        }
        if (pass==2){
            IC++;
            return 1;
        }
    } 

    /* label operand */
    if (type == OPER_LABEL) {
        if (pass==1){
            label = checkLabelExist(operand);
            if (!label) {
                fprintf(stderr, "Error: line %d label %s not found\n", line_num, label_name);
                return -1;
            }
            if (getInstructionType(label->type)==INSTR_EXTERN){
                if(!insertToMemory(INST, 1 , VALUE)){return -1;}
                label->extern_locatoins[label->extern_locatins_num] = IC;
                label->extern_locatins_num++;
                return 1; 
            }
            else {
                IC++;
                return 1; 
            } 
        }
        if (pass==2){
            label = checkLabelExist(operand);
            if (getInstructionType(label->type)!=INSTR_EXTERN){ /*not extern*/
                if(!insertToMemory(INST, label->location , ADDRESS)){return -1;}
                return 1; 
            }
            else {
                IC++;
                return 1; 
            }   
        }
    }
    return -1; 
}


/**
 * Checks whether a string matches a valid opcode.
 * @param word String to check
 * @return opcode index (0–15) or -1 if not found
 */
int getOpCodeIndex(char *word) {
    int i;
    /*initialing opcode_names*/
    char *opcode_names[] = {
        "mov", "cmp", "add", "sub", "lea", "clr","not",
        "inc", "dec", "jmp", "bne", "jsr", "red",
        "prn", "rts", "stop"
    };

    for (i = 0; i < 16; i++) {
        if (strcmp(word, opcode_names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

char* getOpCodeName(int opcode_index) {
    /*initialing opcode_names*/
    char *opcode_names[] = {
        "mov", "cmp", "add", "sub", "lea", "clr","not",
        "inc", "dec", "jmp", "bne", "jsr", "red",
        "prn", "rts", "stop"
    };
    if (opcode_index <0 || opcode_index>15) {
        return NULL;
    }
    return opcode_names[opcode_index];
}
