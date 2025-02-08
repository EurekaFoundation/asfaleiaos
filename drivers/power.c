#include "../include/power.h"
#include "../include/io.h"
#include "../include/acpi.h"
#include "../include/NULL.h"

#define ACPI_PM1A_CONTROL_BLOCK 0x404
#define ACPI_PM1B_CONTROL_BLOCK 0x404

static uint8_t current_power_state = POWER_STATE_ON;
static power_event_handler_t event_handler = NULL;
thermal_zone_t acpi_thermal_zone = {0, 0, FALSE};



void init_power_management(void) {
    // Find ACPI tables
    uint32_t* rsdp = find_rsdp();
    if(rsdp) {
        // Parse ACPI tables
        parse_acpi_tables(rsdp);
        current_power_state = POWER_STATE_ON;
    }
}

void halt_cpu(void) {
    __asm__ volatile("hlt");
}

void set_power_state(uint8_t state) {
    switch(state) {
        case POWER_STATE_ON:
            // Full power state
            break;
            
        case POWER_STATE_IDLE:
            // CPU idle state
            halt_cpu();
            break;
            
        case POWER_STATE_STANDBY:
            // S1 sleep state
            outw(ACPI_PM1A_CONTROL_BLOCK, 0x1400);
            break;
            
        case POWER_STATE_SUSPEND:
            // S3 sleep state
            outw(ACPI_PM1A_CONTROL_BLOCK, 0x3400);
            break;
            
        case POWER_STATE_HIBERNATION:
            // S4 sleep state
            outw(ACPI_PM1A_CONTROL_BLOCK, 0x3c00);
            break;
            
        case POWER_STATE_SOFT_OFF:
            // S5 state - soft off
            outw(ACPI_PM1A_CONTROL_BLOCK, 0x3c00);
            outw(ACPI_PM1B_CONTROL_BLOCK, 0x2000);
            break;
            
        case POWER_STATE_MECH_OFF:
            // G3 state - mechanical off
            outw(ACPI_PM1A_CONTROL_BLOCK, 0x3c00);
            outw(ACPI_PM1B_CONTROL_BLOCK, 0x3000);
            break;
    }
    current_power_state = state;
}

uint8_t get_power_state(void) {
    return current_power_state;
}

void set_fan_speed(uint8_t speed) {
    // EC (Embedded Controller) ports
    outb(0x62, speed);  // Fan control port
}

uint8_t get_thermal_state(void) {
    uint8_t temp = get_cpu_temperature();
    
    // Standard temperature thresholds
    if(temp > 85) return THERMAL_CRITICAL;  // Critical at 85°C
    if(temp > 65) return THERMAL_WARNING;   // Warning at 65°C
    return THERMAL_NORMAL;                  // Normal below 65°C
}

uint8_t get_battery_level(void) {
    return inb(0x68);  // Battery status port
}

void register_power_event_handler(power_event_handler_t handler) {
    event_handler = handler;
}

void handle_power_event(power_event_t event) {
    if(event_handler) {
        event_handler(event);
    }
}

bool is_battery_present(void) {
    return (inb(0x68) & 0x80) != 0;
}

// Add helpers for different temperature sources
static uint8_t acpi_get_temperature(void) {
    if(acpi_thermal_zone.is_valid) {
        // Read from ACPI thermal zone
        uint32_t raw_temp = inl(acpi_thermal_zone.base_address);
        return (raw_temp >> 16) & 0xFF;
    }
    return 0;
}

uint8_t get_cpu_temperature(void) {
    // Try ACPI thermal zone first
    uint8_t temp = acpi_get_temperature();
    if(temp > 0) {
        return temp;
    }

    // Fallback to IO port
    temp = inb(0x66);
    return 30 + (temp & 0x3F); // Limit to 30-94°C range
}