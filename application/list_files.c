#include "../include/print_string.h"
#include "../include/filesystem.h"
#include "../include/list_files.h"

extern FileSystem fs;

int list_files(int argc, char* argv[]) {
    (void)argc;  // Evita il warning per il parametro non utilizzato
    (void)argv;  // Evita il warning per il parametro non utilizzato

    char buffer[1024];
    if (fs_list_files(&fs, buffer, sizeof(buffer)) > 0) {
        print_string("Files:\n");
        print_string(buffer);
    } else {
        print_string("  \n");
    }
    return 0;
}