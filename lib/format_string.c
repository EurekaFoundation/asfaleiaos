#include "../include/format_string.h"

int format_string(char* dest, const char* format, const char* arg) {
    while (*format) {
        if (*format == '%' && *(format + 1) == 's') {
            // Copy arg
            while (*arg) {
                *dest++ = *arg++;
            }
            format += 2;
        } else {
            *dest++ = *format++;
        }
    }
    *dest = '\0';
    return 0;
}