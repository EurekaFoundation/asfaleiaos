#include "../include/io.h"
#include "../include/drivers.h"
#include "../include/print_string.h"
#include "../include/vga.h"

#define KEYBOARD_PORT 0x60
#define BACKSPACE_SCANCODE 0x0E
#define ENTER_SCANCODE 0x1C
#define PROMPT_LENGTH 3  // Length of the '-->' prompt
#define EXTENDED_KEY 0xE0
#define UP_ARROW 0x48
#define DOWN_ARROW 0x50
#define KEY_F1 0x70  // Special code for F1
#define KEY_F2 0x71  // Special code for F2


volatile char key_buffer[256];
int buffer_index = 0;
int prompt_start_x = 0;  // Position X when the prompt starts
int prompt_start_y = 0;  // Position Y when the prompt starts

// Function for saving the prompt position
void save_prompt_position() {
    vga_get_cursor(&prompt_start_x, &prompt_start_y);
}

// Function for verifying if the cursor is after the prompt
int is_after_prompt() {
    int current_x, current_y;
    vga_get_cursor(&current_x, &current_y);
    
    if (current_y > prompt_start_y) return 1;
    if (current_y == prompt_start_y && current_x > prompt_start_x + PROMPT_LENGTH) return 1;
    return 0;
}

// Function for gestire the backspace
void handle_backspace() {
    if (buffer_index > 0) {
        buffer_index--;
        print_char('\b');
        print_char(' ');
        print_char('\b');
    }
}

// Function of gestire the enter
void keyboard_handler() {
    unsigned char scancode = inb(KEYBOARD_PORT);
    char ascii;

    switch(scancode) {
        case BACKSPACE_SCANCODE:
            if (buffer_index > 0) {
                buffer_index--;
                vga_putchar('\b');
                vga_putchar(' ');
                vga_putchar('\b');
                vga_update_cursor();  // Update the cursor position
            }
            break;

        case ENTER_SCANCODE:
            vga_putchar('\n');
            buffer_index = 0;
            vga_update_cursor();  // Update the cursor position
            break;

        default:
            ascii = scancode_to_ascii(scancode);
            if (ascii != 0) {
                vga_putchar(ascii);
                key_buffer[buffer_index++] = ascii;
                if (buffer_index >= 256) {
                    buffer_index = 0;
                }
                vga_update_cursor();  // Update the cursor position
            }
            break;
    }
}

// Function for reading the key
char get_key() {
    static unsigned char last_scancode = 0;
    unsigned char scancode = read_keyboard_scancode();
    
    if (scancode == 0) {
        return 0;
    }
    
    // Gestione speciale per F1 e F2
    if (scancode == 0x3B) {
        last_scancode = scancode;
        return KEY_F1;
    }
    if (scancode == 0x3C) {
        last_scancode = scancode;
        return KEY_F2;
    }
    
    if (scancode == last_scancode) {
        return 0;
    }
    last_scancode = scancode;
    return scancode_to_ascii(scancode);
}

// Function for reading the scancode
unsigned char read_keyboard_scancode() {
    return inb(KEYBOARD_PORT);
}

// Function for converting the scancode to ascii
char scancode_to_ascii(unsigned char scancode) {
    switch(scancode) {
        case 0x29: return '\\';
        case 0x0F: return '\t';
        case 0x02: return '1';
        case 0x03: return '2';
        case 0x04: return '3';
        case 0x05: return '4';
        case 0x06: return '5';
        case 0x07: return '6';
        case 0x08: return '7';
        case 0x09: return '8';
        case 0x0A: return '9';
        case 0x0B: return '0';
        case 0x0C: return '?';
        case 0x4A: return '-';
        case 0x35: return '/';
        case 0x37: return '*';
        case 0x4E: return '+';
        case 0x47: return '7';
        case 0x48: return '8';
        case 0x49: return '9';
        case 0x4B: return '4';
        case 0x4C: return '5';
        case 0x4D: return '6';
        case 0x4F: return '1';
        case 0x50: return '2';
        case 0x51: return '3';
        case 0x52: return '0';
        case 0x53: return '.';
        case 0x56: return '>';
        case 0x33: return ',';
        case 0x34: return '.';
        case 0x0D: return 'i';
        case 0x10: return 'q';
        case 0x11: return 'w';
        case 0x12: return 'e';
        case 0x13: return 'r';
        case 0x14: return 't';
        case 0x15: return 'y';
        case 0x16: return 'u';
        case 0x17: return 'i';
        case 0x18: return 'o';
        case 0x19: return 'p';
        case 0x1E: return 'a';
        case 0x1F: return 's';
        case 0x20: return 'd';
        case 0x21: return 'f';
        case 0x22: return 'g';
        case 0x23: return 'h';
        case 0x24: return 'j';
        case 0x25: return 'k';
        case 0x26: return 'l';
        case 0x2C: return 'z';
        case 0x2D: return 'x';
        case 0x2E: return 'c';
        case 0x2F: return 'v';
        case 0x30: return 'b';
        case 0x31: return 'n';
        case 0x32: return 'm';
        case 0x01: return 27;
        case 0x3B: return 0x70;
        case 0x3C: return KEY_F2;
        case 0x39: return ' '; // Space
        case BACKSPACE_SCANCODE: return '\b'; // Backspace
        case ENTER_SCANCODE: return '\n'; // Enter
        default: return 0;
    }
}

#if 0
#define KEYBOARD_PORT 0x60
#define KEY_F1 0x3B
#define KEY_F2 0x3C
#define BUFFER_SIZE 256

static unsigned char key_buffer[BUFFER_SIZE];
static int buffer_index = 0;

// Scancode lookup table
static const char scancode_map[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' '
};

void byte_to_hex(unsigned char byte, char* buffer) {
    const char hex_chars[] = "0123456789ABCDEF";
    buffer[0] = hex_chars[(byte >> 4) & 0x0F];
    buffer[1] = hex_chars[byte & 0x0F];
    buffer[2] = '\0';
}

void keyboard_handler() {
    unsigned char scancode = inb(KEYBOARD_PORT);
    
    // Debug output
    char debug[3];
    byte_to_hex(scancode, debug);
    print_string("Scancode: 0x");
    print_string(debug);
    print_string("\n");
    
    // Handle function keys
    if(scancode == KEY_F1 || scancode == KEY_F2) {
        key_buffer[buffer_index++] = scancode;
        if (buffer_index >= BUFFER_SIZE) {
            buffer_index = 0;
        }
        return;
    }
    
    // Handle regular keys
    if(scancode < 128) {
        char ascii = scancode_map[scancode];
        if(ascii) {
            key_buffer[buffer_index++] = ascii;
            if (buffer_index >= BUFFER_SIZE) {
                buffer_index = 0;
            }
            vga_putchar(ascii);
            vga_update_cursor();
        }
    }
}

char get_key() {
    if(buffer_index > 0) {
        return key_buffer[--buffer_index];
    }
    return 0;
}

#endif