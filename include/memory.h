// include/memory.h
#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

#define PAGE_SIZE 4096
#define PAGE_ALIGN __attribute__((aligned(PAGE_SIZE)))

void initialize_memory();
void* kernel_malloc(size_t size);
void kernel_free(void* ptr);
void* my_malloc(int size);
void my_free(void* ptr);
void* alloc_pace(void);
void free_page(void* page);
void kfree(void* ptr);
void* memset(void* s, int c, size_t n);

#endif