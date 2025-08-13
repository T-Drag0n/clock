#include "rtc.h"
#include "twi.h"
#include "private_twi.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

const uint32_t SCL_FREQUENCY = 100000;
volatile bool alarm_triggered = false;

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
    time->hour = bcd_to_binary(test_hours);
}

void read_alarm_time(time_value_t *time){
    twi_message_t messages[2] = {
        {
            .address = (DS3231_ADDRESS<<1) | TWI_WRITE,
            .buffer = (uint8_t[1]){ALARM_ADDRESS},
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
    uint8_t test_hours = messages[1].buffer[1];
    time->minute = bcd_to_binary(messages[1].buffer[0]);
    time->minute = messages[1].buffer[2];
    time->hour = bcd_to_binary(test_hours);
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

    uint8_t mins = binary_to_bcd(time->minute) & 0x7F;
    uint8_t hours = binary_to_bcd(time->hour) & 0x7F;
    uint8_t no_date = 0x80;
    uint8_t control = (1<<2) | (1<<1);
    uint8_t status;
    status &= ~(1<<1);

    twi_message_t messages[3] = {
        {
            .address = (DS3231_ADDRESS<<1) | TWI_WRITE,
            .buffer = (uint8_t[4]){ALARM_ADDRESS, mins, hours, no_date},
            .size = 4,
        },
        {
            .address = (DS3231_ADDRESS<<1) | TWI_WRITE,
            .buffer = (uint8_t[2]){CONTROL_ADDRESS, control},
            .size = 2,
        },
        {
            .address = (DS3231_ADDRESS<<1) | TWI_WRITE,
            .buffer = (uint8_t[2]){STATUS_ADDRESS, status},
            .size = 2,
        },
    };

    twi_enqueue(messages, 3);
    while(twi_status() == TWI_STATUS_BUSY);

}

void clear_alarm_flag(void){
    uint8_t status;
    status &= ~(1<<1);

    twi_message_t messages[1] = {
        {
            .address = (DS3231_ADDRESS<<1) | TWI_WRITE,
            .buffer = (uint8_t[2]){STATUS_ADDRESS, status},
            .size = 2,
        },
    };

    twi_enqueue(messages, 1);
    while(twi_status() == TWI_STATUS_BUSY);

}


void rtc_init(void){
    uint8_t pin_mask = (1 << PC5) | (1 << PC4);
    DDRC  &= ~pin_mask;
    PORTC |=  pin_mask;
    twi_status_t status = twi_init(SCL_FREQUENCY);
    if (status != TWI_STATUS_SUCCESS)
        return EXIT_FAILURE;

    EICRA |= (1 << ISC01);
    EICRA &= ~(1 << ISC00);
    EIMSK |= (1 << INT0);
    //rtc
    DDRD &= ~(1 << PD2); 
    PORTD |= (1 << PD2);
}
    