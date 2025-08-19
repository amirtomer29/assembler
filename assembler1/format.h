#ifndef FORMAT_H
#define FORMAT_H

/**
 * Converts a 10-bit binary string to a custom quad-based representation.
 * Each pair of binary digits is mapped to a character:
 * "00" -> 'a', "01" -> 'b', "10" -> 'c', "11" -> 'd'
 *
 * @param value - a binary string of length 10 (must be null-terminated)
 * @return a dynamically allocated string of 5 quad characters (null-terminated)
 */
char* toQuad(char *value);

/**
 * Converts an integer to a 10-bit binary string (as a string of '0' and '1').
 * Handles signed integers using two's complement for negatives.
 *
 * @param value - integer to convert (range: -512 to 1023)
 * @return a dynamically allocated 10-character binary string (null-terminated), or NULL on error
 */
char* toBinary(int value);

/**
 * Adjusts a quad address string by removing its first character and shifting left.
 * The result is a 4-character string (used for printing memory addresses).
 *
 * @param address - a 5-character address string (must be null-terminated)
 * @return pointer to the modified address string
 */
char* toQuadAddressFormat(char* address);

/**
 * Creates a new file name by replacing the extension of the original file name.
 * If no extension exists, the new extension is simply appended.
 *
 * @param filename  - original file name (e.g., "file.as")
 * @param extension - new extension to append (e.g., ".ob")
 * @return a dynamically allocated new file name, or NULL on memory allocation failure
 */
char *buildFileName(const char *filename, const char *extension);



#endif /* FORMAT_H */
