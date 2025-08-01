#include <stdio.h>
#include <stdlib.h>


#define LABEL_HASH_SIZE 31
#define MAX_LABEL_LEN 30

typedef struct Label {
    char *name;
    char *type;
    struct Label *next;
};
