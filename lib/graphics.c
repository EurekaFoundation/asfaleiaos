#if 0

#include "../include/graphics.h"

// Implementazione di abs senza stdlib.h
static int32 abs_val(int32 x) {
    return x < 0 ? -x : x;
}


const int32 sin_lookup[360] = {
    0, 18, 36, 54, 71, 89, 107, 125, 143, 160, 178, 195, 213, 230, 248, 265,
    282, 299, 316, 333, 350, 367, 384, 400, 416, 433, 449, 465, 481, 496, 512, 527,
    543, 558, 573, 587, 602, 616, 630, 644, 658, 672, 685, 698, 711, 724, 737, 749,
    761, 773, 784, 796, 807, 818, 828, 839, 849, 859, 868, 878, 887, 896, 904, 912,
    920, 928, 935, 943, 949, 956, 962, 968, 974, 979, 984, 989, 994, 998, 1002, 1005,
    1008, 1011, 1014, 1016, 1018, 1020, 1022, 1023, 1023, 1024, 1024, 1024, 1023, 1023, 1022, 1020,
    1018, 1016, 1014, 1011, 1008, 1005, 1002, 998, 994, 989, 984, 979, 974, 968, 962, 956,
    949, 943, 935, 928, 920, 912, 904, 896, 887, 878, 868, 859, 849, 839, 828, 818,
    807, 796, 784, 773, 761, 749, 737, 724, 711, 698, 685, 672, 658, 644, 630, 616,
    602, 587, 573, 558, 543, 527, 512, 496, 481, 465, 449, 433, 416, 400, 384, 367,
    350, 333, 316, 299, 282, 265, 248, 230, 213, 195, 178, 160, 143, 125, 107, 89,
    71, 54, 36, 18, 0, -18, -36, -54, -71, -89, -107, -125, -143, -160, -178, -195,
    -213, -230, -248, -265, -282, -299, -316, -333, -350, -367, -384, -400, -416, -433, -449, -465,
    -481, -496, -512, -527, -543, -558, -573, -587, -602, -616, -630, -644, -658, -672, -685, -698,
    -711, -724, -737, -749, -761, -773, -784, -796, -807, -818, -828, -839, -849, -859, -868, -878,
    -887, -896, -904, -912, -920, -928, -935, -943, -949, -956, -962, -968, -974, -979, -984, -989,
    -994, -998, -1002, -1005, -1008, -1011, -1014, -1016, -1018, -1020, -1022, -1023, -1023, -1024, -1024, -1024,
    -1023, -1023, -1022, -1020, -1018, -1016, -1014, -1011, -1008, -1005, -1002, -998, -994, -989, -984, -979,
    -974, -968, -962, -956, -949, -943, -935, -928, -920, -912, -904, -896, -887, -878, -868, -859,
    -849, -839, -828, -818, -807, -796, -784, -773, -761, -749, -737, -724, -711, -698, -685, -672,
    -658, -644, -630, -616, -602, -587, -573, -558, -543, -527, -512, -496, -481, -465, -449, -433,
    -416, -400, -384, -367, -350, -333, -316, -299, -282, -265, -248, -230, -213, -195, -178, -160,
    -143, -125, -107, -89, -71, -54, -36, -18
};

