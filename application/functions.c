#include "../include/print_string.h"
#include "../include/vga.h"
#include "../include/window.h"
#include "../include/color.h"
#include "../include/read_input.h"
#include "../include/math.h"
#include "../include/drivers.h"
#include "../include/functions.h"
#include "../include/index.h"
#include "../include/read_input.h"
#include "../include/string.h"

#define PLOT_W 60
#define PLOT_H 20

static int read_line(char *buf, int max) {
    int i = 0;
    while (1) {
        int c = get_key();
        if (c == 0) continue;             // nessun tasto
        if (c == '\n' || c == '\r') {
            buf[i] = '\0';
            print_char('\n');
            return i;
        }
        if (c == '\b') {
            if (i > 0) {
                i--;
                print_char('\b'); print_char(' '); print_char('\b');
            }
            continue;
        }
        if (c >= 32 && c <= 126 && i < max - 1) {
            buf[i++] = c;
            buf[i] = '\0';
            print_char(c);
        }
    }
}


static void clear_buffer(char buf[PLOT_H][PLOT_W]) {
    for (int r = 0; r < PLOT_H; r++) for (int c = 0; c < PLOT_W; c++) buf[r][c] = ' ';
}

static void render_buffer(char buf[PLOT_H][PLOT_W]) {
    vga_clear_screen();
    for (int r = 0; r < PLOT_H; r++) {
        vga_set_cursor(0, r);
        for (int c = 0; c < PLOT_W; c++) {
            print_char(buf[r][c]);
        }
    }
}

static double eval_function(int type, double x, double a, double b, double c) {
    switch (type) {
        case 1: return a * x + b;                // linear ax + b
        case 2: return a * x * x + b * x + c;    // quadratic ax^2 + bx + c
        case 3: return seno(x);                  // sine (expects degrees)
        case 4: return coseno(x);                // cosine (expects degrees)
        case 5: return a * x * x + b * x + c;    // polynomial degree 2 reusing case 2
        default: return 0.0;
    }
}

void wait_for_key_functions() {
    char c;
    while (1) {
        c = get_key();  // Check if key is pressed

        if (c != 0) {  // If key is pressed
            break;  // Exit loop
        }
    }
}

