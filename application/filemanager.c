#include "../include/filemanager.h"
#include "../include/window.h"
#include "../include/vga.h"
#include "../include/print_string.h"
#include "../include/color.h"
#include "../include/filesystem.h"
#include "../include/fs_global.h"
#include "../include/drivers.h"
#include "../include/string.h"
#include "../include/memory.h"
#include "../include/NULL.h"
#include "../include/index.h"
#include "../include/format_string.h"

#define FM_WIDTH 70
#define FM_HEIGHT 20
#define FM_X 5
#define FM_Y 2
#define MAX_VISIBLE_FILES 15
#define FILENAME_BUFFER 256

// Helper: simple directory listing into buffer
static int get_file_list(char* buffer, int buffer_size) {
    char temp[FILENAME_BUFFER];
    int ret = fs_list_files(&fs, temp, FILENAME_BUFFER);
    if (ret > 0 && ret < buffer_size) {
        strcpy(buffer, temp);
        return ret;
    }
    buffer[0] = '\0';
    return 0;
}

// Parse file list string into an array of filenames (separated by \n)
static int parse_file_list(const char* list, char** files, int max_files) {
    int count = 0;
    const char* p = list;
    
    while (*p && count < max_files) {
        // Skip leading whitespace
        while (*p && (*p == ' ' || *p == '\t')) p++;
        
        if (!*p) break;
        
        // Find end of line
        const char* start = p;
        while (*p && *p != '\n') p++;
        
        int len = p - start;
        if (len > 0) {
            // Trim trailing whitespace
            while (len > 0 && (start[len-1] == ' ' || start[len-1] == '\t')) {
                len--;
            }
            
            if (len > 0) {
                char* filename = my_malloc(len + 1);
                if (filename) {
                    strncpy(filename, start, len);
                    filename[len] = '\0';
                    files[count++] = filename;
                }
            }
        }
        
        if (*p == '\n') p++;
    }
    
    return count;
}

// Free allocated filenames
static void free_file_list(char** files, int count) {
    for (int i = 0; i < count; i++) {
        if (files[i]) my_free(files[i]);
    }
}

void redraw_filemanager(const char** files, int count, int selected, int scroll_offset) {
    // Clear content area
    for (int y = FM_Y + 1; y < FM_Y + FM_HEIGHT - 3; y++) {
        vga_set_cursor(FM_X + 1, y);
        for (int x = 0; x < FM_WIDTH - 2; x++) {
            print_char(' ');
        }
    }
    
    // Draw file list
    int visible_count = (count - scroll_offset < MAX_VISIBLE_FILES) ? 
                        count - scroll_offset : MAX_VISIBLE_FILES;
    
    for (int i = 0; i < visible_count; i++) {
        int file_idx = scroll_offset + i;
        if (file_idx >= count) break;
        
        int y = FM_Y + 2 + i;
        vga_set_cursor(FM_X + 2, y);
        
        if (file_idx == selected) {
            set_color(LIGHT_CYAN);
        } else {
            set_color(WHITE);
        }
        
        // Print filename with padding
        char display[FM_WIDTH - 4];
        int max_len = FM_WIDTH - 6;
        int name_len = strlen(files[file_idx]);
        
        if (name_len > max_len) {
            strncpy(display, files[file_idx], max_len - 3);
            strcpy(display + max_len - 3, "...");
        } else {
            strcpy(display, files[file_idx]);
        }
        
        print_string(display);
        
        // Pad with spaces
        int printed = strlen(display);
        for (int x = printed; x < FM_WIDTH - 4; x++) {
            print_char(' ');
        }
    }
    
    // Reset colors
    set_color(WHITE);
    //set_background_color(BLACK);
    
    // Draw status bar
    vga_set_cursor(FM_X + 2, FM_Y + FM_HEIGHT - 2);
    print_string("File ");
    print_int(selected + 1);
    print_string(" of ");
    print_int(count);
    print_string("  |  F1:Delete  F2:Rename  ESC:Back");
}

