; ---------- .data ERRORS ----------

; Error: not a number
DATA_NOT_NUMBER: .data 5, x, 7

; Error: number out of range (assuming MIN=-128, MAX=127)
DATA_OUT_RANGE: .data 530

; ---------- .string ERRORS ----------

; Error: missing quotes
STRING_NO_QUOTES: .string hello

; Error: only one quote
STRING_ONE_QUOTE: .string "hello

; ---------- .mat ERRORS ----------

; Error: invalid format, missing []
MAT_INVALID_FORMAT: .mat 3,4,5

; Error: invalid size (rows or cols < 1)
MAT_INVALID_SIZE: .mat [0][3]

; Error: element not a number
MAT_NOT_NUMBER: .mat [2][2] 1, a, 3

; Error: element out of range
MAT_OUT_RANGE: .mat [2][2] 570, 1

; Error: too many elements for declared size
MAT_TOO_MUCH: .mat [2][2] 1,2,3,4,5

; ---------- .entry ERRORS ----------

; Error: missing operand
ENTRY_NO_OPERAND: .entry

; Error: label does not exist (pass 1)
.entry NON_EXIST_LABEL


; ---------- .extern ERRORS ----------

; Error: missing operand
EXTERN_NO_OPERAND: .extern


