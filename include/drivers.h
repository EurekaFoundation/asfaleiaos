// include/drivers.h
#ifndef DRIVERS_H
#define DRIVERS_H

#include "types.h"

// Funzioni del driver della tastiera
void keyboard_handler();
char get_key();
char scancode_to_ascii(unsigned char scancode);
unsigned char read_keyboard_scancode();  // Aggiungi questa dichiarazione

// Funzioni I/O
unsigned char inb(unsigned short port);
void outb(unsigned short port, unsigned char data);

#endif
