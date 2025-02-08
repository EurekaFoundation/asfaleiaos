#include "../include/print_string.h"
#include "../include/filesystem.h"
#include "../include/delete_dir.h"

extern FileSystem fs;

int delete_dir(int argc, char* argv[]) {
    if (argc != 2) {
        print_string("Usage: delete <dirname>\n");
        return 1;
    }
    if (fs_delete_directory(&fs, argv[1]) == 0) {
        print_string("Directory deleted successfully\n");
    } else {
        print_string("Error deleting directory\n");
    }
    return 0;
}