#include "../include/print_string.h"
#include "../include/filesystem.h"
#include "../include/pwd.h"
#include "../include/fs_global.h"

int pwd(int argc, char* argv[]) {
    (void)argc;  // Evita il warning per il parametro non utilizzato
    (void)argv;  // Evita il warning per il parametro non utilizzato

    char buffer[1024];
    fs_get_current_directory(&fs, buffer, sizeof(buffer));
    print_string("Current directory: ");
    print_string(buffer);
    print_string("\n");
    return 0;
}