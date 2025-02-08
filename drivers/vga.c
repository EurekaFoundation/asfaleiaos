#include "../include/vga.h"
#include "../include/io.h"

#define VGA_MEMORY 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static int cursor_x = 0;
static int cursor_y = 0;
static unsigned char current_color = 0x07;
volatile unsigned char *vga_buffer = (volatile unsigned char*)VGA_MEMORY;

static void update_hw_cursor(void);

void vga_init(void) {
    current_color = 0x07;
    cursor_x = 0;
    cursor_y = 0;
    vga_clear_screen();
    update_hw_cursor();
}

void vga_set_color(unsigned char color) {
    current_color = color;
}

void vga_get_cursor(int* x, int* y) {
    if (x) *x = cursor_x;
    if (y) *y = cursor_y;
}

void vga_set_cursor(int x, int y) {
    cursor_x = x;
    cursor_y = y;
    update_hw_cursor();
}

void vga_update_cursor(void) {
    unsigned short pos = cursor_y * VGA_WIDTH + cursor_x;
    outb(0x3D4, 14);
    outb(0x3D5, pos >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, pos);
}

static void update_hw_cursor(void) {
    unsigned short pos = cursor_y * VGA_WIDTH + cursor_x;
    outb(0x3D4, 14);
    outb(0x3D5, pos >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, pos);
}

static void scroll_up(void) {
    for (int y = 0; y < VGA_HEIGHT - 1; y++) {
        for (int x = 0; x < VGA_WIDTH * 2; x++) {
            vga_buffer[y * VGA_WIDTH * 2 + x] = vga_buffer[(y + 1) * VGA_WIDTH * 2 + x];
        }
    }
    
    int last_line = (VGA_HEIGHT - 1) * VGA_WIDTH * 2;
    for (int x = 0; x < VGA_WIDTH * 2; x += 2) {
        vga_buffer[last_line + x] = ' ';
        vga_buffer[last_line + x + 1] = current_color;
    }
    cursor_y--;
}

void vga_putchar(char c) {
    unsigned int offset;
    
    switch(c) {
        case '\n':
            cursor_x = 0;
            cursor_y++;
            break;
            
        case '\r':
            cursor_x = 0;
            break;
            
        case '\b':
            if (cursor_x > 0) {
                cursor_x--;
                offset = (cursor_y * VGA_WIDTH + cursor_x) * 2;
                vga_buffer[offset] = ' ';
                vga_buffer[offset + 1] = current_color;
            }
            break;
            
        default:
            // Remove ASCII range check to allow extended characters
            offset = (cursor_y * VGA_WIDTH + cursor_x) * 2;
            vga_buffer[offset] = c;
            vga_buffer[offset + 1] = current_color;
            cursor_x++;
            
            if (cursor_x >= VGA_WIDTH) {
                cursor_x = 0;
                cursor_y++;
            }
            break;
    }
    
    if (cursor_y >= VGA_HEIGHT) {
        scroll_up();
        cursor_y = VGA_HEIGHT - 1;  // Added to prevent cursor going off screen
    }
    
    update_hw_cursor();
}

void vga_write(const char* str) {
    while (*str) {
        vga_putchar(*str);
        str++;
    }
    update_hw_cursor();
}

void vga_write_color(const char* str, unsigned char color) {
    unsigned char old_color = current_color;
    vga_set_color(color);
    vga_write(str);
    vga_set_color(old_color);
}

void vga_clear_screen(void) {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT * 2; i += 2) {
        vga_buffer[i] = ' ';
        vga_buffer[i + 1] = current_color;
    }
    cursor_x = 0;
    cursor_y = 0;
    update_hw_cursor();
}

void disable_cursor() {
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}