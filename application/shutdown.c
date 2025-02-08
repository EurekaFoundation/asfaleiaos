#include "../include/io.h"
#include "../include/print_string.h"

void shutdown(void) {
    print_string("Shutting down...\n");
    
    // Try APM 1.2+ shutdown
    outw(0x9000, 0x07);  // APM installation check
    outw(0x9004, 0x03);  // APM real mode interface connect
    outw(0x9007, 0x04);  // APM Set Power State
    outw(0x9001, 0x0001);  // APM Power Off
    
    // Try ACPI shutdown
    outw(0x604, 0x2000);   // ACPI PM1a Control Block
    outw(0xB004, 0x2000);  // ACPI PM1b Control Block
    
    // Try various legacy ports
    outb(0x8900, 0x01);    // Legacy power management
    outb(0xf4, 0x00);      // Legacy ACPI
    outb(0x64, 0xfe);      // Legacy keyboard controller
    
    // Standard ATX power supply command
    outb(0x64, 0xfe);      // System Control Port A
    
    print_string("Failed to shutdown. System halted.\n");
    
    // Safe halt
    __asm__ volatile ("cli; hlt");
    
    while(1) { }
}