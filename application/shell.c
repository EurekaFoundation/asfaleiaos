#include "../include/kernel.h"
#include "../include/types.h"
#include "../include/drivers.h"
#include "../include/print_string.h"
#include "../include/echo.h"
#include "../include/shutdown.h"
#include "../include/help.h"
#include "../include/reboot.h"
#include "../include/calc.h"
#include "../include/date.h"
//#include "../include/clear_screen.h"
#include "../include/color.h"
#include "../include/vga.h"
#include "../include/NULL.h"
#include "../include/strchr.h"
#include "../include/string.h"
#include "../include/create_file.h"
#include "../include/write_file.h"
#include "../include/read_file.h"
#include "../include/delete_file.h"
#include "../include/list_files.h"
#include "../include/create_directory.h"
#include "../include/cd.h"
#include "../include/pwd.h"
#include "../include/delete_dir.h"
#include "../include/status_of_pc.h"
#include "../include/index.h"
#include "../include/fs_global.h"
#include "../include/read_input.h"
#include "../include/nuntius.h"


//#define MAX_INPUT_LENGTH 128  // Lunghezza massima dell'input
#define MAX_ARGS 16  // Numero massimo di argomenti
#define HISTORY_SIZE 10
#define MAX_COMMAND_LENGTH 256

// Add to top of file with other declarations
char command_history[HISTORY_SIZE][MAX_COMMAND_LENGTH];
int history_count = 0;
int current_history = -1;

void vga_set_cursor(int x, int y);
void vga_get_cursor(int* x, int* y);
void vga_update_cursor(void);

void vga_set_cursor(int x, int y);
void vga_get_cursor(int* x, int* y);
void vga_update_cursor(void);
void int_to_string(int num, char* str);
/*int my_strncmp(const char* s1, const char* s2, int n);
int my_strlen(const char* str);*/
void clear_input(char* buffer);
void read_input(char* buffer);
//void add_to_history(const char* command);
int app_echo(int argc, char* argv[]);
int app_shutdown(void);
int app_help(void);
int app_reboot(void);
int app_calc(int argc, char* argv[]);
int app_date(void);
int appcreate_file(int argc, char* argv[]);
int appwrite_file(int argc, char* argv[]);
int appread_file(int argc, char* argv[]);
int appdelete_file(int argc, char* argv[]);
int applist_files(int argc, char* argv[]);
int appcreate_directory(int argc, char* argv[]);
int appcd(int argc, char* argv[]);
int apppwd(int argc, char* argv[]);
int appdelete_dir(int argc, char* argv[]);
int app_index(void);
int app_nuntius(void);
int appstatus_of_pc(void);

int app_echo(int argc, char* argv[]) {
    echo(argc, argv);
    return 0;
}
int app_shutdown() {

    shutdown();
    return 0;
}
int app_help() {

    help();
    return 0;
}
int app_reboot() {

    reboot();
    return 0;
}
int app_calc(int argc, char* argv[]) {
    calculator(argc ,argv);
    return 0;
}
int app_date(){
    date();
    return 0;
}
int appcreate_file(int argc, char* argv[]){

    create_file(argc, argv);
    return 0;
}
int appwrite_file(int argc, char* argv[]){

    write_file(argc, argv);
    return 0;
}
int appread_file(int argc, char* argv[]){

    read_file(argc, argv);
    return 0;
}
int appdelete_file(int argc, char* argv[]){

    delete_file(argc, argv);
    return 0;
}
int applist_files(int argc, char* argv[]){

    list_files(argc, argv);
    return 0;
}
int appcreate_directory(int argc, char* argv[]){

    create_directory(argc, argv);
    return 0;
}
int appcd(int argc, char* argv[]){

    cd(argc, argv);
    return 0;
}
int apppwd(int argc, char* argv[]){

    pwd(argc, argv);
    return 0;
}
int appdelete_dir(int argc, char* argv[]){

    delete_dir(argc, argv);
    return 0;
}
int app_index(){
    index();
    return 0;
}
int appstatus_of_pc(void) {
    show_battery_status();
    return 0;
}
int app_nuntius(void) {
    nuntius();
    return 0;
}

struct command {
    const char* name;        
    void* handler;           // Generic function pointer  
    int has_args;           // Flag for functions with arguments
    const char* help;        
};

// Add command table
static const struct command commands[] = {
    {"echo", app_echo, 1, "Echo text to screen"},
    {"shutdown", app_shutdown, 0, "Shutdown the system"},
    {"clear", clear_screen, 0, "Clear the screen"},
    {"help", app_help, 0, "Show this help"},
    {"reboot", app_reboot, 0, "Reboot the system"},
    {"calc", app_calc, 1, "Calculator"},
    {"date", app_date, 0, "Show the current date and time"},
    {"create", appcreate_file, 1, "Create a file"},
    {"write", appwrite_file, 1, "Write to a file"},
    {"read", appread_file, 1, "Read from a file"},
    {"delete", appdelete_file, 1, "Delete a file"},
    {"ls", applist_files, 0, "List files"},
    {"mkdir", appcreate_directory, 1, "Create a directory"},
    {"cd", appcd, 1, "Change directory"},
    {"pwd", apppwd, 0, "Show current directory"},
    {"index", app_index, 0, "Show the index"},
    {"status", appstatus_of_pc, 0, "Show the status of the PC"},
    {"deldir", appdelete_dir, 1, "Delete a directory"},
    {"nuntius", app_nuntius, 0, "Chat with the AI"},
    {0, 0, 0, 0}
};

