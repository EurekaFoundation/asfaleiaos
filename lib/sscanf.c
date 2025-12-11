#include "../include/string.h"

// Simple sscanf implementation for freestanding environment
// Handles: %s, %2s, %d, %[^c] patterns

static int my_isspace_sscanf(int c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v');
}

int sscanf(const char* str, const char* format, ...) {
    int count = 0;
    const char* s = str;
    const char* f = format;
    char** pstr;
    int* pint;
    
    // We'll use a simple approach: manually parse based on format
    while (*f) {
        // Skip whitespace in format and input
        while (*f && my_isspace_sscanf(*f)) f++;
        while (*s && my_isspace_sscanf(*s)) s++;
        
        if (*f == '%') {
            f++;
            
            if (*f == 's') {
                // Read string
                // Skip variadic arg access - we'll use a workaround via stack
                f++;
                // This is a hack but necessary for freestanding
                pstr = (char**)(&format + 1 + count);
                if (*pstr) {
                    char* dst = *pstr;
                    while (*s && !my_isspace_sscanf(*s)) {
                        *dst++ = *s++;
                    }
                    *dst = '\0';
                    count++;
                }
            } else if (*f == '[') {
                // Read until character: %[^c]
                f++; // skip [
                f++; // skip ^
                char until_char = *f;
                f++; // skip char
                f++; // skip ]
                
                pstr = (char**)(&format + 1 + count);
                if (*pstr) {
                    char* dst = *pstr;
                    while (*s && *s != until_char && *s != '\n') {
                        *dst++ = *s++;
                    }
                    *dst = '\0';
                    count++;
                }
            } else if (*f == 'd') {
                // Read integer
                f++;
                pint = (int*)(&format + 1 + count);
                if (pint) {
                    *pint = 0;
                    while (*s && *s >= '0' && *s <= '9') {
                        *pint = *pint * 10 + (*s++ - '0');
                    }
                    count++;
                }
            } else if (*f == '2' && *(f+1) == 's') {
                // Read 2-char string
                f += 2;
                pstr = (char**)(&format + 1 + count);
                if (*pstr) {
                    char* dst = *pstr;
                    int limit = 2;
                    while (*s && !my_isspace_sscanf(*s) && limit-- > 0) {
                        *dst++ = *s++;
                    }
                    *dst = '\0';
                    count++;
                }
            } else {
                f++;
            }
        } else if (*f == ' ') {
            f++;
            while (*s && my_isspace_sscanf(*s)) s++;
        } else {
            if (*f == *s) {
                f++;
                s++;
            } else {
                break;
            }
        }
    }
    
    return count;
}
