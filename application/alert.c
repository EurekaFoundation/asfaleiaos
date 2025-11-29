#include "../include/print_string.h"
#include "../include/vga.h"
#include "../include/window.h"
#include "../include/color.h"
#include "../include/alert.h"
#include "../include/NULL.h"
#include "../include/drivers.h"
#include "../include/read_input.h"
#include "../include/index.h"

void wait_for_key1() {
    char c;
    while (1) {
        c = get_key();  // Check if key is pressed

        if (c != 0) {  // If key is pressed
            break;  // Exit loop
        }
    }
}

int alert() {
    
    
    // Clear screen
    vga_clear_screen();
    
    // Create alert window
    Window alert_window;
    init_window(&alert_window, 15, 10, 50, 7, "Alert");
    
    // Draw window border
    draw_window(&alert_window);
    
    // Display alert message
    set_color(RED);
    vga_set_cursor(17, 12);
    print_string("This program is under development!");
    
    vga_set_cursor(17, 14);
    print_string("Press any key to continue...");

    wait_for_key1();  // Wait for key press

    index();  // Return to main menu

    return 0;
}