#include "../include/index.h"
#include "../include/print_string.h"
#include "../include/vga.h"
#include "../include/window.h"
#include "../include/color.h"
#include "../include/filesystem.h"
#include "../include/fs_global.h"
#include "../include/NULL.h"
#include "../include/calc.h"
#include "../include/drivers.h"
#include "../include/shell.h"
#include "../include/shutdown.h"
#include "../include/read_input.h"
#include "../include/scientific.h"
#include "../include/alert.h"
#include "../include/scientificprograms.h"

#define WINDOW_WIDTH 60
#define WINDOW_HEIGHT 15
#define WINDOW_X 10
#define WINDOW_Y 9

int scientificprograms() {
    // Clear screen
    vga_clear_screen();
    
   /* set_color(WHITE);
    print_string("            _____ ______      _      ______ _____             ____   _____   \\\n");
    print_string("     /\\    / ____|  ____/\\   | |    |  ____|_   _|   /\\      / __ \\ / ____|  \\\n");
    print_string("    /  \\  | (___ | |__ /  \\  | |    | |__    | |    /  \\    | |  | | (___     \\\n");
    print_string("   / /\\ \\  \\___ \\|  __/ /\\ \\ | |    |  __|   | |   / /\\ \\   | |  | |\\___ \\    \\\n");
    print_string("  / ____ \\ ____ | | / ____ \\| |____| |____ _| |_ / ____ \\  | |__| |____) |   \\\n");
    print_string(" /_/    \\_\\_____/|_|/_/    \\_\\______|______|____/_/    \\_\\  \\____/|_____/    \\\n");
    print_string("Asfaleia OS 0.0.1 Copyright Eureka Foundation (c) 2025\n");*/


    // Create main window
    Window main_window;
    set_color(WHITE);
    init_window(&main_window, WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT, "Math & Physics Programs Menu");
    
    // Draw window border
    draw_window(&main_window);
    
    // Draw menu items
    set_color(WHITE);
    vga_set_cursor(WINDOW_X + 2, WINDOW_Y + 2);
    print_string("1. Scientific Calculator");
    
    vga_set_cursor(WINDOW_X + 2, WINDOW_Y + 3);
    print_string("2. ");
    
    vga_set_cursor(WINDOW_X + 2, WINDOW_Y + 4);
    print_string("3. ");
    
    vga_set_cursor(WINDOW_X + 2, WINDOW_Y + 5);
    print_string("4. ");

    vga_set_cursor(WINDOW_X + 2, WINDOW_Y + 6);
    print_string("5. ");

    //vga_set_cursor(WINDOW_X + 2, WINDOW_Y + 7);
    //print_string("6. Scientific Calculator");
    
    /*vga_set_cursor(WINDOW_X + 2, WINDOW_Y + 8);
    print_string("7. Power Off");
    
    vga_set_cursor(WINDOW_X + 2, WINDOW_Y + 9);
    print_string("8. Reboot");*/

    vga_set_cursor(WINDOW_X + 2, WINDOW_Y + 10);
    print_string("9. Index Menu");
    
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
                // Scientific Calculator
                vga_clear_screen();
                //list_files(0, NULL);
                scientificalc();
                running = 0;
                break;
                
            case '2':
                
                break;
                
            case '3':
                
                break;
                
            case '4':
                
                return 0;  // Return to shell
            
            case '5':
                
                break;
            case '6':
                
                break;

            case '7':
                
                break;
            case '8':
                    
                    break;
            case '9':
                //index
                vga_clear_screen();
                index();
                running = 0;
                break;
                
        }
    }
    
    return 0;
}