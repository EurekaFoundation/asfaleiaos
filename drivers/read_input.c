#include "../include/read_input.h"
#include "../include/drivers.h"
#include "../include/print_string.h"

void read_input(char* buffer) {
    int i = 0;
    char c;
    int reading = 1;
    
    while(reading) {
        c = get_key();
        if (c != 0) {
            if(c == '\n') {
                buffer[i] = '\0';
                print_char('\n');
                reading = 0;
            }
            else if(c == '\b') {
                if(i > 0) {
                    i--;
                    buffer[i] = '\0';
                    print_char('\b');
                    print_char(' ');
                    print_char('\b');
                }
            }
            else if((c >= 32 && c < 127) || c == ' ') {
                if(i < MAX_INPUT_LENGTH - 1) {
                    buffer[i] = c;
                    print_char(c);
                    i++;
                }
            }
        }
    }
}