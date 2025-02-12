#include "../include/index.h"
#include "../include/print_string.h"
#include "../include/vga.h"
#include "../include/window.h"
#include "../include/color.h"
#include "../include/filesystem.h"
#include "../include/fs_global.h"
#include "../include/NULL.h"
#include "../include/calc.h"
#include "../include/list_files.h"
#include "../include/drivers.h"
#include "../include/shell.h"
#include "../include/shutdown.h"
#include "../include/read_input.h"
#include "../include/reboot.h"
#include "../include/nuntius.h"


#define WINDOW_WIDTH 60
#define WINDOW_HEIGHT 15
#define WINDOW_X 10
#define WINDOW_Y 8

int index() {
    // Clear screen
    vga_clear_screen();
    
    // Create main window
    Window main_window;
    set_color(WHITE);
    init_window(&main_window, WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT, "AsfaleiaOS Main Menu");
    
    // Draw window border
    draw_window(&main_window);
    
    // Draw menu items
    set_color(WHITE);
    vga_set_cursor(WINDOW_X + 2, WINDOW_Y + 2);
    print_string("1. File Manager");
    
    vga_set_cursor(WINDOW_X + 2, WINDOW_Y + 3);
    print_string("2. Text Editor");
    
    vga_set_cursor(WINDOW_X + 2, WINDOW_Y + 4);
    print_string("3. Calculator");
    
    vga_set_cursor(WINDOW_X + 2, WINDOW_Y + 5);
    print_string("4. Terminal");

    vga_set_cursor(WINDOW_X + 2, WINDOW_Y + 6);
    print_string("5. Nuntius");
    
    vga_set_cursor(WINDOW_X + 2, WINDOW_Y + 7);
    print_string("6. Power Off");
    
    vga_set_cursor(WINDOW_X + 2, WINDOW_Y + 8);
    print_string("7. Reboot");
    
    // Draw status bar
    set_color(LIGHT_BLUE);
    vga_set_cursor(WINDOW_X + 2, WINDOW_Y + WINDOW_HEIGHT - 2);
    print_string("Use numbers 1-5 to select an option\n");

    // Handle user input
    char c;
    int running = 1;
    
    while(running) {
        c = get_key();
        switch(c) {
            case '1':
                // File Manager
                vga_clear_screen();
                list_files(0, NULL);
                running = 0;
                break;
                
            case '2':
                // Text Editor
                vga_clear_screen();
                // TODO: Add text editor
                running = 0;
                break;
                
            case '3':
                // Calculator
                vga_clear_screen();
                calculator(0, NULL);
                running = 0;
                break;
                
            case '4':
                // Terminal
                vga_clear_screen();
                shell();
                return 0;  // Return to shell
            
            case '5':
                // AI
                vga_clear_screen();
                nuntius(0, NULL);
                running = 0;
                break;

            case '6':
                // Power off
                vga_clear_screen();
                set_color(LIGHT_RED);
                print_string("Are you sure you want to power off? (y/n)\n");
                c = get_key();
                read_input(&c);
                if(c == 'y') {
                    shutdown();
                }
                else if (c == 'n' || c == '\n' || c == '\r') {
                    vga_clear_screen();
                    index();
                    return 0;
                }
                else {
                    vga_clear_screen();
                    index();
                    return 0;
                }
                break;
            case '7':
                    // Reboot
                    vga_clear_screen();
                    set_color(LIGHT_RED);
                    print_string("Are you sure you want to reboot? (y/n)\n");
                    c = get_key();
                    read_input(&c);
                    if(c == 'y') {
                        reboot();
                    }
                    else if (c == 'n' || c == '\n' || c == '\r') {
                        vga_clear_screen();
                        index();
                        return 0;
                    }
                    else {
                        vga_clear_screen();
                        index();
                        return 0;
                    }
                    break;
                
        }
    }
    
    return 0;
}