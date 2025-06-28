#if 0

#include "../include/prova.h"
#include "../include/graphics.h"
#include "../include/vga.h"
#include "../include/io.h"

// Update screen dimensions to match VGA mode 13h
#define VGA_GRAPHICS_WIDTH 320
#define VGA_GRAPHICS_HEIGHT 200

void rotation_example(GraphicsContext* ctx) {
    Color blue = {0, 0, 255, 255};
    graphics_draw_rotated_rectangle(ctx, 200, 200, 100, 50, 45, blue);
}

void prova() {
    vga_write("Inizializzazione modo grafico...\n");
    
    // Remove return value check since vga_set_graphics_mode is void
    vga_set_graphics_mode();
    
    unsigned char* framebuffer = (unsigned char*)get_vga_framebuffer();
    if (!framebuffer) {
        vga_write("Errore: framebuffer nullo!\n");
        return;
    }
    
    // Debug: Try filling screen with white
    for (unsigned int i = 0; i < VGA_GRAPHICS_WIDTH * VGA_GRAPHICS_HEIGHT; i++) {
        framebuffer[i] = 0x0F;
    }
    
    GraphicsContext* ctx = graphics_init((void*)framebuffer, 
                                       VGA_GRAPHICS_WIDTH,
                                       VGA_GRAPHICS_HEIGHT,
                                       8);
    if (!ctx) {
        vga_write("Errore: context grafico nullo!\n");
        return;
    }
    
    Color white = {15, 15, 15, 255};
    Color red = {4, 4, 4, 255};
    
    graphics_draw_rectangle(ctx, 0, 0, 100, 100, white);
    graphics_draw_rectangle(ctx, 100, 100, 100, 100, red);
    
    graphics_swap_buffers(ctx);
    vga_write("Rendering completato\n");
}

#endif