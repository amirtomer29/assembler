MAIN:   mov r3, LENGTH
        add r2, r3
        sub #5, r1
        cmp r2, r3
        bne LOOP
        clr K
        lea STR, r4
        inc r4
        mov M1[r1][r2], r3
        jsr FUNC
        stop

LOOP:   dec r2
        jnz LOOP
        jmp END

FUNC:   prn r3
        rts

STR:    .string "hello assembler"
LENGTH: .data 6,-9,15
K:      .data 22
M1:     .mat [2][3], 1,2,3,4,5,6
END:    stop

.entry MAIN
.entry LOOP

