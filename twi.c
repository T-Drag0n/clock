#include "twi.h"

const uint32_t SCL_FREQUENCY = 100000;

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

static bool bcd_to_binary(uint8_t bcd_value){
    ;
}

twi_status_t twi_init(const uint32_t scl_frequency){
    if(!calc_scl(scl_frequency)){
        return FAILURE;
    }else{

        TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE);

        return SUCCESS;
    }
}

twi_status_t twi_enqueue(twi_message_t * const messages, size_t message_count)
{
    const uint8_t sreg = SREG;

    cli();

    const uint8_t twcr = TWCR;
    const bool    idle = twi_isr.idle;

    SREG = sreg;

    if (!(twcr & (1 << TWEN)))
        return TWI_STATUS_DISABLED;

    if (!idle)
        return TWI_STATUS_BUSY;

    twi_isr = (twi_isr_t) {
        .idle          = false,
        .messages      = messages,
        .message_count = message_count,
    };


    TWCR = twcr | (1 << TWSTA);

    return TWI_STATUS_SUCCESS;
}
