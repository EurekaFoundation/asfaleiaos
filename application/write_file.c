#include "../include/print_string.h"
#include "../include/filesystem.h"
#include "../include/write_file.h"
#include "../include/string.h"

extern FileSystem fs;

int write_file(int argc, char* argv[]) {
    if (argc != 3) {
        print_string("Usage: write <filename> <data>\n");
        return 1;
    }
    int size = strlen(argv[2]);
    if (size >= MAX_FILE_SIZE) {
        print_string("Error: Data size exceeds file size limit\n");
        return 1;
    }
    char data[MAX_FILE_SIZE];
    strncpy(data, argv[2], size);
    data[size] = '\0';  // Aggiungi il carattere di terminazione
    if (fs_write_file(&fs, argv[1], data, size + 1) == 0) {
        print_string("File written successfully\n");
    } else {
        print_string("Error writing to file\n");
    }
    return 0;
}