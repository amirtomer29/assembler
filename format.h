#ifndef FORMAT_H
#define FORMAT_H


char* toQuad(char *value);

char* toBinary(int value);

char* toQuadAddressFormat(char* address);

char *buildFileName(const char *filename, const char *extension) ;

#endif /*FORMAT_H*/