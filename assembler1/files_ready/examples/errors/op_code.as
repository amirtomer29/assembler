; Error: opcode "stop" should not have operands
stop r1

; Error: opcode "mov" missing one operand
mov r1

; Error: opcode "add" with too many operands
add r1, r2, r3

; Error: opcode "lea" invalid source operand (immediate not allowed)
lea #5, r1

; Error: opcode "lea" invalid source operand (register not allowed)
lea r2, r1

; Error: opcode "mov" invalid destination operand (immediate not allowed)
mov r1, #7

; Error: single-operand opcode "clr" with no operands
clr

; Error: single-operand opcode "inc" with two operands
inc r1, r2

; Error: opcode "not" invalid destination operand (immediate not allowed)
not #8

; Error: opcode "jmp" invalid destination operand (immediate not allowed)
jmp #2

; Error: matrix operand with invalid registers inside
mov mat[r1][#5], r2

; Error: operand not valid (unknown symbol)
mov r1, unknownLabel

; Error: immediate value out of range (>255 or <-256)
mov #300, r1

