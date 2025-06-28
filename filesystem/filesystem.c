#include "../include/filesystem.h"
#include "../include/print_string.h"
#include "../include/string.h"
#include "../include/NULL.h"
#include "../include/memory.h"

extern FileSystem fs;  // Declare `fs` as `extern`

void fs_init(FileSystem* fs) {
    fs->root.name[0] = '/';
    fs->root.name[1] = '\0';
    fs->root.size = 0;
    fs->root.is_directory = 1;
    fs->root.parent = NULL;
    fs->root.child_count = 0;
    fs->current_directory = &fs->root;
}

int fs_create_file(FileSystem* fs, const char* name) {
    if (fs->current_directory->child_count >= MAX_FILES) {
        print_string("Error: Maximum file limit reached\n");
        return -1;
    }
    for (int i = 0; i < fs->current_directory->child_count; i++) {
        if (strncmp(fs->current_directory->children[i]->name, name, MAX_FILENAME_LENGTH) == 0) {
            print_string("Error: File already exists\n");
            return -1;
        }
    }
    File* file = (File*)my_malloc(sizeof(File));  // Use my_malloc
    if (file == NULL) {
        print_string("Error: Memory allocation failed\n");
        return -1;
    }
    strncpy(file->name, name, MAX_FILENAME_LENGTH);
    file->size = 0;
    file->is_directory = 0;
    file->parent = fs->current_directory;
    file->child_count = 0;
    fs->current_directory->children[fs->current_directory->child_count++] = file;
    return 0;
}

int fs_write_file(FileSystem* fs, const char* name, const char* data, int size) {
    for (int i = 0; i < fs->current_directory->child_count; i++) {
        if (strncmp(fs->current_directory->children[i]->name, name, MAX_FILENAME_LENGTH) == 0) {
            if (size > MAX_FILE_SIZE) {
                print_string("Error: File size exceeds limit\n");
                return -1;
            }
            strncpy(fs->current_directory->children[i]->data, data, size);
            fs->current_directory->children[i]->size = size;
            return 0;
        }
    }
    print_string("Error: File not found\n");
    return -1;
}

int fs_read_file(FileSystem* fs, const char* name, char* buffer, int size) {
    for (int i = 0; i < fs->current_directory->child_count; i++) {
        if (strncmp(fs->current_directory->children[i]->name, name, MAX_FILENAME_LENGTH) == 0) {
            if (size < fs->current_directory->children[i]->size) {
                print_string("Error: Buffer size too small\n");
                return -1;
            }
            strncpy(buffer, fs->current_directory->children[i]->data, fs->current_directory->children[i]->size);
            return fs->current_directory->children[i]->size;
        }
    }
    print_string("Error: File not found\n");
    return -1;
}

int fs_delete_file(FileSystem* fs, const char* name) {
    for (int i = 0; i < fs->current_directory->child_count; i++) {
        File* node = fs->current_directory->children[i];
        if (strncmp(node->name, name, MAX_FILENAME_LENGTH) == 0) {
            // Check if it's actually a file
            if (node->is_directory) {
                print_string("Error: Cannot delete directory with delete_file\n");
                return -1;
            }
            
            // Free file content first
            if (node->content != NULL) {
                my_free(node->content);
            }
            
            // Free the node and reorganize array
            my_free(node);
            for (int j = i; j < fs->current_directory->child_count - 1; j++) {
                fs->current_directory->children[j] = fs->current_directory->children[j + 1];
            }
            fs->current_directory->child_count--;
            return 0;
        }
    }
    print_string("Error: File not found\n");
    return -1;
}
int fs_list_files(FileSystem* fs, char* buffer, int size) {
    int offset = 0;
    for (int i = 0; i < fs->current_directory->child_count; i++) {
        int len = strlen(fs->current_directory->children[i]->name);
        if (offset + len + 1 > size) {
            print_string("Error: Buffer size too small\n");
            return -1;
        }
        strncpy(buffer + offset, fs->current_directory->children[i]->name, len);
        offset += len;
        buffer[offset++] = '\n';
    }
    buffer[offset] = '\0';
    return offset;
}

