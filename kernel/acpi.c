#include "../include/acpi.h"
#include "../include/print_string.h"
#include "../include/NULL.h"
#include "../include/string.h"

uint32_t* find_rsdp(void) {
    // Search for RSDP in BIOS areas
    char* ptr;
    
    // First search Extended BIOS area
    for(ptr = (char*)0x000E0000; ptr < (char*)0x00100000; ptr += 16) {
        if(memcmp(ptr, "RSD PTR ", 8) == 0) {
            return (uint32_t*)ptr;
        }
    }
    
    return NULL;
}

bool parse_acpi_tables(uint32_t* rsdp) {
    if(!rsdp) return FALSE;
    
    RSDP_DESCRIPTOR* desc = (RSDP_DESCRIPTOR*)rsdp;
    
    // Validate signature
    if(memcmp(desc->signature, ACPI_RSDP_SIGNATURE, 8) != 0) {
        return FALSE;
    }
    
    return TRUE;
}