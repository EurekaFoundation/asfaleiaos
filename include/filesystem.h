#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#define MAX_FILES 100
#define MAX_FILENAME_LENGTH 32
#define MAX_FILE_SIZE 1024

typedef struct File {
    char name[MAX_FILENAME_LENGTH];
    char data[MAX_FILE_SIZE];
    int size;
    int is_directory;
    struct File* parent;
    struct File* children[MAX_FILES];
    int child_count;
    char* content;
} File;

typedef struct {
    File root;
    File* current_directory;
} FileSystem;

void fs_init(FileSystem* fs);
int fs_create_file(FileSystem* fs, const char* name);
int fs_write_file(FileSystem* fs, const char* name, const char* data, int size);
int fs_read_file(FileSystem* fs, const char* name, char* buffer, int size);
int fs_delete_file(FileSystem* fs, const char* name);
int fs_list_files(FileSystem* fs, char* buffer, int size);
int fs_create_directory(FileSystem* fs, const char* name);
int fs_change_directory(FileSystem* fs, const char* name);
void fs_get_current_directory(FileSystem* fs, char* buffer, int size);
int fs_delete_directory(FileSystem* fs, const char* name);

#endif // FILESYSTEM_H