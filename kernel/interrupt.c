// src/kernel/interrupt.c
#include "../include/kernel.h"
#include "../include/io.h"

#define MAX_INTERRUPTS 256
#define IDT_SIZE 256
#define PIT_FREQUENCY 1193180
#define TIMER_COMMAND 0x43
#define TIMER_DATA 0x40

typedef struct {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attr;
    uint16_t offset_high;
} __attribute__((packed)) IDTEntry;

static IDTEntry idt[IDT_SIZE];

void set_idt_gate(int interrupt, void* handler) {
    uint32_t handler_address = (uint32_t)handler;
    idt[interrupt].offset_low = handler_address & 0xFFFF;
    idt[interrupt].selector = 0x08;  // Codice kernel
    idt[interrupt].zero = 0;
    idt[interrupt].type_attr = 0x8E;  // Interrupt gate
    idt[interrupt].offset_high = (handler_address >> 16) & 0xFFFF;
}

void load_idt() {
    // Carica IDT
}

void timer_interrupt_handler(void);  // Forward declaration

static void (*timer_callback)(void) = 0;

void init_timer(int hz) {
    int divisor = PIT_FREQUENCY / hz;
    outb(TIMER_COMMAND, 0x36);
    outb(TIMER_DATA, divisor & 0xFF);
    outb(TIMER_DATA, divisor >> 8);
    
    // Register timer interrupt handler
    set_idt_gate(32, timer_interrupt_handler);
}

void register_timer_handler(void (*handler)(void)) {
    timer_callback = handler;
}

void timer_interrupt_handler(void) {
    if (timer_callback) {
        timer_callback();
    }
    // Send EOI to PIC
    outb(0x20, 0x20);
}

static void (*interrupt_handlers[MAX_INTERRUPTS])(void);

void register_interrupt_handler(int irq, void (*handler)(void)) {
    if (irq >= 0 && irq < MAX_INTERRUPTS) {
        interrupt_handlers[irq] = handler;
    }
}

void handle_interrupt(int irq) {
    if (irq >= 0 && irq < MAX_INTERRUPTS && interrupt_handlers[irq]) {
        interrupt_handlers[irq]();
    }
}