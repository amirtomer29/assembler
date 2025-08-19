; Macro name cannot be the same as a register
mcro r3
    mov r1, r2
mcroend

; Macro name cannot be the same as an opcode
mcro mov
    rts
mcroend

; Valid macro (used later for conflict tests)
mcro GOOD_MACRO
    add r1, r2
mcroend

; Macro name already used before
mcro GOOD_MACRO
    sub r3, r4
mcroend

; Label cannot start with a digit
123LABEL: .data 5

; Label name too long (exceeds 30 chars)
TOO_LONG_LABEL_NAME_EXCEEDS_LIMIT_OF_30_CHARACTERS: .data 10

; Label name cannot be an opcode
mov: .string "bad"

; Label name cannot be the same as a macro
GOOD_MACRO: .data 7

; Label name cannot be a register
r6: .data 3

; Illegal text after "mcroend"
mcro MYMAC
    clr r2
mcroend extra_word

; Missing operand after .extern
.extern

; instruction cannot use register name
.extern r1

; instruction cannot use opcode name
.extern mov

; instruction cannot use macro name
.extern MYMAC
