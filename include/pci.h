#ifndef PCI_H
#define PCI_H

#include "types.h"

#define PCI_CONFIG_ADDR 0xCF8
#define PCI_CONFIG_DATA 0xCFC
#define PCI_POWER_STATE_D0 0
#define PCI_POWER_STATE_D1 1
#define PCI_POWER_STATE_D2 2
#define PCI_POWER_STATE_D3 3

typedef struct {
    uint16_t vendor_id;
    uint16_t device_id;
    uint16_t command;
    uint16_t status;
    uint8_t revision;
    uint8_t prog_if;
    uint8_t subclass;
    uint8_t class_code;
    uint8_t cache_line_size;
    uint8_t latency_timer;
    uint8_t header_type;
    uint8_t bist;
} __attribute__((packed)) PCI_CONFIG_HEADER;

typedef struct {
    uint16_t bus;
    uint8_t device;
    uint16_t vendor_id;
    uint16_t device_id;
    uint8_t class_code;
    uint8_t subclass;
    uint8_t prog_if;
    uint8_t revision;
    uint32_t bar[6];
    uint8_t interrupt_line;
    uint8_t interrupt_pin;
} PCI_DEVICE;

bool init_pci(void);
uint32_t pci_read_config(uint8_t bus, uint8_t device, uint8_t func, uint8_t offset);
void pci_write_config(uint8_t bus, uint8_t device, uint8_t func, uint8_t offset, uint32_t value);
// Add function declarations
bool pci_enable_msi(uint8_t bus, uint8_t device);
bool pci_set_power_state(uint8_t bus, uint8_t device, uint8_t state);
uint8_t pci_find_capability(uint8_t bus, uint8_t device, uint8_t cap_id);
// Add function declarations
void scan_pci_bus(uint8_t bus);
void scan_pci_device(uint8_t bus, uint8_t device);
void scan_pci_bus(uint8_t bus);
bool register_pci_device(uint8_t bus, uint8_t device);
bool check_multi_function(uint8_t bus, uint8_t device);

#endif