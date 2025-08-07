// SPDX-License-Identifier: MPL-2.0
/*
 * main.c -- sample code to interface with MPU-6050 over TWI
 * Copyright (C) 2025  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>

#include "twi.h"
#include "rtc.h"

const uint32_t SCL_FREQUENCY = 100000;

int main(void)
{
    sei();

    uint8_t pin_mask = (1 << PC5) | (1 << PC4);

    DDRC  &= ~pin_mask;
    PORTC |=  pin_mask;

    twi_status_t status = twi_init(SCL_FREQUENCY);

    if (status != TWI_STATUS_SUCCESS)
        return EXIT_FAILURE;

    twi_message_t messages[2]={
        {
            .address = (0x68<<1) | TWI_WRITE,
            .buffer = (uint8_t[3]){0x01,binary_to_bcd(0x32), binary_to_bcd(0x10)},
            .size = 3,
        },
        {
            .address = (0x68<<1) | TWI_READ,
            .buffer = (uint8_t[2]){},
            .size = 2,
        },
    };

    twi_enqueue(messages,2);

    while(1);

    return EXIT_SUCCESS;
}