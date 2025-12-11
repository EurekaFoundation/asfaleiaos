#include "../include/texteditor.h"
#include "../include/window.h"
#include "../include/drivers.h"
#include "../include/print_string.h"
#include "../include/vga.h"
#include "../include/index.h"
#include "../include/filesystem.h"
#include "../include/fs_global.h"
#include "../include/color.h"
#include "../include/NULL.h"
#include "../include/io.h"
#include "../include/string.h"
#include "../include/memory.h"

#define CHARS_PER_LINE (EDITOR_WIDTH - 4)
#define MAX_LINES (EDITOR_HEIGHT - 4)
#define KEY_F1 240  // Special code for F1
#define KEY_F2 241  // Special code for F2

static char text_buffer[BUFFER_SIZE];
static int cursor_pos = 0;
static char current_filename[32] = {0};

void redraw_editor_window(void);
int fs_save(void);
int load_file(void);
int text_editor(void);

static void get_cursor_xy(int *screen_x, int *screen_y, int cursor_pos, int window_x, int window_y) {
    *screen_x = window_x + 2;
    *screen_y = window_y + 2;
    
    for (int i = 0; i < cursor_pos; i++) {
        if (text_buffer[i] == '\n') {
            *screen_x = window_x + 2;
            (*screen_y)++;
            if (*screen_y >= window_y + EDITOR_HEIGHT - 2) {
                (*screen_y)--;
            }
        } else if (text_buffer[i] >= 32 && text_buffer[i] <= 126) {
            (*screen_x)++;
            if (*screen_x >= window_x + 2 + CHARS_PER_LINE) {
                *screen_x = window_x + 2;
                (*screen_y)++;
                if (*screen_y >= window_y + EDITOR_HEIGHT - 2) {
                    (*screen_y)--;
                }
            }
        }
    }
}


// Implementazione della funzione di ridisegno
void redraw_editor_window(void) {
    Window editor_window;
    init_window(&editor_window, 10, 5, EDITOR_WIDTH, EDITOR_HEIGHT, "Text Editor");
    draw_window(&editor_window);
    
    int screen_x = editor_window.x + 2;
    int screen_y = editor_window.y + 2;
    
    // Pulisci area editing
    for (int y = editor_window.y + 2; y < editor_window.y + EDITOR_HEIGHT - 2; y++) {
        vga_set_cursor(editor_window.x + 2, y);
        for (int x = 0; x < CHARS_PER_LINE; x++) {
            print_char(' ');
        }
    }
    
    // Stampa contenuto
    vga_set_cursor(screen_x, screen_y);
    for (int j = 0; j < cursor_pos && j < BUFFER_SIZE; j++) {
        if (text_buffer[j] == '\n') {
            screen_x = editor_window.x + 2;
            screen_y++;
            if (screen_y >= editor_window.y + EDITOR_HEIGHT - 2) {
                break;
            }
            vga_set_cursor(screen_x, screen_y);
        } else if (text_buffer[j] >= 32 && text_buffer[j] <= 126) {
            print_char(text_buffer[j]);
            screen_x++;
            if (screen_x >= editor_window.x + 2 + CHARS_PER_LINE) {
                screen_x = editor_window.x + 2;
                screen_y++;
                if (screen_y >= editor_window.y + EDITOR_HEIGHT - 2) {
                    break;
                }
                vga_set_cursor(screen_x, screen_y);
            }
        }
    }
    
    // Barra di stato
    vga_set_cursor(editor_window.x + 2, editor_window.y + EDITOR_HEIGHT - 2);
    print_string("ESC:Exit  F1:Save  F2:Load");
}

int fs_save(void) {
    if (current_filename[0] == '\0') {
        vga_clear_screen();
        print_string("Enter filename: ");
        int i = 0;
        
        while (get_key() != 0);
        
        while (1) {
            int c = get_key();
            
            if (c == 0) continue;
            
            if (c == '\n') {
                if (i > 0) {
                    current_filename[i] = '\0';
                    break;
                }
                continue;
            }
            
            if (c == '\b' && i > 0) {
                i--;
                print_char('\b');
                print_char(' ');
                print_char('\b');
                continue;
            }
            
            if (c >= 32 && c <= 126 && i < 31) {
                current_filename[i] = c;
                current_filename[i + 1] = '\0';
                print_char(c);
                i++;
            }
        }
        print_char('\n');
    }

    if (current_filename[0] == '\0') {
        print_string("Error: No filename specified!\n");
        for (int i = 0; i < 100000; i++);
        return 1;
    }

    int save_size = (cursor_pos < BUFFER_SIZE) ? cursor_pos : BUFFER_SIZE - 1;

    // Elimina file se esiste
    fs_delete_file(&fs, current_filename);
    
    // Crea file nuovo
    if (fs_create_file(&fs, current_filename) != 0) {
        vga_clear_screen();
        print_string("Error: Cannot create file!\n");
        for (int i = 0; i < 100000; i++);
        return 1;
    }

    // Scrivi contenuto
    FileHandle* file = fs_open(&fs, current_filename, "w");
    if (!file) {
        vga_clear_screen();
        print_string("Error: Cannot open file for writing!\n");
        for (int i = 0; i < 100000; i++);
        return 1;
    }

    int write_result = fs_write_file(&fs, current_filename, text_buffer, save_size);
    fs_close(&fs, file);

    if (write_result < 0) {
        vga_clear_screen();
        print_string("Error: Write failed!\n");
        for (int i = 0; i < 100000; i++);
        return 1;
    }

    vga_clear_screen();
    print_string("File saved successfully!\n");
    for (int i = 0; i < 100000; i++);
    
    return 0;
}