int fs_create_directory(FileSystem* fs, const char* name) {
    if (fs->current_directory->child_count >= MAX_FILES) {
        print_string("Error: Maximum file limit reached\n");
        return -1;
    }
    for (int i = 0; i < fs->current_directory->child_count; i++) {
        if (strncmp(fs->current_directory->children[i]->name, name, MAX_FILENAME_LENGTH) == 0) {
            print_string("Error: Directory already exists\n");
            return -1;
        }
    }
    File* dir = (File*)my_malloc(sizeof(File));  // Use my_malloc per allocare memoria
    if (dir == NULL) {
        print_string("Error: Memory allocation failed\n");
        return -1;
    }
    strncpy(dir->name, name, MAX_FILENAME_LENGTH);
    dir->size = 0;
    dir->is_directory = 1;
    dir->parent = fs->current_directory;
    dir->child_count = 0;
    fs->current_directory->children[fs->current_directory->child_count++] = dir;
    return 0;
}

int fs_change_directory(FileSystem* fs, const char* name) {
    if (strncmp(name, "..", 2) == 0) {
        if (fs->current_directory->parent != NULL) {
            fs->current_directory = fs->current_directory->parent;
        }
        return 0;
    }
    for (int i = 0; i < fs->current_directory->child_count; i++) {
        if (strncmp(fs->current_directory->children[i]->name, name, MAX_FILENAME_LENGTH) == 0 &&
            fs->current_directory->children[i]->is_directory) {
            fs->current_directory = fs->current_directory->children[i];
            return 0;
        }
    }
    print_string("Error: Directory not found\n");
    return -1;
}

void fs_get_current_directory(FileSystem* fs, char* buffer, int size) {
    File* dir = fs->current_directory;
    int offset = size - 1;
    buffer[offset] = '\0';
    while (dir != NULL) {
        int len = strlen(dir->name);
        if (offset < len + 1) {
            break;
        }
        offset -= len;
        strncpy(buffer + offset, dir->name, len);
        if (offset > 0) {
            buffer[--offset] = '/';
        }
        dir = dir->parent;
    }
    if (offset > 0) {
        strncpy(buffer, buffer + offset, size - offset);
    }
}

int fs_delete_directory(FileSystem* fs, const char* name) {
    // Prevent root directory deletion
    if (fs->current_directory == &fs->root && strncmp(name, "/", 2) == 0) {
        print_string("Error: Cannot delete root directory\n");
        return -1;
    }

    for (int i = 0; i < fs->current_directory->child_count; i++) {
        File* node = fs->current_directory->children[i];
        if (strncmp(node->name, name, MAX_FILENAME_LENGTH) == 0) {
            // Check if it's a directory
            if (!node->is_directory) {
                print_string("Error: Not a directory\n");
                return -1;
            }
            
            // Check if directory is empty
            if (node->child_count > 0) {
                print_string("Error: Directory not empty\n");
                return -1;
            }
            
            // Free directory node and reorganize array
            my_free(node);
            for (int j = i; j < fs->current_directory->child_count - 1; j++) {
                fs->current_directory->children[j] = fs->current_directory->children[j + 1];
            }
            fs->current_directory->child_count--;
            return 0;
        }
    }
    print_string("Error: Directory not found\n");
    return -1;
}

FileHandle* fs_open(FileSystem* fs, const char* filename, const char* mode) {
    if (!fs || !filename || !mode) return NULL;
    
    FileHandle* handle = (FileHandle*)my_malloc(sizeof(FileHandle));
    if (!handle) return NULL;
    
    handle->name = (char*)filename;
    handle->mode = (mode[0] == 'r') ? 0 : 1; // 0 for read, 1 for write
    handle->handle = 0; // First available handle
    
    return handle;
}

void fs_close(FileSystem* fs, FileHandle* file) {
    if (!fs || !file) return;
    my_free(file);
}

int fs_read(FileSystem* fs, FileHandle* file, char* buffer, int size) {
    if (!fs || !file || !buffer || size <= 0) return -1;
    if (file->mode != 0) return -1; // Not opened for reading
    
    return fs_read_file(fs, file->name, buffer, size);
}