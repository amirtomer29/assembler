#ifndef MAIN_H
#define MAIN_H

/*
 * Global limits and table sizes used across the assembler.
 * MAX_LINE_LEN includes the terminating null; see spec references in comments.
 */
#define MAX_NAME_LEN 30
#define MAX_MACRO_LINES 20 
#define MAX_LINE_LEN 81 
#define MAX_LABEL_NAME 30 
#define MACRO_HASH_SIZE 31 
#define LABEL_HASH_SIZE 31 
#define ADRRESS_SIZE 5
#define REAL_MEMORY_LOAD 100
#define MEMORY_SIZE 256

/*
 * Forward declarations for cross-module references.
 */
struct Macro;
struct Label;


/*
 * Global hash tables storing macros and labels during assembly.
 */
extern struct Macro *macro_table[MACRO_HASH_SIZE];
extern struct Label *label_table[LABEL_HASH_SIZE];

/*
 * Global counters for code and data words.
 * IC - instruction counter, DC - data counter.
 */
extern int IC; /* instruction counter */
extern int DC; /* data counter */

/*
 * Global memory images:
 * memory_c holds encoded instruction words.
 * memory_d holds encoded data words.
 */
extern char *memory_d[256];    /* array of instructions coded in the file, used in firstPass.c */
extern char *memory_c[256]; 

/*check if IC + DC + new_words + REAL_MEMORY_LOAD no exceed memory size if yes retorn 0 else 1*/
int memory_check (int new_words);


/**
 * insertToMemory - Store a value in instruction (IC) or data (DC) memory.
 *
 * @inst_data: INST for instruction memory, DATA for data memory
 * @value:     The number to store
 * @format:    VALUE for normal value, ADDRESS for address format
 *
 * Returns: 1 on success, 0 on failure (invalid input or no memory left).
 */
int insertToMemory (int inst_data, int value, int format);

/*used by memory_insert function*/
typedef enum {
    INST = 0,
    DATA
} Memory_Type;

/*used by memory_insert function*/
typedef enum {
    VALUE = 0,
    ADDRESS
} Formats;

#endif /* MAIN_H */