const int32 cos_lookup[360] = {
    1024, 1024, 1023, 1023, 1022, 1020, 1018, 1016, 1014, 1011, 1008, 1005, 1002, 998, 994, 989,
    984, 979, 974, 968, 962, 956, 949, 943, 935, 928, 920, 912, 904, 896, 887, 878,
    868, 859, 849, 839, 828, 818, 807, 796, 784, 773, 761, 749, 737, 724, 711, 698,
    685, 672, 658, 644, 630, 616, 602, 587, 573, 558, 543, 527, 512, 496, 481, 465,
    449, 433, 416, 400, 384, 367, 350, 333, 316, 299, 282, 265, 248, 230, 213, 195,
    178, 160, 143, 125, 107, 89, 71, 54, 36, 18, 0, -18, -36, -54, -71, -89,
    -107, -125, -143, -160, -178, -195, -213, -230, -248, -265, -282, -299, -316, -333, -350, -367,
    -384, -400, -416, -433, -449, -465, -481, -496, -512, -527, -543, -558, -573, -587, -602, -616,
    -630, -644, -658, -672, -685, -698, -711, -724, -737, -749, -761, -773, -784, -796, -807, -818,
    -828, -839, -849, -859, -868, -878, -887, -896, -904, -912, -920, -928, -935, -943, -949, -956,
    -962, -968, -974, -979, -984, -989, -994, -998, -1002, -1005, -1008, -1011, -1014, -1016, -1018, -1020,
    -1022, -1023, -1023, -1024, -1024, -1024, -1023, -1023, -1022, -1020, -1018, -1016, -1014, -1011, -1008, -1005,
    -1002, -998, -994, -989, -984, -979, -974, -968, -962, -956, -949, -943, -935, -928, -920, -912,
    -904, -896, -887, -878, -868, -859, -849, -839, -828, -818, -807, -796, -784, -773, -761, -749,
    -737, -724, -711, -698, -685, -672, -658, -644, -630, -616, -602, -587, -573, -558, -543, -527,
    -512, -496, -481, -465, -449, -433, -416, -400, -384, -367, -350, -333, -316, -299, -282, -265,
    -248, -230, -213, -195, -178, -160, -143, -125, -107, -89, -71, -54, -36, -18, 0, 18,
    36, 54, 71, 89, 107, 125, 143, 160, 178, 195, 213, 230, 248, 265, 282, 299,
    316, 333, 350, 367, 384, 400, 416, 433, 449, 465, 481, 496, 512, 527, 543, 558,
    573, 587, 602, 616, 630, 644, 658, 672, 685, 698, 711, 724, 737, 749, 761, 773,
    784, 796, 807, 818, 828, 839, 849, 859, 868, 878, 887, 896, 904, 912, 920, 928,
    935, 943, 949, 956, 962, 968, 974, 979, 984, 989, 994, 998, 1002, 1005, 1008, 1011,
    1014, 1016, 1018, 1020, 1022, 1023, 1023, 1024
};

GraphicsContext* graphics_init(unsigned char* fb_addr, int32 width, int32 height, int32 bpp) {
    static GraphicsContext ctx;
    ctx.framebuffer = fb_addr;
    ctx.width = width;
    ctx.height = height;
    ctx.bpp = bpp;
    return &ctx;
}

static unsigned char color_to_vga(Color color) {
    // Convert RGB to closest VGA palette index
    return color.r & 0x0F; // Use red component as palette index
}

void graphics_clear(GraphicsContext* ctx, Color color) {
    unsigned char color_value = color.r & 0x0F;
    for (int32 i = 0; i < ctx->width * ctx->height; i++) {
        ctx->framebuffer[i] = color_value;
    }
}

