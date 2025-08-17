#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parsers.h"


char* trim_whitespace(char *str) {
    char *end;

    /* Return NULL if input is NULL */
    if (!str) {
        return NULL;
    }

    /* Trim leading whitespace */
    while (isspace((unsigned char)*str)) {
        str++;
    }

    /* Check if string is now empty */
    if (*str == '\0') {
        return str;
    }

    /* Find end of string */
    end = str + strlen(str) - 1;

    /* Trim trailing whitespace */
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }

    /* Null terminate after the last non-whitespace character */
    *(end + 1) = '\0';

    /* Return trimmed string */
    return str;
}

char* get_first_space(char *str){
    if (!str) {
        return NULL;
    }
    str = strchr(str, ' ');
    return str;
}

char* jump_next_word(char* str) {
    char *space;

    if (!str) {
        return NULL;
    }
    str = trim_whitespace(str);
    space = get_first_space(str);
    if (!space) {
        return NULL;
    }
    space = trim_whitespace(space);
    return space;
}

/* Function to skip spaces in a line
// Returns the number of spaces skipped*/
int skipSpaces (char *line) {
    int i;
    if (!line) {
        return 0;
    }
    i = 0;
    while (line[i] == '\t' || line[i] == ' ') {i++;};
    return i;
}

/* Function to get a word from a line
// Returns a dynamically allocated string containing the word,
or NULL if no word is found */
char* getWord(char *line){
    int i = skipSpaces(line);
    int start = i;
    int len = 0;
    char *word = NULL;
    if (line[i] == '\0' || line[i] == '\n') {
        return NULL; /*no word found*/
    }
    /*find the end of the word*/
    while (line[i] != '\t' && line[i] != ' ' && line[i] != '\n' && line[i] != '\0'){
        i++;
    }
    len = i - start;
    word = malloc(len + 1);
    if (!word) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    strncpy(word, &line[start], len);
    word[len] = '\0';
    return word;
}

char* trim_char(char *str) {
    if (!str) return NULL;
    while(str[0] != ' ' && str[0] != '\t' && str[0] != '\n' && str[0] != '\0') {str++;}
    return str; /*return the pointer to the first whitespace character*/
}

int getRegisterNum(char *word) {
    word = trim_whitespace(word);
    if (word[0] == 'r' && word[1] >= '0' && word[1] <= '7' && word[2] == '\0') {
        return word[1] - '0';
    }
    return 0;
}