#include "twi.h"
#include <until.twi.h>
#include <avr/io.h>

const uint32_t SCL_FREQUENCY = 100000;
bool INTERUPT_FLAG_CHECK = (TWCR&(1<<TWINT))==0 ? 1:0;

static bool calc_scl(uint32_t scl){
    uint32_t const prescaler[4] = {1,4,16,64};
    
    for(int i = 0; i < 4; i++){
        uint32_t const twbr = ((F_CPU/scl)-16) / (2 * prescaler[i]);

        if(twbr < 256){
            TWSR = (TWSR & (~((1<<TWPS1) | (1<<TWPS0)))) | (i & 0x03);

            return SUCCESS;
        }
    }

    return FAILURE;
}

void twi_init(){
        TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE);
        while(INTERUPT_FLAG_CHECK);
}

void twi_write(uint16_t *data){
    TWDR = data;
    TWCR = (1<<TWINT) | (1<<TWEN);
    while(INTERUPT_FLAG_CHECK)
}

void twi_read_ack(){

}

void twi_read_nack(){
    
}
