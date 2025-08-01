#include "preAssembler.h"
#include "macroTable.h"
#include "labelTable.h"

/*loading the files*/
int preAssembler(char *filename){
    char line[MAX_LINE_LEN+2];
    int line_num = 0;
    char ch = ' ';
    char *word;
    int eof;
    int macroF = 0;
    char name[MAX_LABEL_NAME];


    
    
    /*loading the files*/
    FILE *src = fopen(filename, "r");
    FILE *new = fopen("temp.txt", "w");

    /*check error while loading the files*/
    if (!src||!new) {
        perror("cant open file");
    } else {
        /*main loop - while the file not over*/
        while (fgets(line, MAX_LINE_LEN+2, src) != NULL) /*+2 for \0 and \n*/{
            line_num++;
            
            /*if line excceeds maximum length of line*/
            if (strchr(line, '\n')== NULL) {
                fprintf(stderr, "Error: line %d excceeds %d charcters", line_num, MAX_LINE_LEN);
                continue;
            } 
            
            word = getWord(line);

            /*if line is comment line skip line*/
            if (word[0] == ';') {
                free(word);
                continue;
            }

            /*if line is body line of macro*/
            if (macroF) {

                /*if line is the end of body line of macro*/
                if (strncmp(word, "mcroend", 7)== 0) {
                    macroF = 0;
                }
                /*if line is body line of macro*/
                 else {
                    insertLineToMacro(line);
                }
                free(word);
                continue;
            }    

            /*if word is macro*/
            if (checkMacroExist(word)) {
                fputs(getMacroBody(macro), new);
                free(word);
                continue;
            } 
            
            /*if word is labal*/
            if (line[(strlen(word))+1] == ':'){
                insertLabelToTbl(word);
                fputs(line, new);
                free(word);
                continue;
            } 

            /*if line is macro definition line*/
            if ((strncmp(word, "mcro" , 4))== 0) {
                sscanf(line, "%*s %s", name);
                if (checkMacroName(word)) {
                    insertMacroNameToTbl(word);
                    macroF = 1;
                } else {
                    fprintf(stderr, "Error: line %d macro %s name illegal", line_num, word);
                }
                free(word);
                continue;
            }

            /*if line is opcode line*/
            else {
                fputs(line, new);
                free(word);
            }
        }
    }
    return 1;
}


int skipSpaces (char *line) {
    int i = 0;
    while (line[i] == '\t' || line[i] == ' ') {i++};
    return i;
}

char* getWord(char *line){
    int i = skipSpaces(line);
    int start = i;
    int len = 0;
    while (line[i] != '\t' && line[i] != ' ' && line[i] != '\n' && line[i] != '\0'){
        i++;
    }
    len = i - start;
    char *word = malloc(len + 1);
    if (!word) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    strncpy(word, &line[start], len);
    word[len] = '\0';
    return word;
}



 

/*read line after line*/

/*check if the first field is name of macro,
if yes - copy from the macros table  the lines
*/


/*check if the first field is new macro
1. switch macro flag on,
2. enter to the macros table the name of the macro.
3. read lines until 'mcroend'.
4. delete the macro from the file.
5. switch macro flag off.
*/

/*save to new file*/