/*int function_plotter(void) {
    print_string("Function plotter\n");
    print_string("Choose function type:\n");
    print_string("1) linear ax + b\n");
    print_string("2) quadratic ax^2 + bx + c\n");
    print_string("3) sine(x) (x in degrees)\n");
    print_string("4) cosine(x) (x in degrees)\n");
    print_string("5) custom quadratic\n");
    print_string("6) Exit to menu\n");
    print_string("Select (1-5): ");

    int choice = (int)get_float(); // input integer as float
    if (choice < 1 || choice > 6) {
        print_string("Invalid choice\n");
        return 1;
    }

    double a=0, b=0, c=0;
    if (choice == 1) {
        print_string("Enter a: "); a = get_float();
        print_string("Enter b: "); b = get_float();
    } else if (choice == 2 || choice == 5) {
        print_string("Enter a: "); a = get_float();
        print_string("Enter b: "); b = get_float();
        print_string("Enter c: "); c = get_float();
    } else if (choice == 3 || choice == 4) {
        print_string("Enter amplitude A (default 1): "); a = get_float();
        print_string("Enter frequency B (multiplier, default 1): "); b = get_float();
        print_string("Enter phase C (degrees, default 0): "); c = get_float();
    }else if (choice == 6) {
        index();
        return 0; // exit
    } else {
        print_string("No coefficients needed.\n");
    }

    print_string("Enter X min: "); double x_min = get_float();
    print_string("Enter X max: "); double x_max = get_float();
    print_string("Enter Y min: "); double y_min = get_float();
    print_string("Enter Y max: "); double y_max = get_float();

    if (x_max <= x_min || y_max <= y_min) {
        print_string("Invalid ranges\n");
        return 1;
    }

    char buf[PLOT_H][PLOT_W];
    clear_buffer(buf);

    // draw axes
    int x_axis = -1, y_axis = -1;
    if (y_min <= 0 && y_max >= 0) {
        x_axis = (int)((y_max - 0.0) / (y_max - y_min) * (PLOT_H - 1) + 0.5);
    }
    if (x_min <= 0 && x_max >= 0) {
        y_axis = (int)((0.0 - x_min) / (x_max - x_min) * (PLOT_W - 1) + 0.5);
    }
    if (x_axis >= 0) for (int c = 0; c < PLOT_W; c++) buf[x_axis][c] = '-';
    if (y_axis >= 0) for (int r = 0; r < PLOT_H; r++) buf[r][y_axis] = '|';
    if (x_axis >= 0 && y_axis >= 0) buf[x_axis][y_axis] = '+';

    // sample function
    for (int cx = 0; cx < PLOT_W; cx++) {
        double x = x_min + (double)cx * (x_max - x_min) / (PLOT_W - 1);
        double y = eval_function(choice, x, a, b, c);

        // If trig functions chosen, they expect degrees (we use seno/coseno)
        // Map y to row
        if (y >= y_min && y <= y_max) {
            double fy = (y_max - y) / (y_max - y_min) * (PLOT_H - 1);
            int row = (int)(fy + 0.5);
            if (row >= 0 && row < PLOT_H) {
                buf[row][cx] = '*';
            }
        }
    }

    // render
    render_buffer(buf);
    print_string("\nPress Enter to continue...");
    // consume enter
    wait_for_key_functions();

    function_plotter();

    return 0;
}*/
int function_plotter(void) {
    char tmp[64];

    print_string("Function plotter\n");
    print_string("Choose function type:\n");
    print_string("1) linear ax + b\n");
    print_string("2) quadratic ax^2 + bx + c\n");
    print_string("3) sine(x) (x in degrees)  A*sin(B*x + C)\n");
    print_string("4) cosine(x) (x in degrees) A*cos(B*x + C)\n");
    print_string("5) custom quadratic\n");
    print_string("6) Exit to menu\n");
    print_string("Select (1-6): ");

    if (read_line(tmp, sizeof(tmp)) == 0) {
        print_string("No selection\n");
        return 1;
    }
    int choice = (int)string_to_double(tmp);
    if (choice < 1 || choice > 6) {
        print_string("Invalid choice\n");
        return 1;
    }
    if (choice == 6) {
        index();
        return 0;
    }

    double A = 0.0, B = 0.0, C = 0.0;

    if (choice == 1) {
        print_string("Enter a: ");
        if (read_line(tmp, sizeof(tmp)) == 0) { print_string("Missing coefficient a\n"); return 1; }
        A = string_to_double(tmp);
        print_string("Enter b: ");
        if (read_line(tmp, sizeof(tmp)) == 0) { print_string("Missing coefficient b\n"); return 1; }
        B = string_to_double(tmp);
    } else if (choice == 2 || choice == 5) {
        print_string("Enter a: ");
        if (read_line(tmp, sizeof(tmp)) == 0) { print_string("Missing coefficient a\n"); return 1; }
        A = string_to_double(tmp);
        print_string("Enter b: ");
        if (read_line(tmp, sizeof(tmp)) == 0) { print_string("Missing coefficient b\n"); return 1; }
        B = string_to_double(tmp);
        print_string("Enter c: ");
        if (read_line(tmp, sizeof(tmp)) == 0) { print_string("Missing coefficient c\n"); return 1; }
        C = string_to_double(tmp);
    } else if (choice == 3 || choice == 4) {
        print_string("Enter amplitude A (default 1): ");
        if (read_line(tmp, sizeof(tmp)) == 0) A = 1.0; else A = string_to_double(tmp);
        print_string("Enter frequency B (multiplier, default 1): ");
        if (read_line(tmp, sizeof(tmp)) == 0) B = 1.0; else B = string_to_double(tmp);
        print_string("Enter phase C (degrees, default 0): ");
        if (read_line(tmp, sizeof(tmp)) == 0) C = 0.0; else C = string_to_double(tmp);
    }

    print_string("Enter X min: ");
    if (read_line(tmp, sizeof(tmp)) == 0) { print_string("Missing X min\n"); return 1; }
    double x_min = string_to_double(tmp);
    print_string("Enter X max: ");
    if (read_line(tmp, sizeof(tmp)) == 0) { print_string("Missing X max\n"); return 1; }
    double x_max = string_to_double(tmp);
    print_string("Enter Y min: ");
    if (read_line(tmp, sizeof(tmp)) == 0) { print_string("Missing Y min\n"); return 1; }
    double y_min = string_to_double(tmp);
    print_string("Enter Y max: ");
    if (read_line(tmp, sizeof(tmp)) == 0) { print_string("Missing Y max\n"); return 1; }
    double y_max = string_to_double(tmp);

    if (x_max <= x_min || y_max <= y_min) {
        print_string("Invalid ranges\n");
        return 1;
    }

    char buf[PLOT_H][PLOT_W];
    clear_buffer(buf);

    /* draw axes if inside ranges */
    int x_axis = -1, y_axis = -1;
    if (y_min <= 0.0 && y_max >= 0.0) {
        /* row where y = 0 (0 = top) */
        double t = (y_max - 0.0) / (y_max - y_min);
        x_axis = (int)(t * (PLOT_H - 1) + 0.5);
    }
    if (x_min <= 0.0 && x_max >= 0.0) {
        double t = (0.0 - x_min) / (x_max - x_min);
        y_axis = (int)(t * (PLOT_W - 1) + 0.5);
    }

    if (x_axis >= 0 && x_axis < PLOT_H) for (int cc = 0; cc < PLOT_W; cc++) buf[x_axis][cc] = '-';
    if (y_axis >= 0 && y_axis < PLOT_W) for (int rr = 0; rr < PLOT_H; rr++) buf[rr][y_axis] = '|';
    if (x_axis >= 0 && y_axis >= 0 && x_axis < PLOT_H && y_axis < PLOT_W) buf[x_axis][y_axis] = '+';

    /* sample function */
    for (int cx = 0; cx < PLOT_W; cx++) {
        double x = x_min + (double)cx * (x_max - x_min) / (PLOT_W - 1);
        double y = eval_function(choice, x, A, B, C);

        if (y >= y_min && y <= y_max) {
            double fy = (y_max - y) / (y_max - y_min) * (PLOT_H - 1);
            int row = (int)(fy + 0.5);
            if (row >= 0 && row < PLOT_H) {
                buf[row][cx] = '*';
            }
        }
    }

    render_buffer(buf);

    print_string("\nPress Enter to continue...");
    wait_for_key_functions();

    /* ritorna al chiamante (menu) */
    index();
    return 0;
}