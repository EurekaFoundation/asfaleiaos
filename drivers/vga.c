#include "../include/vga.h"
#include "../include/io.h"
#include "../include/graphics.h"

#define VGA_MEMORY 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static int cursor_x = 0;
static int cursor_y = 0;
static unsigned char current_color = 0x07;
volatile unsigned char *vga_buffer = (volatile unsigned char*)VGA_MEMORY;
volatile unsigned char* graphics_buffer = (volatile unsigned char*)VGA_GRAPHICS_MEMORY;


static void update_hw_cursor(void);

void vga_set_graphics_mode(void) {
    // Setup Mode 13h
    outb(0x3C2, 0x63);
    outb(0x3D4, 0x0A); outb(0x3D5, 0x20);
    outb(0x3D4, 0x0B); outb(0x3D5, 0x00);
    outb(0x3D4, 0x0C); outb(0x3D5, 0x00);
    outb(0x3D4, 0x0D); outb(0x3D5, 0x00);
    outb(0x3C4, 0x00); outb(0x3C5, 0x03);
    outb(0x3C4, 0x01); outb(0x3C5, 0x01);
    outb(0x3CE, 0x05); outb(0x3CF, 0x40);
    outb(0x3CE, 0x06); outb(0x3CF, 0x05);
    outb(0x3C0, 0x30); outb(0x3C0, 0x41);
    outb(0x3C0, 0x33); outb(0x3C0, 0x00);
    
    // Set up a basic VGA palette
    outb(0x3C8, 0x00);  // Start at color 0
    
    // Color 0: Black
    outb(0x3C9, 0); outb(0x3C9, 0); outb(0x3C9, 0);
    
    // Color 1: White
    outb(0x3C9, 63); outb(0x3C9, 63); outb(0x3C9, 63);
    
    // Color 2: Red
    outb(0x3C9, 63); outb(0x3C9, 0); outb(0x3C9, 0);
    
    // Color 3: Green
    outb(0x3C9, 0); outb(0x3C9, 63); outb(0x3C9, 0);
    
    // Color 4: Blue
    outb(0x3C9, 0); outb(0x3C9, 0); outb(0x3C9, 63);
    
    // Color 5: Yellow
    outb(0x3C9, 63); outb(0x3C9, 63); outb(0x3C9, 0);
    
    // Color 6: Magenta
    outb(0x3C9, 63); outb(0x3C9, 0); outb(0x3C9, 63);
    
    // Color 7: Cyan
    outb(0x3C9, 0); outb(0x3C9, 63); outb(0x3C9, 63);
}

uint32_t* get_vga_framebuffer(void) {
    return (uint32*)VGA_GRAPHICS_MEMORY;
}

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