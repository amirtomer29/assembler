#ifndef MAIN_H
#define MAIN_H



#define MAX_NAME_LEN 30 /*arbituary*/
#define MAX_MACRO_LINES 20 /*arbituary*/
#define MAX_LINE_LEN 81 /*page 37*/
#define MAX_LABEL_NAME 30 /*page 40*/
#define MACRO_HASH_SIZE 31 /*arbituary*/
#define LABEL_HASH_SIZE 31 /*arbituary*/
#define ADRRESS_SIZE 5
#define REAL_MEMORY_LOAD 100




/*declarations*/
struct Macro;
struct Label;

extern const char *opcode_names[];
extern const char *instruction_names[];

extern struct Macro *macro_table[MACRO_HASH_SIZE];
extern struct Label *label_table[LABEL_HASH_SIZE];

extern int IC; /*instruction counter*/
extern int DC; /*data counter*/

extern char *memory_d[256];    /*array of instructions coded in the file, used in firstPass.c*/
extern char *memory_c[256]; 

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