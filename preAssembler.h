#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 30 /*arbituary*/
#define MAX_MACRO_LINES 20 /*arbituary*/
#define MAX_LINE_LEN 81 /*page 37*/
#define MAX_LABEL_NAME 30 /*page 40*/

/*opcode table*/
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

/*types of instruction lines*/
typedef enum {
    date=0,
    string,
    mat,
    entry,
    extern
} instruction;

/*definition of macro type*/


