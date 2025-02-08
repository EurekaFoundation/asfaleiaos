#include "../include/print_string.h"
#include "../include/filesystem.h"
#include "../include/create_directory.h"

extern FileSystem fs;

int create_directory(int argc, char* argv[]) {
    if (argc != 2) {
        print_string("Usage: mkdir <dirname>\n");
        return 1;
    }
    if (fs_create_directory(&fs, argv[1]) == 0) {
        print_string("Directory created successfully\n");
    } else {
        print_string("Error creating directory\n");
    }
    return 0;
}