int load_file(void) {
    vga_clear_screen();
    print_string("Enter filename to load: ");
    int i = 0;
    
    while (get_key() != 0);
    
    for (i = 0; i < 32; i++) {
        current_filename[i] = '\0';
    }
    
    i = 0;
    
    while (1) {
        int c = get_key();
        
        if (c == 0) continue;
        
        if (c == '\n') {
            if (i > 0) {
                current_filename[i] = '\0';
                break;
            }
            continue;
        }
        
        if (c == '\b' && i > 0) {
            i--;
            print_char('\b');
            print_char(' ');
            print_char('\b');
            continue;
        }
        
        if (c >= 32 && c <= 126 && i < 31) {
            current_filename[i] = c;
            print_char(c);
            i++;
        }
    }
    
    print_char('\n');

    if (current_filename[0] == '\0') {
        print_string("Error: No filename specified!\n");
        for (int i = 0; i < 100000; i++);
        return 1;
    }
    
    FileHandle* file = fs_open(&fs, current_filename, "r");
    if (!file) {
        print_string("Error: File not found!\n");
        for (int i = 0; i < 100000; i++);
        return 1;
    }
    
    char temp_buffer[BUFFER_SIZE];
    int bytes_read = fs_read_file(&fs, current_filename, temp_buffer, BUFFER_SIZE);
    
    if (bytes_read < 0) {
        print_string("Error reading file!\n");
        fs_close(&fs, file);
        for (int i = 0; i < 100000; i++);
        return 1;
    }
    
    // Pulisci buffer
    for (int j = 0; j < BUFFER_SIZE; j++) {
        text_buffer[j] = '\0';
    }
    
    // Copia contenuto
    for (int j = 0; j < bytes_read && j < BUFFER_SIZE - 1; j++) {
        text_buffer[j] = temp_buffer[j];
    }
    
    cursor_pos = (bytes_read < BUFFER_SIZE - 1) ? bytes_read : BUFFER_SIZE - 1;
    
    fs_close(&fs, file);
    print_string("File loaded successfully!\n");
    for (int i = 0; i < 100000; i++);
    
    return 0;
}

int text_editor(void) {
    Window editor_window;
    init_window(&editor_window, 10, 5, EDITOR_WIDTH, EDITOR_HEIGHT, "Text Editor");
    draw_window(&editor_window);
    
    // Pulisci buffer
    for (int i = 0; i < BUFFER_SIZE; i++) {
        text_buffer[i] = '\0';
    }
    
    // Reset filename per evitare che il file precedente venga riscritto
    for (int i = 0; i < 32; i++) {
        current_filename[i] = '\0';
    }
    
    cursor_pos = 0;
    
    // Barra di stato
    vga_set_cursor(editor_window.x + 2, editor_window.y + EDITOR_HEIGHT - 2);
    print_string("ESC:Exit  F1:Save  F2:Load");
    
    while (1) {
        int screen_x, screen_y;
        get_cursor_xy(&screen_x, &screen_y, cursor_pos, editor_window.x, editor_window.y);
        
        vga_set_cursor(screen_x, screen_y);
        print_char('_');
        
        int c = get_key();
        
        vga_set_cursor(screen_x, screen_y);
        print_char(' ');
        
        if (c == 27) { // ESC
            index();
        } else if (c == KEY_F1) { // F1: Save
            fs_save();
            redraw_editor_window();
        } else if (c == KEY_F2) { // F2: Load
            load_file();
            redraw_editor_window();
        } else if (c == '\n' && cursor_pos < BUFFER_SIZE - 1) {
            text_buffer[cursor_pos] = '\n';
            cursor_pos++;
        } else if (c == '\b' && cursor_pos > 0) {
            cursor_pos--;
            text_buffer[cursor_pos] = '\0';
        } else if (cursor_pos < BUFFER_SIZE - 1 && c >= 32 && c <= 126) {
            text_buffer[cursor_pos] = c;
            cursor_pos++;
            vga_set_cursor(screen_x, screen_y);
            print_char(c);
        }
    }
    return 0;
}