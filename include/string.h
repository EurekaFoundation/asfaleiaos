#ifndef STRING_H
#define STRING_H

#include "types.h"

// Memory operations
int memcmp(const void* s1, const void* s2, size_t n);
void* memcpy(void* dest, const void* src, size_t n);
//void* memset(void* s, int c, size_t n);

// String operations
char* strncpy(char* dest, const char* src, size_t n);
int strcmp(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, size_t n);
size_t strlen(const char* s);
char* strchr(const char* s, int c);
char* strtok(char* str, const char* delim);
char* strcpy(char* dest, const char* src);
double string_to_double(const char* str);
#endif