#include "../include/filesystem.h"
#include "../include/print_string.h"
#include "../include/create_file.h"

extern FileSystem fs;

int create_file(int argc, char* argv[]) {
    if (argc != 2) {
        print_string("Usage: create <filename>\n");
        return 1;
    }
    if (fs_create_file(&fs, argv[1]) == 0) {
        print_string("File created successfully\n");
    } else {
        print_string("Error creating file\n");
    }
    return 0;
}