int filemanager(void) {
    // Initialize window
    Window fm_window;
    init_window(&fm_window, FM_X, FM_Y, FM_WIDTH, FM_HEIGHT, " File Manager ");
    
    vga_clear_screen();
    set_color(WHITE);
    draw_window(&fm_window);
    
    // Get file list
    char file_list_buffer[FILENAME_BUFFER];
    get_file_list(file_list_buffer, FILENAME_BUFFER);
    
    // Parse into array
    char** files = my_malloc(sizeof(char*) * 100);
    if (!files) {
        vga_clear_screen();
        print_string("Error: Memory allocation failed!\n");
        for (int i = 0; i < 100000; i++);
        return 1;
    }
    
    int file_count = parse_file_list(file_list_buffer, files, 100);
    
    if (file_count == 0) {
        vga_set_cursor(FM_X + 2, FM_Y + 3);
        set_color(LIGHT_GRAY);
        print_string("No files found.");
        
        vga_set_cursor(FM_X + 2, FM_Y + FM_HEIGHT - 2);
        print_string("ESC: Back");
        
        while (1) {
            int c = get_key();
            if (c == 27) {  // ESC
                my_free(files);
                vga_clear_screen();
                index();
                return 0;
            }
        }
    }
    
    // Main loop
    int selected = 0;
    int scroll_offset = 0;
    int running = 1;
    
    redraw_filemanager((const char**)files, file_count, selected, scroll_offset);
    
    while (running) {
        int c = get_key();
        
        if (c == 0) continue;  // No key pressed
        
        if (c == 27) {  // ESC - Back to menu
            free_file_list(files, file_count);
            my_free(files);
            vga_clear_screen();
            index();
            return 0;
        }
        else if (c == 224) {  // Extended key (arrow keys)
            int c2 = get_key();
            
            if (c2 == 72) {  // Up arrow
                if (selected > 0) {
                    selected--;
                    if (selected < scroll_offset) {
                        scroll_offset = selected;
                    }
                    redraw_filemanager((const char**)files, file_count, selected, scroll_offset);
                }
            }
            else if (c2 == 80) {  // Down arrow
                if (selected < file_count - 1) {
                    selected++;
                    if (selected >= scroll_offset + MAX_VISIBLE_FILES) {
                        scroll_offset = selected - MAX_VISIBLE_FILES + 1;
                    }
                    redraw_filemanager((const char**)files, file_count, selected, scroll_offset);
                }
            }
        }
        else if (c == 240) {  // F1 - Delete file
            if (file_count > 0) {
                vga_set_cursor(FM_X + 2, FM_Y + FM_HEIGHT - 3);
                set_color(LIGHT_RED);
                print_string("Delete '");
                print_string(files[selected]);
                print_string("'? (y/n)");
                set_color(WHITE);
                
                while (1) {
                    int response = get_key();
                    if (response == 'y' || response == 'Y') {
                        fs_delete_file(&fs, files[selected]);
                        
                        // Refresh file list
                        free_file_list(files, file_count);
                        get_file_list(file_list_buffer, FILENAME_BUFFER);
                        file_count = parse_file_list(file_list_buffer, files, 100);
                        
                        if (selected >= file_count) selected = file_count - 1;
                        scroll_offset = 0;
                        
                        vga_clear_screen();
                        draw_window(&fm_window);
                        redraw_filemanager((const char**)files, file_count, selected, scroll_offset);
                        break;
                    }
                    else if (response == 'n' || response == 'N') {
                        vga_clear_screen();
                        draw_window(&fm_window);
                        redraw_filemanager((const char**)files, file_count, selected, scroll_offset);
                        break;
                    }
                }
            }
        }
        else if (c == 241) {  // F2 - Rename (placeholder)
            vga_set_cursor(FM_X + 2, FM_Y + FM_HEIGHT - 3);
            set_color(LIGHT_GRAY);
            print_string("Rename not yet implemented.     ");
            set_color(WHITE);
            for (int i = 0; i < 500000; i++);
            vga_clear_screen();
            draw_window(&fm_window);
            redraw_filemanager((const char**)files, file_count, selected, scroll_offset);
        }
        else if (c == '\n' || c == '\r') {  // Enter - View/Edit file details (placeholder)
            vga_set_cursor(FM_X + 2, FM_Y + FM_HEIGHT - 3);
            set_color(LIGHT_BLUE);
            print_string("Open file: ");
            print_string(files[selected]);
            print_string("                      ");
            set_color(WHITE);
            for (int i = 0; i < 500000; i++);
            vga_clear_screen();
            draw_window(&fm_window);
            redraw_filemanager((const char**)files, file_count, selected, scroll_offset);
        }
    }
    
    return 0;
}
