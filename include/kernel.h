// include/kernel.h
#ifndef KERNEL_H
#define KERNEL_H

#include "types.h"

// Funzioni kernel
void kernel_main();
void clear_screen();
void print_string(const char* str);
void timer_handler(void);

// Gestione interrupt
void set_idt_gate(int interrupt, void* handler);
void load_idt();

#endif