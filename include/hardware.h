#ifndef HARDWARE_H
#define HARDWARE_H

#include "types.h"

// CPU detection
struct cpu_info {
    uint32_t vendor_id[3];
    uint32_t features;
    uint32_t family;
    uint32_t model;
};

// Memory info
struct mem_info {
    uint32_t total_kb;
    uint32_t available_kb;
};

// Function prototypes
void detect_cpu(void);
void detect_memory(void);
void detect_pci_devices(void);

#endif