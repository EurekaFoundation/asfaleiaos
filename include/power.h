#ifndef _POWER_H_
#define _POWER_H_

#include "types.h"

// Add ACPI thermal zone structure
typedef struct {
    uint32_t base_address;
    uint32_t length;
    bool is_valid;
} thermal_zone_t;

extern thermal_zone_t acpi_thermal_zone;

// Power States
typedef enum {
    POWER_STATE_ON = 0,         // S0 - Full power
    POWER_STATE_IDLE = 1,       // CPU idle
    POWER_STATE_STANDBY = 2,    // S1 - Standby
    POWER_STATE_SUSPEND = 3,    // S3 - Suspend to RAM
    POWER_STATE_HIBERNATION = 4, // S4 - Suspend to disk
    POWER_STATE_SOFT_OFF = 5,   // S5 - Soft off
    POWER_STATE_MECH_OFF = 6    // G3 - Mechanical off
} power_state_t;

// ACPI Power Management Ports
#define PM1A_CONTROL_PORT 0x404
#define PM1B_CONTROL_PORT 0x404

// Thermal States
typedef enum {
    THERMAL_NORMAL = 0,
    THERMAL_WARNING = 1,
    THERMAL_CRITICAL = 2
} thermal_state_t;

// ACPI Events
typedef enum {
    POWER_EVENT_BUTTON = 0,
    POWER_EVENT_SLEEP = 1,
    POWER_EVENT_THERMAL = 2,
    POWER_EVENT_BATTERY_LOW = 3
} power_event_t;

typedef void (*power_event_handler_t)(power_event_t event);


// Function declarations
void init_power_management(void);
void set_power_state(uint8_t state);
void halt_cpu(void);
uint8_t get_power_state(void);
uint8_t get_thermal_state(void);
void set_fan_speed(uint8_t speed);
uint8_t get_battery_level(void);
void register_power_event_handler(power_event_handler_t handler);
void handle_power_event(power_event_t event);
bool is_battery_present(void);
uint8_t get_battery_percentage(void);
uint8_t get_cpu_temperature(void);

#endif /* _POWER_H_ */