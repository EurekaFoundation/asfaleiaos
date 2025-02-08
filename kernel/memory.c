// src/kernel/memory.c
#include "../include/memory.h"
#include "../include/types.h"
#include "../include/NULL.h"


#define KERNEL_HEAP_SIZE 1024 * 1024  // 1 MB
#define MEMORY_SIZE 1024 * 1024  // 1 MB of memory


typedef struct {
    void* start;
    size_t size;
    int is_free;
} MemoryBlock;

static char memory_pool[MEMORY_SIZE];
static int memory_index = 0;
static char kernel_heap[KERNEL_HEAP_SIZE];
static MemoryBlock memory_map[256];
static int memory_map_entries = 0;

void* my_malloc(int size) {
    if (memory_index + size > MEMORY_SIZE) {
        return NULL;  // Memory exhausted
    }
    void* ptr = &memory_pool[memory_index];
    memory_index += size;
    return ptr;
}

void my_free(void* ptr) {
    (void)ptr;  // Evita il warning per il parametro non utilizzato
}

void initialize_memory() {
    memory_map[0].start = kernel_heap;
    memory_map[0].size = KERNEL_HEAP_SIZE;
    memory_map[0].is_free = 1;
    memory_map_entries = 1;
}

void* kernel_malloc(size_t size) {
    for(int i = 0; i < memory_map_entries; i++) {
        if(memory_map[i].is_free && memory_map[i].size >= size) {
            memory_map[i].is_free = 0;
            return memory_map[i].start;
        }
    }
    return 0;  // Allocation failed
}

void kernel_free(void* ptr) {
    for(int i = 0; i < memory_map_entries; i++) {
        if(memory_map[i].start == ptr) {
            memory_map[i].is_free = 1;
            break;
        }
    }
}