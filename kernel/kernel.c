// src/kernel/kernel.c
#include "../include/kernel.h"
#include "../include/types.h"
#include "../include/print_string.h"
#include "../include/clear_screen.h"
#include "../include/drivers.h"
#include "../include/interrupt.h"
#include "../include/vga.h"
#include "../include/filesystem.h"
#include "../include/fs_global.h"
#include "../include/index.h"
#include "../include/hardware.h"
#include "../include/power.h"

FileSystem fs;

/*void test_filesystem() {
    char buffer[128];

    // Create a file
    if (fs_create_file(&fs, "test.txt") == 0) {
        print_string("File 'test.txt' created with success\n");
    }

    // Write in the file
    if (fs_write_file(&fs, "test.txt", "Hello, World!", 13) == 0) {
        print_string("Writed file 'test.txt' with success\n");
    }

    // Read the file
    if (fs_read_file(&fs, "test.txt", buffer, sizeof(buffer)) > 0) {
        print_string("Content of file 'test.txt': ");
        print_string(buffer);
        print_string("\n");
    }

    // Delete the file
    if (fs_delete_file(&fs, "test.txt") == 0) {
        print_string("File 'test.txt' deleted\n");
    }
}*/

void wait_for_key() {
    char c;
    while (1) {
        c = get_key();  // Check if key is pressed

        if (c != 0) {  // If key is pressed
            break;  // Exit loop
        }
    }
}

void start_index() {
    print_string("Welcome to AsfaleiaOS!\n");
    print_string("\nPress Any Keys for start...\n");

    wait_for_key();  // Wait for key press

    index();  // Start main menu
}

void handle_power_events(power_event_t event) {
    switch(event) {
        case POWER_EVENT_BUTTON:
            print_string("Power button pressed\n");
            break;
            
        case POWER_EVENT_SLEEP:
            print_string("System going to sleep\n");
            set_power_state(POWER_STATE_STANDBY);
            break;
            
        case POWER_EVENT_BATTERY_LOW:
            print_string("Warning: Battery low!\n");
            break;
            
        case POWER_EVENT_THERMAL:
            if(get_thermal_state() == THERMAL_CRITICAL) {
                print_string("Warning: CPU temperature critical!\n");
                set_fan_speed(255); // Max speed
            }
            break;
    }
}

void kernel_main() {
    vga_init();
    fs_init(&fs);
    clear_screen();
    
    // Initialize timer before shell
    init_timer(18);
    register_timer_handler(timer_handler);

    init_power_management();

    register_power_event_handler(handle_power_events);

    // Check initial battery state
    if(is_battery_present()) {
        if(get_battery_level() < 20) {
            print_string("Warning: Low battery!\n");
        }
    }
    // Start main menu
    start_index();
    
    while(1){
        halt_cpu();
    }
}
