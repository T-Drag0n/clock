#include "twi.h"

int main(void)
{
    uint8_t pin_mask = (1 << PC5) | (1 << PC4);

    DDRC  &= ~pin_mask;
    PORTC |=  pin_mask;

}
