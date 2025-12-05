#include "../include/print_string.h"
#include "../include/string.h"
#include "../include/drivers.h"
#include "../include/vga.h"
#include "../include/window.h"
#include "../include/color.h"
#include "../include/NULL.h"
#include "../include/math.h"
#include "../include/equation.h"
#include "../include/read_input.h"
#include "../include/scientificprograms.h"

int equation(){

    #if 0
    print_string("Quadrato di 5: ");
    print_double(quadrato(5));
    print_string("\n");
    print_string("Radice di 25: ");
    print_double(radice(25));  // Change this from print_string to print_double
    print_string("\n");
    print_string("Seno di 90: ");
    print_double(seno(9));
    print_string("\n");
    print_string("Coseno di 90: ");
    print_double(coseno(9));
    print_string("\n");
    #endif
    
    print_string("Risoluzione di un'equazione di secondo grado\n");

    float a, b, c, delta, x1, x2;
    print_string("Inserisci il coefficiente a: ");
    a = get_float();
    print_string("Inserisci il coefficiente b: ");
    b = get_float();
    print_string("Inserisci il coefficiente c: ");
    c = get_float();

    delta = quadrato(b) - 4 * a * c;

    if (delta < 0) {
        print_string("L'equazione non ha soluzioni reali\n");
    } else if (delta == 0) {
        x1 = -b / (2 * a);
        print_string("L'equazione ha una soluzione reale: ");
        print_double(x1);
        print_string("\n");
    } else {
        x1 = (-b + radice(delta)) / (2 * a);
        x2 = (-b - radice(delta)) / (2 * a);
        print_string("L'equazione ha due soluzioni reali: ");
        print_double(x1);
        print_string(" e ");
        print_double(x2);
        print_string("\n");
    }

    char input[256];

    print_string("Vuoi continuare o uscire?\n");
    print_string("Premi un tasto per continuare, o 'q' per uscire: ");
    read_input(input);
    if (input[0] == 'q' || input[0] == 'Q') {
        
    vga_clear_screen();
    scientificprograms();
     return 0;
    } else {
        equation();
    }
    return 0;
}