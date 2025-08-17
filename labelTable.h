#ifndef LABEL_TABLE_H
#define LABEL_TABLE_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_LABEL_TYPE_NAME 10
#define MAX_EXTERN_APEARS 30




typedef struct Label {
    char *name;
    char *type;
    int location; /*location of the label in the file*/
    int extern_locatoins[MAX_EXTERN_APEARS];
    int extern_locatins_num;
    struct Label *next;
} Label;



void initialLabelTable ();

unsigned int hashLabel (const char* str);

int insertLabelToTbl (char* name, int line_num);

Label* checkLabelExist(char *word);

void printLabelTable();

char* trim_whitespace(char *str);

void updateDataLabelsLocations(int size_ic);

#endif /* LABEL_TABLE_H */
