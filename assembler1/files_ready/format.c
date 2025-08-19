#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "format.h"

/**
 * Function: toQuadAddressFormat
 * -----------------------------
 * Shifts a 5-character address string one character to the left,
 * effectively removing the first character and null-terminating the result.
 * 
 * Example:
 *   Input:  "0abcd"
 *   Output: "abcd"
 *
 * @param address - the address string to transform (must be at least 5 characters)
 * @return the modified address string
 */
char* toQuadAddressFormat(char* address){
    int i;
    for(i = 0; i<4; i++){
       address[i]=address[i+1];
    }
    address[4]='\0';
    return address;
}

/**
 * Function: toBinary
 * ------------------
 * Converts an integer to a 10-bit binary string representation.
 * Supports signed integers using two's complement for negative values.
 *
 * @param value - integer in the range [-512, 1023]
 * @return a dynamically allocated string of 10 binary digits (NULL on error)
 */
char* toBinary(int value) {
    int i;
    char *binary = malloc(11); /*10 bits for binary + 1 for \0*/
    if (!binary) {
        perror("malloc");
        return NULL;
    }
    /*if the value is not in the size of 10*/
    if (value > 1023) {
        free(binary);
        fprintf(stderr,"ERROR:%d is out of range!\n", value);
        return NULL; /*error*/
    }
    if (value < 0) {
        value = (1 << 10) + value; /*complete 2*/
    }
    for (i = 9; i >= 0; i--) {
        binary[i] = (value % 2) ? '1' : '0';
        value >>= 1;
    }
    binary[10] = '\0'; /*null terminate the string*/
    return binary;
}

/**
 * Function: toQuad
 * ----------------
 * Converts a 10-bit binary string to a custom base-4 encoding using characters:
 *   "00" -> 'a', "01" -> 'b', "10" -> 'c', "11" -> 'd'
 * Processes 2 bits at a time for a total of 5 characters.
 *
 * @param value - a binary string of length 10
 * @return a dynamically allocated 5-character encoded string (NULL on error)
 */
char* toQuad(char *value) {
    int i;
    char *quad = malloc(6);
    if (!quad) {
        perror("malloc");
        return NULL;
    }
    for (i = 0; i <= 8; i+=2) {
        if(value[i] == '0' && value[i+1] == '0') {
            quad[i/2] = 'a';
            continue;
        }
        if(value[i] == '0' && value[i+1] == '1') {
            quad[i/2] = 'b';
            continue;
        }
        if(value[i] == '1' && value[i+1] == '0') {
            quad[i/2] = 'c';
            continue;
        }
        if(value[i] == '1' && value[i+1] == '1') {
            quad[i/2] = 'd';
            continue;
        }
    }
    quad[5] = '\0'; /*null terminate the string*/
    return quad;
}

/**
 * Function: buildFileName
 * -----------------------
 * Constructs a new file name by replacing (or appending) a given extension.
 * If the input file name has an existing extension (e.g., ".as"), it is removed.
 * The new extension is added after the base name.
 *
 * @param filename  - original file name (e.g., "code.as")
 * @param extension - new extension (e.g., ".ob")
 * @return dynamically allocated new file name (e.g., "code.ob"), or NULL on error
 */
char *buildFileName(const char *filename, const char *extension) {
    char *dot = strrchr(filename, '.');
    size_t base_len = dot ? (size_t)(dot - filename) : strlen(filename);
    char *new_name = malloc(base_len + strlen(extension) + 1);

    if (!new_name) {
        perror("Memory allocation failed");
        return NULL;
    }

    strncpy(new_name, filename, base_len);
    new_name[base_len] = '\0';
    strcat(new_name, extension);
    return new_name;
}
