#include "../include/clear_screen.h"

#define VIDEO_MEMORY 0xB8000
#define WHITE_ON_BLACK 0x0F
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define CHAR_ATTRIBUTE 0x0F

void clear_screen(void) {
    volatile char* video = (volatile char*)VIDEO_MEMORY;
    for(int i = 0; i < 80 * 25 * 2; i++) {
        video[i] = 0;
    }
}

void clear_line(int line) {
    volatile char* video = (volatile char*)VIDEO_MEMORY;
    int offset = line * SCREEN_WIDTH * 2;
    
    // Clear one line
    for(int i = 0; i < SCREEN_WIDTH * 2; i += 2) {
        video[offset + i] = ' ';
        video[offset + i + 1] = CHAR_ATTRIBUTE;
    }
}