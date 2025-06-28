#include "../include/print_string.h"
#include "../include/color.h"
#include "../include/vga.h"

void print_string(const char* str) {
    vga_write(str);
}

void print_colored_string(const char* str, unsigned char color) {
    vga_write_color(str, color);
}

void set_color(unsigned char foreground) {
    vga_set_color(foreground);
}

void print_dec(unsigned int value) {
    char buffer[32];
    int i = 0;
    
    if(value == 0) {
        print_char('0');
        return;
    }
    
    while(value > 0) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }
    
    while(--i >= 0) {
        print_char(buffer[i]);
    }
}

void print_double(double number) {
    // Handle negative numbers
    if (number < 0) {
        print_char('-');
        number = -number;
    }
    
    // Extract integer part
    int int_part = (int)number;
    
    // Extract decimal part (6 decimal places)
    int decimal_part = (int)((number - int_part) * 1000000 + 0.5);
    
    // Print integer part
    if (int_part == 0) {
        print_char('0');
    } else {
        char int_buffer[32];
        int idx = 0;
        
        while (int_part > 0) {
            int_buffer[idx++] = '0' + (int_part % 10);
            int_part /= 10;
        }
        
        while (idx > 0) {
            print_char(int_buffer[--idx]);
        }
    }
    
    // Print decimal part
    print_char('.');
    
    char dec_buffer[7];
    int idx = 0;
    
    // Handle trailing zeros
    while (decimal_part > 0 && idx < 6) {
        dec_buffer[idx++] = '0' + (decimal_part % 10);
        decimal_part /= 10;
    }
    
    // Pad with leading zeros if needed
    while (idx < 6) {
        dec_buffer[idx++] = '0';
    }
    
    // Print decimal digits in correct order
    while (idx > 0) {
        print_char(dec_buffer[--idx]);
    }
}

void print_int(int num) {
    char buffer[32];
    int i = 0;
    
    // Handle 0 separately
    if (num == 0) {
        print_char('0');
        return;
    }
    
    // Convert number to string (reversed)
    while (num > 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }
    
    // Print in correct order
    while (i > 0) {
        print_char(buffer[--i]);
    }
}