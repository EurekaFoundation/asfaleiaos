#include "../include/hardware.h"
#include "../include/io.h"
#include "../include/print_string.h"

static struct cpu_info cpu;
static struct mem_info memory;

void detect_cpu(void) {
    // Basic CPU detection using CPUID
    __asm__ volatile(
        "cpuid"
        : "=b"(cpu.vendor_id[0]),
          "=d"(cpu.vendor_id[1]),
          "=c"(cpu.vendor_id[2])
        : "a"(0)
    );
    print_string("CPU detected\n");
}

void detect_memory(void) {
    // Basic memory detection through BIOS
    memory.total_kb = *((uint32_t*)0x413);
    memory.available_kb = memory.total_kb;
    print_string("Memory detected: ");
    // TODO: Add print_int function
}

void detect_pci_devices(void) {
    // Basic PCI scan
    for(uint16_t bus = 0; bus < 256; bus++) {
        for(uint16_t device = 0; device < 32; device++) {
            uint32_t id = inl(0xCF8 | (bus << 16) | (device << 11));
            if(id != 0xFFFFFFFF) {
                // Device found
            }
        }
    }
}