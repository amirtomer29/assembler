#include "macroTable.h"
#include "labelTable.h"
#include "parsers.h"
#include "instructions.h"
#include "format.h"
#include "preAssembler.h"

/**
 * preAssembler.c
 * Implements the pre-assembler phase of the assembler.
 * Processes macros, labels, and prepares the .am file.
 */
char* preAssembler(char *filename) {
    int error = 0;
    char line[MAX_LINE_LEN + 2];
    int line_num = 0;
    char *line_p = NULL;
    char *word, *word_2;
    Macro *macro = NULL;
    char name[MAX_LABEL_NAME];
    char extra[MAX_LINE_LEN];
    char *out_file_name;
    size_t word_len;
    FILE *src;
    FILE *out;

    out_file_name = buildFileName(filename, ".am");
    
    src = fopen(filename, "r");
    out = fopen(out_file_name, "w");



    if (!src || !out) {
        perror("Cannot open file");
        return NULL;
    }

    while (fgets(line, MAX_LINE_LEN + 2, src) != NULL) {
        line_num++;
        line[strcspn(line, "\n")] = '\0';

        if (strlen(line) > MAX_LINE_LEN) {
            fprintf(stderr, "Error: line %d exceeds %d characters\n", line_num, MAX_LINE_LEN);
            error = 1;
            continue;
        }

        line_p = trim_whitespace(line);
        if (line_p[0] == '\0' || line_p[0] == ';') {
            continue;
        }

        word = getWord(line_p);
        if (!word) {
            continue;
        }

        if (macro) {
            if (strcmp(word, "mcroend") == 0) {
                macro = NULL;
                if (sscanf(line_p, "%*s %s", extra) == 1) {
                    fprintf(stderr, "Error: line %d macro %s end of line illegal\n", line_num, name);
                    error = 1;
                }
            } else {
                insertLineToMacro(macro, line_p);
            }
            free(word);
            continue;
        }

        if (checkMacroExist(word)) {
            fputs(getMacroBody(word), out);
            free(word);
            continue;
        }

        word_len = strlen(word);
        if (word_len > 0 && word[word_len - 1] == ':') {
            word[word_len - 1] = '\0';
            if (!insertLabelToTbl(word, line_num)) {error = 1;}
            fputs(line_p, out);
            fputc('\n', out);
            free(word);
            continue;
        }

        if (strcmp(word, "mcro") == 0) {
            if (sscanf(line_p, "%*s %s", name) != 1) {
                fprintf(stderr, "Error: line %d missing macro name\n", line_num);
                error = 1;
            } else if (sscanf(line_p, "%*s %*s %s", line) == 1) {
                fprintf(stderr, "Error: line %d macro %s end of line illegal\n", line_num, name);
                error = 1;
            } else if (checkMacroName(name, line_num)) {
                macro = insertMacroNameToTbl(name);
            } else {
                error = 1;
            }
            free(word);
            continue;
        }

        if (getInstructionType(word) == 4) {
            word_2 = jump_next_word(line_p);
            if (word_2 != NULL) {
                if (!insertLabelToTbl(word_2, line_num)) {error = 1;}
            } else {
                fprintf(stderr, "Warning: line %d has no operand after instruction\n", line_num);
            }
        }
        fputs(line_p, out);
        fputc('\n', out);
        free(word);
        continue;
    }

    fclose(src);
    fclose(out);

    if (error) {
        fprintf(stderr, "\nError occurred while reading the file %s\n", filename);
        remove("temp.txt");
        return NULL;
    }

    printLabelTable();
    printMacroTable();

    return out_file_name;
}


/**
 * Checks whether a word is a register (r0 to r7).
 * Returns 1 if it is, 0 otherwise.
 */
int checkIfRegister(char *word) {
    word = trim_whitespace(word);
    if (word[0] == 'r' && word[1] >= '0' && word[1] <= '7' && word[2] == '\0') {
        return 1;
    }
    return 0;
}
