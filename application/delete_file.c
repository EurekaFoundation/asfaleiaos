#include "../include/print_string.h"
#include "../include/filesystem.h"
#include "../include/delete_file.h"

extern FileSystem fs;

int delete_file(int argc, char* argv[]) {
    if (argc != 2) {
        print_string("Usage: delete <filename> or delete <dirname>\n");
        return 1;
    }
    if (fs_delete_file(&fs, argv[1]) == 0) {
        print_string("File/Directory deleted successfully\n");
    } else {
        print_string("Error deleting file/directory\n");
    }
    return 0;
}