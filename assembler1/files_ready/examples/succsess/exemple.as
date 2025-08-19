mcro RESET
    clr r0
	stop
mcroend

ARR: .data 1, 2, 3
.extern EXT_FUNC
STR: .string "ok"
.entry MAIN
MAT: .mat [2][2] 5, 6, 7, 8

mcro HELLO
    prn STR
mcroend

RESET
MAIN: mov #5, r2
lea ARR, r3
HELLO
add r2, r3
jsr EXT_FUNC
stop

