#ifndef LABEL_TABLE_H
#define LABEL_TABLE_H

#include <stdio.h>
#include <stdlib.h>


#define LABEL_HASH_SIZE 31
#define MAX_LABEL_LEN 30

typedef struct Label {
    char *name;
    char *type;
    struct Label *next;
} Label;



void initialLabelTable ();

unsigned int hashLabel (const char* str);

void insertLabelToTbl (const char* name);

Label* checkLabelExist(char *word);

void printLabelTable();

#endif /* LABEL_TABLE_H */
