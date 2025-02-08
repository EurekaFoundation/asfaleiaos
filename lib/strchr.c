#include "../include/NULL.h"
#include "../include/strchr.h"

char* my_strchr(const char* str, int c) {
    while (*str) {
        if (*str == (char)c) {
            return (char*)str;
        }
        str++;
    }
    return NULL;
}