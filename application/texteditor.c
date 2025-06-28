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
#define KEY_F1 0x70
#define KEY_F2 0x71

static char text_buffer[BUFFER_SIZE];
static int cursor_pos = 0;
static char current_filename[32] = {0};

void redraw_editor_window(void);
int fs_save(void);
int load_file(void);
int text_editor(void);

// Implementazione della funzione di ridisegno
void redraw_editor_window(void) {
    Window editor_window;
    init_window(&editor_window, 10, 5, EDITOR_WIDTH, EDITOR_HEIGHT, "Text Editor");
    draw_window(&editor_window);
    
    // Ridisegna il contenuto
    int screen_x = editor_window.x + 2;
    int screen_y = editor_window.y + 2;
    
    // Pulisci l'area di editing
    for(int y = editor_window.y + 2; y < editor_window.y + EDITOR_HEIGHT - 2; y++) {
        vga_set_cursor(editor_window.x + 2, y);
        for(int x = 0; x < CHARS_PER_LINE; x++) {
            print_char(' ');
        }
    }
    
    // Stampa il contenuto
    vga_set_cursor(screen_x, screen_y);
    for(int j = 0; j < cursor_pos; j++) {
        if (text_buffer[j] == '\n') {
            screen_x = editor_window.x + 2;
            screen_y++;
            if(screen_y >= editor_window.y + EDITOR_HEIGHT - 2) {
                break;
            }
        }
        else if (text_buffer[j] >= 32 && text_buffer[j] <= 126) {
            print_char(text_buffer[j]);
            screen_x++;
            if(screen_x >= editor_window.x + 2 + CHARS_PER_LINE) {
                screen_x = editor_window.x + 2;
                screen_y++;
                if(screen_y >= editor_window.y + EDITOR_HEIGHT - 2) {
                    break;
                }
            }
        }
        vga_set_cursor(screen_x, screen_y);
    }
    
    // Ridisegna la barra di stato
    vga_set_cursor(editor_window.x + 2, editor_window.y + EDITOR_HEIGHT - 2);
    print_string("ESC:Exit  F1:Save  F2:Load");
}

int fs_save(void) {
    // Se non abbiamo un file corrente, chiedi il nome
    if (current_filename[0] == '\0') {
        print_string("Enter filename: ");
        int i = 0;
        
        // Svuota il buffer della tastiera
        while (get_key() != 0);
        
        // Leggi il nome del file
        while(1) {
            char c = get_key();
            
            if (c == 0 || c == KEY_F1) {
                continue;
            }
            
            if(c == '\n') {
                if (i > 0) {
                    current_filename[i] = '\0';
                    break;
                }
                continue;
            }
            
            if(c == '\b' && i > 0) {
                i--;
                print_char('\b');
                print_char(' ');
                print_char('\b');
                continue;
            }
            
            if(c >= 32 && c <= 126 && i < 31) {
                current_filename[i] = c;
                current_filename[i + 1] = '\0';
                print_char(c);
                i++;
            }
        }
        print_char('\n');
    }

    // Verifica nome file valido
    if (current_filename[0] == '\0') {
        print_string("\nError: No filename specified!\n");
        return 1;
    }

    // Calcola dimensione da salvare
    int save_size = (cursor_pos < BUFFER_SIZE) ? cursor_pos : BUFFER_SIZE - 1;

    // Prima prova a creare il file direttamente
    int create_result = fs_create_file(&fs, current_filename);
    
    // Se il file esiste già, prova a cancellarlo e ricrearlo
    if (create_result != 0) {
        if (fs_delete_file(&fs, current_filename) == 0) {
            // File cancellato con successo, prova a ricrearlo
            if (fs_create_file(&fs, current_filename) != 0) {
                print_string("\nError: Cannot create file after deletion!\n");
                return 1;
            }
        } else {
            print_string("\nError: Cannot create or modify file!\n");
            return 1;
        }
    }

    // A questo punto dovremmo avere un file nuovo, proviamo ad aprirlo
    FileHandle* file = fs_open(&fs, current_filename, "w");
    if (!file) {
        print_string("\nError: Cannot open file for writing!\n");
        return 1;
    }

    // Scrivi il contenuto
    int write_result = fs_write_file(&fs, current_filename, text_buffer, save_size);
    fs_close(&fs, file);

    if (write_result < 0) {
        print_string("\nError: Write failed!\n");
        return 1;
    }

    print_string("\nFile saved successfully!\n");
    return 0;

    // Nella parte finale di fs_save(), dopo il salvataggio:
    if (write_result < 0) {
        print_string("\nError: Write failed!\n");
        // Attendi un momento per mostrare l'errore
        for(int i = 0; i < 10000; i++);
        redraw_editor_window();
        return 1;
    }

    print_string("\nFile saved successfully!\n");
    // Attendi un momento per mostrare il messaggio di successo
    for(int i = 0; i < 10000; i++);
    redraw_editor_window();
    return 0;
}


