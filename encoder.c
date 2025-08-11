#include "encoder.h"

encoder_t encoder_mode = {
    .pin_reg = &PINB,
    .CLK = (1<<PB0),
    .DT = (1<<PB1),
    .CLK_last_value = 1,
    .value = 0,
    .min = 0,
    .max = 2,
};

encoder_t encoder_hours = {
    .pin_reg = &PINB,
    .CLK = (1<<PB2),
    .DT = (1<<PB3),
    .CLK_last_value = 1,
    .value = 0,
    .min = 0,
    .max = 23,
};

encoder_t encoder_mins = {
    .pin_reg = &PINB,
    .CLK = (1<<PB4),
    .DT = (1<<PB5),
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

void encoder(encoder_t *encoder) {

    uint8_t clk_now = *(encoder->pin_reg) & encoder->CLK ? 1 : 0;

    if (clk_now != encoder->CLK_last_value) {
        if (clk_now == 0) {
            
            uint8_t dt_now = *(encoder->pin_reg) & encoder->DT ? 1 : 0;

            if (dt_now != clk_now) {
                encoder->value++;
                if (encoder->value > encoder->max){
                    encoder->value = encoder->min;
                } 
            }else {
                encoder->value--;
                if (encoder->value < encoder->min){
                    encoder->value = encoder->max;
                } 
            }
        }
    }

    encoder->CLK_last_value = clk_now;
}
