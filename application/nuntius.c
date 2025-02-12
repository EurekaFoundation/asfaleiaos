#include "../include/io.h"
#include "../include/print_string.h"
#include "../include/vga.h"
#include "../include/string.h"
#include "../include/NULL.h"
#include "../include/strchr.h"
#include "../include/read_input.h"
#include "../include/calc.h"
#include "../include/index.h"
#include "../include/color.h"
#include "../include/shell.h"
#include "../include/nuntius.h"
#include "../include/drivers.h"
#include "../include/kernel.h"
#include "../include/types.h"
#include "../include/echo.h"
#include "../include/shutdown.h"
#include "../include/help.h"
#include "../include/reboot.h"
#include "../include/calc.h"
#include "../include/date.h"
#include "../include/clear_screen.h"
#include "../include/NULL.h"
#include "../include/strchr.h"
#include "../include/index.h"
#include "../include/format_string.h"


#define MAX_ARGS 16

int appai_index(void){
    index();
    return 0;
}
int appai_desktop(void){
    
    index();
    return 0;
}
int appai_ciao(void){
    shutdown();
    return 0;
}

static char user_name[100] = "";
static const char* temp_name = NULL;

int set_user_name_helper(const char* name) {
    strcpy(user_name, name);
    return 1;
}

int set_user_name(void) {
    if (temp_name) {
        set_user_name_helper(temp_name);
        temp_name = NULL;
        return 1;
    }
    return 0;
}

// Add these structures at the top of the file
struct chat_response {
    const char* trigger;
    const char* response;
    int (*action)(void);  // Optional function to execute
};

// Custom case-insensitive string search function
static char* str_case_search(const char* haystack, const char* needle) {
    for (; *haystack; haystack++) {
        const char* h = haystack;
        const char* n = needle;
        while (*h && *n && (
            (*h == *n) || 
            (*h + 32 == *n) || 
            (*h == *n + 32)
        )) {
            h++;
            n++;
        }
        if (!*n) return (char*)haystack;
    }
    return NULL;
}

static const struct chat_response chat_responses[] = {
    {"hello", "Hello! How can I help you? \n", NULL},
    {"hi", "Hi there! What can I do for you?\n", NULL},
    {"hey", "Hey! What can I do for you?\n", NULL},
    {"how are you", "I'm fine, thank you\n", NULL},
    //{"help", "\n", NULL},
    {"what is your name", "I am Nuntius! Nice to meet you! \n", NULL},
    {"thanks", "You're welcome!\n", NULL},
    {"my name is", "Nice to meet you, %s! How can I assist you? \n", set_user_name},
    {"bye", "Goodbye! Have a great day!", appai_desktop},
    {NULL, NULL, NULL}
};

static char response_buffer[256];

static int handle_chat(const char* input) {
    for (int i = 0; chat_responses[i].trigger != NULL; i++) {
        if (str_case_search(input, chat_responses[i].trigger) != NULL) {
            if (strchr(chat_responses[i].response, '%')) {
                const char* name = input + strlen("my name is ");
                format_string(response_buffer, chat_responses[i].response, name);
                print_string(response_buffer);
            } else {
                print_string(chat_responses[i].response);
            }
            if (chat_responses[i].action != NULL) {
                return chat_responses[i].action();
            }
            return 1;
        }
    }
    print_string("I'm not sure how to respond to that. \n");
    return 0;
}

    struct commandai {
    const char* name;        
    void* handler;           // Generic function pointer  
    int has_args;           // Flag for functions with arguments
    const char* help;        
};

/*static const struct commandai commandsai[] = {
    {"exit", appai_desktop, 1, "Exit the AI"},
    {"ciao", appai_ciao, 0, "Shutdown the os"},
    {"chat", handle_chat, 1, "Chat with the AI"},
    {0, 0, 0, 0}
};*/



int execute_command_ai(char* input) {
    char* args[MAX_ARGS];
    int argc = 0;
    char* token;
    char* current = input;
    
    // Skip leading spaces
    while (*current == ' ') current++;
    
    // Parse input into tokens
    while (*current != '\0' && argc < MAX_ARGS) {
        token = current;
        while (*current != '\0' && *current != ' ') current++;
        
        if (*current != '\0') {
            *current = '\0';
            current++;
        }
        while (*current == ' ') current++;
        args[argc++] = token;
    }
    
    if (argc == 0) return 0;

    // Check for chat command
    if (strcmp(args[0], "ave") == 0) {
        if (argc < 2) {
            print_string("Please provide a message to chat\n");
            return 1;
        }
        // Reconstruct message from remaining tokens
        char* message = args[1];
        for (int i = 2; i < argc; i++) {
            *(args[i] - 1) = ' ';  // Restore spaces between words
        }
        return handle_chat(message);
    }

    // Execute regular commands
    /*for (int i = 0; commandsai[i].name != 0; i++) {
        if (strncmp(args[0], commandsai[i].name, strlen(commandsai[i].name)) == 0) {
            if (commandsai[i].has_args) {
                return ((int(*)(int, char*[]))commandsai[i].handler)(argc, args);
            } else {
                if (argc > 1) {
                    print_string("Command does not accept arguments\n");
                    return 1;
                }
                return ((int(*)(void))commandsai[i].handler)();
            }
        }
    }*/
    
    print_string("Unknown command\n");
    return 1;
}

int app_exit(/*int argc, char* argv[]*/){
    return 0;
}

int app_ciao(){
    shutdown();
    return 0;
}


int nuntius(/*int argc, char* argv[]**/){

char input[MAX_INPUT_LENGTH];
    vga_clear_screen();
    #if 0
    vga_set_color(LIGHT_RED);
    print_string("AI is not implemented yet\n");
    print_string("Press any key to return to the shell\n");
    char c = get_key();
    read_input(&c);
    if (c == 'c') {
        vga_clear_screen();
        index();
        return 0;
    }
    else if (c == 'r') {
        vga_clear_screen();
        index();
        return 0;
    }
    #endif
    
    set_color(LIGHT_CYAN);
    print_string("AI is in BETA Version 0.0.1\n");
    print_string("Type 'ave and a question' to chat with the AI\n");
    print_string("Example: 'ave hello' \n");
    print_string("Type 'ave bye' to return to the desktop\n");

    while(1) {
        print_string("AI: ");
        read_input(input);
        execute_command_ai(input);
    }


    return 0;
}