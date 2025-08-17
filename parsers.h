#ifndef PARSERS_H
#define PARSERS_H

char* trim_whitespace(char *str);
char* get_first_space(char *str);
char* jump_next_word(char* str);
char* getWord(char *line);
int skipSpaces (char *line);
char* trim_char(char *str);

int getRegisterNum(char *word);

#endif /* PARSERS_H */