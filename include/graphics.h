#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "io.h"
#include "vga.h"

// Tipi base
typedef unsigned char  uint8;
typedef unsigned int   uint32;
typedef signed int    int32;

extern const int32 sin_lookup[360];
extern const int32 cos_lookup[360];

// Strutture per il sistema di font
typedef struct {
    uint8* data;        // Bitmap del carattere
    int32 width;        // Larghezza del carattere
    int32 height;       // Altezza del carattere
} Glyph;

typedef struct {
    Glyph* glyphs;     // Array di glifi
    int32 glyph_count; // Numero di glifi
    int32 glyph_height;// Altezza uniforme dei glifi
} Font;

// Struttura per immagini
typedef struct {
    uint32* data;      // Dati dell'immagine
    int32 width;       // Larghezza
    int32 height;      // Altezza
} Image;

// Struttura per pattern di riempimento
typedef struct {
    uint32* pattern;   // Pattern di riempimento
    int32 width;       // Larghezza del pattern
    int32 height;      // Altezza del pattern
} FillPattern;

// Struttura per colore con alpha
typedef struct {
    uint8 r;
    uint8 g;
    uint8 b;
    uint8 a;           // Canale alpha per trasparenza
} Color;

typedef struct {
    unsigned char* framebuffer;
    int32 width;
    int32 height;
    int32 bpp;
} GraphicsContext;


// Funzioni base esistenti
GraphicsContext* graphics_init(unsigned char* fb_addr, int32 width, int32 height, int32 bpp);
void graphics_clear(GraphicsContext* ctx, Color color);
void graphics_put_pixel(GraphicsContext* ctx, int32 x, int32 y, Color color);
void graphics_draw_line(GraphicsContext* ctx, int32 x1, int32 y1, int32 x2, int32 y2, Color color);
void graphics_draw_rectangle(GraphicsContext* ctx, int32 x, int32 y, int32 width, int32 height, Color color);
void graphics_fill_rectangle(GraphicsContext* ctx, int32 x, int32 y, int32 width, int32 height, Color color);
void graphics_draw_circle(GraphicsContext* ctx, int32 x0, int32 y0, int32 radius, Color color);
void graphics_fill_circle(GraphicsContext* ctx, int32 x0, int32 y0, int32 radius, Color color);

// Nuove funzioni per il testo
Font* graphics_load_font(uint8* font_data, int32 glyph_count, int32 glyph_height);
void graphics_set_font(GraphicsContext* ctx, Font* font);
void graphics_draw_char(GraphicsContext* ctx, char c, int32 x, int32 y, Color color);
void graphics_draw_text(GraphicsContext* ctx, const char* text, int32 x, int32 y, Color color);

// Nuove funzioni per la rotazione
void graphics_rotate_point(int32* x, int32* y, int32 cx, int32 cy, int32 angle);
void graphics_draw_rotated_rectangle(GraphicsContext* ctx, int32 x, int32 y, int32 width, int32 height, int32 angle, Color color);
void graphics_draw_rotated_image(GraphicsContext* ctx, Image* img, int32 x, int32 y, int32 angle);

// Nuove funzioni per pattern e riempimenti
FillPattern* graphics_create_pattern(uint32* data, int32 width, int32 height);
void graphics_fill_with_pattern(GraphicsContext* ctx, int32 x, int32 y, int32 width, int32 height, FillPattern* pattern);
void graphics_set_pattern_opacity(FillPattern* pattern, uint8 opacity);

// Nuove funzioni per layer e buffer
void graphics_create_layer(GraphicsContext* ctx);
void graphics_set_active_layer(GraphicsContext* ctx, int32 layer_index);
void graphics_merge_layers(GraphicsContext* ctx);
void graphics_swap_buffers(GraphicsContext* ctx);

// Nuove funzioni per immagini
Image* graphics_create_image(int32 width, int32 height);
void graphics_draw_image(GraphicsContext* ctx, Image* img, int32 x, int32 y);
void graphics_scale_image(Image* src, Image* dst, int32 new_width, int32 new_height);

#endif