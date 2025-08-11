#include "7seg.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>

#define DIGITNUMBER 8
#define DIGITS 10
// static unsigned long timepassed = 0;
#define SHIFTSPEED 700
#define NUMBEROFLETTERSANDDIGITS 36
#define LATCH_PIN PB2

const uint8_t LETTER2SEG[NUMBEROFLETTERSANDDIGITS+1] = {
  [0]=0b00111111,
  [1]=0b00000110,
  [2]=0b01011011,
  [3]=0b01001111,
  [4]=0b01100110,
  [5]=0b01101101,
  [6]=0b01111101,
  [7]=0b00000111,
  [8]=0b01111111,
  [9]=0b01101111,
  [10]=0b01110111,
  [11]=0b01111111,
  [12]=0b00111001,
  [13]=0b00111111,
  [14]=0b01111001,
  [15]=0b01110001,
  [16]=0b01111101,
  [17]=0b01110110,
  [18]=0b00000110,
  [19]=0b00001110,
  [20]=0b01111000,
  [21]=0b00111000,
  [22]=0b00110111,
  [23]=0b01010100,
  [24]=0b00111111,
  [25]=0b01110011,
  [26]=0b01100111,
  [27]=0b01010000,
  [28]=0b01101101,
  [29]=0b01111000,
  [30]=0b00111110,
  [31]=0b01100010,
  [32]=0b00110110,
  [33]=0b01001001,
  [34]=0b01101110,
  [35]=0b00001001,
  [36]=0b00000000
};

static uint8_t ascii_to_index(const unsigned char c){
    if ((c >= '0') && (c <= '9'))
        return c - '0';

    if ((c >= 'A') && (c <= 'Z'))
        return c - 'A' + DIGITS;

    if ((c >= 'a') && (c <= 'z'))
        return c - 'a' + DIGITS;

    return NUMBEROFLETTERSANDDIGITS;
}

static void show(char letter, uint8_t digit, bool decimal){
    SPDR = ~(1<<digit);
    while (!(SPSR & (1 << SPIF)));

    SPDR = LETTER2SEG[ascii_to_index(letter)]|(decimal<<7);
    while (!(SPSR & (1 << SPIF))); // Wait for completion
    PORTB &= ~(1 << LATCH_PIN);
    _delay_us(1);
    PORTB |= (1 << LATCH_PIN);
}

void disp_init(void){
    DDRB |= (1 << PB3) | (1 << PB5) | (1 << LATCH_PIN);
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
    SPCR &= ~(1 << SPIE);
    SPCR &= ~(1 << DORD);
}

void display(char MESSAGE[], uint8_t decimal){
    for (int digit=0; digit<DIGITNUMBER; digit++){
            show(MESSAGE[digit], digit, (decimal&(1<<(7-digit))));
            _delay_ms(1);
    }
}