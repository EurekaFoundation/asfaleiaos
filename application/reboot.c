#include "../include/io.h"

int reboot() {
    // Tell BIOS to reset (should work better with VirtualBox)
    outb(0x70, 0x8F);
    outb(0x71, 0x00);
    
    // Try keyboard controller reset
    outb(0x64, 0xFE);
    
    // Try system reset via port 0x92 (Fast A20 and reset)
    outb(0x92, 0x01);
    
    // Try PCI reset command
    outw(0xCF9, 0x06);
    
    // If all else fails, halt the CPU
    __asm__ volatile("cli; hlt");
    
    return 0;
}