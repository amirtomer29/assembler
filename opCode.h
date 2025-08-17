#ifndef OP_CODE_H
#define OP_CODE_H
#define MAX_OP_CODE_NAME_LENGTH 5  /*arbitrary length for opcode names*/

extern int IC; /*instruction counter*/
extern int DC; /*data counter*/

int opCodeHandler(char *line, int line_num, int opcode_index, Label *label, int pass);
int getOperandType(char *operand, int line_num);
int handleOperand(char *operand, int type, int pass, int line_num, int s_d);
int checkIfOpCode(char *word); 


#endif /* OP_CODE_H */