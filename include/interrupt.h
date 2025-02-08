#ifndef INTERRUPT_H
#define INTERRUPT_H

void init_timer(int hz);
void register_timer_handler(void (*handler)(void));
void timer_interrupt_handler(void);
void set_idt_gate(int interrupt, void* handler);
void load_idt(void);
void register_interrupt_handler(int irq, void (*handler)(void));
void register_interrupt_handler(int irq, void (*handler)(void));
void handle_interrupt(int irq);

#endif