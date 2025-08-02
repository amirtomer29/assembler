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



#endif /* MAIN_H */