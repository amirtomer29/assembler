#ifndef OP_CODE_H
#define OP_CODE_H

#define MAX_OP_CODE_NAME_LENGTH 5  /* arbitrary length for opcode names */

/**
 * IC - Instruction Counter
 * DC - Data Counter
 * Both are managed globally during assembly passes
 */
extern int IC; /* instruction counter */
extern int DC; /* data counter */

/**
 * Handles opcode parsing, validation, and encoding
 * @param line        current line (may contain operands)
 * @param line_num    current line number
 * @param opcode_index index of the opcode in the opcode_names array
 * @param label       pointer to label (if exists)
 * @param pass        assembly pass (1 = first, 2 = second)
 * @return 1 on success, -1 on error
 */
int opCodeHandler(char *line, int line_num, int opcode_index, Label *label, int pass);

/**
 * Determines the type of an operand
 * @param operand   operand string
 * @param line_num  current line number (for error messages)
 * @return 0=immediate, 1=label, 2=matrix, 3=register, -1=error
 */
int getOperandType(char *operand, int line_num);

/**
 * Encodes and processes a single operand during assembly
 * @param operand   operand string
 * @param type      operand type (0,1,2,3)
 * @param pass      assembly pass (1 or 2)
 * @param line_num  current line number
 * @param s_d       0=source, 1=destination
 * @return 1 on success, -1 on error
 */
int handleOperand(char *operand, int type, int pass, int line_num, int s_d);

/**
 * Checks if a string is a valid opcode name
 * @param word  string to check
 * @return opcode index (0–15) or -1 if not found
 */
int getOpCodeIndex(char *word); 

/**
 * return the index of valid opcode name
 * @param opcode_index integr to check
 * @return opcode name or NULL if its out of range
 */
char* getOpCodeName(int opcode_index);

typedef enum {
    OP_MOV = 0,
    OP_CMP,
    OP_ADD,
    OP_SUB,
    OP_LEA,
    OP_CLR,
    OP_NOT,
    OP_INC,
    OP_DEC,
    OP_JMP,
    OP_BNE,
    OP_JSR,
    OP_RED,
    OP_PRN,
    OP_RTS,
    OP_STOP
} OpCode;

typedef enum {
    OPER_IMMIDIATE = 0,
    OPER_LABEL,
    OPER_MATRIX,
    OPER_REGISTER
} OpernadType;

#endif /* OP_CODE_H */
