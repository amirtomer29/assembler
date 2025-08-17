#ifndef instructions_h
#define instructions_h  
#define MAX_INSTRUCTION_NAME_LENGTH 20 /*arbitrary length for instruction names*/




int getInstructionType(char *word);

int instructionHandler(int type, char *line, Label *label, int line_num, int pass);

extern int IC; /*instruction counter*/
extern int DC; /*data counter*/




#endif /* instructions_h */
