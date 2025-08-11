#include "7seg.h"
#include <avr/io.h>

int main (void){
    disp_init();
    char msg[] = "12345678";
    uint8_t decimal_index = 0b00001000;
    while(1){
        display(msg,decimal_index);
    }
}
