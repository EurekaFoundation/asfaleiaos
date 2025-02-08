#include "../include/power.h"
#include "../include/print_string.h"
#include "../include/status_of_pc.h"

void show_battery_status(void) {
    if(is_battery_present()) {
        uint8_t level = get_battery_level();
        uint8_t temp = get_cpu_temperature();
        
        print_string("Battery Status:\n");
        print_string("Level: ");
        print_dec(level);
        print_string("%\n");
        
        print_string("CPU Temperature: ");
        print_dec(temp);
        print_string("°C\n");
        
        print_string("Thermal State: ");
        switch(get_thermal_state()) {
            case THERMAL_NORMAL:   print_string("Normal\n"); break;
            case THERMAL_WARNING:  print_string("Warning\n"); break;
            case THERMAL_CRITICAL: print_string("Critical\n"); break;
        }
    } else {
        print_string("No battery detected\n");
    }
}