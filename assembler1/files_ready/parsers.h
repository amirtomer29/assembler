#ifndef PARSERS_H
#define PARSERS_H

/**
 * Trim leading and trailing whitespace from a string.
 * @param str Input string
 * @return Pointer to trimmed string, or NULL if input is NULL
 */
char* trim_whitespace(char *str);

/**
 * Find the first space character in a string.
 * @param str Input string
 * @return Pointer to the first space, or NULL if none
 */
char* get_first_space(char *str);

/**
 * Skip the current word and move pointer to the next word.
 * @param str Input string
 * @return Pointer to the start of the next word, or NULL if none
 */
char* jump_next_word(char* str);

/**
 * Extract the first word from a line.
 * @param line Input string
 * @return Dynamically allocated string containing the word, or NULL if none
 */
char* getWord(char *line);

/**
 * Count spaces or tabs at the beginning of a line.
 * @param line Input string
 * @return Number of spaces skipped
 */
int skipSpaces (char *line);

/**
 * Move pointer until whitespace or newline is found.
 * @param str Input string
 * @return Pointer to the first whitespace character
 */
char* trim_char(char *str);

/**
 * Check if a line is empty or only whitespace.
 * @param line Input string
 * @return 1 if empty or whitespace only, 0 otherwise
 */
int is_empty_or_whitespace(const char *line);

/**
 * Get a register number from a string in the form "r0".."r7".
 * @param word Input string
 * @return Register number (0–7), or 0 if invalid
 */
int getRegisterNum(char *word);

#endif /* PARSERS_H */
