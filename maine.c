// SPDX-License-Identifier: MPL-2.0
/*
 * main.c -- sample code to interface with MPU-6050 over TWI
 * Copyright (C) 2025  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "twi.h"
#include "rtc.h"
#include "encoder.h"

const uint32_t SCL_FREQUENCY = 100000;
volatile bool alarm_triggered = false;

typedef enum{
    CLOCK,
    SET_TIME,
    SET_ALARM,
    ALARM_ON,
}mode_t;

ISR(INT0_vect) {
    alarm_triggered = true;
}

int main(void)
{
    sei();

    uint8_t pin_mask = (1 << PC5) | (1 << PC4);
    DDRC  &= ~pin_mask;
    PORTC |=  pin_mask;

    twi_status_t status = twi_init(SCL_FREQUENCY);
    if (status != TWI_STATUS_SUCCESS)
        return EXIT_FAILURE;

    mode_t mode = CLOCK;
    time_value_t time;
    DDRB &= ~((1<<PB0) | (1<<PB1) | (1<<PB2) | (1<<PB3) | (1<<PB4) | (1<<PB5));
    PORTB |= (1<<PB0) | (1<<PB1) | (1<<PB2) | (1<<PB3) | (1<<PB4) | (1<<PB5);

    EICRA |= (1 << ISC01);
    EICRA &= ~(1 << ISC00);
    EIMSK |= (1 << INT0);
    DDRD &= ~(1 << PD2); 
    PORTD |= (1 << PD2);

    while(1){

        if(alarm_triggered){
            //wait for the return of function here
            //clear_alarm_flag();
            while(1);
            alarm_triggered = false;
        }

        encoder(&encoder_mode);
        mode = (mode_t)encoder_mode.value;

        if (mode != CLOCK) {
            encoder(&encoder_mins);
            encoder(&encoder_hours);
            encoder(&encoder_confirm);

            if (encoder_confirm.value) {

                time.minute = encoder_mins.value;
                time.hour = encoder_hours.value;

                if (mode == SET_TIME){
                    write_current_time(&time);
                    mode = CLOCK;
                }else if (mode == SET_ALARM){
                    write_alarm_time(&time);
                    mode = CLOCK;
               }
                
               encoder_confirm.value = 0;
               encoder_mode.value = 0;
            }

        }else{
            read_current_time(&time);
            //display 
        }

    }

    return EXIT_SUCCESS;
}