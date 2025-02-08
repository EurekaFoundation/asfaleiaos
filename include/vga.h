#ifndef VGA_H
#define VGA_H

#define VGA_MEMORY 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

void vga_init(void);
void vga_set_color(unsigned char color);
void vga_putchar(char c);
void vga_write(const char* str);
void vga_write_color(const char* str, unsigned char color);
void vga_clear_screen(void);
void vga_clear_line(int line);
void vga_set_cursor(int x, int y);
void vga_get_cursor(int* x, int* y);
void vga_update_cursor(void);
void disable_cursor(void);
// Funzione per ottenere il puntatore al buffer VGA
extern volatile unsigned char *vga_buffer;

#endif