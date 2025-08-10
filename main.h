#ifndef MAIN_H
#define MAIN_H

#include "preAssembler.h"
#include "macroTable.h"
#include "labelTable.h"


#define MAX_NAME_LEN 30 /*arbituary*/
#define MAX_MACRO_LINES 20 /*arbituary*/
#define MAX_LINE_LEN 81 /*page 37*/
#define MAX_LABEL_NAME 30 /*page 40*/


/*declarations*/
extern const char *opcode_names[];
extern const char *instruction_names[];

extern Macro *macro_table[MACRO_HASH_SIZE];
extern Label *label_table[LABEL_HASH_SIZE];

extern int IC; /*instruction counter*/
extern int DC; /*data counter*/

extern char[156] instructions_coded;    /*array of instructions coded in the file, used in firstPass.c*/
extern char[100] data_coded;      /*array of data coded in the file, used in firstPass.c*/

/*opcode enum*/
typedef enum {
    mov=0,
    cmp,
    add,
    sub,
    not,
    clr,
    lea,
    inc,
    dec,
    jmp,
    bne,
    red,
    prn,
    jsr,
    rts,
    stop
} opcode;

/*registers enum*/
typedef enum {
    r0=0,
    r1,
    r2,
    r3,
    r4,
    r5,
    r6,
    r7
} registers;




#endif /* MAIN_H */