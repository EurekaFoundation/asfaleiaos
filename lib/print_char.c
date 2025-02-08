// Function for printing a character to the screen

#include "../include/io.h"
#include "../include/vga.h"

extern void scroll_screen(void);
#define VIDEO_MEMORY 0xB8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define CHAR_ATTRIBUTE 0x07

static int cursor = 0;

void scroll_screen() {
    volatile char* video = (volatile char*)VIDEO_MEMORY;
    
    // Move each line up
    for (int i = 0; i < (SCREEN_HEIGHT - 1) * SCREEN_WIDTH * 2; i++) {
        video[i] = video[i + SCREEN_WIDTH * 2];
    }
    
    // Clear last line
    int last_line = (SCREEN_HEIGHT - 1) * SCREEN_WIDTH * 2;
    for (int i = 0; i < SCREEN_WIDTH * 2; i += 2) {
        video[last_line + i] = ' ';
        video[last_line + i + 1] = CHAR_ATTRIBUTE;
    }
    
    // Move cursor to start of last line
    cursor = (SCREEN_HEIGHT - 1) * SCREEN_WIDTH * 2;
}

void print_char(char c) {
    int x, y;
    vga_get_cursor(&x, &y);
    
    switch(c) {
        case '\n':
            x = 0;
            y++;
            break;
            
        case '\r':
            x = 0;
            break;
            
        case '\b':
            if (x > 0) {
                x--;
                unsigned int offset = (y * VGA_WIDTH + x) * 2;
                *(volatile unsigned char*)(VGA_MEMORY + offset) = ' ';
                *(volatile unsigned char*)(VGA_MEMORY + offset + 1) = 0x07;
            }
            break;
            
        default:
            if (c >= 32 && c < 127) {
                unsigned int offset = (y * VGA_WIDTH + x) * 2;
                *(volatile unsigned char*)(VGA_MEMORY + offset) = c;
                *(volatile unsigned char*)(VGA_MEMORY + offset + 1) = 0x07;
                x++;
                
                if (x >= VGA_WIDTH) {
                    x = 0;
                    y++;
                }
            }
            break;
    }
    
    vga_set_cursor(x, y);
}