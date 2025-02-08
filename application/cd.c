#include "../include/print_string.h"
#include "../include/filesystem.h"
#include "../include/cd.h"

extern FileSystem fs;

int cd(int argc, char* argv[]) {
    if (argc != 2) {
        print_string("Usage: cd <dirname>\n");
        return 1;
    }
    if (fs_change_directory(&fs, argv[1]) == 0) {
        print_string("Directory changed successfully\n");
    } else {
        print_string("Error changing directory\n");
    }
    return 0;
}