// window.c
#include "../include/window.h"  // Includi la libreria delle finestre
#include "../include/vga.h"  // Includi la libreria VGA per la gestione della memoria video
#include "../include/string.h"  // Per usare strlen() e strncpy()
#include "../include/print_string.h"  // Per usare print_string()
#include "../include/color.h"  // Per usare i colori
#include "../include/drivers.h"  // Per usare print_char()

void init_window(Window* window, int x, int y, int width, int height, const char* title) {
    window->x = x;
    window->y = y;
    window->width = width;
    window->height = height;
    strncpy(window->title, title, MAX_TITLE_LENGTH);
}

void draw_window_with_background(int x, int y, int width, int height, unsigned char color) {
    // Disegna il background della finestra
    for (int i = x; i < x + width; i++) {
        for (int j = y; j < y + height; j++) {
            unsigned int offset = (j * VGA_WIDTH + i) * 2;
            vga_buffer[offset] = ' ';   // Spazio per il background
            vga_buffer[offset + 1] = color; // Colore di sfondo
        }
    }
}

// Funzione per disegnare una finestra
void draw_window(Window* window) {
    // Draw top border
    vga_set_cursor(window->x, window->y);
    print_char('+');
    for(int i = 0; i < window->width - 2; i++) {
        print_char('-');
    }
    print_char('+');
    
    // Draw sides
    for(int i = 1; i < window->height - 1; i++) {
        vga_set_cursor(window->x, window->y + i);
        print_char('|');
        vga_set_cursor(window->x + window->width - 1, window->y + i);
        print_char('|');
    }
    
    // Draw bottom border
    vga_set_cursor(window->x, window->y + window->height - 1);
    print_char('+');
    for(int i = 0; i < window->width - 2; i++) {
        print_char('-');
    }
    print_char('+');
    
    // Draw title
    vga_set_cursor(window->x + 2, window->y);
    print_string(window->title);
}
