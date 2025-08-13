#include "encoder.h"
#include "digital.h"

encoder_t encoder_mode = {
    .CLK_last_value = 1,
    .value = 0,
    .min = 0,
    .max = 2,
};

encoder_t encoder_hours = {
    .CLK_last_value = 1,
    .value = 0,
    .min = 0,
    .max = 23,
};

encoder_t encoder_mins = {
    .CLK_last_value = 1,
    .value = 0,
    .min = 0,
    .max = 59,
};

encoder_t encoder_confirm = {
    .pin_reg = &PIND,
    .CLK = (1<<PD7),
    .DT = (1<<PD6),
    .CLK_last_value = 1,
    .value = 0,
    .min = 0,
    .max = 1,
};


void encoder_init(encoder_t *encoder, uint8_t pinA, uint8_t pinB) {
    // Set pins as input

    digital_pin_mode(pinA, INPUT_PULLUP);
    digital_pin_mode(pinB, INPUT_PULLUP);
    encoder->CLK = (1<<digital_pin_to_bit(pinA));
    encoder->DT = (1<<digital_pin_to_bit(pinB));
    encoder->pin_reg = digital_port_offset(digital_pin_to_port(pinA),PINx);

}

void encoder(encoder_t *encoder) {
    uint8_t clk_now = (*(encoder->pin_reg) & encoder->CLK) ? 1 : 0;

    if (clk_now != encoder->CLK_last_value && clk_now == 0) {
        
       // if (clk_now == 0) { // On falling edge
            uint8_t dt_now = (*(encoder->pin_reg) & encoder->DT) ? 1 : 0;

            if (dt_now == 1) {
                encoder->value--;
                if (encoder->value < encoder->min) {
                    encoder->value = encoder->max;
                }
            } else {
                encoder->value++;
                if (encoder->value > encoder->max) {
                    encoder->value = encoder->min;
                }
            }            
        
    }

    encoder->CLK_last_value = clk_now;
}