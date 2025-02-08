#include "../include/filesystem.h"
#include "../include/print_string.h"
#include "../include/read_file.h"

extern FileSystem fs;

int read_file(int argc, char* argv[]) {
    char buffer[MAX_FILE_SIZE];
    if (argc != 2) {
        print_string("Usage: read <filename>\n");
        return 1;
    }
    int size = fs_read_file(&fs, argv[1], buffer, sizeof(buffer));
    if (size > 0) {
        buffer[size] = '\0';  // Aggiungi il carattere di terminazione
        print_string("File content: ");
        print_string(buffer);
        print_string("\n");
    } else {
        print_string("Error reading file\n");
    }
    return 0;
}