#ifndef WINDOW_H
#define WINDOW_H

#include "vga.h"

#define MAX_TITLE_LENGTH 32

typedef struct {
    int x;
    int y;
    int width;
    int height;
    char title[MAX_TITLE_LENGTH];
} Window;

void init_window(Window* window, int x, int y, int width, int height, const char* title);
void draw_window(Window* window);

#endif