void graphics_put_pixel(GraphicsContext* ctx, int32 x, int32 y, Color color) {
    if (x < 0 || x >= ctx->width || y < 0 || y >= ctx->height) return;
    ((unsigned char*)ctx->framebuffer)[y * ctx->width + x] = color_to_vga(color);
}
void graphics_draw_line(GraphicsContext* ctx, int32 x1, int32 y1, int32 x2, int32 y2, Color color) {
    int32 dx = abs_val(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int32 dy = -abs_val(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int32 err = dx + dy, e2;

    while (1) {
        graphics_put_pixel(ctx, x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
}

void graphics_draw_rectangle(GraphicsContext* ctx, int32 x, int32 y, int32 width, int32 height, Color color) {
    graphics_draw_line(ctx, x, y, x + width - 1, y, color);
    graphics_draw_line(ctx, x + width - 1, y, x + width - 1, y + height - 1, color);
    graphics_draw_line(ctx, x + width - 1, y + height - 1, x, y + height - 1, color);
    graphics_draw_line(ctx, x, y + height - 1, x, y, color);
}

void graphics_draw_rotated_rectangle(GraphicsContext* ctx, int32 x, int32 y, 
                                   int32 width, int32 height, int32 angle, Color color) {
    // Calcola i quattro vertici del rettangolo
    int32 x1 = x, y1 = y;                          // Top-left
    int32 x2 = x + width, y2 = y;                  // Top-right
    int32 x3 = x + width, y3 = y + height;         // Bottom-right
    int32 x4 = x, y4 = y + height;                 // Bottom-left
    
    // Calcola il centro del rettangolo
    int32 cx = x + width / 2;
    int32 cy = y + height / 2;
    
    // Ruota ogni vertice attorno al centro
    graphics_rotate_point(&x1, &y1, cx, cy, angle);
    graphics_rotate_point(&x2, &y2, cx, cy, angle);
    graphics_rotate_point(&x3, &y3, cx, cy, angle);
    graphics_rotate_point(&x4, &y4, cx, cy, angle);
    
    // Disegna le linee tra i vertici ruotati
    graphics_draw_line(ctx, x1, y1, x2, y2, color);
    graphics_draw_line(ctx, x2, y2, x3, y3, color);
    graphics_draw_line(ctx, x3, y3, x4, y4, color);
    graphics_draw_line(ctx, x4, y4, x1, y1, color);
}

void graphics_fill_rectangle(GraphicsContext* ctx, int32 x, int32 y, int32 width, int32 height, Color color) {
    int32 i, j;
    for (i = y; i < y + height; i++) {
        for (j = x; j < x + width; j++) {
            graphics_put_pixel(ctx, j, i, color);
        }
    }
}

void graphics_draw_circle(GraphicsContext* ctx, int32 x0, int32 y0, int32 radius, Color color) {
    int32 x = radius;
    int32 y = 0;
    int32 err = 0;

    while (x >= y) {
        graphics_put_pixel(ctx, x0 + x, y0 + y, color);
        graphics_put_pixel(ctx, x0 + y, y0 + x, color);
        graphics_put_pixel(ctx, x0 - y, y0 + x, color);
        graphics_put_pixel(ctx, x0 - x, y0 + y, color);
        graphics_put_pixel(ctx, x0 - x, y0 - y, color);
        graphics_put_pixel(ctx, x0 - y, y0 - x, color);
        graphics_put_pixel(ctx, x0 + y, y0 - x, color);
        graphics_put_pixel(ctx, x0 + x, y0 - y, color);

        if (err <= 0) {
            y += 1;
            err += 2*y + 1;
        }
        if (err > 0) {
            x -= 1;
            err -= 2*x + 1;
        }
    }
}

void graphics_fill_circle(GraphicsContext* ctx, int32 x0, int32 y0, int32 radius, Color color) {
    int32 x, y;
    for (y = -radius; y <= radius; y++) {
        for (x = -radius; x <= radius; x++) {
            if (x*x + y*y <= radius*radius) {
                graphics_put_pixel(ctx, x0 + x, y0 + y, color);
            }
        }
    }
}

void graphics_draw_char(GraphicsContext* ctx, char c, int32 x, int32 y, Color color) {
    if (!ctx->current_font) return;
    
    Glyph* glyph = &ctx->current_font->glyphs[c - 32]; // Assumiamo ASCII
    int32 i, j;
    
    for (i = 0; i < glyph->height; i++) {
        for (j = 0; j < glyph->width; j++) {
            if (glyph->data[i * glyph->width + j]) {
                graphics_put_pixel(ctx, x + j, y + i, color);
            }
        }
    }
}

void graphics_draw_text(GraphicsContext* ctx, const char* text, int32 x, int32 y, Color color) {
    int32 cursor_x = x;
    while (*text) {
        graphics_draw_char(ctx, *text, cursor_x, y, color);
        cursor_x += ctx->current_font->glyphs[*text - 32].width + 1;
        text++;
    }
}

// Implementazione rotazione base
void graphics_rotate_point(int32* x, int32* y, int32 cx, int32 cy, int32 angle) {
    // Implementazione semplificata usando tabella di seni/coseni pre-calcolata
    int32 dx = *x - cx;
    int32 dy = *y - cy;
    
    // Usa lookup table per sin/cos
    int32 cos_a = cos_lookup[angle % 360];
    int32 sin_a = sin_lookup[angle % 360];
    
    *x = cx + (dx * cos_a - dy * sin_a) / 1024; // Scala fissa 1024
    *y = cy + (dx * sin_a + dy * cos_a) / 1024;
}

// Implementazione layer e buffer
void graphics_swap_buffers(GraphicsContext* ctx) {
    uint32* temp = ctx->framebuffer;
    ctx->framebuffer = ctx->backbuffer;
    ctx->backbuffer = temp;
    
    // Copia il contenuto del backbuffer nel framebuffer
    int32 size = ctx->width * ctx->height;
    int32 i;
    for (i = 0; i < size; i++) {
        ctx->framebuffer[i] = ctx->backbuffer[i];
    }
}

// Implementazione pattern
void graphics_fill_with_pattern(GraphicsContext* ctx, int32 x, int32 y, int32 width, int32 height, FillPattern* pattern) {
    int32 i, j;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            uint32 pattern_pixel = pattern->pattern[(i % pattern->height) * pattern->width + (j % pattern->width)];
            graphics_put_pixel(ctx, x + j, y + i, *(Color*)&pattern_pixel);
        }
    }
}

#endif