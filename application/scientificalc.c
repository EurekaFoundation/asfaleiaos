#include "../include/print_string.h"
#include "../include/string.h"
#include "../include/drivers.h"
#include "../include/vga.h"
#include "../include/window.h"
#include "../include/color.h"
#include "../include/scientific.h"
#include "../include/index.h"
#include "../include/read_input.h"

int scientificalc(/*int args, char* argv[]*/) {
    
    #if 0
    // Create window
    Window window;
    init_window(&window, 10, 5, 60, 20, "Scientific Calculator");
    draw_window(&window);
    #endif
    vga_clear_screen();
    set_color(WHITE);
    char input[256];
    print_string("Scientific Calculator\n\n");
    print_string("This feature is not yet implemented.\n\n");
    print_string("Press any key to return to the main menu.\n");
    print_string("Type 'q' to quit\n");
    print_string("Type 'help' for a list of commands.\n");
    while(1){
    print_string("\n> ");
    read_input(input);

    if(input[0] == 'q' && input[1] == 'u' && input[2] == 'i' && input[3] == 't'){
       index();
       return 0;
    }else if(strncmp(input, "help", 4) == 0){
        print_string("Commands:\n");
        print_string("quit - return to the main menu\n");
        print_string("help - display this message\n");
     
        continue;
    }
    else{
        print_string("Invalid command. Type 'help' for a list of commands.\n");
    }
}
    return 0;
}