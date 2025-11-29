#include "../include/print_string.h"
#include "../include/string.h"
#include "../include/drivers.h"
#include "../include/vga.h"
#include "../include/window.h"
#include "../include/color.h"
#include "../include/scientific.h"
#include "../include/index.h"
#include "../include/read_input.h"
#include "../include/math.h"

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
    print_string("This calculator works in degrees.\n");
    print_string("Type 'quit' to quit\n");
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
        print_string("radice <number> - calculate the square root of a number\n");
        print_string("quadrato <number> - calculate the square of a number\n");
        print_string("seno <number> - calculate the sine of a number (in degrees)\n");
        print_string("coseno <number> - calculate the cosine of a number (in degrees)\n");
        continue;
    }else if(strncmp(input, "radice ", 7) == 0) {
    double valore = string_to_double(input + 7); // Converte la parte dopo "radice "
    double risultato = radice(valore);
    print_string("Risultato: ");
    if (risultato < 0) {
        print_string("Errore: radice di un numero negativo non definita.\n");
    }
    print_double(risultato);
    print_string("\n");
    } else if(strncmp(input, "quadrato ", 9) == 0) {
        double valore = string_to_double(input + 9); // Converte la parte dopo "quadrato "
        double risultato = quadrato(valore);
        print_string("Risultato: ");
        print_double(risultato);
        print_string("\n");
    } else if(strncmp(input, "seno ", 5) == 0) {
        double valore = string_to_double(input + 5); // Converte la parte dopo "seno "
        double risultato = seno(valore);
        print_string("Risultato: ");
        print_double(risultato);
        print_string("\n");
    } else if(strncmp(input, "coseno ", 7) == 0) {
        double valore = string_to_double(input + 7); // Converte la parte dopo "coseno "
        double risultato = coseno(valore);
        print_string("Risultato: ");
        print_double(risultato);
        print_string("\n");
    }
    else{
        print_string("Invalid command. Type 'help' for a list of commands.\n");
    }
}
    return 0;
}