#include "../include/print_string.h"

#define MAX_INPUT_LENGTH 128
#define MAX_ARGS 16

void int_to_string(int num, char* str);

void echo(int argc, char* argv[]) {

    // Print each argument
    for (int i = 1; i < argc; i++) {  // Start from 1 to skip the command itself
        print_string(argv[i]);
        if (i < argc - 1) {
            print_string(" ");
        }
    }
    print_string("\n");
}