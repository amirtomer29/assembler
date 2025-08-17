#ifndef SECOND_PASS_H
#define SECOND_PASS_H
#define LINE_BUFFER_SIZE (4 + 8 + MAX_LABEL_NAME + 2) /* address + tab + name + \n + null */

int secondPass(char *filename);


int createExtEntFile(FILE *ent, FILE *ext) ;

int createObjFile(FILE *obj) ;
#endif /*SECOND_PASS_H*/


