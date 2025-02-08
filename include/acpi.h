#ifndef ACPI_H
#define ACPI_H

#include "types.h"

#define ACPI_RSDP_SIGNATURE "RSD PTR "

typedef struct {
    char signature[8];
    uint8_t checksum;
    char oem_id[6];
    uint8_t revision;
    uint32_t rsdt_address;
} __attribute__((packed)) RSDP_DESCRIPTOR;

uint32_t* find_rsdp(void);
bool parse_acpi_tables(uint32_t* rsdp);
bool acpi_shutdown(void);

#endif