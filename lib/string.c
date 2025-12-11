#include "../include/string.h"
#include "../include/types.h"
#include "../include/io.h"
#include "../include/NULL.h"

int memcmp(const void* s1, const void* s2, size_t n) {
    const unsigned char* p1 = s1;
    const unsigned char* p2 = s2;
    
    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] - p2[i];
        }
    }
    return 0;
}

char* strncpy(char* dest, const char* src, size_t n) {
    size_t i;
    
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    
    for (; i < n; i++) {
        dest[i] = '\0';
    }
    
    return dest;
}

char* strcpy(char* dest, const char* src) {
    char* original_dest = dest;
    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return original_dest;
}

void* memset(void* s, int c, size_t n) {
    unsigned char* p = s;
    for (size_t i = 0; i < n; i++) {
        p[i] = (unsigned char)c;
    }
    return s;
}

void* memmove(void* dest, const void* src, size_t n) {
    unsigned char* d = dest;
    const unsigned char* s = src;
    if (d < s) {
        for (size_t i = 0; i < n; i++) d[i] = s[i];
    } else if (d > s) {
        for (size_t i = n; i > 0; i--) d[i-1] = s[i-1];
    }
    return dest;
}

void* memcpy(void* dest, const void* src, size_t n) {
    unsigned char* d = dest;
    const unsigned char* s = src;
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

size_t strlen(const char* str) {
    size_t len = 0;
    while(str[len] != '\0') len++;
    return len;
}

int strncmp(const char* s1, const char* s2, size_t n) {
    while(n > 0 && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
        n--;
    }
    if (n == 0) return 0;
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

char* strchr(const char* s, int c) {
    while (*s != '\0') {
        if (*s == c)
            return (char*)s;
        s++;
    }
    return NULL;
}

char* strrchr(const char* s, int c) {
    const char* last = NULL;
    while (*s != '\0') {
        if (*s == c)
            last = s;
        s++;
    }
    return (char*)last;
}

char* strtok(char* str, const char* delim) {
    static char* last;
    if (str) last = str;
    if (!last) return NULL;
    
    // Skip leading delimiters
    while (*last && strchr(delim, *last))
        last++;
        
    if (!*last) return NULL;
    
    // Find end of token
    char* token = last;
    while (*last && !strchr(delim, *last))
        last++;
        
    if (*last) {
        *last = '\0';
        last++;
    } else {
        last = NULL;
    }
    
    return token;
}

double string_to_double(const char* str) {
    double result = 0.0;
    double factor = 1.0;
    int sign = 1;
    int point_seen = 0;

    // Gestione segno
    if (*str == '-') {
        sign = -1;
        str++;
    }

    for (; *str; str++) {
        if (*str == '.') {
            point_seen = 1;
            continue;
        }
        if (*str < '0' || *str > '9') break;
        int digit = *str - '0';
        if (point_seen) {
            factor /= 10.0;
            result += digit * factor;
        } else {
            result = result * 10.0 + digit;
        }
    }
    return sign * result;
}