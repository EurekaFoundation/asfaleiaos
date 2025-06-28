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

float get_float(void) {
    char buffer[32];
    int i = 0;
    float result = 0;
    float decimal = 0;
    int decimal_places = 0;
    int is_negative = 0;
    
    while(1) {
        char c = get_key();
        
        if(c == '\n') {
            buffer[i] = '\0';
            break;
        }
        
        if(c == '-' && i == 0) {
            is_negative = 1;
            print_char(c);
            buffer[i++] = c;
            continue;
        }
        
        if(c == '.' && decimal_places == 0) {
            print_char(c);
            buffer[i++] = c;
            decimal_places = 1;
            continue;
        }
        
        if(c >= '0' && c <= '9') {
            print_char(c);
            buffer[i++] = c;
            if(i >= 31) break;
        }
    }
    
    print_char('\n');
    
    // Convert string to float
    i = 0;
    if(buffer[0] == '-') i++;
    
    while(buffer[i] != '\0' && buffer[i] != '.') {
        result = result * 10 + (buffer[i] - '0');
        i++;
    }
    
    if(buffer[i] == '.') {
        i++;
        float multiplier = 0.1;
        while(buffer[i] != '\0') {
            decimal += (buffer[i] - '0') * multiplier;
            multiplier *= 0.1;
            i++;
        }
    }
    
    result += decimal;
    if(is_negative) result = -result;
    
    return result;
}