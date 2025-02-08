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