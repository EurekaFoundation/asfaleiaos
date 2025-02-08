// desktop.c
/*#include "../include/desktop.h"
#include "../include/window.h"  // Includi la libreria delle finestre
#include "../include/vga.h"  // Includi la libreria VGA per la gestione della memoria video

// Funzione per disegnare il desktop con più finestre
void draw_desktop(void) {
    vga_set_color(0x07); // Colore del testo bianco sullo sfondo nero

    // Definisci alcune finestre con colori migliorati
    Window win1 = {10, 5, 50, 20, 0x00, 0x0F, "Finestra 1"};  // Testo bianco su sfondo nero
    Window win2 = {30, 10, 60, 25, 0x07, 0x00, "Finestra 2"};  // Testo nero su sfondo bianco

    // Se non vuoi usare il titolo, metti NULL come campo titolo
    // Window win1 = {10, 5, 50, 20, 0x00, 0x0F, NULL};  // Testo bianco su sfondo nero
    // Window win2 = {30, 10, 60, 25, 0x07, 0x00, NULL};  // Testo nero su sfondo bianco

    // Usa un colore di background più visibile per la finestra
    draw_window_with_background(0, 0, VGA_WIDTH, VGA_HEIGHT, 0x70); // Grigio chiaro come background principale

    // Disegna le finestre sul desktop
    draw_window(&win1);
    draw_window(&win2);
}*/
