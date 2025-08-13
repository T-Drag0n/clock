#include "7seg.h"
#include "rtc.h"
#include "twi.h"
#include "encoder.h"
#include <avr/io.h>
#include <stdio.h> 
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

ISR(INT0_vect) {
            alarm_triggered = true;
        }

int main (void){

    // buzzer
    DDRC |= (1<<PC0);
    PORTC |= (1<<PC0);
    disp_init();
    encoder_init(&encoder_mode,8,9);
    encoder_init(&encoder_hours,3,4);
    encoder_init(&encoder_mins,5,6);
    encoder_init(&encoder_confirm,0,7);
    rtc_init();
    sei();
    // char msg[] = "12345678";
    char msg[9];
    uint8_t decimal_index = 0b00000100;

    uint8_t mode = CLOCK;
    time_value_t time;
    time_value_t alarm_time;
    uint8_t A;
    uint8_t B;
    
    while(1){
        // display(msg, decimal_index);
        encoder(&encoder_mode);
        encoder(&encoder_hours);
        encoder(&encoder_mins);
        encoder(&encoder_confirm);
        
        mode = encoder_mode.value;

        switch (mode){
            case CLOCK:{
                read_current_time(&time);
                sprintf(msg, "C   %02d%02d", time.hour, time.minute);
                display(msg, decimal_index);
                break;
            }
            case SET_TIME:{
                sprintf(msg, "St  %02d%02d", encoder_hours.value, encoder_mins.value);
                display(msg, decimal_index);
                if (encoder_confirm.value) {
                    time.hour = encoder_hours.value;
                    time.minute = encoder_mins.value;
                    write_current_time(&time);
                    mode = CLOCK;
                    encoder_confirm.value = 0;
                    encoder_mode.value = 0;
                }
                break;
            }
            case SET_ALARM:{
                sprintf(msg, "ALRM%02d%02d", encoder_hours.value, encoder_mins.value);
                display(msg, decimal_index);
                if (encoder_confirm.value) {
                    alarm_time.hour = encoder_hours.value;
                    alarm_time.minute = encoder_mins.value;
                    write_alarm_time(&alarm_time);
                    mode = CLOCK;
                    encoder_confirm.value = 0;
                    encoder_mode.value = 0;
                }
                break;
            }
        }

        if (alarm_triggered){
            clear_alarm_flag();
            PORTC &= ~(1<<PC0);
            // PORTB &= ~(1<<PB4);
            if (encoder_confirm.value){
                PORTC |= (1<<PC0);
                
                // PORTB |= (1<<PB4);
                alarm_triggered = false;
            }
        }

    }
    return EXIT_SUCCESS;
}
