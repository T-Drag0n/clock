#include "time.h"
#include <avr/io.h>
#include <avr/interrupt.h>


void timer0_init() {

TCCR0A |= (0b10 << WGM00);
TCCR0B |= (0 << WGM02) | (0b011 << CS00); 

OCR0A = 249;

TIMSK0 = 1 << OCIE0A;
TCNT0 = 0;

} 

volatile uint32_t millis_counter = 0;

ISR(TIMER0_COMPA_vect) {
    millis_counter++;
}

uint32_t millis(void) {
    uint32_t ms;
    cli();
    ms = millis_counter;
    sei();
    return ms;
}