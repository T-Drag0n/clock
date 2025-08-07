#include "rtc.h"
#include <avr/io.h>

uint8_t bcd_to_binary(uint8_t bcd){
    return ((bcd>>4)*10) + (bcd & 0x0F);
}

uint8_t binary_to_bcd(uint8_t value){
    return (((value/10)<<4) | (value%10));
}

/*void readTime(){
    
}*/

