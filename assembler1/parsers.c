#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parsers.h"

/**
 * Trim leading and trailing whitespace from a string.
 * @param str Input string
 * @return Pointer to trimmed string, or NULL if input is NULL
 */
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

/**
 * Find the first space in a string.
 * @param str Input string
 * @return Pointer to first space, or NULL if not found
 */
char* get_first_space(char *str){
    if (!str) {
        return NULL;
    }
    str = strchr(str, ' ');
    return str;
}

/**
 * Skip the current word and move pointer to the next one.
 * @param str Input string
 * @return Pointer to the start of the next word, or NULL if none
 */
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

/**
 * Skip whitespace characters in a line.
 * @param line Input string
 * @return Number of spaces skipped
 */
int skipSpaces (char *line) {
    int i;
    if (!line) {
        return 0;
    }
    i = 0;
    while (line[i] == '\t' || line[i] == ' ') {i++;};
    return i;
}

/**
 * Extract the first word from a line.
 * @param line Input string
 * @return Newly allocated string containing the word, or NULL if none
 */
char* getWord(char *line){
    int i = skipSpaces(line);
    int start = i;
    int len = 0;
    char *word = NULL;
    if (line[i] == '\0' || line[i] == '\n') {
        return NULL; /* no word found */
    }
    /* find the end of the word */
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

/**
 * Move pointer until a whitespace or newline is found.
 * @param str Input string
 * @return Pointer to first whitespace character
 */
char* trim_char(char *str) {
    if (!str) return NULL;
    while(*str != ' ' && *str != '\t' && *str != '\n') {str++;}
    return str; /* return pointer to the first whitespace character */
}

/**
 * Get register number from a string like "r0".."r7".
 * @param word Input string
 * @return Register number 0–7, or 0 if invalid
 */
int getRegisterNum(char *word) {
    word = trim_whitespace(word);
    if (word[0] == 'r' && word[1] >= '0' && word[1] <= '7' && word[2] == '\0') {
        return word[1] - '0';
    }
    return 0;
}

/**
 * Check if a line is empty or contains only whitespace.
 * @param line Input string
 * @return 1 if empty/whitespace, 0 otherwise
 */
int is_empty_or_whitespace(const char *line) {
    if (!line) return 1; /* NULL pointer treated as empty */

    while (*line) {
        if (!isspace((unsigned char)*line)) {
            return 0; /* found a non-whitespace character */
        }
        line++;
    }
    return 1; /* all characters were whitespace */
}
