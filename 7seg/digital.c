#include "digital.h"
#include <avr/io.h>
#include <stddef.h>
#include <stdbool.h>


volatile uint8_t *digital_pin_to_port(uint8_t pin){
    if (pin<8){
        return &PORTD;
    }
    else if (pin<14){
        return &PORTB;
    }
    else if (pin>=14 && pin <=19){
        return &PORTC;
    }
    return NULL;
}


volatile uint8_t *digital_port_offset(volatile uint8_t * const port, digital_register_type_t type){
    return port-type;
}

int digital_pin_to_bit(uint8_t pin){
    if (pin < 8){
        return pin;
    }
    else if (pin>7 && pin<14){
        return pin-8;
    }
    else if(pin>=14&&pin<=19){
        return pin-14;
    }
    return -1;
}

bool digital_pin_mode(uint8_t pin, digital_mode_t mode){
    int bit = digital_pin_to_bit(pin);
    if (bit!=-1){
        volatile uint8_t *ddr = digital_port_offset(digital_pin_to_port(pin),DDRx);
        if (mode == OUTPUT){
            *ddr |= (1<<bit);
            return true;
        }
        else if (mode == INPUT){
            *ddr &= ~(1<<bit);
            return true;
        }
        else if (mode == INPUT_PULLUP){
            *ddr &= ~(1<<bit);
            volatile uint8_t *port = digital_port_offset(digital_pin_to_port(pin),PORTx);
            *port |= (1<<bit);
            return true;
        }

    }
    return false;
    
}

bool digital_write(uint8_t pin, bool high){
    int bit = digital_pin_to_bit(pin);
    if (bit!=-1){
        volatile uint8_t *port = digital_port_offset(digital_pin_to_port(pin),PORTx);
        if (high){
            *port |= (1<<digital_pin_to_bit(pin));
        }
        else{
            *port &= ~(1<<digital_pin_to_bit(pin));
        }
        return true;
    }
    return false;
    
}

bool digital_read(uint8_t pin, bool * const pin_state){
    int bit = digital_pin_to_bit(pin);
    volatile uint8_t *pinx = digital_port_offset(digital_pin_to_port(pin),PINx);
    if (bit!=-1){
        *pin_state = (*pinx & (1<<digital_pin_to_bit(pin)));
        return true;
    }
    return false;
}




