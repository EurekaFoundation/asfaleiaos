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
#define KEY_F1 240  // Special code for F1 (non-ASCII value)
#define KEY_F2 241  // Special code for F2 (non-ASCII value)


volatile char key_buffer[256];
int buffer_index = 0;
int prompt_start_x = 0;  // Position X when the prompt starts
int prompt_start_y = 0;  // Position Y when the prompt starts
static int shift_pressed = 0;  // Track SHIFT key state
static int ctrl_pressed = 0;   // Track CTRL key state

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
int get_key() {
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
// Complete scancode table for Italian QWERTY keyboard with SHIFT support
char scancode_to_ascii(unsigned char scancode) {
    // Gestione tasti SHIFT
    if (scancode == 0x2A || scancode == 0x36) {  // LEFT SHIFT or RIGHT SHIFT
        shift_pressed = 1;
        return 0;
    }
    
    // Gestione tasti CTRL
    if (scancode == 0x1D) {  // LEFT CTRL
        ctrl_pressed = 1;
        return 0;
    }
    
    // Se il tasto è rilasciato (scancode > 0x80), gestisco il rilascio
    if (scancode > 0x80) {
        unsigned char key_released = scancode & 0x7F;
        if (key_released == 0x2A || key_released == 0x36) {
            shift_pressed = 0;
        }
        if (key_released == 0x1D) {
            ctrl_pressed = 0;
        }
        return 0;
    }
    
    switch(scancode) {
        // Row 1 - Number row
        case 0x01: return 27;       // ESC
        
        // Numbers with and without SHIFT
        case 0x02: return shift_pressed ? '!' : '1';      // 1 !
        case 0x03: return shift_pressed ? '"' : '2';      // 2 "
        case 0x04: return shift_pressed ? 156 : '3';      // 3 £ (156 is £ in codepage)
        case 0x05: return shift_pressed ? '$' : '4';      // 4 $
        case 0x06: return shift_pressed ? '%' : '5';      // 5 %
        case 0x07: return shift_pressed ? '&' : '6';      // 6 &
        case 0x08: return shift_pressed ? '/' : '7';      // 7 /
        case 0x09: return shift_pressed ? '(' : '8';      // 8 (
        case 0x0A: return shift_pressed ? ')' : '9';      // 9 )
        case 0x0B: return shift_pressed ? '=' : '0';      // 0 =
        case 0x0C: return shift_pressed ? '?' : '\'';     // ' ?
        case 0x0D: return shift_pressed ? '^' : '`';      // ` ^
        case BACKSPACE_SCANCODE: return '\b'; // Backspace
        
        // Row 2 - QWERTY row (letters uppercase with SHIFT)
        case 0x0F: return '\t';     // TAB
        case 0x10: return shift_pressed ? 'Q' : 'q';      // Q
        case 0x11: return shift_pressed ? 'W' : 'w';      // W
        case 0x12: return shift_pressed ? 'E' : 'e';      // E
        case 0x13: return shift_pressed ? 'R' : 'r';      // R
        case 0x14: return shift_pressed ? 'T' : 't';      // T
        case 0x15: return shift_pressed ? 'Y' : 'y';      // Y
        case 0x16: return shift_pressed ? 'U' : 'u';      // U
        case 0x17: return shift_pressed ? 'I' : 'i';      // I
        case 0x18: return shift_pressed ? 'O' : 'o';      // O
        case 0x19: return shift_pressed ? 'P' : 'p';      // P
        case 0x1A: return shift_pressed ? '[' : '\xE8';      // è [ (Latin-1 0xE8)
        case 0x1B: return shift_pressed ? ']' : '+';      // + ]
        
        // Row 3 - ASDFGH row (letters uppercase with SHIFT)
        case 0x1E: return shift_pressed ? 'A' : 'a';      // A
        case 0x1F: return shift_pressed ? 'S' : 's';      // S
        case 0x20: return shift_pressed ? 'D' : 'd';      // D
        case 0x21: return shift_pressed ? 'F' : 'f';      // F
        case 0x22: return shift_pressed ? 'G' : 'g';      // G
        case 0x23: return shift_pressed ? 'H' : 'h';      // H
        case 0x24: return shift_pressed ? 'J' : 'j';      // J
        case 0x25: return shift_pressed ? 'K' : 'k';      // K
        case 0x26: return shift_pressed ? 'L' : 'l';      // L
        case 0x27: return shift_pressed ? ':' : '\xF2';      // ò : (Latin-1 0xF2)
        case 0x28: return shift_pressed ? '"' : '\xE0';      // à " (Latin-1 0xE0)
        case 0x29: return shift_pressed ? '|' : '\\';     // \ |
        case ENTER_SCANCODE: return '\n'; // Enter
        
        // Row 4 - ZXCVBN row (letters uppercase with SHIFT)
        case 0x2C: return shift_pressed ? 'Z' : 'z';      // Z
        case 0x2D: return shift_pressed ? 'X' : 'x';      // X
        case 0x2E: return shift_pressed ? 'C' : 'c';      // C
        case 0x2F: return shift_pressed ? 'V' : 'v';      // V
        case 0x30: return shift_pressed ? 'B' : 'b';      // B
        case 0x31: return shift_pressed ? 'N' : 'n';      // N
        case 0x32: return shift_pressed ? 'M' : 'm';      // M
        case 0x33: return shift_pressed ? '<' : ',';      // , <
        case 0x34: return shift_pressed ? '>' : '.';      // . >
        case 0x35: return shift_pressed ? '?' : '/';      // / ?
        
        // Space and numpad
        case 0x39: return ' ';      // Space
        
        // Numpad
        case 0x4A: return '-';      // Numpad -
        case 0x4E: return '+';      // Numpad +
        case 0x37: return '*';      // Numpad *
        case 0x47: return '7';      // Numpad 7
        case 0x48: return '8';      // Numpad 8
        case 0x49: return '9';      // Numpad 9
        case 0x4B: return '4';      // Numpad 4
        case 0x4C: return '5';      // Numpad 5
        case 0x4D: return '6';      // Numpad 6
        case 0x4F: return '1';      // Numpad 1
        case 0x50: return '2';      // Numpad 2
        case 0x51: return '3';      // Numpad 3
        case 0x52: return '0';      // Numpad 0
        case 0x53: return '.';      // Numpad .
        
        // Function keys (handled separately in get_key)
        case 0x3B: return KEY_F1;   // F1
        case 0x3C: return KEY_F2;   // F2
        case 0x3D: return 243;      // F3
        case 0x3E: return 244;      // F4
        case 0x3F: return 245;      // F5
        case 0x40: return 246;      // F6
        case 0x41: return 247;      // F7
        case 0x42: return 248;      // F8
        case 0x43: return 249;      // F9
        case 0x44: return 250;      // F10
        case 0x57: return 251;      // F11
        case 0x58: return 252;      // F12
        
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