int load_file(void) {
    print_string("Enter filename to load: ");
    int i = 0;
    
    // Svuota buffer tastiera
    while (get_key() != 0);
    
    // Pulisci nome file corrente
    for(i = 0; i < 32; i++) {
        current_filename[i] = '\0';
    }
    
    i = 0;
    
    // Leggi nome file
    while(1) {
        char c = get_key();
        
        if (c == 0 || c == 0x71) {
            continue;
        }
        
        if(c == '\n') {
            if (i > 0) {
                current_filename[i] = '\0';
                break;
            }
            continue;
        }
        
        if(c == '\b' && i > 0) {
            i--;
            print_char('\b');
            print_char(' ');
            print_char('\b');
            continue;
        }
        
        if(c >= 32 && c <= 126 && i < 31) {
            current_filename[i] = c;
            print_char(c);
            i++;
        }
    }
    
    print_char('\n');

    if (current_filename[0] == '\0') {
        print_string("\nError: No filename specified!\n");
        return 1;
    }
    
    FileHandle* file = fs_open(&fs, current_filename, "r");
    if (!file) {
        print_string("\nError: File not found!\n");
        return 1;
    }
    
    char temp_buffer[BUFFER_SIZE];
    int bytes_read = fs_read_file(&fs, current_filename, temp_buffer, BUFFER_SIZE);
    
    if (bytes_read < 0) {
        print_string("\nError reading file!\n");
        fs_close(&fs, file);
        return 1;
    }
    
    // Pulisci buffer corrente
    for(int j = 0; j < BUFFER_SIZE; j++) {
        text_buffer[j] = '\0';
    }
    
    // Copia contenuto
    for(int j = 0; j < bytes_read && j < BUFFER_SIZE - 1; j++) {
        text_buffer[j] = temp_buffer[j];
    }
    
    cursor_pos = (bytes_read < BUFFER_SIZE - 1) ? bytes_read : BUFFER_SIZE - 1;
    
    // Ridisegna editor
    Window editor_window;
    init_window(&editor_window, 10, 5, EDITOR_WIDTH, EDITOR_HEIGHT, "Text Editor");
    draw_window(&editor_window);
    
    int screen_x = editor_window.x + 2;
    int screen_y = editor_window.y + 2;
    
    // Pulisci area editing
    for(int y = editor_window.y + 2; y < editor_window.y + EDITOR_HEIGHT - 2; y++) {
        vga_set_cursor(editor_window.x + 2, y);
        for(int x = 0; x < CHARS_PER_LINE; x++) {
            print_char(' ');
        }
    }
    
    // Stampa contenuto
    vga_set_cursor(screen_x, screen_y);
    for(int j = 0; j < cursor_pos; j++) {
        if (text_buffer[j] == '\n') {
            screen_x = editor_window.x + 2;
            screen_y++;
            if(screen_y >= editor_window.y + EDITOR_HEIGHT - 2) {
                break;
            }
        }
        else if (text_buffer[j] >= 32 && text_buffer[j] <= 126) {
            print_char(text_buffer[j]);
            screen_x++;
            if(screen_x >= editor_window.x + 2 + CHARS_PER_LINE) {
                screen_x = editor_window.x + 2;
                screen_y++;
                if(screen_y >= editor_window.y + EDITOR_HEIGHT - 2) {
                    break;
                }
            }
        }
        vga_set_cursor(screen_x, screen_y);
    }
    
    // Ridisegna barra stato
    vga_set_cursor(editor_window.x + 2, editor_window.y + EDITOR_HEIGHT - 2);
    print_string("ESC:Exit  F1:Save  F2:Load");
    
    fs_close(&fs, file);
    print_string("\nFile loaded successfully!\n");
    return 0;
}

int text_editor(void) {
    Window editor_window;
    init_window(&editor_window, 10, 5, EDITOR_WIDTH, EDITOR_HEIGHT, "Text Editor");
    draw_window(&editor_window);
    
    // Clear buffer
    for(int i = 0; i < BUFFER_SIZE; i++) {
        text_buffer[i] = '\0';
    }
    
    // Status bar
    vga_set_cursor(editor_window.x + 2, editor_window.y + EDITOR_HEIGHT - 2);
    print_string("ESC:Exit  F1:Save  F2:Load");
    
    int screen_x = editor_window.x + 2;
    int screen_y = editor_window.y + 2;
    
    while(1) {
        vga_set_cursor(screen_x, screen_y);
        print_char('_');  // Cursor display
        
        char c = get_key();
        
        vga_set_cursor(screen_x, screen_y);
        print_char(' ');
        
        if(c == 27) { // ESC
            index();
        }
        else if(c == 0x70) {  // F1 key
            int old_x, old_y;
            vga_get_cursor(&old_x, &old_y);
            fs_save();
            vga_set_cursor(old_x, old_y);
        }
        else if(c == 0x71) {  // F2 key
            load_file();
        }
        else if(c == '\n' && cursor_pos < BUFFER_SIZE - 1) {
            text_buffer[cursor_pos] = '\n';
            cursor_pos++;
            screen_x = editor_window.x + 2;
            screen_y++;
            if(screen_y >= editor_window.y + EDITOR_HEIGHT - 2) {
                screen_y--;
            }
        }
        else if(c == '\b' && cursor_pos > 0) {
            cursor_pos--;
            text_buffer[cursor_pos] = '\0';
            if(screen_x > editor_window.x + 2) {
                screen_x--;
            } else if(screen_y > editor_window.y + 2) {
                screen_y--;
                screen_x = editor_window.x + 2 + CHARS_PER_LINE - 1;
            }
            vga_set_cursor(screen_x, screen_y);
            print_char(' ');
        }
        else if(cursor_pos < BUFFER_SIZE - 1 && c >= 32 && c <= 126) {
            text_buffer[cursor_pos] = c;
            cursor_pos++;
            vga_set_cursor(screen_x, screen_y);
            print_char(c);
            screen_x++;
            if(screen_x >= editor_window.x + 2 + CHARS_PER_LINE) {
                screen_x = editor_window.x + 2;
                screen_y++;
                if(screen_y >= editor_window.y + EDITOR_HEIGHT - 2) {
                    screen_y--;
                }
            }
        }
    }
    return 0;
}