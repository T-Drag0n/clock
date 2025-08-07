#include "twi.h"

int main(void)
{
    sei();

    uint8_t pin_mask = (1 << PC5) | (1 << PC4);

    DDRC  &= ~pin_mask;
    PORTC |=  pin_mask;

    twi_status_t status = twi_init(SCL_FREQUENCY);

    if (status != TWI_STATUS_SUCCESS)
        return FAILURE;

    return SUCCESS;
}