static int execute_command(char* input) {
    char* args[MAX_ARGS];
    int argc = 0;
    char* token;
    char* current = input;
    
    // Skip leading spaces
    while (*current == ' ') current++;
    
    // Parse input into tokens
    while (*current != '\0' && argc < MAX_ARGS) {
        // Start of a new token
        token = current;
        
        // Find the end of the token
        while (*current != '\0' && *current != ' ') current++;
        
        // Null-terminate the token
        if (*current != '\0') {
            *current = '\0';
            current++;
        }
        
        // Skip any additional spaces
        while (*current == ' ') current++;
        
        // Store the token
        args[argc++] = token;
    }
    
    if (argc == 0) {
        return 0;
    }

    // Execute command
    for (int i = 0; commands[i].name != 0; i++) {
        if (strncmp(args[0], commands[i].name, strlen(commands[i].name)) == 0) {
            if (commands[i].has_args) {
                return ((int(*)(int, char*[]))commands[i].handler)(argc, args);
            } else {
                if (argc > 1) {
                    print_string(" Command does not accept arguments\n");
                    return 1;
                }
                return ((int(*)(void))commands[i].handler)();
            }
        }
    }
    
    print_string(" Unknown command: ");
    print_string(args[0]);
    print_string("\n");
    return 1;
}


void int_to_string(int num, char* str) {
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
    
    // Reverse the string
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

// Funzione per resettare il buffer dell'input
void clear_input(char* buffer) {
    for (int i = 0; i < MAX_INPUT_LENGTH; i++) {
        buffer[i] = '\0';  // Resetta la stringa
    }
}

// Funzione per leggere l'input dell'utente
/*void read_input(char* buffer) {
    int i = 0;
    char c;
    int reading = 1;
    
    while(reading) {
        c = get_key();
        if (c != 0) {  // Verifica che ci sia effettivamente un carattere da leggere
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
                    print_char(' ');  // Cancella il carattere dallo schermo
                    print_char('\b');
                }
            }
            else if((c >= 32 && c < 127) || c == ' ') {  // Includi esplicitamente lo spazio
                if(i < MAX_INPUT_LENGTH - 1) {
                    buffer[i] = c;
                    print_char(c);
                    i++;
                }
            }
        }
    }
}*/

void shell() {

    int shell_x, shell_y;
    char input[MAX_INPUT_LENGTH];
    char current_path[1024];
    
    vga_clear_screen();
    
    set_color(YELLOW);
    print_string("            _____ ______      _      ______ _____             ____   _____   \\\n");
    print_string("     /\\    / ____|  ____/\\   | |    |  ____|_   _|   /\\      / __ \\ / ____|  \\\n");
    print_string("    /  \\  | (___ | |__ /  \\  | |    | |__    | |    /  \\    | |  | | (___     \\\n");
    print_string("   / /\\ \\  \\___ \\|  __/ /\\ \\ | |    |  __|   | |   / /\\ \\   | |  | |\\___ \\    \\\n");
    print_string("  / ____ \\ ____ | | / ____ \\| |____| |____ _| |_ / ____ \\  | |__| |____) |   \\\n");
    print_string(" /_/    \\_\\_____/|_|/_/    \\_\\______|______|____/_/    \\_\\  \\____/|_____/    \\\n");
    print_string("Asfaleia OS 0.0.1 Copyright Eureka Foundation (c) 2025\n");
    print_string("Type 'help' for a list of available commands or 'index' for the Desktop of Asfaleia\n");
    print_string("\n");

    vga_get_cursor(&shell_x, &shell_y);

    while(1) {
        vga_set_cursor(shell_x, shell_y);
        set_color(YELLOW);
        print_string(" ");
        //print_string(get_date_string());
        print_string(date());
        print_string("\n");
        print_string("root@asfaleia:");

        print_string(" ");

         // Get and display current path
        set_color(CYAN);
        fs_get_current_directory(&fs, current_path, sizeof(current_path));
        print_string(current_path);

        set_color(YELLOW);
        print_string(" $ ");
        
        read_input(input);
        execute_command(input);
        
        // Update shell position after command
        vga_get_cursor(&shell_x, &shell_y);
    }
}

void timer_handler() {
    static int ticks = 0;
    ticks++;
    if (ticks % 18 == 0) {
        clear_line(7);
        set_color(YELLOW);  // Set color for date
        //print_string(get_date_string());
        print_string(date());
        set_color(WHITE);   // Reset color
    }
}
