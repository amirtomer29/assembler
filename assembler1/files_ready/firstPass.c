#include "preAssembler.h"
#include "main.h"
#include "labelTable.h"
#include "macroTable.h"
#include "instructions.h"
#include "opCode.h"
#include <ctype.h>
#include "firstPass.h"
#include "parsers.h"

/**
 * firstPass
 * Performs the first pass of the assembler over the source file.
 * Responsibilities:
 *   - Parses each line to detect labels, instructions, and opcodes.
 *   - Calls instructionHandler and opCodeHandler for validation and encoding.
 *   - Updates label references and prepares for the second pass.
 * Returns 1 on success, 0 on error.
 */
int firstPass(char *filename) {
    int error = 0;                         /* flag to track errors */
    int line_num = 0;                      /* current line number */
    int instr_index;                       /* index for instruction type */
    int opcode_index;                      /* index for opcode */
    char line[MAX_LINE_LEN + 2];           /* buffer for line (+2 for \0 and \n) */
    char* line_p;                          /* pointer to the current line */
    char* first_word= NULL;                /* stores the first word of the line */
    char* pnt = NULL;                      /* generic pointer for parsing */
    char* instruction_or_opcode = NULL;    /* points to instruction or opcode word */
    char* end_line = NULL;                 /* pointer to end of current line */
    Label *label = NULL;                   /* label reference if exists */
    FILE *src;                             /* source file pointer */

    /* loading the file */
    src = fopen(filename, "r");
    
    /* check error while opening the file */
    if (!src) {
        perror("cant open file");
        return 0;
    } else {
        /* main loop - process each line until EOF */
        while (fgets(line, MAX_LINE_LEN+2, src) != NULL) { /* read one line */
            label = NULL; /* reset label for each new line */
            line_num++; 
            line_p = line; /* set pointer to start of line */
            line_p = trim_whitespace(line_p);
            end_line = line_p + strlen(line_p);

            /* skip empty lines */
            if (line_p == end_line) {
                continue;
            }  

            /* detect first word */
            first_word = line_p;
            /* check if line starts with a label (contains ':') */
            pnt = strchr(first_word, ':');
            
            if (pnt != NULL) {
                line_p = pnt + 1; /* advance pointer past the label */
                if (line_p >= end_line) {
                    fprintf(stderr, "WARNING: empty label at line %d", line_num);
                    continue;
                }  
                line_p = trim_whitespace(line_p);
                *pnt = '\0'; /* temporarily terminate string at label */
                label = checkLabelExist(first_word); /* check if label exists */
                *pnt = ':'; /* restore ':' */
            }

            /* extract instruction or opcode */
            instruction_or_opcode = line_p;
            pnt = trim_char(line_p);
            if (pnt == NULL) {

            } else if (pnt >= line_p && pnt < end_line) {
                *pnt = '\0'; 
                line_p = pnt+1;
            } else {
                line_p = end_line;
            }
            
            /* check if line contains an instruction */
            instr_index = getInstructionType(instruction_or_opcode);
            if (instr_index != -1) { 
                if (line_p < end_line) { 
                    line_p = trim_whitespace(line_p);
                }
                /* call instruction handler */
                if(instructionHandler(instr_index, line_p >= end_line ? NULL : line_p, label, line_num, 1) == -1) {
                    error = 1; /* error in instruction handling */
                }
                continue;
            }

            /* check if line contains an opcode */
            opcode_index = getOpCodeIndex(instruction_or_opcode);
            if (opcode_index != -1) {
                if (line_p < end_line) {
                    line_p = trim_whitespace(line_p);
                }
                /* call opcode handler */
                if (opCodeHandler(line_p >= end_line ? NULL : line_p, line_num, opcode_index , label, 1) == -1) {
                    error = 1; /* error in opcode handling */ 
                }
                continue;
            }
        } 
        
        /* handle errors if occurred */
        if (error) {
            fprintf(stderr, "\nError occured in firstPass in the file %s\n", filename);
            return 0;
        } else {
            /* update data labels with final IC value */
            updateDataLabelsLocations(IC);
            return 1; /* success */
        }
    }
}
