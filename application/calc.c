#include "../include/io.h"
#include "../include/print_string.h"
#include "../include/vga.h"
#include "../include/string.h"
#include "../include/NULL.h"
#include "../include/strchr.h"
#include "../include/read_input.h"
#include "../include/calc.h"
#include "../include/index.h"


void int_to_string(int num, char* str);

void int_to_str(int num, char* str) {
    int i = 0;
    int is_negative = 0;
    
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }
    
    do {
        str[i++] = num % 10 + '0';
        num = num / 10;
    } while (num > 0);
    
    if (is_negative) {
        str[i++] = '-';
    }
    str[i] = '\0';
    
    // Reverse string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void float_to_str(float num, char* str) {
    int intPart = (int)num;
    float decPart = num - intPart;
    int i = 0;
    
    // Handle integer part
    if (num < 0) {
        str[i++] = '-';
        intPart = -intPart;
        decPart = -decPart;
    }
    
    int_to_str(intPart, &str[i]);
    while(str[i]) i++;
    
    // Add decimal part
    str[i++] = '.';
    
    // Convert 2 decimal places
    decPart *= 100;
    int decimal = (int)decPart;
    if (decimal < 10) str[i++] = '0';
    int_to_str(decimal, &str[i]);
}

float str_to_float(const char* str) {
    float result = 0;
    float fraction = 1.0;
    int decimal = 0;
    int sign = 1;
    
    if (*str == '-') {
        sign = -1;
        str++;
    }
    
    while (*str) {
        if (*str == '.') {
            decimal = 1;
            str++;
            continue;
        }
        
        if (*str >= '0' && *str <= '9') {
            if (decimal) {
                fraction *= 0.1;
                result += (*str - '0') * fraction;
            } else {
                result = result * 10 + (*str - '0');
            }
        }
        str++;
    }
    
    return sign * result;
}

int calculator(int argc, char* argv[]) {
    (void)argv; // Suppress unused parameter warning
    
    if (argc > 1) {
        print_string("Usage: calc\n");
        print_string("Interactive calculator mode\n");
        print_string("Example: 2.0 + 3.0\n");
        return 1;
    }

    char input[256];
    char *token;
    float num1, num2, result;
    char op;
    char result_str[32];
    
    while(1) {
        print_string("Enter calculation (e.g. 2 + 3) or 'q' to quit: ");
        read_input(input);
        
        if (input[0] == 'q' || input[0] == 'Q') {
            
            index();
            return 0;
        }
        
        // Parse input
        token = strtok(input, " ");
        if (!token) continue;
        num1 = str_to_float(token);
        
        token = strtok(NULL, " ");
        if (!token) continue;
        op = token[0];
        
        token = strtok(NULL, " ");
        if (!token) continue;
        num2 = str_to_float(token);
        
        // Calculate result
        switch(op) {
            case '+': result = num1 + num2; break;
            case '-': result = num1 - num2; break;
            case '*': result = num1 * num2; break;
            case '/':
                if (num2 == 0) {
                    print_string("Error: Division by zero\n");
                    continue;
                }
                result = num1 / num2;
                break;
            default:
                print_string("Invalid operator. Use +, -, *, /\n");
                continue;
        }
        
        print_string("Result: ");
        float_to_str(result, result_str);
        print_string(result_str);
        print_string("\n\n");
    }
    
    return 0;
}

/*int calculator(int argc, char* argv[]) {
    
    if (argc != 4) {
        print_string("Usage: calc <number> <op> <number>\n");
        print_string("Example: calc 2.0 + 3.0\n");
        return 1;
    }
    
    float num1 = str_to_float(argv[1]);
    float num2 = str_to_float(argv[3]);
    char op = argv[2][0];
    float result = 0;
    char result_str[32];
    
    switch(op) {
        case '+':
            result = num1 + num2;
            break;
        case '-':
            result = num1 - num2;
            break;
        case '*':
            result = num1 * num2;
            break;
        case '/':
            if (num2 == 0) {
                print_string("Error: Division by zero\n");
                return 1;
            }
            result = num1 / num2;
            break;
        default:
            print_string("Invalid operator. Use +, -, *, /\n");
            return 1;
    }
    
    print_string(" ");
    float_to_str(result, result_str);
    print_string(result_str);
    print_string("\n");
    
    return 0;
}*/