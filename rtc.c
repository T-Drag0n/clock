#include "rtc.h"
#include "twi.h"
#include "private_twi.h"

uint8_t bcd_to_binary(uint8_t bcd){
    return ((bcd>>4)*10) + (bcd & 0x0F);
}

uint8_t binary_to_bcd(uint8_t value){
    return (((value/10)<<4) | (value%10));
}

void read_current_time(time_value_t *time){
    twi_message_t messages[2] = {
        {
            .address = (DS3231_ADDRESS<<1) | TWI_WRITE,
            .buffer = (uint8_t[1]){CLOCK_ADDRESS},
            .size = 1,
        },
        {
            .address = (DS3231_ADDRESS<<1) | TWI_READ,
            .buffer = (uint8_t[3]){},
            .size = 4,
        },
    };

    twi_enqueue(messages, 2);
    while(twi_status() == TWI_STATUS_BUSY);

    uint8_t test_hours = messages[1].buffer[2] & 0x3F;
    time->minute = bcd_to_binary(messages[1].buffer[1]);
    time->hour = test_hours;
}

void read_alarm_time(time_value_t *time){ //this kinda useless ngl
    twi_message_t messages[2] = {
        {
            .address = (DS3231_ADDRESS<<1) | TWI_WRITE,
            .buffer = (uint8_t[1]){ALARM_ADDRESS},
            .size = 1,
        },
        {
            .address = (DS3231_ADDRESS<<1) | TWI_READ,
            .buffer = (uint8_t[2]){},
            .size = 3,
        },
    };

    twi_enqueue(messages, 2);
    while(twi_status() == TWI_STATUS_BUSY);
    uint8_t test_hours = messages[1].buffer[1] & 0x3F;
    time->minute = bcd_to_binary(messages[1].buffer[0]);
    time->hour = test_hours;
}

void write_current_time(time_value_t *time){

    uint8_t mins = binary_to_bcd(time->minute);
    uint8_t hours = binary_to_bcd(time->hour) & 0x3F;

    twi_message_t messages[1] = {
        {
            .address = (DS3231_ADDRESS<<1) | TWI_WRITE,
            .buffer = (uint8_t[4]){CLOCK_ADDRESS,0,mins, hours},
            .size = 4,
        },
    };

    twi_enqueue(messages, 1);
    while(twi_status() == TWI_STATUS_BUSY);

}

void write_alarm_time(time_value_t *time){

    uint8_t mins = binary_to_bcd(time->minute) & 0x8F;
    uint8_t hours = binary_to_bcd(time->hour) & 0x8F;

    twi_message_t messages[1] = {
        {
            .address = (DS3231_ADDRESS<<1) | TWI_WRITE,
            .buffer = (uint8_t[3]){ALARM_ADDRESS, mins, hours},
            .size = 3,
        },
    };

    twi_enqueue(messages, 1);
    while(twi_status() == TWI_STATUS_BUSY);

}
