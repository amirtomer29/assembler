#ifndef instructions_h
#define instructions_h  

#define MAX_INSTRUCTION_NAME_LENGTH 20 /* Arbitrary max length for instruction directive names like .data, .string, etc. */
#define MAX_INTEGER_VALUE 511
#define MIN_INTEGER_VALUE -512
/**
 * Function: getInstructionType
 * Checks whether the given word is a recognized instruction directive (.data, .string, .mat, etc.).
 * 
 * @param word - the word to be checked (should not be NULL)
 * @return index of instruction if valid (0-4), -1 if not recognized
 */
int getInstructionType(char *word);

/**
 * Function: instructionHandler
 * Handles parsing and processing of instruction lines (e.g. .data, .string, .mat, .entry, .extern).
 * 
 * @param type     - type of instruction (as returned from getInstructionType)
 * @param line     - content after instruction keyword
 * @param label    - pointer to associated label, if any
 * @param line_num - current line number in the source file (for error reporting)
 * @param pass     - current pass (1 for first pass, 2 for second pass)
 * 
 * @return type if successful, -1 on error
 */
int instructionHandler(int type, char *line, Label *label, int line_num, int pass);

/** Global instruction and data counters (updated during assembly parsing) */
extern int IC; /* instruction counter */
extern int DC; /* data counter */

/**
 * Function: getInstructionName
 * return string of the instruction name by index
 * 
 * @param instruction_index - the integer for the index
 * @return name if valid (0-4), NULL if out of range
 */
char* getInstructionName(int instruction_index);


/* Instruction enum aligned with instruction_names order */
typedef enum {
    INSTR_DATA = 0,
    INSTR_STRING,
    INSTR_MAT,
    INSTR_ENTRY,
    INSTR_EXTERN,
    INSTR_DEFAULT
} Instruction;

#endif /* instructions_h */
