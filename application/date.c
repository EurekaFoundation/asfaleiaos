#include "../include/io.h"
#include "../include/print_string.h"

unsigned char is_rtc_updating() {
    outb(0x70, 0x0A);
    return (inb(0x71) & 0x80);
}

unsigned char read_rtc(unsigned char reg) {
    // Wait until RTC is not updating
    while (is_rtc_updating());
    outb(0x70, reg);
    return inb(0x71);
}

// Convert BCD to binary if needed
unsigned char bcd_to_binary(unsigned char bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

char* get_date_string() {
    static char buffer[32];
    unsigned char second, minute, hour, day, month, year;
    
    // Wait for an RTC update cycle
    while (is_rtc_updating());
    
    // Read RTC values - now dynamic
    second = bcd_to_binary(read_rtc(0x00));
    minute = bcd_to_binary(read_rtc(0x02));
    hour = bcd_to_binary(read_rtc(0x04));
    day = bcd_to_binary(read_rtc(0x07));
    month = bcd_to_binary(read_rtc(0x08));
    year = bcd_to_binary(read_rtc(0x09));
    
    // Format string with time
    buffer[0] = (day / 10) + '0';
    buffer[1] = (day % 10) + '0';
    buffer[2] = '/';
    buffer[3] = (month / 10) + '0';
    buffer[4] = (month % 10) + '0';
    buffer[5] = '/';
    buffer[6] = '2';
    buffer[7] = '0';
    buffer[8] = (year / 10) + '0';
    buffer[9] = (year % 10) + '0';
    buffer[10] = ' ';
    buffer[11] = (hour / 10) + '0';
    buffer[12] = (hour % 10) + '0';
    buffer[13] = ':';
    buffer[14] = (minute / 10) + '0';
    buffer[15] = (minute % 10) + '0';
    buffer[16] = ':';
    buffer[17] = (second / 10) + '0';
    buffer[18] = (second % 10) + '0';
    buffer[19] = '\0';
    
    return buffer;
}

int date() {
    print_string(get_date_string());
    print_string("\n");
    return 0;
}

